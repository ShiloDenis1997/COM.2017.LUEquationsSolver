#include "CFactory.h"

void factTrace(const char* msg) { std::cout << msg << std::endl; }

CFactory::CFactory(): m_cRef(1) {factTrace("\tCFactory: constructor");}

CFactory::~CFactory(){ factTrace("\tCFactory:\t\tDestroy self."); }
//
// Class factory IUnknown implementation
//
HRESULT __stdcall CFactory::QueryInterface(const IID& iid, void** ppv)
{
	factTrace("\tCFactory: query interface factory");
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
	factTrace("\tCFactory: add ref factory");
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CFactory::Release()
{
	factTrace("\tCFactory: release ref factory");
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
	factTrace("\tCFactory:\t\tCreate component.");

	// when aggregate iid should be IUnknown.
	if ((pUnknownOuter != NULL) && (iid != IID_IUnknown))
	{
		factTrace("\tCFactory: To aggregate you should ask for IUnknown");
		return CLASS_E_NOAGGREGATION;
	}

	// Create component.
	factTrace("\tCFactory: Call CEquations ctor");
	CEquationSolver* pEq = new CEquationSolver(pUnknownOuter);
	if (pEq == NULL)
	{
		return E_OUTOFMEMORY;
	}
	// Get the requested interface.
	factTrace("\tCFactory: Call nondelegating query interface");
	HRESULT hr = pEq->NondelegatingQueryInterface(iid, ppv);

	factTrace("\tCFactory: Call delegating release");
	// Release the IUnknown pointer.
	// (If QueryInterface failed, component will delete itself.)
	pEq->NondelegatingRelease(); //to do changed from non delegating release
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