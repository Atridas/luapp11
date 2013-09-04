/*
 * Class.hpp
 *
 *  Created on: 24/02/2013
 *      Author: Atridas
 */

#ifndef LUAPP11_CLASS_HPP_
#define LUAPP11_CLASS_HPP_

#include "luapp11.hpp"

#include <string>
#include <set>

namespace luapp11 {

class CBaseClass {
public:
	virtual ~CBaseClass() {};

	virtual CLuaState::TClassName GetClassName() const = 0;
	virtual bool IsCompatibleWith(CLuaState::TClassName _ClassName) const = 0;

protected:
	CBaseClass() {};
};

template<class Class, class...Bases>
class CClass;

template<class Class>
class CClass<Class> : public CBaseClass {
public:
	CClass() = delete;
	virtual ~CClass() {};

	static std::shared_ptr<CBaseClass> RegisterClass(CLuaState& _luaState);

	CLuaState::TClassName GetClassName() const {return GetSClassName();};
	static CLuaState::TClassName GetSClassName() {return std::type_index(typeid(Class));};

	virtual bool IsCompatibleWith(CLuaState::TClassName _ClassName) const {
		if(_ClassName == GetClassName()) {
			return true;
		}

		return false;
	}

	virtual CBaseObject* GetAsObject(Class* _pObject);
	virtual CBaseObject* GetAsObject(std::shared_ptr<Class> _pObject);
	virtual CBaseObject* GetAsObject(std::unique_ptr<Class>&& _pObject);


	CClass(CLuaState& _luaState);
};

template<class Class, class FirstBase, class...OtherBases>
class CClass<Class, FirstBase, OtherBases...> : public CClass<Class, OtherBases...> {
public:
	CClass() = delete;
	virtual ~CClass() {};

	static std::shared_ptr<CBaseClass> RegisterClass(CLuaState& _luaState);

	virtual bool IsCompatibleWith(CLuaState::TClassName _ClassName) const {
		if(_ClassName == std::type_index(typeid(FirstBase))) {
			return true;
		}

		return CClass<Class, OtherBases...>::IsCompatibleWith(_ClassName);
	}

	CBaseObject* GetAsObject(Class* _pObject);
	CBaseObject* GetAsObject(std::shared_ptr<Class> _pObject);
	CBaseObject* GetAsObject(std::unique_ptr<Class>&& _pObject);


	CClass(CLuaState& _luaState);
};


} /* namespace luapp11 */

#include "Class.inl"

#endif /* CLASS_HPP_ */
