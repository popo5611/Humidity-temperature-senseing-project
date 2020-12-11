/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME "type_your_adafruit_username"
#define IO_KEY "type_your_adafruit_key"

/******************************* WIFI **************************************/

#define WIFI_SSID "type_your_wifi_name"
#define WIFI_PASS "type_your_wifi_password"

#include "AdafruitIO_WiFi.h"

#if defined(USE_AIRLIFT) || defined(ADAFRUIT_METRO_M4_AIRLIFT_LITE) ||         \
    defined(ADAFRUIT_PYPORTAL)
// Configure the pins used for the ESP32 connection
#if !defined(SPIWIFI_SS) // if the wifi definition isnt in the board variant
// Don't change the names of these #define's! they match the variant ones
#define SPIWIFI SPI
#define SPIWIFI_SS 10 // Chip select pin
#define NINA_ACK 9    // a.k.a BUSY or READY pin
#define NINA_RESETN 6 // Reset pin
#define NINA_GPIO0 -1 // Not connected
#endif
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS, SPIWIFI_SS,
                   NINA_ACK, NINA_RESETN, NINA_GPIO0, &SPIWIFI);
#else
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
#endif

/************************Temperature & Humidity Code Starts Here *******************************/
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// pin connected to DHT11 data line
#define DATA_PIN 4

// create DHT11 instance
DHT_Unified dht(DATA_PIN, DHT11);

// set up the 'temperature' and 'humidity' feeds
// comment out the lines for the respective ESP32 boards
// feeds from ESP32 Board 1
AdafruitIO_Feed *temperature = io.feed("temperature_1");
AdafruitIO_Feed *humidity = io.feed("humidity_1");

// feeds from ESP32 Board 2
//AdafruitIO_Feed *temperature = io.feed("temperature_2");
//AdafruitIO_Feed *humidity = io.feed("humidity_2");

// feeds from ESP32 Board 3
//AdafruitIO_Feed *temperature = io.feed("temperature_3");
//AdafruitIO_Feed *humidity = io.feed("humidity_3");

// feeds from ESP32 Board 4
//AdafruitIO_Feed *temperature = io.feed("temperature_4");
//AdafruitIO_Feed *humidity = io.feed("humidity_4");

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // initialize dht11
  dht.begin();

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  sensors_event_t event;
  dht.temperature().getEvent(&event);

  float celsius = event.temperature;
  float fahrenheit = (celsius * 1.8) + 32;

  Serial.print("celsius: ");
  Serial.print(celsius);
  Serial.println("C");

  Serial.print("fahrenheit: ");
  Serial.print(fahrenheit);
  Serial.println("F");

  // save fahrenheit (or celsius) to Adafruit IO
  temperature->save(celsius);

  dht.humidity().getEvent(&event);

  Serial.print("humidity: ");
  Serial.print(event.relative_humidity);
  Serial.println("%");

  // save humidity to Adafruit IO
  humidity->save(event.relative_humidity);

  // wait 5 seconds (5000 milliseconds == 5 seconds)
  delay(5000);

}
