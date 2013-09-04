/*
 * FunctionHelper.h
 *
 *  Created on: 16/02/2013
 *      Author: Atridas
 */

#ifndef LUAPP11_FUNCTION_INL_
#define LUAPP11_FUNCTION_INL_

#include <functional>
#include <memory>

namespace luapp11 {
/****************************************************************************************
 ***********************                 Final Caller              **********************
 ****************************************************************************************/

template<class ReturnType>
class CFinalCaller;

template<>
class CFinalCaller<void> {
public:
	static int MakeFinalCall(lua_State* _pLuaState,
			const std::function<void(void)>& _f) {
		_f();
		return 0;
	}
};

template<>
class CFinalCaller<int> {
public:
	static int MakeFinalCall(lua_State* _pLuaState,
			const std::function<int(void)>& _f) {
		int ret = _f();

		lua_pushnumber(_pLuaState, ret);

		return 1;
	}
};

template<>
class CFinalCaller<float> {
public:
	static int MakeFinalCall(lua_State* _pLuaState,
			const std::function<float(void)>& _f) {
		float ret = _f();

		lua_pushnumber(_pLuaState, ret);

		return 1;
	}
};

template<>
class CFinalCaller<double> {
public:
	static int MakeFinalCall(lua_State* _pLuaState,
			const std::function<double(void)>& _f) {
		double ret = _f();

		lua_pushnumber(_pLuaState, ret);

		return 1;
	}
};

template<>
class CFinalCaller<std::string> {
public:
	static int MakeFinalCall(lua_State* _pLuaState,
			const std::function<std::string(void)>& _f) {
		std::string ret = _f();

		lua_pushstring(_pLuaState, ret.c_str());

		return 1;
	}
};

template<>
class CFinalCaller<const char*> {
public:
	static int MakeFinalCall(lua_State* _pLuaState,
			const std::function<const char*(void)>& _f) {
		const char* ret = _f();

		lua_pushstring(_pLuaState, ret);

		return 1;
	}
};


template<class Class> void PushPointer(lua_State* _pLuaState, Class* _pObject);
template<class Class> void PushPointer(lua_State* _pLuaState, std::shared_ptr<Class> _pObject);
template<class Class> void PushPointer(lua_State* _pLuaState, std::unique_ptr<Class> _pObject);

template<class T>
class CFinalCaller<T*> {
public:
	static int MakeFinalCall(lua_State* _pLuaState,
			const std::function<T*(void)>& _f) {
		T* ret = _f();

		PushPointer(_pLuaState, ret);

		return 1;
	}
};

template<class T>
class CFinalCaller<std::shared_ptr<T>> {
public:
	static int MakeFinalCall(lua_State* _pLuaState,
			const std::function<std::shared_ptr<T>(void)>& _f) {
		std::shared_ptr<T> ret = _f();

		PushPointer(_pLuaState, ret);

		return 1;
	}
};

template<class T>
class CFinalCaller<std::unique_ptr<T>> {
public:
	static int MakeFinalCall(lua_State* _pLuaState,
			const std::function<std::unique_ptr<T>(void)>& _f) {
		//std::unique_ptr<T> ret = _f();

		//PushPointer(_pLuaState, std::move(ret));
		PushPointer(_pLuaState,  _f());

		return 1;
	}
};

/****************************************************************************************
 ***********************                 Arguments                 **********************
 ****************************************************************************************/

template<class ArgumentType>
class CArgumentParserTop {
public:

};

template<>
class CArgumentParserTop<int> {
public:

	static int ParseArg(lua_State* _pLuaState, int _iStackPosition) {
		if (lua_isnumber(_pLuaState, _iStackPosition)) {
			int r = (int) lua_tonumber(_pLuaState, _iStackPosition);
			return r;
		} else {
			return 0;
		}
	}
};

template<>
class CArgumentParserTop<float> {
public:

	static int ParseArg(lua_State* _pLuaState, int _iStackPosition) {
		if (lua_isnumber(_pLuaState, _iStackPosition)) {
			float r = (float) lua_tonumber(_pLuaState, _iStackPosition);
			return r;
		} else {
			return 0;
		}
	}
};

template<>
class CArgumentParserTop<double> {
public:

	static int ParseArg(lua_State* _pLuaState, int _iStackPosition) {
		if (lua_isnumber(_pLuaState, _iStackPosition)) {
			double r = (double) lua_tonumber(_pLuaState, _iStackPosition);
			return r;
		} else {
			return 0;
		}
	}
};

template<>
class CArgumentParserTop<const std::string&> {
public:

	static std::string ParseArg(lua_State* _pLuaState, int _iStackPosition) {
		if (lua_isstring(_pLuaState, _iStackPosition)) {
			const char* c = lua_tostring(_pLuaState, _iStackPosition);
			return std::string(c);
		} else {
			return std::string("");
		}
	}
};

template<>
class CArgumentParserTop<std::string> {
public:

	static std::string ParseArg(lua_State* _pLuaState, int _iStackPosition) {
		if (lua_isstring(_pLuaState, _iStackPosition)) {
			const char* c = lua_tostring(_pLuaState, _iStackPosition);
			return std::string(c);
		} else {
			return std::string("");
		}
	}
};

template<>
class CArgumentParserTop<const char*> {
public:

	static const char* ParseArg(lua_State* _pLuaState, int _iStackPosition) {
		if (lua_isstring(_pLuaState, _iStackPosition)) {
			const char* c = lua_tostring(_pLuaState, _iStackPosition);
			return c;
		} else {
			return "";
		}
	}
};

template<class Class> Class* GetNakedPointer(lua_State* _pLuaState, int _iStackIndex);
template<class Class> std::shared_ptr<Class> GetSharedPointer(lua_State* _pLuaState, int _iStackIndex);

template<class T>
class CArgumentParserTop<T*> {
public:

	static T* ParseArg(lua_State* _pLuaState, int _iStackPosition) {
		return GetNakedPointer<T>(_pLuaState, _iStackPosition);
	}
};

template<class T>
class CArgumentParserTop<std::shared_ptr<T>> {
public:

	static std::shared_ptr<T> ParseArg(lua_State* _pLuaState, int _iStackPosition) {
		return GetSharedPointer<T>(_pLuaState, _iStackPosition);
	}
};

/****************************************************************************************
 ***********************                Pasing                     **********************
 ****************************************************************************************/

template<class ReturnType, class ...Args>
class CArgumentParser;

template<class ReturnType>
class CArgumentParser<ReturnType> {
public:
	static std::function<ReturnType(void)> ParseArgs(lua_State* _pLuaState,
			const std::function<ReturnType(void)>& _function) {
		return _function;
	}

protected:

	template<class FunctionType, class ArgFirst, class... ArgOthers>
	static std::function<ReturnType(void)> ParseArgsInternal(lua_State* _pLuaState,
				const FunctionType& _function, ArgFirst _argFirst,
				ArgOthers... _argOthers) {
		return std::bind(
				_function,
				_argFirst,
				_argOthers...);
	}
};

template<class ReturnType, class First, class ... Others>
class CArgumentParser<ReturnType, First, Others...>
	: private CArgumentParser<ReturnType, Others...>
{
public:
	static std::function<ReturnType(void)> ParseArgs(lua_State* _pLuaState,
			const std::function<ReturnType(First, Others...)>& _function)
	{
		int stackPos = sizeof...(Others) + 1;
		//using FunctionType = std::function<ReturnType(First, Others...)>;
		return CArgumentParser<ReturnType, Others...>::ParseArgsInternal
			(
					_pLuaState,
					_function,
					CArgumentParserTop<First>::ParseArg(_pLuaState,-stackPos)
			);
	}

protected:

	template<class FunctionType, class ArgFirst, class... ArgOthers>
	static std::function<ReturnType(void)> ParseArgsInternal(lua_State* _pLuaState,
				const FunctionType& _function, ArgFirst _argFirst,
				ArgOthers... _argOthers) {
		int stackPos = sizeof...(Others) + 1;
		return CArgumentParser<ReturnType, Others...>::ParseArgsInternal(
				_pLuaState,
				_function,
				_argFirst,
				_argOthers...,
				CArgumentParserTop<First>::ParseArg(_pLuaState,-stackPos));
	}
};


/****************************************************************************************
 ********************                CFunction functions              *******************
 ****************************************************************************************/

template<class ReturnType, class... Args>
int CFunction<ReturnType, Args...>::CallFromLua(lua_State* _pLuaState) {
	int params = lua_gettop(_pLuaState);
	if(params == s_iNumParams) {
		TReturnFunction returnF = CArgumentParser<ReturnType, Args...>::ParseArgs(_pLuaState, m_Function);


		return CFinalCaller<ReturnType>::MakeFinalCall(_pLuaState, returnF);
	}
	return 0;
}

int luapp11_function(lua_State* _pLuaState);
int luapp11_gc_function(lua_State* _pLuaState);

template<class ReturnType, class... Args>
void CFunction<ReturnType, Args...>::RegisterFunction(
			const CLuaState& _LuaState,
			const std::string& luaFunctionName,
			TFunction function) {

	lua_State* l_pLuaState = _LuaState.GetLuaState();

	if(luaL_newmetatable(l_pLuaState, "luapp11_CFunction")) {
		lua_pushstring(l_pLuaState, "__call");
		lua_pushcfunction(l_pLuaState, luapp11_function);
		lua_settable(l_pLuaState, -3);


		lua_pushstring(l_pLuaState, "__gc");
		lua_pushcfunction(l_pLuaState, luapp11_gc_function);
		lua_settable(l_pLuaState, -3);


		lua_pop(l_pLuaState, 1);
	}

	CFunction<ReturnType, Args...>* l_pFun = new CFunction<ReturnType, Args...>(function);

	CBaseFunction** l_ppFun = (CBaseFunction**)lua_newuserdata(l_pLuaState, sizeof(CBaseFunction*));

	*l_ppFun = l_pFun;

	luaL_setmetatable(l_pLuaState, "luapp11_CFunction");

	lua_setglobal(l_pLuaState, luaFunctionName.c_str());
}


}

#endif /* LUAPP11_FUNCTION_HELPER_H_ */
