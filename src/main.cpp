#include "main.hpp"
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
#define DEBOUNCE_TO			50

extern Adafruit_SSD1306 display ;
float humidity_set_point    = DEFAULT_HUMIDITY_SET_POINT ;
s_encoder encoder {0, ENCODER_SWITCH_OFF} ;

volatile bool turned_CW    = false ;
volatile bool turned_CCW   = false ;
volatile bool last_was_CW  = false ;
volatile bool last_was_CCW = false ;
uint32_t debounce_time     = 0 ;

IRAM_ATTR void ISR_check_encoder(void) ;

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
	attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN), ISR_check_encoder, RISING) ;
}

void loop() {
	// Flags
	bool humidifier_fired = false ;
	/* bool humidity_set_point_config = false ; */
	/* uint32_t measurement_time = millis() ; */
	
	// Reading temperature or humidity takes about 250 milliseconds!
	// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
	float humidity = dht.readHumidity() ;

	// Read temperature as Fahrenheit (isFahrenheit = true)
	float temp_f = dht.readTemperature(true) ;

	// Check if any reads failed and exit early (to try again).
	if (!verify_dht_read(humidity, temp_f)) {
		return ;
	}

	// If encoder button is pressed, reset to default humidity set point
	if (digitalRead(ENCODER_SW_PIN) == ENCODER_SWITCH_ON) {
		humidity_set_point = DEFAULT_HUMIDITY_SET_POINT ;
		/* setup_oled() ; */
		/* display_oled(humidity, temp_f, humidity_set_point) ; */
	}

	if (turned_CW) {
		humidity_set_point++ ;
		turned_CW     = false ;
		last_was_CW   = true ;
		debounce_time = millis() ;
		/* setup_oled() ; */
		/* display_oled(humidity, temp_f, humidity_set_point) ; */
	}

	if (turned_CCW) {
		humidity_set_point-- ;
		turned_CCW    = false ;
		last_was_CCW  = true ;
		debounce_time = millis() ;
		/* setup_oled() ; */
		/* display_oled(humidity, temp_f, humidity_set_point) ; */
	}

	if (humidity_set_point >= 100) {
		humidity_set_point = 100 ;
	}

	if (humidity_set_point <= 0) {
		humidity_set_point = 0 ;
	}

	if (millis() - debounce_time > DEBOUNCE_TO) {
		last_was_CW  = false ;
		last_was_CCW = false ;
	}

	setup_oled() ;
	display_oled(humidity, temp_f, humidity_set_point) ;

	// TODO: Case statements. If humidity is less than 50, do 10s. If 60, 5s. 70 = 3s
	// TODO: Use non-blocking code like an ISR or millis to display new measurements
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
