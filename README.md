# ESPHomeConfig

Configurations and flashing instructions for my ESPHome converted Gosund, Sonoff, and Magic Home smart home devices.    <br>

## Includes:
* Docker Compose configuration for ESPHome.
* ESPHome YAML configuration for the ```Magic Home ESP-IR-B-v2.3``` RGB LED strip controller with some custom lighting effects.
* ESPHome YAML configuration for the ```Magic Home ZJ-WFMN-A V1.1``` RGB LED strip controller with some custom lighting effects.
* ESPHome YAML configuration for the ```Gosund WP3 (ESP8285)``` Smart Plug
* ESPHome YAML configuration for the ```Sonoff S31``` Smart Plug
* Instructions (with photos) on flashing custom firmware to the aforementioned devices.

# Secrets File Configuration:
The configuration files in this repository import and depend on properties set in a secrets file.    <br>

Create a file called secrets.yaml in the config folder containing the following properties:
* ```wifissid```: "\<YOUR_SSID\>"    <br>
    ```wifipass```: "\<YOUR_PASSWORD\>"    <br>
    ```fallbackpass```: "\<YOUR_HOTSPOT_FALLBACK_PASSWORD\>"    <br>
    ```encryption_key```: "\<YOUR_ENCRYPTION_KEY\>"    <br>
    ```otauser```: "\<YOUR_OTA_USERNAME\>"    <br>
    ```otapass```: "\<YOUR_OTA_PASSWORD\>"    <br>

# Flashing Instructions (coming soon)

## Magic Home ESP-IR-B-v2.3 RGB LED Strip Controller  


## Magic Home ZJ-WFMN-A V1.1 RGB LED Strip Controller   


## Gosund WP3 (ESP8285) Smart Plug  


## Sonoff S31 Smart Plug

