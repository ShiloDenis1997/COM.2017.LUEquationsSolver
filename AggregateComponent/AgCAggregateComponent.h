#pragma once
#include <Unknwn.h>
#include "IWhoAmI.h"
#include "AggregateComponent.h"
#include "IEquationSolver.h"
#include <iostream>

static long g_cComponents = 0;     // Count of active components
static long g_cServerLocks = 0;    // Count of locks

class AgCAggregateComponent : public IWhoAmI
{
public:
	AgCAggregateComponent();
	~AgCAggregateComponent() override;
	HRESULT __stdcall QueryInterface(const IID& riid, void** ppvObject) override;
	ULONG __stdcall AddRef() override;
	ULONG __stdcall Release() override;
	void WhoAmI() const override;

	HRESULT __stdcall Init();
private:
	long m_cRef;

	IEquationSolver* m_pEquationSolver;
	IUnknown* m_pUnknownInner;
};