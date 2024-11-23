#include <Arduino.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "wifi_helpers.h"
#include <secrets.h>
#include <sensor_constants.h>

#define INIT 0
#define YELLOW_LED 25
#define BLUE_LED 26
#define ONE_WIRE_BUS 27
// 1 min
#define INTERVAL 60000
#define MAX_RETRIES 12
#define RETRY_DELAY 5000

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to DallasTemperature library
DallasTemperature ds18b20(&oneWire);

String createPayload(SensorLocation location, SensorValueType type, float value)
{
    // Create the payload string using String concatenation
    String payload = "{\"requests\":[";
    payload += "{\"type\":\"execute\",\"stmt\":{\"sql\":\"INSERT INTO sensor_data (location, type, value) VALUES (" + String(location) + ", " + String(type) + ", " + String(value, 2) + ")\"}},"; // Added formatting for value (2 decimals)
    payload += "{\"type\":\"close\"}";
    payload += "]}";

    return payload;
}

int postData(String payload)
{
    HTTPClient http;
    http.begin(TURSO_ENDPOINT);

    // Send GET request
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", TURSO_TOKEN);

    int httpResponseCode = http.POST(payload);

    // // UNCOMMENT FOR DEBUGGING
    // // Check the HTTP response code
    // if (httpResponseCode > 0)
    // {
    //     // Success, print the response payload
    //     String response = http.getString();
    //     Serial.println("Response:");
    //     Serial.println(response);
    // }
    // else
    // {
    //     Serial.print("Error code: ");
    //     Serial.println(httpResponseCode);
    // }

    // Free resources
    http.end();

    return httpResponseCode;
}

// We round to the nearest .5 value because more precision is not needed for the temperate
// Then, we multiply the value by 10 to store an INTEGER in the DB instead of a REAL
int roundToHalfTimesTen(float num)
{
    int value = (int)(num * 2 + (num > 0 ? 0.5 : -0.5)) / 2;
    return (value * 10);
}

void setup()
{
    ds18b20.begin(); // Start up the library
    // // UNCOMMENT FOR DEBUGGING
    // Serial.begin(9600);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
}

unsigned long previousMillis = INIT;
// Init to ~0 Kelvin * 10 in Celsius
int previousTemp = -2735;
void loop()
{
    unsigned long currentMillis = millis();

    if (previousMillis == INIT || (currentMillis - previousMillis >= INTERVAL))
    {
        int retries = 0;
        int httpReturnCode = -1;
        previousMillis = currentMillis;
        while (retries < MAX_RETRIES)
        {
            digitalWrite(BLUE_LED, LOW);
            connectToWiFi(SSID, SSID_PASSWORD);
            digitalWrite(BLUE_LED, HIGH);

            ds18b20.requestTemperatures();
            // Temp is in Celsius

            int currentTemp = roundToHalfTimesTen(ds18b20.getTempCByIndex(0));

            // If the new temperature is the same as the previous one, we don't post
            if (currentTemp == previousTemp)
            {
                break;
            }

            previousTemp = currentTemp;

            digitalWrite(YELLOW_LED, HIGH);
            int httpReturnCode = postData(createPayload(ATTIC, TEMPERATURE, currentTemp));

            // If the POST was successful, we turn off the LED and break out of the loop
            if (httpReturnCode == 200)
            {
                digitalWrite(YELLOW_LED, LOW);
                break;
            }
            else
            {
                retries++;
                delay(RETRY_DELAY);
            }

            // // UNCOMMENT FOR DEBUGGING
            // Serial.println(httpReturnCode);
        }
    }
}