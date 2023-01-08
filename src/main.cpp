#include <Arduino.h>

#define BAUD_RATE			115200

#include "DHT.h"
#define DHT_PIN 2

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHT_PIN is
// Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY (if your sensor has 4 pins)
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHT_PIN, DHTTYPE);

/*-----------------------------------------------------------------------------
 *  OLED Screen Set up
 *-----------------------------------------------------------------------------*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH			128
#define SCREEN_HEIGHT			64
#define OLED_ADDR			0x3C

#define uS_TO_S				1000
#define TIME_BETWEEN_MEASURE		(1 * uS_TO_S)
#define TIME_BETWEEN_FAILED_MEASURE	(2 * uS_TO_S)
#define DISPLAY_WELCOME_TIME		(5 * uS_TO_S)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT) ;

/*-----------------------------------------------------------------------------
 * Water Atomizer 
 *-----------------------------------------------------------------------------*/
#define ATOMIZER_PIN			15

/*-----------------------------------------------------------------------------
 * Rotary Encoder 
 *-----------------------------------------------------------------------------*/
#define ENCODER_SW_PIN			16
#define ENCODER_A_PIN			14
#define ENCODER_B_PIN			12

float humidity_set_point    = 60 ;

enum e_encoder_switch_state {
	ENCODER_SWITCH_ON  = LOW ,
	ENCODER_SWITCH_OFF = HIGH
} ;

struct s_encoder {
	uint8_t press_count ;
	e_encoder_switch_state pressed ;
	uint8_t a_current_position ;
	uint8_t a_previous_position ;
} ;

s_encoder encoder {0, ENCODER_SWITCH_OFF} ;

/*-----------------------------------------------------------------------------
 * Function prototypes 
 *-----------------------------------------------------------------------------*/

void display_oled_welcome(void) ;
void display_oled(float humidity, float temp, float humidity_set_point) ;
bool verify_dht_read(float humidity, float temp) ;
void setup_oled(void) ;

IRAM_ATTR void ISR_encoder(void) ;

void setup() {
	Serial.begin(BAUD_RATE) ;

	pinMode(ENCODER_A_PIN,  INPUT_PULLUP) ;
	pinMode(ENCODER_B_PIN,  INPUT_PULLUP) ;
	pinMode(ATOMIZER_PIN,   OUTPUT) ;

	Serial.println("DHTxx test!") ;
	dht.begin();

	display_oled_welcome() ;
	
	encoder.pressed = (e_encoder_switch_state)digitalRead(ENCODER_A_PIN) ;
	// TODO: Attach an interrupt for encoder button
	attachInterrupt(digitalPinToInterrupt(ENCODER_SW_PIN), ISR_encoder, RISING) ;
}

void loop() {
	/* // Flags */
	bool humidifier_fired          = false ;
	/* bool humidity_set_point_config = false ; */
	
	// Reading temperature or humidity takes about 250 milliseconds!
	// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
	float humidity = dht.readHumidity() ;

	// Read temperature as Fahrenheit (isFahrenheit = true)
	float temp_f = dht.readTemperature(true) ;

	// Check if any reads failed and exit early (to try again).
	if (!verify_dht_read(humidity,  temp_f)) {
		return ;
	}

	/* encoder.a_current_position = digitalRead(ENCODER_A_PIN) ; */
#ifdef later
	// TODO: make interrupt to set encoder flag
	// The interrupt will increment an encoder counter and set the flag.
	// the screen will then set to change the humidity set point
	// If the user presses the encoder again, the interrupt will increment
	// the counter, and if it is even, it resets the counter to zero. This
	// will prevent overflow for unsigned ints
	// When the number of times the button is pressed is even, the screen will
	// return to normal.
	// If the user does not do anything within 5 seconds, a timer will expire and
	// return the user to the normal screen.
#endif /* later */
	// if encoder pressed or 5 second timer goes off
	/* if (encoder.pressed == ENCODER_SWITCH_ON) { */

	/* while (encoder.pressed == ENCODER_SWITCH_ON) { */
		/* if (!encoder.press_count % 2) { */
		/* 	break ; */
		/* } */
		/* Serial.println("Encoder pressed") ; */
		/* Serial.print("Pressed ") ; */
		/* Serial.print(encoder.press_count) ; */
		/* Serial.println(" times") ; */
		encoder.a_current_position = digitalRead(ENCODER_A_PIN) ;
		if (encoder.a_previous_position != encoder.a_current_position) {
			if (digitalRead(ENCODER_B_PIN) != encoder.a_previous_position) {
				humidity_set_point++ ;
			}
			else {
				humidity_set_point-- ;
			}
		}
		setup_oled() ;
		// TODO: Will these variables ONLY stay in this scope?
		display_oled(humidity, temp_f, humidity_set_point) ;
		encoder.a_previous_position = encoder.a_current_position ;

	/* } */

/* 	display_oled(humidity, temp_f, humidity_set_point) ; */

/* 	// TODO: Case statements. If humidity is less than 50, do 10s. If 60, 5s. 70 = 3s */
	if (humidity < humidity_set_point) {
		humidifier_fired = true ;
		digitalWrite(ATOMIZER_PIN, HIGH) ;
		humidity = digitalRead(ATOMIZER_PIN) ;
		/* delay(TIME_BETWEEN_MEASURE) ; */
		digitalWrite(ATOMIZER_PIN, LOW) ;
	}

	if (!humidifier_fired) {
		/* delay(TIME_BETWEEN_MEASURE); */
	}
	humidifier_fired = false ;
}


/*-----------------------------------------------------------------------------
 * Functions 
 *-----------------------------------------------------------------------------*/

/* ----------------------------------------------------------------------------*/
/**
 * @brief			Display humidity (%), temperature (F), and humidity set point (%)
 * @param humidity		Current humidity locally
 * @param temp			Current temperature locally (in Fahrenheit)
 * @param humidity_set_point	Minimum humidity level before atomizer turns on
 */
/* ------------------------------------------------------------------------------------*/
void display_oled(float humidity, float temp, float humidity_set_point) {
	display.print("H: ") ;
	display.print(humidity) ;
	display.println("%") ;

	display.print("T: ") ;

	display.print(temp);
	display.println("F\n") ;

	display.print("S: ") ;

	display.print(humidity_set_point) ;
	display.println("%") ;

	display.display() ;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief			Displays a welcome screen on the OLED on power up or reboot
 */
/* ------------------------------------------------------------------------------------*/
void display_oled_welcome(void) {
	Serial.println("SSD1306 test!") ;
	display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR) ;
	display.clearDisplay() ;
	
	display.setTextSize(3) ;
	display.setTextColor(WHITE) ;
	display.setCursor(0,0) ;
	display.println("DHT22\nClock") ;

	display.display() ;
	
	delay(DISPLAY_WELCOME_TIME) ;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief			Sets up OLED text size, position, and color
 */
/* ------------------------------------------------------------------------------------*/
void setup_oled(void) {
	display.clearDisplay() ;
	display.setTextSize(2) ;
	display.setTextColor(WHITE) ;
	display.setCursor(0,0) ;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief			Verify if the DHT sensor correctly took a reading
 * @param humidity		Current humidity locally
 * @param temp			Current temperature locally (in Fahrenheit)
 * @param humidity_set_point	Minimum humidity level before atomizer turns on
 *
 * @return bool			True if all values read correctly, false if any failed to get read
 */
/* ------------------------------------------------------------------------------------*/
bool verify_dht_read(float humidity, float temp) {
	if (isnan(humidity) ||  isnan(temp)) {
		Serial.println("Failed to read from DHT sensor!") ;
		delay(TIME_BETWEEN_FAILED_MEASURE) ;
		return false ;
	}
	return true ;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief			ISR for encoder button press
 */
/* ------------------------------------------------------------------------------------*/
void IRAM_ATTR ISR_encoder(void) {
	encoder.press_count++ ;
	encoder.pressed = ENCODER_SWITCH_ON ;
}
