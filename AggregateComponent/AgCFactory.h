#pragma once
#include <Unknwn.h>
#include "AggregateComponent.h"
#include <iostream>

static HMODULE g_hModule = NULL;   // DLL module handle
								   //Factory
class AgCFactory : public IClassFactory
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
	AgCFactory();

	// Destructor
	~AgCFactory();

private:
	long m_cRef;
};