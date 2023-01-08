/*
 * =====================================================================================
 *
 *       Filename:  main.hpp
 *
 *    Description:  Main header file 
 *
 *        Version:  1.0
 *        Created:  01/07/2023 06:40:18 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Michael Ly
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef MAIN_HPP
#define MAIN_HPP

#include <Arduino.h>

#define BAUD_RATE			115200

#define uS_TO_S				1000

#define TIME_BETWEEN_MEASURE		(1 * uS_TO_S)
#define TIME_BETWEEN_FAILED_MEASURE	(2 * uS_TO_S)
#define DISPLAY_WELCOME_TIME		(5 * uS_TO_S)


/*-----------------------------------------------------------------------------
 * PINS 
 *-----------------------------------------------------------------------------*/

#define ATOMIZER_PIN		15
#define DHT_PIN			 2
#define ENCODER_SW_PIN		16
#define ENCODER_A_PIN		14
#define ENCODER_B_PIN		12

#endif /* MAIN_HPP */
