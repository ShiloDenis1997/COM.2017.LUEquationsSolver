#include <iostream>
#include <Unknwn.h>
#include "IEquationSolver.h"
#include "Create.h"
using namespace std;

const int N = 3;
double matr[N][N] =
{
	{1, 2, 3},
	{2, 3, 4},
	{-2, 3, 1}
};

int main()
{
	HRESULT hr;
	IUnknown* pIUnknown = CallCreateInstance(TEXT("EquationsDll.dll"));
	if (pIUnknown == nullptr)
	{
		return 1;
	}

	IEquationSolver* pIEquationSolver = nullptr;
	hr = pIUnknown->QueryInterface(IID_IEquationSolver, reinterpret_cast<void**>(&pIEquationSolver));
	pIUnknown->Release();
	if (SUCCEEDED(hr))
	{
		double **m = new double*[N];
		for (int i = 0; i < N; i++)
		{
			m[i] = new double[N];
			for (int j = 0; j < N; j++)
			{
				m[i][j] = matr[i][j];
			}
		}

		pIEquationSolver->LoadMatrix(m, N);
		double *result = new double[N];
		double *bVector = new double[N]{14, 20, 7};
		pIEquationSolver->SolveWithVector(bVector, result);
		cout << "Result: " << endl;
		for (int i = 0; i < N; i++)
			cout << result[i] << endl;
		for (int i = 0; i < N; i++)
			delete[] m[i];
		delete[] m;
		delete[] result;
		delete[] bVector;
		pIEquationSolver->Release();
	}
	return 0;
}