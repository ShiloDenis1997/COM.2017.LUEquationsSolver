#include "CEquationsSolver.h"
#include "CEquationsPrinter.h"

void compTrace(const char* msg) { std::cout << msg << std::endl; }

CEquationSolver::CEquationSolver()
{
	compTrace("Component ctor");
	m_cRef = 1;
	m_N = 0;
	m_LUmatr = nullptr;
	m_pRow = nullptr;
	m_pCol = nullptr;
	printerIface = nullptr;
}

HRESULT CEquationSolver::QueryInterface(const IID& riid, void** ppvObject)
{
	compTrace("component query interface");
	if (riid == IID_IUnknown)
	{
		*ppvObject = static_cast<IEquationSolver*>(this);
	}
	else if (riid == IID_IEquationSolver)
	{
		*ppvObject = static_cast<IEquationSolver*>(this);
	}
	else if (riid == IID_IEquationPrinter)
	{
		if (printerIface == nullptr)
		{
			compTrace("Printer interface requested");
			printerIface = new CEquationsPrinter(this);
			if (printerIface == NULL)
			{
				return E_OUTOFMEMORY;
			}
			*ppvObject = static_cast<IEquationPrinter*>(printerIface);
			return S_OK;
		}
		else
		{
			return printerIface->QueryInterface(riid, ppvObject);
		}
	}
	else
	{
		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(this)->AddRef();
	return S_OK;
}

ULONG CEquationSolver::AddRef()
{
	compTrace("Add ref");
	return InterlockedIncrement(&m_cRef);
}

ULONG CEquationSolver::Release()
{
	compTrace("Release");
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		compTrace("Deleting component");
		DeleteMatrix();
		delete this;
		return 0;
	}

	return m_cRef;
}

bool CEquationSolver::LoadMatrix(double** a, int n)
{
	DeleteMatrix();
	m_N = n;
	m_LUmatr = new double*[n];
	for (int i = 0; i < n; i++)
	{
		m_LUmatr[i] = new double[n];
		for (int j = 0; j < n; j++)
		{
			m_LUmatr[i][j] = a[i][j];
		}
	}
	m_pRow = new int[n];
	m_pCol = new int[n];
	return MakeLU(m_LUmatr, m_pRow, m_pCol, n);
}

void CEquationSolver::SolveWithVector(double* b, double* result)
{
	double *temp = new double[m_N];
	for (int i = 0; i < m_N; i++)
	{
		temp[i] = b[m_pRow[i]];
	}
	SolveL(temp);
	SolveU(temp);
	for (int i = 0; i < m_N; i++)
	{
		result[m_pCol[i]] = temp[i];
	}
	delete[] temp;
}

bool CEquationSolver::MakeLU(double ** a, int * pr, int * pc, int n) const
{
	for (int i = 0; i < n; i++)
		pr[i] = pc[i] = i;
	for (int k = 0; k <= n - 2; k++)
	{
		double mx = abs(a[k][k]);
		int psr = k, psc = k;
		for (int i = k; i <= n - 1; i++)
			for (int j = k; j <= n - 1; j++)
			{
				if (abs(a[i][k]) > mx)
				{
					mx = abs(a[i][k]);
					psr = i;
					psc = j;
				}
			}
		if (mx < 1e-15)
			return false;
		if (psr != k || psc != k)
		{
			std::swap(a[psr], a[k]);
			swapColumns(a, psc, k, n);
			std::swap(pr[psr], pr[k]);
			std::swap(pc[psc], pc[k]);
		}
		for (int i = k + 1; i <= n - 1; i++)
		{
			a[i][k] /= a[k][k];
			for (int j = k + 1; j <= n - 1; j++)
				a[i][j] -= a[k][j] * a[i][k];
		}
	}
	return true;
}

void CEquationSolver::SolveL(double* y) const
{
	for (int i = 0; i < m_N; i++)
	{
		for (int j = 0; j < i; j++)
		{
			y[i] -= y[j] * m_LUmatr[i][j];
		}
	}
}

void CEquationSolver::SolveU(double* x) const
{
	for (int i = m_N - 1; i >= 0; i--)
	{
		for (int j = m_N - 1; j > i; j--)
		{
			x[i] -= x[j] * m_LUmatr[i][j];
		}
		x[i] /= m_LUmatr[i][i];
	}
}

void CEquationSolver::DeleteMatrix()
{
	compTrace("Delete matrix");
	if (m_LUmatr == nullptr)
		return;

	for (int i = 0; i < m_N; i++)
	{
		delete[] m_LUmatr[i];
	}

	delete[] m_LUmatr;
	delete[] m_pCol;
	delete[] m_pRow;
	m_LUmatr = nullptr;
	compTrace("Deleted");
}

void CEquationSolver::swapColumns(double** a, int c1, int c2, int n)
{
	if (c1 == c2)
		return;
	for (int i = 0; i < n; i++)
	{
		std::swap(a[i][c1], a[i][c2]);
	}
}

CEquationSolver::~CEquationSolver()
{
	compTrace("Component destructor");
}