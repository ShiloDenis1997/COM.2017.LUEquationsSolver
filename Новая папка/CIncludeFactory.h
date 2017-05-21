#pragma once
#include <Unknwn.h>
#include "IncludeComponent.h"
#include <iostream>

static HMODULE g_hModule = NULL;   // DLL module handle
static long g_cComponents = 0;     // Count of active components
static long g_cServerLocks = 0;    // Count of locks

								   //Factory
class CIncludeFactory : public IClassFactory
{
public:
	// IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv);
	virtual ULONG   __stdcall AddRef();
	virtual ULONG   __stdcall Release();

	// Interface IClassFactory
	virtual HRESULT __stdcall CreateInstance(IUnknown* pUnknownOuter,
		const IID& iid,
		void** ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);

	// Constructor
	CIncludeFactory();

	// Destructor
	~CIncludeFactory();

private:
	long m_cRef;
};