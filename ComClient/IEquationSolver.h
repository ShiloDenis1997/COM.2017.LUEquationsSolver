#pragma once
#include <Unknwn.h>

interface IEquationSolver : IUnknown
{
	virtual ~IEquationSolver()
	{
	}

	virtual bool LoadMatrix(double **a, int n) = 0;
	virtual void SolveWithVector(double *b, double *result) = 0;
};

extern "C"
{
	extern const IID IID_IEquationSolver;
}