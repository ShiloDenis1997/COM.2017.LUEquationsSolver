#pragma once
#include <iostream>
#include <Unknwn.h>
#include "IEquationPrinter.h"
#include "CEquationsSolver.h"

class CEquationsPrinter : public IEquationPrinter
{
public:
	//Constructor - destructor
	CEquationsPrinter(CEquationSolver* owner);
	~CEquationsPrinter() override;
	
	//IUnknown
	HRESULT __stdcall QueryInterface(const IID& riid, void** ppvObject) override;
	ULONG __stdcall AddRef() override;
	ULONG __stdcall Release() override;
	
	//IEquationPrinter
	void PrintLMatrix() const override;
	void PrintUMatrix() const override;

private:
	long m_cRef;
	CEquationSolver* owner;
};