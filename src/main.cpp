#include <Arduino.h>

/*-----------------------------------------------------------------------------
 * DHT Set up 
 *-----------------------------------------------------------------------------*/
// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
// - Adafruit OLED library: adafruit/Adafruit SSD1306@^2.5.7

#include "DHT.h"
#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY (if your sensor has 4 pins)
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

/*-----------------------------------------------------------------------------
 *  OLED Screen Set up
 *-----------------------------------------------------------------------------*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BAUD_RATE			115200

#define SCREEN_WIDTH			128
#define SCREEN_HEIGHT			64
#define OLED_ADDR			0x3C

#define uS_TO_S				1000
#define TIME_BETWEEN_MEASURE		(10 * uS_TO_S)
#define TIME_BETWEEN_FAILED_MEASURE	(2 * uS_TO_S)
#define DISPLAY_WELCOME_TIME		(5 * uS_TO_S)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT) ;


/*-----------------------------------------------------------------------------
 * Water Atomizer 
 *-----------------------------------------------------------------------------*/
#define ATOMIZER_PIN			15

void setup() {
	Serial.begin(BAUD_RATE) ;

	Serial.println("DHTxx test!") ;
	dht.begin();

	Serial.println("SSD1306 test!") ;
	display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR) ;
	display.clearDisplay() ;
	
	display.setTextSize(3) ;
	display.setTextColor(WHITE) ;
	display.setCursor(0,0) ;
	display.println("DHT22\nClock") ;

	display.display() ;
	
	delay(DISPLAY_WELCOME_TIME) ;

	pinMode(ATOMIZER_PIN, OUTPUT) ;
}

void loop() {
	bool humidifier_fired = false ;
	uint8_t humidity_threshold = 70 ;
	display.clearDisplay() ;

	display.setTextSize(2) ;
	display.setTextColor(WHITE) ;
	display.setCursor(0,0) ;

	// Reading temperature or humidity takes about 250 milliseconds!
	// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
	float humidity = dht.readHumidity() ;

	// Read temperature as Celsius (the default)
	float temp_c = dht.readTemperature() ;

	// Read temperature as Fahrenheit (isFahrenheit = true)
	float temp_f = dht.readTemperature(true) ;

	// Check if any reads failed and exit early (to try again).
	if (isnan(humidity) || isnan(temp_c) || isnan(temp_f)) {
		Serial.println("Failed to read from DHT sensor!") ;
		delay(TIME_BETWEEN_FAILED_MEASURE) ;
	return;
	}

	// Compute heat index in Fahrenheit (the default)
	float heat_index_f = dht.computeHeatIndex(temp_f, humidity) ;
	// Compute heat index in Celsius (isFahreheit = false)
	/* float heat_index_c = dht.computeHeatIndex(temp_c, humidity, false); */

	display.print("H: ") ;
	display.print(humidity) ;
	display.println("%") ;

	display.print("T: ") ;
	/* display.print(temp_c) ; */
	/* display.println("C ") ; */

	display.print(temp_f);
	display.println("F\n") ;

	display.print("Idx: ") ;
	/* display.print(heat_index_c) ; */
	/* display.println("C ") ; */

	display.print(heat_index_f) ;
	display.println("F") ;

	display.display() ;

	// TODO: Case statements. If humidity is less than 50, do 10s. If 60, 5s. 70 = 3s
	if (humidity < humidity_threshold) {
		humidifier_fired = true ;
		digitalWrite(ATOMIZER_PIN, HIGH) ;
		humidity = digitalRead(ATOMIZER_PIN) ;
		delay(TIME_BETWEEN_MEASURE) ;
		digitalWrite(ATOMIZER_PIN, LOW) ;
	}

	if (!humidifier_fired) {
		delay(TIME_BETWEEN_MEASURE);
	}
	humidifier_fired = false ;
}
