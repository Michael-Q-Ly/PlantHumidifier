/*
 * =====================================================================================
 *
 *       Filename:  oled.hpp
 *
 *    Description:  OLED header file 
 *
 *        Version:  1.0
 *        Created:  01/07/2023 06:40:04 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Michael Ly
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef OLED_HPP
#define OLED_HPP
#include "main.hpp"

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

void setup_oled(void) ;
void display_oled_welcome(void) ;
void display_oled(float humidity, float temp, float humidity_set_point) ;

#endif /* OLED_HPP */
