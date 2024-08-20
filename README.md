# ESPHome Configurations and Flashing Instructions

Configurations and flashing instructions for my ESPHome converted Gosund, Sonoff, and Magic Home smart home devices.  

## Table of Contents  

* [Description](#esphome-configurations-and-flashing-instructions)  
* [Included In This Repository](#included-in-this-repository)  
* [Getting Started](#getting-started)  
* [Flashing Instructions](#flashing-instructions)
* [License](#license)  
* [Disclaimer](#disclaimer)  

## Included In This Repository

* Docker Compose configuration for ESPHome CLI  
* ESPHome YAML configuration for the `Magic Home ESP-IR-B-v2.3` RGB LED strip controller with some custom lighting effects.  
* ESPHome YAML configuration for the `Magic Home ZJ-WFMN-A V1.1` RGB LED strip controller with some custom lighting effects.  
* ESPHome YAML configuration for the `Gosund WP3 (ESP8285)` Smart Plug  
* ESPHome YAML configuration for the `Sonoff S31` Smart Plug  
* Instructions (with photos) on flashing custom firmware to the aforementioned devices  

## Getting Started  

1. Clone the repository:  

    ```shell
    git clone https://github.com/mwdle/ESPHomeConfig.git
    ```  

2. Create a folder on your system for Docker bind mounts / storing container files. The folder should have the following structure:

    ```shell
    docker_volumes/
    ├── ESPHome/
    │   └── .esphome/
    ```  

3. Change the `.env` file properties for your configuration:  

    ```properties
    DOCKER_VOLUMES=<PATH_TO_DOCKER_VOLUMES_FOLDER> # The folder created in the previous step.
    ```  

4. Create a file called secrets.yaml in the "config" of this project containing the following properties:  

    ```YAML
    wifissid: "<YOUR_SSID>"
    wifipass: "<YOUR_PASSWORD>"
    fallbackpass: "<YOUR_HOTSPOT_FALLBACK_PASSWORD>"
    encryption_key: "<YOUR_ENCRYPTION_KEY>"
    otauser: "<YOUR_OTA_USERNAME>"
    otapass: "<YOUR_OTA_PASSWORD>"
    ```  

5. Open a terminal in the directory containing the docker-compose file.  
6. Start the container:  

    ```shell
    docker compose up -d
    ```  

Your container should be up and running and you should be able to execute ESPHome commands via `docker exec`.  

## Flashing Instructions  

### Magic Home ESP-IR-B-v2.3 RGB LED Strip Controller  

### Magic Home ZJ-WFMN-A V1.1 RGB LED Strip Controller  

### Gosund WP3 (ESP8285) Smart Plug  

### Sonoff S31 Smart Plug  

## License  

This project is licensed under the GNU General Public License v3.0 (GPL-3.0). See the [LICENSE](LICENSE.txt) file for details.  

## Disclaimer  

This repository is provided as-is and is intended for informational and reference purposes only. The author assumes no responsibility for any errors or omissions in the content or for any consequences that may arise from the use of the information provided. Always exercise caution and seek professional advice if necessary.  
