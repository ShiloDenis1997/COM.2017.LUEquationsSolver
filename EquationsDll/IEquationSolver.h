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

DEFINE_GUID(IID_IEquationSolver,
	0x10dc16aa, 0x7007, 0x4267,
	0xbf, 0x4c, 0x87, 0x3d, 0x8c, 0x5c, 0x76, 0x79);