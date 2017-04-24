#include "AgCAggregateComponent.h"

void compTrace(const char* msg) { std::cout << msg << std::endl; }

AgCAggregateComponent::AgCAggregateComponent()
{
	compTrace("aggregate Component ctor");
	m_cRef = 1;
}

HRESULT AgCAggregateComponent::QueryInterface(const IID& riid, void** ppvObject)
{
	compTrace("component query interface");
	if (riid == IID_IUnknown)
	{
		*ppvObject = static_cast<IWhoAmI*>(this);
	}
	else if (riid == IID_IWhoAmI)
	{
		*ppvObject = static_cast<IWhoAmI*>(this);
	}
	else
	{
		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(this)->AddRef();
	return S_OK;
}

ULONG AgCAggregateComponent::AddRef()
{
	compTrace("aggregate Add ref");
	return InterlockedIncrement(&m_cRef);
}

ULONG AgCAggregateComponent::Release()
{
	compTrace("aggregate Release");
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		compTrace("aggregate Deleting component");
		delete this;
		return 0;
	}

	return m_cRef;
}

void AgCAggregateComponent::WhoAmI() const
{
	compTrace("I am a component which aggregates equations component");
}

AgCAggregateComponent::~AgCAggregateComponent()
{
	compTrace("aggregate Component destructor");
}