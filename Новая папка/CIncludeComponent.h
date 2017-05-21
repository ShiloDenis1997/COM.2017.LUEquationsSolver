#pragma once
#include <Unknwn.h>
#include "IWhoAmI.h"
#include "IncludeComponent.h"
#include "IEquationSolver.h"
#include <iostream>

class CIncludeComponent : public IWhoAmI, public IEquationSolver
{
public:
	CIncludeComponent();
	~CIncludeComponent() override;
	HRESULT __stdcall QueryInterface(const IID& riid, void** ppvObject) override;
	ULONG __stdcall AddRef() override;
	ULONG __stdcall Release() override;
	void WhoAmI() const override;
	virtual bool LoadMatrix(double **a, int n) override;
	virtual void SolveWithVector(double *b, double *result) override;
	HRESULT __stdcall Init();
private:
	long m_cRef;
	IEquationSolver* m_pIEquationSolver;
};