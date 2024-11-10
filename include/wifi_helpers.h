#include <WiFi.h>

// Function to ensure WiFi connection with memory cleanup before reconnecting
extern void connectToWiFi(const char *ssid, const char *password)
{
    // Early return if already connected
    if (WiFi.status() == WL_CONNECTED)
    {
        return;
    }

    // Disconnect and free memory if previously connected
    WiFi.disconnect(true);

    // Set WiFi to Station Mode (WIFI_STA is dafault, though)
    WiFi.mode(WIFI_STA);

    // Start the WiFi connection
    WiFi.begin(ssid, password);

    // Wait until connected
    while (WiFi.status() != WL_CONNECTED)
    {
        // Delay for a short period to avoid tight looping
        delay(50);
    }
}