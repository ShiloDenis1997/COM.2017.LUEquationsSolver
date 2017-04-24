#pragma once
#include <Unknwn.h>
#include "CEquationsSolver.h"
#include <iostream>

static HMODULE g_hModule = NULL;   // DLL module handle

//Factory
class CFactory : public IClassFactory
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
	CFactory();

	// Destructor
	~CFactory();

private:
	long m_cRef;
};