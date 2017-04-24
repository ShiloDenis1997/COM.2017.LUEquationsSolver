#include "AgCFactory.h"
#include "AgCAggregateComponent.h"

void factTrace(const char* msg) { std::cout << msg << std::endl; }

AgCFactory::AgCFactory() : m_cRef(1) { factTrace("Aggregate Factory constructor"); }

AgCFactory::~AgCFactory() { factTrace("Aggregate Class factory:\t\tDestroy self."); }
//
// Class factory IUnknown implementation
//
HRESULT __stdcall AgCFactory::QueryInterface(const IID& iid, void** ppv)
{
	factTrace("query interface aggregate factory");
	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG __stdcall AgCFactory::AddRef()
{
	factTrace("add ref aggregate factory");
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall AgCFactory::Release()
{
	factTrace("release ref aggregate factory");
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

//
// IClassFactory implementation
//
HRESULT __stdcall AgCFactory::CreateInstance(IUnknown* pUnknownOuter,
	const IID& iid,
	void** ppv)
{
	factTrace("Class aggregate factory:\t\tCreate component.");

	// Cannot aggregate.
	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	// Create component.
	AgCAggregateComponent* pCAg = new AgCAggregateComponent;
	if (pCAg == NULL)
	{
		return E_OUTOFMEMORY;
	}
	// Get the requested interface.
	HRESULT hr = pCAg->QueryInterface(iid, ppv);

	// Release the IUnknown pointer.
	// (If QueryInterface failed, component will delete itself.)
	pCAg->Release();
	return hr;
}

// LockServer
HRESULT __stdcall AgCFactory::LockServer(BOOL bLock)
{
	if (bLock)
	{
		InterlockedIncrement(&g_cServerLocks);
	}
	else
	{
		InterlockedDecrement(&g_cServerLocks);
	}
	return S_OK;
}