/*
 * =====================================================================================
 *
 *       Filename:  dhtxx.hpp
 *
 *    Description:  DHTxx sensor header file 
 *
 *        Version:  1.0
 *        Created:  01/07/2023 06:53:50 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Michael Ly
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef DHTXX_HPP
#define DHTXX_HPP
#include "main.hpp"

#include "DHT.h"

bool verify_dht_read(float humidity, float temp) ;

#endif /* DHTXX_HPP */
