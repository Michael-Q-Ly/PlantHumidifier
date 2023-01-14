#include "main.hpp"

#include "atomizer.hpp"
#include "dhtxx.hpp"
#include "encoder.hpp"
#include "oled.hpp"

// Uncomment whatever type you're using!
//#define DHT_TYPE DHT11   // DHT 11
#define DHT_TYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHT_TYPE DHT21   // DHT 21 (AM2301)

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
DHT dht(DHT_PIN, DHT_TYPE);
#define DEFAULT_HUMIDITY_SET_POINT	60
#define DEBOUNCE_ENCODER		50
#define DEBOUNCE_ATOMIZER		(1 * uS_TO_S)

extern Adafruit_SSD1306 display ;
float humidity_set_point    = DEFAULT_HUMIDITY_SET_POINT ;

volatile bool turned_CW      = false ;
volatile bool turned_CCW     = false ;
volatile bool last_was_CW    = false ;
volatile bool last_was_CCW   = false ;
uint32_t debounce_time       = 0 ;
uint32_t debounce_humidifier = 0 ;

void IRAM_ATTR ISR_check_encoder(void) ;
void reset_humidity_set_point(float humidity_set_point) ;
void update_humidity_set_point(void) ;

void setup() {
	pinMode(ENCODER_A_PIN,  INPUT_PULLUP) ;
	pinMode(ENCODER_B_PIN,  INPUT_PULLUP) ;
	pinMode(ATOMIZER_PIN,   OUTPUT) ;

	dht.begin();

	init_oled() ;
	/* display_oled_welcome() ; */
	
	attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN), ISR_check_encoder, RISING) ;
}

void loop() {
	// Flags
	// TODO: Get measurements every 10 seconds?
	/* uint32_t measurement_time; */
	/* measurement_time = millis() ; */
	
	// Reading temperature or humidity takes about 250 milliseconds!
	// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
	float humidity = dht.readHumidity() ;

	// Read temperature as Fahrenheit (isFahrenheit = true)
	float temp_f = dht.readTemperature(true) ;

	// Check if any reads failed and exit early (to try again).
	if (!verify_dht_read(humidity, temp_f)) {
		return ;
	}

	// Allow user to control humidity set point with encoder
	update_humidity_set_point() ;

	// If humidity goes beyond 0% or 100%, keep it at these upper and lower boundaries
	if (humidity_set_point     >= HUMIDITY_UPPER_THRESHOLD) {
		humidity_set_point  = HUMIDITY_UPPER_THRESHOLD ;
	}
	if (humidity_set_point     <= HUMIDITY_LOWER_THRESHOLD) {
		humidity_set_point  = HUMIDITY_LOWER_THRESHOLD ;
	}

	// Clear and set up OLED to display DHT data and set point
	setup_oled() ;
	display_oled(humidity, temp_f, humidity_set_point) ;

	// Turn on atomizer if humidity is below set point
	if (humidity < humidity_set_point) {
		digitalWrite(ATOMIZER_PIN, ATOMIZER_ON) ;
		debounce_humidifier = millis() ;
	}
	// Allow the atomizer to turn off
	if (millis() - debounce_humidifier > DEBOUNCE_ATOMIZER) {
		digitalWrite(ATOMIZER_PIN, ATOMIZER_OFF) ;
	}
}


/*-----------------------------------------------------------------------------
 * Functions 
 *-----------------------------------------------------------------------------*/

/* ----------------------------------------------------------------------------*/
/**
 * @fn				ISR_check_encoder
 *
 * @brief			ISR for encoder button press
 */
/* ------------------------------------------------------------------------------------*/
void IRAM_ATTR ISR_check_encoder(void) {
	if ( (!turned_CW) && (!turned_CCW)) {
		int pin_a = digitalRead(ENCODER_A_PIN) ;
		delayMicroseconds(1500) ;
		int pin_b = digitalRead(ENCODER_B_PIN) ;

		if (pin_a == pin_b) {
			if (last_was_CW) {
				turned_CW = true ;
			}
			else {
				turned_CCW = true ;
			}
		}
		else {
			if (last_was_CCW) {
				turned_CCW = true ;
			}
			else {
				turned_CW = true ;
			}
		}
	}
}

/* ----------------------------------------------------------------------------*/
/**
 * @fn				update_humidity_set_point
 *
 * @brief			Update the humidity threshold up or down based off a clockwise
 *				or counter-clockwise movement of the encoder
 *
 * @param turned_CW		Was encoder turned clockwise just now?
 * @param last_was_CW		Was encoder turned clockwise before this?
 * @param turned_CCW		Was encoder turned counter-clockwise just now?
 * @param last_was_CCW		was encoder turned counter-clockwise before this?
 * @param humidity_set_point float	Minimum humidity level before atomizer turns on
 */
/* ------------------------------------------------------------------------------------*/
void update_humidity_set_point(void) {
	if (turned_CW) {
		humidity_set_point++ ;
		turned_CW     = false ;
		last_was_CW   = true ;
		debounce_time = millis() ;
	}

	if (turned_CCW) {
		humidity_set_point-- ;
		turned_CCW    = false ;
		last_was_CCW  = true ;
		debounce_time = millis() ;
	}

	if (millis() - debounce_time > DEBOUNCE_ENCODER) {
		last_was_CW  = false ;
		last_was_CCW = false ;
	}
}
