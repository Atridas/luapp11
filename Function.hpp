/*
 * CLUApp11.h
 *
 *  Created on: 13/02/2013
 *      Author: Atridas
 */

#ifndef LUAPP11_FUNCTION_HPP_
#define LUAPP11_FUNCTION_HPP_

#include <functional>
#include <iostream>

namespace luapp11 {

class CBaseFunction {
public:
	virtual int CallFromLua(lua_State* _pLuaState) = 0;

	virtual ~CBaseFunction() {};
};

template<class ReturnType, class... Args>
class CFunction: public CBaseFunction {
public:
	typedef std::function<ReturnType(void)> TReturnFunction;
	typedef std::function<ReturnType(Args...)> TFunction;


	inline static void RegisterFunction(
				const CLuaState& _LuaState,
				const std::string& luaFunctionName,
				TFunction function);

private:

	const TFunction m_Function;

	static const int s_iNumParams = sizeof...(Args) + 1;

	CFunction(const TFunction& _Function):m_Function(_Function) {};

	int CallFromLua(lua_State* _pLuaState);
};


template<class ReturnType, class... Args>
void RegisterCppFunction(
			const CLuaState& _LuaState,
			const std::string& luaFunctionName,
			ReturnType(*function)(Args...)) {
	CFunction<ReturnType, Args...>::RegisterFunction(_LuaState, luaFunctionName, function);
}

};

#include "Function.inl"

#endif
