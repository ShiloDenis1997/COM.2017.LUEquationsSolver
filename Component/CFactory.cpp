#include "CFactory.h"

void factTrace(const char* msg) { std::cout << msg << std::endl; }

CFactory::CFactory(): m_cRef(1) {factTrace("Factory constructor");}

CFactory::~CFactory(){ factTrace("Class factory:\t\tDestroy self."); }
//
// Class factory IUnknown implementation
//
HRESULT __stdcall CFactory::QueryInterface(const IID& iid, void** ppv)
{
	factTrace("query interface factory");
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

ULONG __stdcall CFactory::AddRef()
{
	factTrace("add ref factory");
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CFactory::Release()
{
	factTrace("release ref factory");
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
HRESULT __stdcall CFactory::CreateInstance(IUnknown* pUnknownOuter,
	const IID& iid,
	void** ppv)
{
	factTrace("Class factory:\t\tCreate component.");

	// Cannot aggregate.
	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	// Create component.
	CEquationSolver* pEq = new CEquationSolver;
	if (pEq == NULL)
	{
		return E_OUTOFMEMORY;
	}
	pEq->AddRef();
	// Get the requested interface.
	HRESULT hr = pEq->QueryInterface(iid, ppv);

	// Release the IUnknown pointer.
	// (If QueryInterface failed, component will delete itself.)
	pEq->Release();
	return hr;
}

// LockServer
HRESULT __stdcall CFactory::LockServer(BOOL bLock)
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