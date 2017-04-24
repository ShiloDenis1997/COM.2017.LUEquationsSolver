#include <Unknwn.h>
#include <iostream>
#include "../EquationsDll/REGISTRY.H"
#include "AgCFactory.h"
#include "AgCAggregateComponent.h"
#include "AggregateComponent.h"

// Friendly name of component
const char g_szFriendlyName[] = "Aggregate component for lab 4";

// Version-independent ProgID
const char g_szVerIndProgID[] = "Lab4.AggregateComponent";

// ProgID
const char g_szProgID[] = "Lab4.AggregateComponent.1";

void trace(const char* msg) { std::cout << msg << std::endl; }

///////////////////////////////////////////////////////////
//
// Exported functions
//

//
// Can DLL unload now?
//
STDAPI DllCanUnloadNow()
{
	if ((g_cComponents == 0) && (g_cServerLocks == 0))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

//
// Get class factory
//
STDAPI DllGetClassObject(const CLSID& clsid,
	const IID& iid,
	void** ppv)
{
	trace("DllGetClassObject:\tCreate aggregate class factory.");

	// Can we create this component?
	if (clsid != CLSID_AggregateComponent)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	// Create class factory.
	AgCFactory* pFactory = new AgCFactory;  // No AddRef in constructor
	if (pFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	// Get requested interface.
	HRESULT hr = pFactory->QueryInterface(iid, ppv);
	pFactory->Release();

	return hr;
}

//
// Server registration
//
STDAPI DllRegisterServer()
{
	return RegisterServer(g_hModule,
		CLSID_AggregateComponent,
		g_szFriendlyName,
		g_szVerIndProgID,
		g_szProgID);
}


//
// Server unregistration
//
STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_AggregateComponent,
		g_szVerIndProgID,
		g_szProgID);
}

///////////////////////////////////////////////////////////
//
// DLL module information
//
BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD dwReason,
	void* lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HMODULE)hModule;
	}
	return TRUE;
}