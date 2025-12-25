/**
 * @file esp8266_music_remote.h
 * @brief ESPHome C++ implementation for the ESP8266 Music Remote project to render all display pages, handle playlist management, and more.
 * @author mwdle
 *
 * This file contains various functions to render and handle all display pages, including the music player, playlist selection, and statistics page. More pages may be implemented in the future.
 * The Music Player page displays the current track title and artist, scrolling them across the screen as needed (a 128x64 display is expected).
 * The playlist selection pages relies on a custom Home Assistant template sensor which allows you to pull playlist names from Music Assistant to show them on the display and allow selecting them for playback.
 * The statistics page shows information about wifi connectivity and signal, and Home Assistant API connectivity status.
 * This file also contains a function to load Music Assistant playlist names into a global vector of c strings using a custom Home Assistant template which allows for viewing and selecting any number of playlists from Music Assistant from within ESPHome.
 */

const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int HALF_SCREEN_WIDTH = SCREEN_WIDTH / 2;

const int SCROLL_INCREMENT = 1;  // The number of pixels to move scrolling text by each time the display updates. For 24fps, 1 pixel is recommended.

/**
 * Aligns a given text reset position to an interval of the scroll increment relative to the center of the display.
 * Given the potentially limited resolution of the scrolling effect (e.g. it moves left at a defined increment each time), this macro ensures that the scroll positions will align to an interval of the scroll increment with respect to the center of the display.
 */
#define ALIGN_RESET_POS(reset_pos) (reset_pos + (SCROLL_INCREMENT-((reset_pos-HALF_SCREEN_WIDTH)%SCROLL_INCREMENT)))

/**
 * Draws the volume bar and icon on the display.
 */
void draw_volume() {
    screen->print(round(SCREEN_WIDTH * 0.078), round(SCREEN_HEIGHT * 0.64), mdi_18, TextAlign::CENTER, "󰕾"); // Print volume-high icon
    screen->rectangle(20, 36, 105, 7); // Volume bar outline
    screen->filled_rectangle(21, 37, (int)(media_volume->state * 104), 6); // Volume bar fill
}

/**
 * Draws physical button icon representations on the display depending on the currently selected display page.
 * Includes icons for indicating the currently selected page (playback, playlist selection, or stats), as well as indicators for the physical buttons such page change, skip-previous, play/pause, skip-next, and volume sync/balance.
 * 
 * Takes up the bottom 18 pixels on the display and is meant to be rendered on every display page.
 */
void render_ui_controls() {
    screen->line(0, (SCREEN_HEIGHT * 0.73), (SCREEN_WIDTH - 1), (SCREEN_HEIGHT * 0.73)); // Horizontal line
    if (screen->get_active_page() == playbackPage)
        screen->print(13, 56, mdi_16, TextAlign::CENTER, "󰀟"); // cast-variant icon
    else if (screen->get_active_page() == playlistSelectionPage)
        screen->print(14, 56, mdi_20, TextAlign::CENTER, "󰲸"); // playlist-music icon
    else if (screen->get_active_page() == statsPage)
        screen->print(14, 56, mdi_16, TextAlign::CENTER, "󰋼"); // information icon
    screen->print(38, 56, mdi_21, TextAlign::CENTER, "󰒮"); // skip-previous icon
    if (media_status->state == "playing" || media_status->state == "buffering")
        screen->print(HALF_SCREEN_WIDTH, 56, mdi_21, TextAlign::CENTER, "󰏤"); // pause icon
    else
        screen->print(63, 56, mdi_21, TextAlign::CENTER, "󰐊"); // play icon
    screen->print(89, 56, mdi_21, TextAlign::CENTER, "󰒭"); // skip-next icon
    screen->print(115, 56, mdi_18, TextAlign::CENTER, "󱬐"); // volume-equal icon
}

/**
 * Draws statistics such as wifi signal strength and API connection status on the display.
 * Allows users to determine the connection status to wifi and Home Assistant API.
 */
void render_statistics() {
    auto wifi_signal = wifi_signal_sensor->state;
    if (isnan(wifi_signal) || wifi_signal > 0)
        screen->print(3, 0, mdi_16, "󰖪"); // wifi-off icon
    else
        screen->print(3, 0, mdi_16, "󰖩"); // wifi icon
    screen->printf(22, 0, opensans_12, "%.0f dBm", wifi_signal);
    bool api_connected = global_api_server->is_connected();
    if (api_connected)
        screen->print(2, 17, mdi_16, "󰄙"); // cast-connected icon
    else
        screen->print(2, 16, mdi_16, "󰄘"); // cast icon
    screen->print(22, 16, opensans_12, api_connected ? "Connected" : "Disconnected");
    draw_volume();
}

void render_playlist_selection() {
    if (global_api_server->is_connected()) {
        auto playlistSize = playlists->value().size();
        if (playlistSize == 0)
            screen->print(HALF_SCREEN_WIDTH, 24, opensans_12, TextAlign::CENTER, "No Playlists Found");
        else {
            static int last_selected_width = 0;
            int selected_width = 0;
            int ignored;

            opensans_12->measure(playlists->value()[selected_playlist->value()], &selected_width, &ignored, &ignored, &ignored);

            static int selected_playlist_pos = HALF_SCREEN_WIDTH;
            static int wraparound_playlist_pos = ALIGN_RESET_POS(SCREEN_WIDTH + (selected_width / 2));

            // If the selected playlist has changed, re-calculate the scroll positions
            if (selected_width != last_selected_width) {
                last_selected_width = selected_width;
                selected_playlist_pos = HALF_SCREEN_WIDTH;
                wraparound_playlist_pos = ALIGN_RESET_POS(SCREEN_WIDTH + (selected_width / 2));
            }

            // Handle scroll positions
            if (selected_playlist_pos + (selected_width / 2) < 0) { // Reset the wrap around scroll position if the selected playlist has scrolled off the display
                selected_playlist_pos = wraparound_playlist_pos;
                wraparound_playlist_pos = ALIGN_RESET_POS(SCREEN_WIDTH + (selected_width / 2));
            } else if (selected_playlist_pos - (selected_width / 2) < 0 && selected_playlist_pos + (selected_width / 2) < HALF_SCREEN_WIDTH) { // If the selected playlist is on the left half of the display, print the wrap around playlist on the right half
               screen->print(wraparound_playlist_pos, 22, opensans_12, TextAlign::CENTER, playlists->value()[selected_playlist->value()]);
               wraparound_playlist_pos -= SCROLL_INCREMENT;
            }

            // Print the playlists. 3 cases: the first playlist is selected, the last playlist is selected, or any playlist inbetween is selected.
            if (selected_playlist->value() == 0) {
                screen->print(selected_playlist_pos, 22, opensans_12, TextAlign::CENTER, playlists->value()[0]);
                if (playlistSize > 1)
                    screen->print(HALF_SCREEN_WIDTH, 38, opensans_10, TextAlign::CENTER, playlists->value()[1]);
            }
            else if (selected_playlist->value() == playlistSize - 1) {
                screen->print(HALF_SCREEN_WIDTH, 8, opensans_10, TextAlign::CENTER, playlists->value()[playlistSize - 2]); 
                screen->print(selected_playlist_pos, 22, opensans_12, TextAlign::CENTER, playlists->value()[playlistSize - 1]);
            }
            else {
                screen->print(HALF_SCREEN_WIDTH, 8, opensans_10, TextAlign::CENTER, playlists->value()[selected_playlist->value() - 1]);
                screen->print(selected_playlist_pos, 22, opensans_12, TextAlign::CENTER, playlists->value()[selected_playlist->value()]);
                screen->print(HALF_SCREEN_WIDTH, 38, opensans_10, TextAlign::CENTER, playlists->value()[selected_playlist->value() + 1]);
            }
            selected_playlist_pos -= SCROLL_INCREMENT;
            screen->line((HALF_SCREEN_WIDTH - ((selected_width + 4) / 2)), 31, HALF_SCREEN_WIDTH + ((selected_width + 4) / 2), 31); // Underline the selected playlist
        }
    }
    else {
        screen->print(HALF_SCREEN_WIDTH, 15, opensans_12, TextAlign::CENTER, "Music Assistant");
        screen->print(HALF_SCREEN_WIDTH, 30, opensans_12, TextAlign::CENTER, "is not connected!");
    }
}

/**
 * Draws the currently playing media title and artist on the display.
 * The title is scrolled across the screen since it will not fit on the display, and the artist is centered under the title since it will fit on the display.
 */
void scroll_media_title_only(int* title_pos, int* wraparound_title_pos, int half_title_width, const char* title, const char* artist) {
    if (*title_pos + half_title_width < 0) { // Reset the wrap around scroll position if the title has scrolled off the display
        *title_pos = *wraparound_title_pos;
        *wraparound_title_pos = ALIGN_RESET_POS(SCREEN_WIDTH + half_title_width);
    } else if (*title_pos + half_title_width < HALF_SCREEN_WIDTH) { // If the title is on the left half of the display, print the wrap around title on the right half
        screen->print(*wraparound_title_pos, 10, opensans_12, TextAlign::CENTER, title);
        *wraparound_title_pos -= SCROLL_INCREMENT;
    }
    screen->print(*title_pos, 10, opensans_12, TextAlign::CENTER, title);
    screen->print(HALF_SCREEN_WIDTH, 24, opensans_12, TextAlign::CENTER, artist);
}

/**
 * Draws the currently playing media artist and title on the display.
 * The artist is scrolled across the screen since it will not fit on the display, and the title is centered above the artist since it will fit on the display.
 */
void scroll_media_artist_only(int* artist_pos, int* wraparound_artist_pos, int half_artist_width, const char* title, const char* artist) {
    if (*artist_pos + half_artist_width < 0) { // Reset the wrap around scroll position if the artist has scrolled off the display
        *artist_pos = *wraparound_artist_pos;
        *wraparound_artist_pos = ALIGN_RESET_POS(SCREEN_WIDTH + half_artist_width);
    } else if (*artist_pos + half_artist_width < HALF_SCREEN_WIDTH) { // If the artist is on the left half of the display, print the wrap around artist on the right half
        screen->print(*wraparound_artist_pos, 24, opensans_12, TextAlign::CENTER, artist);
        *wraparound_artist_pos -= SCROLL_INCREMENT;
    }
    screen->print(*artist_pos, 24, opensans_12, TextAlign::CENTER, artist);
    screen->print(HALF_SCREEN_WIDTH, 10, opensans_12, TextAlign::CENTER, title);
}

/**
 * Draws the currently playing media artist and title on the display.
 * The artist and title are scrolled *together* across the screen since they will not fit on the display.
 */
void scroll_media_artist_and_title(bool titleWidthGreaterThanArtist, int* title_pos, int* wraparound_title_pos, int half_title_width, int* artist_pos, int* wraparound_artist_pos, int half_artist_width, const char* title, const char* artist) {
    if (titleWidthGreaterThanArtist) {
        if (*title_pos + half_title_width < 0) { // Reset the wrap around scroll positions if the title has scrolled off the display
            *title_pos = *wraparound_title_pos;
            *artist_pos = *wraparound_artist_pos;
            *wraparound_title_pos = ALIGN_RESET_POS(SCREEN_WIDTH + half_title_width);
            *wraparound_artist_pos = ALIGN_RESET_POS(SCREEN_WIDTH + half_title_width);
        } else if (*title_pos + half_title_width < HALF_SCREEN_WIDTH) { // If the title is on the left half of the display, print the wrap around title and artist on the right half
            screen->print(*wraparound_title_pos, 10, opensans_12, TextAlign::CENTER, title);
            screen->print(*wraparound_artist_pos, 24, opensans_12, TextAlign::CENTER, artist);
            *wraparound_title_pos -= SCROLL_INCREMENT;
            *wraparound_artist_pos -= SCROLL_INCREMENT;
        }
    }
    else {
        if (*artist_pos + half_artist_width < 0) { // Reset the wrap around scroll positions if the artist has scrolled off the display
            *artist_pos = *wraparound_artist_pos;
            *title_pos = *wraparound_title_pos;
            *wraparound_artist_pos = ALIGN_RESET_POS(SCREEN_WIDTH + half_artist_width);
            *wraparound_title_pos = ALIGN_RESET_POS(SCREEN_WIDTH + half_artist_width);
        } else if (*artist_pos + half_artist_width < HALF_SCREEN_WIDTH) { // If the artist is on the left half of the display, print the wrap around artist and title on the right half
            screen->print(*wraparound_artist_pos, 24, opensans_12, TextAlign::CENTER, artist);
            screen->print(*wraparound_title_pos, 10, opensans_12, TextAlign::CENTER, title);
            *wraparound_artist_pos -= SCROLL_INCREMENT;
            *wraparound_title_pos -= SCROLL_INCREMENT;
        }
    }
    screen->print(*title_pos, 10, opensans_12, TextAlign::CENTER, title);
    screen->print(*artist_pos, 24, opensans_12, TextAlign::CENTER, artist);
}

/**
 * @brief Renders the currently playing media title and artist on the display.
 * If the title does not fit comfortably on the display but the artist does, scrolls the title text and centers the artist.
 * If the artist does not fit comfortably on the display but the title does, scrolls the artist text and centers the title.
 * If neither the title nor artist fit comfortably on the display, scrolls both.
 * If both the title and artist fit comfortably on the display, centers both.
 * If the player is idle or disconnected, prints a message on the display.
 */
void render_active_media() {
    const char* title = media_title->state.c_str();
    const char* artist = media_artist->state.c_str();

    if (global_api_server->is_connected() && title[0] != '\0') {
        static int width_threshold = SCREEN_WIDTH + 2;
        static int last_title_width = 0;
        static int last_artist_width = 0;

        int title_width = 0, artist_width = 0; int ignored;
        opensans_12->measure(title, &title_width, &ignored, &ignored, &ignored);
        opensans_12->measure(artist, &artist_width, &ignored, &ignored, &ignored);
        static int half_title_width = title_width / 2;
        static int half_artist_width = artist_width / 2;

        static int title_pos = HALF_SCREEN_WIDTH, wraparound_title_pos = ALIGN_RESET_POS(SCREEN_WIDTH + (title_width >= artist_width ? half_title_width : half_artist_width));
        static int artist_pos = HALF_SCREEN_WIDTH, wraparound_artist_pos = ALIGN_RESET_POS(SCREEN_WIDTH + (artist_width > title_width ? half_artist_width : half_title_width));

        // If the track changed, recalculate the starting positions
        if (title_width != last_title_width || artist_width != last_artist_width) {
            last_title_width = title_width;
            last_artist_width = artist_width;
            half_title_width = title_width / 2;
            half_artist_width = artist_width / 2;
            title_pos = HALF_SCREEN_WIDTH, wraparound_title_pos = ALIGN_RESET_POS(SCREEN_WIDTH + (title_width >= artist_width ? half_title_width : half_artist_width));
            artist_pos = HALF_SCREEN_WIDTH, wraparound_artist_pos = ALIGN_RESET_POS(SCREEN_WIDTH + (artist_width > title_width ? half_artist_width : half_title_width));
        }

        // If title and artist fit comfortably on the display, center both.
        if (title_width < width_threshold && artist_width < width_threshold) {
            screen->print(HALF_SCREEN_WIDTH, 10, opensans_12, TextAlign::CENTER, title);
            screen->print(HALF_SCREEN_WIDTH, 24, opensans_12, TextAlign::CENTER, artist);
        }
        // If the title does not fit comfortably on the display but the artist does, scroll the title text, and center the artist.
        else if (title_width >= width_threshold && artist_width < width_threshold)
            scroll_media_title_only(&title_pos, &wraparound_title_pos, half_title_width, title, artist);
        // If the artist does not fit comfortably on the display but the title does, scroll the artist text, and center the title.
        else if (artist_width >= width_threshold && title_width < width_threshold)
            scroll_media_artist_only(&artist_pos, &wraparound_artist_pos, half_artist_width, title, artist);
        // If the title nor artist fit comfortably on the display, scroll both.
        else if (artist_width >= width_threshold && title_width >= width_threshold)
            scroll_media_artist_and_title((title_width >= artist_width), &title_pos, &wraparound_title_pos, half_title_width, &artist_pos, &wraparound_artist_pos, half_artist_width, title, artist);

        title_pos -= SCROLL_INCREMENT;
        artist_pos -= SCROLL_INCREMENT;

        // Draw Volume Bar:
        draw_volume();
    } 
    else {
        screen->print(HALF_SCREEN_WIDTH, 15, opensans_12, TextAlign::CENTER, "Player is idle");
        screen->print(HALF_SCREEN_WIDTH, 30, opensans_12, TextAlign::CENTER, "or disconnected!");
    }
}

/**
 * Loads a string of playlist names separated by commas into the global vector of c string playlists.
 * @param playlistList A string of playlist names separated by commas.
 */
void load_playlist_sensor_data(const char* playlistList) {
    // Sets up the global vector of c string playlists
    playlists->value().clear();
    playlists->value().shrink_to_fit();

    const char* start = playlistList; // Pointer to the start of the current token
    const char* current = playlistList; // Pointer to traverse the string

    while (*current != '\0') {
        if (*current == ',') {
            // Found a delimiter, process the current token
            int tokenLength = (int)(current - start); // Determines the length of the string in bytes NOT including a null terminator
            char* playlist = new char[tokenLength + 1]; // Accounts for the null terminator
            playlist[tokenLength] = '\0';  // Null terminate the playlist string
            std::strncpy(playlist, start, tokenLength);
            playlists->value().push_back(playlist); // Add the playlist to the list
            start = current + 1; // Update start to the character after the comma
        }
        current++; // Move to the next character
    }
    
    // Process the last token if it exists
    if (start != current) {
        int tokenLength = (int)(current - start); // Determines the length of the string in bytes NOT including the null terminator
        char* playlist = new char[tokenLength + 1]; // Account for the null terminator
        std::strncpy(playlist, start, tokenLength);
        playlist[tokenLength] = '\0';  // Null terminate the playlist string
        playlists->value().push_back(playlist); // Add the playlist to the list
    }
}