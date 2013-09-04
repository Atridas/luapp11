/*
 * Function.cpp
 *
 *  Created on: 15/02/2013
 *      Author: Atridas
 */

#include "luapp11.hpp"

namespace luapp11 {

int luapp11_gc_function(lua_State* _pLuaState) {
	int params = lua_gettop(_pLuaState);
	assert(params == 1, "parameters should be 1");
	if(params == 1) {
		void* l_UserData = lua_touserdata(_pLuaState, 1);
		CBaseFunction** l_AsFunction = reinterpret_cast<CBaseFunction**>(l_UserData);

		delete *l_AsFunction;
	} else {
		// TODO error!
	}
	return 0;
}

int luapp11_function(lua_State* _pLuaState) {
	int params = lua_gettop(_pLuaState);
	assert(params > 0, "parameters should be at least 1");
	if(params > 0) {
		if(lua_isuserdata(_pLuaState, 1)) {
			void* l_UserData = lua_touserdata(_pLuaState, 1);
			CBaseFunction** l_AsFunction = reinterpret_cast<CBaseFunction**>(l_UserData);
			return (*l_AsFunction)->CallFromLua(_pLuaState);
		}
	}

	return 0;
}

}
