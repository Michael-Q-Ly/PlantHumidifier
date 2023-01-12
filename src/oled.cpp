/*
 * =====================================================================================
 *
 *       Filename:  oled.cpp
 *
 *    Description   OLED function definitions
 *
 *        Version:  1.0
 *        Created:  01/07/2023 06:45:24 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Michael Ly
 *   Organization:  
 *
 * =====================================================================================
 */
#include "oled.hpp"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT) ;

/* ----------------------------------------------------------------------------*/
/**
 * @brief			Initializes OLED address and clears display
 */
/* ------------------------------------------------------------------------------------*/
void init_oled(void) {
	display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR) ;
	display.clearDisplay() ;
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
 * @brief			Displays a welcome screen on the OLED on power up or reboot
 */
/* ------------------------------------------------------------------------------------*/
void display_oled_welcome(void) {
	display.setTextSize(3) ;
	display.setTextColor(WHITE) ;
	display.setCursor(0,0) ;
	display.println("DHT22\nClock") ;

#ifdef later
	display.setTextSize(1) ;
	display.setTextColor(WHITE) ;
	display.setCursor(0,0) ;
	display.println("Hi Alyssa,\n") ;
	display.println("Here's your make") ;
	display.println("plant happy machine.\n") ;
	display.println("Thanks for the") ;
	display.println("pothos,") ;
	display.println("-Michael Ly") ;
#endif

	display.display() ;
	
	delay(DISPLAY_WELCOME_TIME) ;
}

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
