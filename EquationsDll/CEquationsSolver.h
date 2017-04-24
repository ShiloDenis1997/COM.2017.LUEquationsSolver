#pragma once
#include <Unknwn.h>
#include "IEquationSolver.h"
#include "EquationsComponent.h"
#include "IEquationPrinter.h"
#include <iostream>

class CEquationsPrinter;

class CEquationSolver : public IEquationSolver//,
	//public IEquationPrinter
{
public:
	CEquationSolver();
	~CEquationSolver() override;
	HRESULT __stdcall QueryInterface(const IID& riid, void** ppvObject) override;
	ULONG __stdcall AddRef() override;
	ULONG __stdcall Release() override;
	bool LoadMatrix(double** a, int n) override;
	void SolveWithVector(double* b, double* result) override;
private:
	bool MakeLU(double **a, int* pr, int* pc, int n) const;
	void SolveL(double* y) const;
	void SolveU(double* x) const;
	void DeleteMatrix();
	static void swapColumns(double **a, int c1, int c2, int n);
public:
	
private:
	friend class CEquationsPrinter;
	long m_cRef;
	int m_N;
	int *m_pRow;
	int *m_pCol;
	double **m_LUmatr;
	CEquationsPrinter* printerIface;
};