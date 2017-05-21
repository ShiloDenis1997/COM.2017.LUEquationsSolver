#include "CIncludeFactory.h"
#include "CIncludeComponent.h"

void factTrace(const char* msg) { std::cout << msg << std::endl; }

CIncludeFactory::CIncludeFactory() : m_cRef(1) { factTrace("Include Factory constructor"); }

CIncludeFactory::~CIncludeFactory() { factTrace("Include Class factory:\t\tDestroy self."); }
//
// Class factory IUnknown implementation
//
HRESULT __stdcall CIncludeFactory::QueryInterface(const IID& iid, void** ppv)
{
	factTrace("query interface include factory");
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

ULONG __stdcall CIncludeFactory::AddRef()
{
	factTrace("add ref include factory");
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CIncludeFactory::Release()
{
	factTrace("release ref include factory");
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
HRESULT __stdcall CIncludeFactory::CreateInstance(IUnknown* pUnknownOuter,
	const IID& iid,
	void** ppv)
{
	factTrace("Class include factory:\t\tCreate component.");

	// Cannot include.
	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	// Create component.
	CIncludeComponent* pCInclude = new CIncludeComponent;
	if (pCInclude == NULL)
	{
		return E_OUTOFMEMORY;
	}
	HRESULT hr = pCInclude->Init();
	if (FAILED(hr))
	{
		pCInclude->Release();
		return hr;
	}
	// Get the requested interface.
	hr = pCInclude->QueryInterface(iid, ppv);

	// Release the IUnknown pointer.
	// (If QueryInterface failed, component will delete itself.)
	pCInclude->Release();
	return hr;
}

// LockServer
HRESULT __stdcall CIncludeFactory::LockServer(BOOL bLock)
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