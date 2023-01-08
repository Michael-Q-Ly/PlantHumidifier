/*
 * =====================================================================================
 *
 *       Filename:  dhtxx.cpp
 *
 *    Description:  DHTxx sensor function definitions 
 *
 *        Version:  1.0
 *        Created:  01/07/2023 06:56:13 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Michael Ly
 *   Organization:  
 *
 * =====================================================================================
 */
#include "dhtxx.hpp"


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
