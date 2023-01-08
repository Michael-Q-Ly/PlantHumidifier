/*
 * =====================================================================================
 *
 *       Filename:  encoder.hpp
 *
 *    Description:  Rotary encoder header file 
 *
 *        Version:  1.0
 *        Created:  01/07/2023 06:59:24 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Michael Ly
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef ENCODER_HPP
#define ENCODER_HPP
#include "main.hpp"

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

/* void reset_humidity_set_point(float &humidity_set_point) ; */
/* void update_humidity_set_point(bool &turned_CW, bool &last_was_CW, bool &turned_CCW, bool &last_was_CCW, float &humidity_set_point) ; */

#endif /* ENCODER_HPP */
