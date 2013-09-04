/*
 * CLUApp11.cpp
 *
 *  Created on: 13/02/2013
 *      Author: Atridas
 */

#include "luapp11.hpp"

#include <iostream>

#define LUA_STATE_OBJECT_LUA_GLOBAL_IDENTIFIER "luapp11_luaStateObject"

namespace luapp11 {

CLuaState::CLuaState() {
	m_LuaState = luaL_newstate();

	luaopen_base(m_LuaState);
	luaopen_string(m_LuaState);
	luaopen_table(m_LuaState);

	CLuaState** l_ppThis = (CLuaState**)lua_newuserdata(m_LuaState, sizeof(CLuaState*));

	*l_ppThis = this;

	lua_setglobal(m_LuaState, LUA_STATE_OBJECT_LUA_GLOBAL_IDENTIFIER);
}

CLuaState::~CLuaState() {
	if(m_LuaState) {
		lua_close(m_LuaState);
		m_LuaState = nullptr;
	}
}

CLuaState::CLuaState(CLuaState&& _original) {
	m_LuaState = _original.m_LuaState;
	_original.m_LuaState = nullptr;
}

void CLuaState::Execute(const std::string& _script) {
	int result = luaL_dostring(m_LuaState, _script.c_str());
	//TODO
	switch(result) {
	case LUA_OK:
		break;
	case LUA_YIELD:
		std::cout << "LUA_YIELD" << std::endl;
		if(lua_isstring(m_LuaState, -1)) {
			const char* str = lua_tostring(m_LuaState, -1);
			std::cout << str << std::endl;
		}
		break;
	case LUA_ERRRUN:
		std::cout << "LUA_ERRRUN" << std::endl;
		break;
	case LUA_ERRSYNTAX:
		std::cout << "LUA_ERRSYNTAX" << std::endl;
		break;
	case LUA_ERRMEM:
		std::cout << "LUA_ERRMEM" << std::endl;
		break;
	case LUA_ERRGCMM:
		std::cout << "LUA_ERRGCMM" << std::endl;
		break;
	case LUA_ERRERR:
		std::cout << "LUA_ERRERR" << std::endl;
		break;
	default:
		std::cout << "unknown LUA ERROR " << result << std::endl;
		break;
	}
}


std::shared_ptr<CBaseClass> CLuaState::GetClass(TClassName _ClassName) const {
	auto l_it = m_ClassMap.find(_ClassName);
	if(l_it == m_ClassMap.cend()) {
		return std::shared_ptr<CBaseClass>();
	} else {
		return l_it->second;
	}
}

void CLuaState::RegisterClass(std::shared_ptr<CBaseClass> _Class) {
	m_ClassMap[_Class->GetClassName()] = _Class;
}

CLuaState* CLuaState::GetLuaState(lua_State* _pLuaState) {
	lua_getglobal(_pLuaState, LUA_STATE_OBJECT_LUA_GLOBAL_IDENTIFIER);

	void* l_UserData = lua_touserdata(_pLuaState, -1);
	CLuaState** l_ppLuaStateObject = reinterpret_cast<CLuaState**>(l_UserData);

	lua_pop(_pLuaState, 1);

	return *l_ppLuaStateObject;
}

};
