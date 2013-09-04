/*
 * CClass.h
 *
 *  Created on: 24/02/2013
 *      Author: Atridas
 */

#ifndef LUAPP11_CCLASS_HPP_
#define LUAPP11_CCLASS_HPP_

#include "luapp11.hpp"

#include <memory>

namespace luapp11 {

template<class Class, class...Bases>
class CObject : public CObject<Class>, public CObject<Bases...> {
public:
	CObject() {};
	virtual ~CObject() {};

	virtual Class* GetNakedPointer(Class*) const = 0;
	virtual std::shared_ptr<Class> GetSharedPointer(Class*) const = 0;
	virtual std::weak_ptr<Class> GetWeakPointer(Class*) const = 0;
	virtual std::unique_ptr<Class> GetUniquePointer(Class*) const = 0;

protected:

	//virtual void* GetAsSingleCObject(CLuaState::TClassName _ClassName);
};


template<class Class, class...Bases>
class CObjectAsNakedPointer;

template<class Class, class...Bases>
class CObjectAsSharedPointer;

template<class Class, class...Bases>
class CObjectAsWeakPointer;


} /* namespace luapp11 */

#include "Object.inl"

#endif /* CCLASS_H_ */
