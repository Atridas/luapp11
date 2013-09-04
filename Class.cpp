/*
 * Class.cpp
 *
 *  Created on: 24/02/2013
 *      Author: Atridas
 */


#include "luapp11.hpp"

namespace luapp11 {

std::set<std::string> InitSet(const std::string& _sName) {
	std::set<std::string> set;
	if(_sName != "") {
		set.insert(_sName);
	}
	return set;
}

std::set<std::string>& AppendToSet(std::set<std::string>& _Set, const std::string& _sName) {
	if(_sName != "") {
		_Set.insert(_sName);
	}
	return _Set;
}

int luapp11_gc_cpp_class_and_object(lua_State* _pLuaState) {
	int params = lua_gettop(_pLuaState);
	assert(params == 1, "parameters should be 1");
	if(params == 1) {
		SClassAndObject* l_pCAO = (SClassAndObject*)luaL_checkudata(_pLuaState, 1, "luapp11_CClassAndObject");
		if(l_pCAO != nullptr) {
			//l_pCAO->m_pClass;
			delete l_pCAO->m_pObject;
		}
	}
	return 0;
}

};
