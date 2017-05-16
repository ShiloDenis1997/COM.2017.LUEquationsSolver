#include <iostream>
#include <Unknwn.h>
#include "IEquationSolver.h"
#include "IEquationPrinter.h"
#include "AggregateComponent.h"
#include "EquationsComponent.h"
#include "IWhoAmI.h"
using namespace std;

const int N = 3;
double matr[N][N] =
{
	{ 1, 2, 3 },
	{ 2, 3, 4 },
	{ -2, 3, 1 }
};

void OnlyComponent();
void Aggregation();

int main()
{
	CoInitialize(nullptr);
	cout << "Calling CoCreateInstance" << endl;
	int chose = 1;
	cout << " 1 - only component\n 2 - aggregation\n";
	cin >> chose;
	if (chose == 1)
	{
		OnlyComponent();
	}
	else if (chose == 2)
	{
		Aggregation();
	}

	CoUninitialize();
	return 0;
}

void OnlyComponent()
{
	HRESULT hr;
	IUnknown* pIUnknown = nullptr;// = CallCreateInstance(TEXT("EquationsDll.dll"));
	hr = ::CoCreateInstance(CLSID_EquationsComponent,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_IEquationSolver,
		(void**)&pIUnknown);
	cout << "Call done" << endl;
	if (SUCCEEDED(hr))
	{
		cout << "Component created" << endl;
	}
	else
	{
		cout << "Failed to create component" << endl;
		CoUninitialize();
		return;
	}

	if (pIUnknown == nullptr)
	{
		CoUninitialize();
		return;
	}

	IEquationSolver* pIEquationSolver = nullptr;
	hr = pIUnknown->QueryInterface(IID_IEquationSolver, reinterpret_cast<void**>(&pIEquationSolver));
	pIUnknown->Release();
	IEquationPrinter* pIEquationPrinter = nullptr;
	pIUnknown->QueryInterface(IID_IEquationPrinter, reinterpret_cast<void**>(&pIEquationPrinter));

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
		double *bVector = new double[N] {14, 20, 7};
		pIEquationSolver->SolveWithVector(bVector, result);
		if (pIEquationPrinter != nullptr)
		{
			cout << "L matrix: " << endl;
			pIEquationPrinter->PrintLMatrix();
			cout << endl << "U matrix:" << endl;
			pIEquationPrinter->PrintUMatrix();
			pIEquationPrinter->Release();
		}

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
}

void Aggregation()
{
	cout << "Aggregation";
	HRESULT hr;
	IWhoAmI* pIWhoAmI = nullptr;// = CallCreateInstance(TEXT("EquationsDll.dll"));
	hr = ::CoCreateInstance(CLSID_AggregateComponent,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_IWhoAmI,
		(void**)&pIWhoAmI);
	cout << "\t\tCall done" << endl;
	if (SUCCEEDED(hr))
	{
		cout << "aggregate Component created" << endl;
	}
	else
	{
		cout << "Failed to create aggregate component" << endl;
		CoUninitialize();
		return;
	}

	if (pIWhoAmI == nullptr)
	{
		CoUninitialize();
		return;
	}

	IEquationSolver* pIEquationSolver = nullptr;
	pIWhoAmI->WhoAmI();// ask a main question
	hr = pIWhoAmI->QueryInterface(IID_IEquationSolver, reinterpret_cast<void**>(&pIEquationSolver));
	pIWhoAmI->Release();
	IEquationPrinter* pIEquationPrinter = nullptr;
	pIWhoAmI->QueryInterface(IID_IEquationPrinter, reinterpret_cast<void**>(&pIEquationPrinter));

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
		double *bVector = new double[N] {14, 20, 7};
		pIEquationSolver->SolveWithVector(bVector, result);
		if (pIEquationPrinter != nullptr)
		{
			cout << "L matrix: " << endl;
			pIEquationPrinter->PrintLMatrix();
			cout << endl << "U matrix:" << endl;
			pIEquationPrinter->PrintUMatrix();
			pIEquationPrinter->Release();
		}
		else
		{
			cout << "Does not implementing Printer interface!!!!!!\n";
		}

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
}