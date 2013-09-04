/*
 * LUApp11.cpp
 *
 *  Created on: 15/02/2013
 *      Author: Atridas
 */

#include "luapp11.hpp"

#include <iostream>
#include <string>

namespace luapp11 {

static void printAssert(const std::string& _assertMsg) {
	std::cerr << "Assert Failed:" << std::endl;
	std::cerr << _assertMsg << std::endl;
	// TODO breakpoint
}

FOnFailAssert* s_OnFailAssert = printAssert;

}
