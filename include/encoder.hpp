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

struct s_encoder {
	uint8_t press_count ;
	uint8_t a_current_position ;
	uint8_t a_previous_position ;
} ;

#endif /* ENCODER_HPP */
