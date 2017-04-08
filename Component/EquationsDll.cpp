#include <Unknwn.h>
#include "IEquationSolver.h"
#include <iostream>
#include "REGISTRY.H"
#include "EquationsComponent.h"
#include "IEquationPrinter.h"
#include "CEquationsSolver.h"
#include "CFactory.h"

								   // Friendly name of component
const char g_szFriendlyName[] = "Equations solver via LU-decomposition";

// Version-independent ProgID
const char g_szVerIndProgID[] = "EquationsSolver.LUdecomp";

// ProgID
const char g_szProgID[] = "EquationsSolver.LUdecomp.1";

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
	trace("DllGetClassObject:\tCreate class factory.");

	// Can we create this component?
	if (clsid != CLSID_EquationsComponent)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	// Create class factory.
	CFactory* pFactory = new CFactory;  // No AddRef in constructor
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
		CLSID_EquationsComponent,
		g_szFriendlyName,
		g_szVerIndProgID,
		g_szProgID);
}


//
// Server unregistration
//
STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_EquationsComponent,
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