# ESPHome Configurations and Flashing Instructions

Configurations and flashing instructions for my ESPHome converted Gosund, Sonoff, and Magic Home smart home devices.

## Table of Contents

- [Description](#esphome-configurations-and-flashing-instructions)
- [Included In This Repository](#included-in-this-repository)
- [Getting Started](#getting-started)
- [Flashing Instructions](#flashing-instructions-coming-soon)
- [License](#license)
- [Disclaimer](#disclaimer)

## Included In This Repository

- Docker Compose configuration for ESPHome CLI
- ESPHome YAML configuration for the `Magic Home ESP-IR-B-v2.3` RGB LED strip controller with some custom lighting effects.
- ESPHome YAML configuration for the `Magic Home ZJ-WFMN-A V1.1` RGB LED strip controller with some custom lighting effects.
- ESPHome YAML configuration for the `Gosund WP3 (ESP8285)` Smart Plug
- ESPHome YAML configuration for the `Sonoff S31` Smart Plug
- Custom YAML configurations for `ESP8266` and `ESP32` boards
- Instructions (with photos) on flashing custom firmware to the aforementioned devices

## Getting Started

1. Clone the repository:

   ```shell
   git clone https://github.com/mwdle/ESPHomeConfig.git
   ```

2. Copy `.env.example` to `.env` and configure the properties for your environment.

3. Copy `config/secrets.yaml.template` to `config/secrets.yaml` and update the variables with your own values.

4. Open a terminal in the directory containing the docker-compose file.
5. Create docker macvlan and bridge network for the container.

   ```shell
   docker network create -d macvlan --subnet=192.168.0.0/24 --gateway=192.168.0.1 -o parent=eno1 AAA_LAN
   docker network create --subnet 172.16.1.0/24 --gateway 172.16.1.1 ESPHome
   ```

   - Ensure the gateway and subnet match your LAN network.
   - AAA in the network name ensures Docker uses this network as the primary interface for all connected containers.
   - eno1 is the name of the network interface on my system. Replace this with the name of your network interface.

6. Start the container:

   ```shell
   docker compose up -d
   ```

Your container should be up and running and you should be able to execute ESPHome commands via `docker exec` like so:

```shell
docker exec -it ESPHome esphome run YOUR_CONFIG.yaml
```

**Accessing the Web Interface:**

Due to the MacVLAN network configuration, the ESPHome container receives a DHCP-assigned IP on your LAN. The web interface runs on port 6052. You can access it using one of the following options:

- Finding the container's IP address via your router's DHCP lease table or using `docker inspect ESPHome`
- Configuring a static IP in the MacVLAN network setup
- Using a reverse proxy (advanced users)

Note: Standard Docker port forwarding (`-p` flag) does not work with MacVLAN networks.

## Flashing Instructions (coming soon)

### Magic Home ESP-IR-B-v2.3 RGB LED Strip Controller

### Magic Home ZJ-WFMN-A V1.1 RGB LED Strip Controller

### Gosund WP3 (ESP8285) Smart Plug

### Sonoff S31 Smart Plug

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Disclaimer

This repository is provided as-is and is intended for informational and reference purposes only. The author assumes no responsibility for any errors or omissions in the content or for any consequences that may arise from the use of the information provided. Always exercise caution and seek professional advice if necessary.
