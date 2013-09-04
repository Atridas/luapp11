
#ifndef LUAPP11_MAIN_HPP_
#define LUAPP11_MAIN_HPP_

#include <string>

namespace luapp11 {

	typedef void(FOnFailAssert)(const std::string& _assertMsg);

	extern FOnFailAssert* s_OnFailAssert;

	//TODO _DEBUG
	inline void assert(bool _condition, const std::string& _assertMsg) {
		if(!_condition) {
			s_OnFailAssert(_assertMsg);
		}
	}
};

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "LuaState.hpp"
#include "Function.hpp"
#include "Object.hpp"
#include "Class.hpp"

#endif
