#ifndef LUAPP11_CCLASS_INL_
#define LUAPP11_CCLASS_INL_

namespace luapp11 {

class CBaseObject {
public:
	virtual ~CBaseObject() {};

	//virtual void* GetAsSingleCObject(CLuaState::TClassName _ClassName) = 0;

protected:
	CBaseObject() {};

};

template<class Class>
class CObject<Class> : public virtual CBaseObject {
public:
	CObject() {};
	virtual ~CObject() {};

	virtual Class* GetNakedPointer(Class*) const = 0;
	virtual std::shared_ptr<Class> GetSharedPointer(Class*) const = 0;

	/*virtual void* GetAsSingleCObject(CLuaState::TClassName _ClassName) {
		if(_ClassName == std::type_index(typeid(Class))) {
			return this;
		} else {
			return nullptr;
		}
	}*/
};
/*
template<class Class, class...Bases>
void* CObject<Class, Bases...>::GetAsSingleCObject(CLuaState::TClassName _ClassName) {
	void* firstCandidate = CObject<Class>::GetAsSingleCObject(_ClassName);
	if(firstCandidate != nullptr) {
		return firstCandidate;
	} else {
		return CObject<Bases...>::GetAsSingleCObject(_ClassName);
	}
}*/

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------


template<class Class>
class CObjectAsNakedPointer<Class> : public virtual CObject<Class> {
public:
	CObjectAsNakedPointer(Class* _pObject):m_pObject(_pObject) {};
	CObjectAsNakedPointer(const CObjectAsNakedPointer<Class>& _original) = delete;
	CObjectAsNakedPointer(CObjectAsNakedPointer<Class>&& _original) = delete;

	virtual ~CObjectAsNakedPointer() {m_pObject = nullptr;};

	CObjectAsNakedPointer& operator=(const CObjectAsNakedPointer<Class>& _original) = delete;
	void operator=(CObjectAsNakedPointer<Class>&& _original) = delete;

	virtual Class* GetNakedPointer(Class*) const { return m_pObject? m_pObject : GetNakedPointerInternal(); };
	virtual std::shared_ptr<Class> GetSharedPointer(Class*) const { return std::shared_ptr<Class>(); };
	virtual std::weak_ptr<Class> GetWeakPointer(Class*) const { return std::weak_ptr<Class>(); };
	virtual std::unique_ptr<Class> GetUniquePointer(Class*) const { return std::unique_ptr<Class>(); };

protected:
	CObjectAsNakedPointer():m_pObject(nullptr) {};


	virtual Class* GetNakedPointerInternal() const {return m_pObject;};

private:
	Class* m_pObject;
};


template<class Class, class FirstBase, class... OtherBases>
class CObjectAsNakedPointer<Class, FirstBase, OtherBases...>
: public virtual CObject<FirstBase>
, public virtual CObjectAsNakedPointer<Class, OtherBases...>
{
public:
	CObjectAsNakedPointer(Class* _pObject):m_pObject(_pObject) {};
	CObjectAsNakedPointer(const CObjectAsNakedPointer<Class, FirstBase, OtherBases...>& _original) = delete;
	CObjectAsNakedPointer(CObjectAsNakedPointer<Class, FirstBase, OtherBases...>&& _original) = delete;

	virtual ~CObjectAsNakedPointer() {m_pObject = nullptr;};

	CObjectAsNakedPointer& operator=(const CObjectAsNakedPointer<Class, FirstBase, OtherBases...>& _original) = delete;
	void operator=(CObjectAsNakedPointer<Class, FirstBase, OtherBases...>&& _original) = delete;

	Class* GetNakedPointer(Class*) const { return m_pObject? m_pObject : GetNakedPointerInternal(); };

	virtual FirstBase* GetNakedPointer(FirstBase*) const { return m_pObject? (FirstBase*)m_pObject : (FirstBase*)GetNakedPointerInternal(); };
	virtual std::shared_ptr<FirstBase> GetSharedPointer(FirstBase*) const { return std::shared_ptr<FirstBase>(); };
	virtual std::weak_ptr<FirstBase> GetWeakPointer(FirstBase*) const { return std::weak_ptr<FirstBase>(); };
	virtual std::unique_ptr<FirstBase> GetUniquePointer(FirstBase*) const { return std::unique_ptr<FirstBase>(); };
/*
	void* GetAsSingleCObject(CLuaState::TClassName _ClassName) {
		void* firstCandidate = CObject<Class>::GetAsSingleCObject(_ClassName);
		if(firstCandidate != nullptr) {
			return firstCandidate;
		} else {
			return CObjectAsNakedPointer<Class, OtherBases...>::GetAsSingleCObject(_ClassName);
		}
	}
*/
protected:
	CObjectAsNakedPointer():m_pObject(nullptr) {};

	virtual Class* GetNakedPointerInternal() const {return m_pObject;};


private:
	Class* m_pObject;
};


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------


template<class Class>
class CObjectAsSharedPointer<Class> : public CObject<Class> {
public:
	CObjectAsSharedPointer(std::shared_ptr<Class> _pObject):m_pObject(_pObject) {};
	CObjectAsSharedPointer(std::unique_ptr<Class>&& _pObject):m_pObject(std::move(_pObject)) {};
	CObjectAsSharedPointer(const CObjectAsSharedPointer<Class>& _original) = delete;
	CObjectAsSharedPointer(CObjectAsSharedPointer<Class>&& _original) = delete;

	virtual ~CObjectAsSharedPointer() {m_pObject.reset();};

	CObjectAsSharedPointer& operator=(const CObjectAsSharedPointer<Class>& _original) = delete;
	void operator=(CObjectAsSharedPointer<Class>&& _original) = delete;

	virtual Class* GetNakedPointer(Class*) const
	{
		return GetSharedPointer(nullptr).get();
	};

	virtual std::shared_ptr<Class> GetSharedPointer(Class*) const
	{
		return m_pObject? m_pObject : GetSharedPointerInternal();
	};

	virtual std::weak_ptr<Class> GetWeakPointer(Class*) const
	{
		return std::weak_ptr<Class>(GetSharedPointer(nullptr));
	};

	virtual std::unique_ptr<Class> GetUniquePointer(Class*) const
	{
		return std::unique_ptr<Class>();
	};

protected:
	CObjectAsSharedPointer() {};


	virtual std::shared_ptr<Class> GetSharedPointerInternal() const {return m_pObject;};

private:
	std::shared_ptr<Class> m_pObject;
};


template<class Class, class FirstBase, class... OtherBases>
class CObjectAsSharedPointer<Class, FirstBase, OtherBases...>
: public CObject<FirstBase>
, public CObjectAsSharedPointer<Class, OtherBases...>
{
public:
	CObjectAsSharedPointer(std::shared_ptr<Class> _pObject):m_pObject(_pObject)
	{

	};
	CObjectAsSharedPointer(std::unique_ptr<Class>&& _pObject):m_pObject(std::move(_pObject)) {};
	CObjectAsSharedPointer(const CObjectAsSharedPointer<Class, FirstBase, OtherBases...>& _original) = delete;
	CObjectAsSharedPointer(CObjectAsSharedPointer<Class, FirstBase, OtherBases...>&& _original) = delete;

	virtual ~CObjectAsSharedPointer() {m_pObject.reset();};

	CObjectAsSharedPointer& operator=(const CObjectAsSharedPointer<Class, FirstBase, OtherBases...>& _original) = delete;
	void operator=(CObjectAsSharedPointer<Class, FirstBase, OtherBases...>&& _original) = delete;

	virtual Class* GetNakedPointer(Class*) const
	{
		return GetSharedPointer((Class*)nullptr).get();
	};

	virtual std::shared_ptr<Class> GetSharedPointer(Class*) const
	{
		if (m_pObject) {
			return m_pObject;
		} else {
			return GetSharedPointerInternal();
		}
	};

	virtual std::weak_ptr<Class> GetWeakPointer(Class*) const
	{
		return std::weak_ptr<Class>(GetSharedPointer((Class*)nullptr));
	};


	virtual FirstBase* GetNakedPointer(FirstBase*) const
	{
		return GetSharedPointer((FirstBase*)nullptr).get();
	};

	virtual std::shared_ptr<FirstBase> GetSharedPointer(FirstBase*) const
	{
		return std::shared_ptr<FirstBase>(m_pObject? m_pObject : GetSharedPointerInternal());
	};

	virtual std::weak_ptr<FirstBase> GetWeakPointer(FirstBase*) const
	{
		return std::weak_ptr<FirstBase>(GetSharedPointer((FirstBase*)nullptr));
	};

	virtual std::unique_ptr<FirstBase> GetUniquePointer(FirstBase*) const
	{
		return std::unique_ptr<FirstBase>();
	};
/*
	void* GetAsSingleCObject(CLuaState::TClassName _ClassName) {
		void* firstCandidate = CObject<Class>::GetAsSingleCObject(_ClassName);
		if(firstCandidate != nullptr) {
			return firstCandidate;
		} else {
			return CObjectAsSharedPointer<Class, OtherBases...>::GetAsSingleCObject(_ClassName);
		}
	}
*/
protected:
	CObjectAsSharedPointer():m_pObject(0) {};

	virtual std::shared_ptr<Class> GetSharedPointerInternal() const {return m_pObject;};


private:
	std::shared_ptr<Class> m_pObject;
};




};

#endif
