/*
 * CLUApp11.h
 *
 *  Created on: 13/02/2013
 *      Author: Atridas
 */

#ifndef LUAPP11_LUA_STATE_HPP_
#define LUAPP11_LUA_STATE_HPP_

#include <string>
#include <map>
#include <set>
#include <typeindex>
#include <memory>

#include "luapp11.hpp"

struct lua_State;

namespace luapp11 {

class CBaseClass;

class CLuaState {
public:

	typedef std::type_index TClassName;
	typedef std::map<TClassName, std::shared_ptr<CBaseClass>> TClassNamesMap;

	CLuaState();
	CLuaState(CLuaState&&);
	virtual ~CLuaState();

	CLuaState(const CLuaState&) = delete;
	CLuaState& operator=(const CLuaState&) = delete;

	void Execute(const std::string& _script);

	lua_State* GetLuaState() const { return m_LuaState; };

	template<class T>
	void RegisterFunction(
				const std::string& luaFunctionName,
				T* function);

	std::shared_ptr<CBaseClass> GetClass(TClassName _ClassName) const;

	void RegisterClass(std::shared_ptr<CBaseClass>);

	static CLuaState* GetLuaState(lua_State*);

private:
	lua_State* m_LuaState;

	TClassNamesMap m_ClassMap;
};


template<class T>
void CLuaState::RegisterFunction(
			const std::string& luaFunctionName,
			T* function) {
	static_assert(std::is_function<T>::value, "Function type must be a function pointer!");
	RegisterCppFunction(*this, luaFunctionName, function);
}

};
#endif /* CLUAPP11_H_ */
