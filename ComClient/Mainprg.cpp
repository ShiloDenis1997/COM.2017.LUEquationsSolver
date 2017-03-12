#include <iostream>
#include <Unknwn.h>
using namespace std;

void trace(const char* msg) { cout << msg << endl; }

interface IEquationSolver : IUnknown
{
	virtual ~IEquationSolver()
	{
	}

	virtual bool LoadMatrix(double **a, int n) = 0;
	virtual void SolveWithVector(double *b, double *result) = 0;
};

extern const IID IID_IEquationSolver;

class CEquationSolver : public IEquationSolver
{
public:
	CEquationSolver();
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
private:
	long m_cRef;
	int m_N;
	int *m_pRow;
	int *m_pCol;
	double **m_LUmatr;
};

CEquationSolver::CEquationSolver()
{
	m_cRef = 0;
	m_N = 0;
	m_LUmatr = nullptr;
}

HRESULT CEquationSolver::QueryInterface(const IID& riid, void** ppvObject)
{
	if (riid == IID_IUnknown)
	{
		*ppvObject = static_cast<IEquationSolver*>(this);
	}
	else if (riid == IID_IEquationSolver)
	{
		*ppvObject = static_cast<IEquationSolver*>(this);
	}
	else
	{
		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}
	static_cast<IUnknown*>(this)->AddRef();
	return S_OK;
}

ULONG CEquationSolver::AddRef()
{
	trace("Add ref\n");
	return InterlockedIncrement(&m_cRef);
}

ULONG CEquationSolver::Release()
{
	trace("Release\n");
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		trace("Deleting component\n");
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
			swap(a[psr], a[k]);
			swapColumns(a, psc, k, n);
			swap(pr[psr], pr[k]);
			swap(pc[psc], pc[k]);
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
	if (m_LUmatr != nullptr)
	{
		for (int i = 0; i < m_N; i++)
		{
			delete[] m_LUmatr[i];
		}
	}
	delete[] m_LUmatr;
	delete[] m_pCol;
	delete[] m_pRow;
	m_LUmatr = nullptr;
}

void CEquationSolver::swapColumns(double** a, int c1, int c2, int n)
{
	if (c1 == c2)
		return;
	for (int i = 0; i < n; i++)
	{
		swap(a[i][c1], a[i][c2]);
	}
}

IUnknown* CreateInstance()
{
	IUnknown* pI = static_cast<IEquationSolver*>(new CEquationSolver);
	pI->AddRef();
	return pI;
}

static const IID IID_IEquationSolver =
	{ 0x32bb8320, 0xb41b, 0x11cf,
	{ 0xa6, 0xbb, 0x0, 0x80, 0xc7, 0xb2, 0xd6, 0x82 } };

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
	IUnknown* pIUnknown = CreateInstance();
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