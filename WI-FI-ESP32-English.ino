// This sketch connects the ESP32 to a Wi-Fi network.
#include <WiFi.h>
// This header provides the network client used by the server.
#include <WiFiClient.h>
// This header provides the HTTP server.
#include <WebServer.h>

// These variables store the Wi-Fi network name and password.
String wifiNetworkName = "";
String wifiNetworkPassword = "";

// This constant sets the HTTP server port.
const int SERVER_PORT = 80;

// These variables track connection attempts and whether the connection succeeded.
int connectionAttempts = 0;
int connectionAttemptLimit = 150;
bool connectionSucceeded = false;

// This creates the web server on the selected port.
WebServer server(SERVER_PORT);

// This function builds and sends the web page shown at the server's root URL.
void handleRootPage()
{
  // This raw string contains the HTML page returned to the browser.
  String response = R"(
    <!DOCTYPE html><html>
      <head>
        <title>ESP32 HTTP</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
      </head>
      <body>
        <h1>ESP32 Web Server Online</h1>
      </body>
    </html>
  )";
  // This sends the HTTP status and HTML page to the browser.
  server.send(200, "text/html", response);
}

// This function initializes serial communication and connects to Wi-Fi.
void setup()
{
  Serial.begin(115200);
  // This disables automatic Wi-Fi reconnection.
  WiFi.setAutoReconnect(false);

  // This loop keeps asking for credentials until a connection succeeds.
  while (connectionSucceeded == false)
  {
    // This resets the attempt counter before each new connection attempt.
    connectionAttempts = 0;
    // This asks the user to enter the Wi-Fi network name.
    Serial.println("Enter the Wi-Fi network name");
    // This waits until the user types something in the serial monitor.
    while (Serial.available() == 0)
    {
      delay(100);
    }
    // This reads the network name until the user presses Enter.
    wifiNetworkName = Serial.readStringUntil('\n');
    // This removes leading and trailing spaces and line breaks.
    wifiNetworkName.trim();
    // This prints a blank line.
    Serial.println();
    // This asks the user to enter the Wi-Fi password.
    Serial.println("Enter the Wi-Fi password");
    // This prints a blank line.
    Serial.println();
    // This waits until the user types something in the serial monitor.
    while (Serial.available() == 0)
    {
      delay(100);
    }
    // This reads the password until the user presses Enter.
    wifiNetworkPassword = Serial.readStringUntil('\n');
    // This removes leading and trailing spaces and line breaks.
    wifiNetworkPassword.trim();

    // This starts connecting to the selected Wi-Fi network.
    WiFi.begin(wifiNetworkName.c_str(), wifiNetworkPassword.c_str());
    // This tells the user that the credentials are being checked.
    Serial.println("Checking credentials, please wait!");
    // This waits for connection or until all 150 attempts have been used.
    while (WiFi.status() != WL_CONNECTED && (connectionAttempts < connectionAttemptLimit))
    {
      delay(100);
      // This prints a dot to show that the connection is still being checked.
      Serial.print(".");
      // This counts the current connection attempt.
      connectionAttempts = connectionAttempts + 1;
    }
    // This prints a blank line after the connection check.
    Serial.println();
    // This checks whether the ESP32 connected to Wi-Fi.
    if (WiFi.status() == WL_CONNECTED)
    {
      // This records the successful connection and ends the credential loop.
      connectionSucceeded = true;
      Serial.println();
      // This prints the connection details and project credit.
      Serial.print(
          "Connected: " + String(wifiNetworkName) + '\n' +
          "IP address: " + WiFi.localIP().toString() + '\n' +
          "Port: " + String(SERVER_PORT) + '\n' +
          "ESP32 HTTP server online..." + "\n\n\n" +
          " ===...Developed by: Bruno Maker - 2026 - All rights reserved...=== ");
    }
    else
    {
      // This disconnects from Wi-Fi after a failed connection attempt.
      WiFi.disconnect(false);
      // This prints a blank line.
      Serial.println();
      // This tells the user that the network name or password was incorrect.
      Serial.println("Invalid network name or password!");
      // This prints a blank line.
      Serial.println();
    }
  }
  // This connects the root URL to the HTML page handler.
  server.on("/", handleRootPage);
  // This starts the HTTP server.
  server.begin();
};

// This function handles incoming browser requests.
void loop()
{
  // This checks whether a browser has made a request.
  server.handleClient();
  delay(2);
};
