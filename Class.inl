/*
 * Class.inl
 *
 *  Created on: 24/02/2013
 *      Author: Atridas
 */

#ifndef LUAPP11_CLASS_INL_
#define LUAPP11_CLASS_INL_

#include <typeindex>

namespace luapp11 {

// --------------

template<class Class, class FirstBase, class...OtherBases>
CClass<Class, FirstBase, OtherBases...>::CClass(CLuaState& _luaState)
:CClass<Class, OtherBases...>(_luaState)
{
	static_assert(std::is_convertible<Class*, FirstBase*>::value, "The first template parameter must be convertible to the others!");
};

template<class Class>
CClass<Class>::CClass(CLuaState& _luaState)
{
};


template<class Class, class FistBase, class...OtherBases>
std::shared_ptr<CBaseClass> CClass<Class, FistBase, OtherBases...>::RegisterClass(CLuaState& _luaState)
{
	std::shared_ptr<CBaseClass> l_pClass = _luaState.GetClass(std::type_index(typeid(Class)));

	if(l_pClass != nullptr) {
		return l_pClass;
	}

	l_pClass = std::make_shared<CClass<Class, FistBase, OtherBases...>>(_luaState);
	_luaState.RegisterClass(l_pClass);
	return l_pClass;
}

template<class Class>
std::shared_ptr<CBaseClass> CClass<Class>::RegisterClass(CLuaState& _luaState)
{
	std::shared_ptr<CBaseClass> l_pClass = _luaState.GetClass(std::type_index(typeid(Class)));

	if(l_pClass != nullptr) {
		return l_pClass;
	}

	l_pClass = std::make_shared<CClass<Class>>(_luaState);
	_luaState.RegisterClass(l_pClass);
	return l_pClass;
}

template<class Class>
CBaseObject* CClass<Class>::GetAsObject(Class* _pObject)
{
	return new CObjectAsNakedPointer<Class>(_pObject);
}

template<class Class, class FistBase, class...OtherBases>
CBaseObject* CClass<Class, FistBase, OtherBases...>::GetAsObject(Class* _pObject)
{
	return new CObjectAsNakedPointer<Class, FistBase, OtherBases...>(_pObject);
}

template<class Class>
CBaseObject* CClass<Class>::GetAsObject(std::shared_ptr<Class> _pObject)
{
	return new CObjectAsSharedPointer<Class>(_pObject);
}

template<class Class>
CBaseObject* CClass<Class>::GetAsObject(std::unique_ptr<Class>&& _pObject)
{
	return new CObjectAsSharedPointer<Class>(std::move(_pObject));
}

template<class Class, class FistBase, class...OtherBases>
CBaseObject* CClass<Class, FistBase, OtherBases...>::GetAsObject(std::shared_ptr<Class> _pObject)
{
	return new CObjectAsSharedPointer<Class, FistBase, OtherBases...>(_pObject);
}

template<class Class, class FistBase, class...OtherBases>
CBaseObject* CClass<Class, FistBase, OtherBases...>::GetAsObject(std::unique_ptr<Class>&& _pObject)
{
	return new CObjectAsSharedPointer<Class, FistBase, OtherBases...>(std::move(_pObject));
}

struct SClassAndObject {
	CBaseClass* m_pClass;
	CBaseObject* m_pObject;
};


int luapp11_gc_cpp_class_and_object(lua_State* _pLuaState);

template<class Class>
void PushPointer(lua_State* _pLuaState, Class* _pObject) {

	if(luaL_newmetatable(_pLuaState, "luapp11_CClassAndObject")) {

		lua_pushstring(_pLuaState, "__gc");
		lua_pushcfunction(_pLuaState, luapp11_gc_cpp_class_and_object);
		lua_settable(_pLuaState, -3);

		lua_pop(_pLuaState, 1);
	}

	SClassAndObject* l_pCAO = (SClassAndObject*)lua_newuserdata(_pLuaState, sizeof(SClassAndObject));

	CClass<Class>* l_pClass;

	l_pCAO->m_pClass = CLuaState::GetLuaState(_pLuaState)->GetClass(CClass<Class>::GetSClassName()).get();
	l_pClass = dynamic_cast<CClass<Class>*>(l_pCAO->m_pClass);
	l_pCAO->m_pObject = l_pClass->GetAsObject(_pObject);

	luaL_setmetatable(_pLuaState, "luapp11_CClassAndObject");
}

template<class Class>
void PushPointer(lua_State* _pLuaState, std::shared_ptr<Class> _pObject) {

	if(luaL_newmetatable(_pLuaState, "luapp11_CClassAndObject")) {

		lua_pushstring(_pLuaState, "__gc");
		lua_pushcfunction(_pLuaState, luapp11_gc_cpp_class_and_object);
		lua_settable(_pLuaState, -3);

		lua_pop(_pLuaState, 1);
	}

	SClassAndObject* l_pCAO = (SClassAndObject*)lua_newuserdata(_pLuaState, sizeof(SClassAndObject));
	l_pCAO = new (l_pCAO) SClassAndObject();

	CClass<Class>* l_pClass;

	CLuaState *l_pLuaState = CLuaState::GetLuaState(_pLuaState);
	CLuaState::TClassName l_ClassName = CClass<Class>::GetSClassName();
	std::shared_ptr<CBaseClass> l_spClass = l_pLuaState->GetClass(l_ClassName);

	l_pCAO->m_pClass = l_spClass.get();

	l_pClass = dynamic_cast<CClass<Class>*>(l_pCAO->m_pClass);
	l_pCAO->m_pObject = l_pClass->GetAsObject(_pObject);

	luaL_setmetatable(_pLuaState, "luapp11_CClassAndObject");
}

template<class Class>
void PushPointer(lua_State* _pLuaState, std::unique_ptr<Class> _pObject) {

	if(luaL_newmetatable(_pLuaState, "luapp11_CClassAndObject")) {

		lua_pushstring(_pLuaState, "__gc");
		lua_pushcfunction(_pLuaState, luapp11_gc_cpp_class_and_object);
		lua_settable(_pLuaState, -3);

		lua_pop(_pLuaState, 1);
	}

	SClassAndObject* l_pCAO = (SClassAndObject*)lua_newuserdata(_pLuaState, sizeof(SClassAndObject));
	l_pCAO = new (l_pCAO) SClassAndObject();

	CClass<Class>* l_pClass;

	l_pCAO->m_pClass = CLuaState::GetLuaState(_pLuaState)->GetClass(CClass<Class>::GetSClassName()).get();
	l_pClass = dynamic_cast<CClass<Class>*>(l_pCAO->m_pClass);
	l_pCAO->m_pObject = l_pClass->GetAsObject(_pObject);

	luaL_setmetatable(_pLuaState, "luapp11_CClassAndObject");
}

template<class Class>
Class* GetNakedPointer(lua_State* _pLuaState, int _iStackIndex) {
	SClassAndObject* l_pCAO = (SClassAndObject*)luaL_checkudata(_pLuaState, _iStackIndex, "luapp11_CClassAndObject");
	if(l_pCAO != nullptr) {
		if(CClass<Class>::IsCompatibleWith(l_pCAO->m_pClass)) {
			CObject<Class> *l_pObject = dynamic_cast<CObject<Class>*>(l_pCAO->m_pObject);
			return l_pObject->GetNakedPointer(nullptr);
		}
	}
	return nullptr;
}

template<class Class>
std::shared_ptr<Class> GetSharedPointer(lua_State* _pLuaState, int _iStackIndex) {
	SClassAndObject* l_pCAO = (SClassAndObject*)luaL_checkudata(_pLuaState, _iStackIndex, "luapp11_CClassAndObject");
	if(l_pCAO != nullptr) {
		if(l_pCAO->m_pClass->IsCompatibleWith(CClass<Class>::GetSClassName())) {
			//CBaseObject* l_p = l_pCAO->m_pObject;
			//CObject<Class> *l_pObject = (CObject<Class>*)l_p->GetAsSingleCObject(CClass<Class>::GetSClassName());// dynamic_cast<CObject<Class>*>(l_p);
			CObject<Class> *l_pObject = dynamic_cast<CObject<Class>*>(l_pCAO->m_pObject);
			return l_pObject->GetSharedPointer(nullptr);
		}
	}
	return nullptr;
}

} /* namespace luapp11 */
#endif /*  */
