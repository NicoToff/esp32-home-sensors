# ESP32 Home Sensors

On boot and then every 10 minutes, an ESP32 writes a temperature entry into a `libsql` database.

I'm using a cloud database provided by [Turso](https://docs.turso.tech/introduction). The ESP32 simply uses the [Turso HTTP API](https://docs.turso.tech/sdk/http/quickstart) to write new entries into the database.

## Wiring example

[Interfacing DS18B20 1-Wire Digital Temperature Sensor with Arduino](https://lastminuteengineers.com/ds18b20-arduino-tutorial/)