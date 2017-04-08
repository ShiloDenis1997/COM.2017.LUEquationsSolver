#include <Unknwn.h>
#include "IEquationSolver.h"
#include <iostream>
#include "REGISTRY.H"
#include "EquationsComponent.h"
#include "IEquationPrinter.h"

void trace(const char* msg) { std::cout << msg << std::endl; }

static HMODULE g_hModule = NULL;   // DLL module handle
static long g_cComponents = 0;     // Count of active components
static long g_cServerLocks = 0;    // Count of locks

								   // Friendly name of component
const char g_szFriendlyName[] = "Equations solver via LU-decomposition";

// Version-independent ProgID
const char g_szVerIndProgID[] = "EquationsSolver.LUdecomp";

// ProgID
const char g_szProgID[] = "EquationsSolver.LUdecomp.1";

class CEquationSolver : public IEquationSolver,
	public IEquationPrinter
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
public:
	~CEquationSolver() override;
	void PrintLMatrix() const override;
	void PrintUMatrix() const override;
private:
	long m_cRef;
	int m_N;
	int *m_pRow;
	int *m_pCol;
	double **m_LUmatr;
};

void CEquationSolver::PrintLMatrix() const
{
	if (m_LUmatr == nullptr)
	{
		std::cout << TEXT("No matrix provided") << std::endl;
	}

	for (int i = 0; i < m_N; i++)
	{
		for (int j = 0; j < m_N; j++)
		{
			if (j > i)
			{
				std::cout << 0 << " ";
			}
			else if (j < i)
			{
				std::cout << m_LUmatr[i][j] << " ";
			}
			else
			{
				std::cout << "1 ";
			}
		}

		std::cout << std::endl;
	}
}

void CEquationSolver::PrintUMatrix() const
{
	if (m_LUmatr == nullptr)
	{
		std::cout << TEXT("No matrix provided") << std::endl;
	}

	for (int i = 0; i < m_N; i++)
	{
		for (int j = 0; j < m_N; j++)
		{
			if (j < i)
			{
				std::cout << 0 << " ";
			}
			else if (j >= i)
			{
				std::cout << m_LUmatr[i][j] << " ";
			}
		}

		std::cout << std::endl;
	}
}

CEquationSolver::CEquationSolver()
{
	trace("Component ctor");
	m_cRef = 0;
	m_N = 0;
	m_LUmatr = nullptr;
	m_pRow = nullptr;
	m_pCol = nullptr;
}

HRESULT CEquationSolver::QueryInterface(const IID& riid, void** ppvObject)
{
	trace("component query interface");
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
		*ppvObject = static_cast<IEquationPrinter*>(this);
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
	trace("Delete matrix");
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
	trace("Deleted");
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
}

extern "C" IUnknown* CreateInstance()
{
	IUnknown* pI = reinterpret_cast<IUnknown*>(new CEquationSolver);
	pI->AddRef();
	return pI;
}

//Factory
class CFactory : public IClassFactory
{
public:
	// IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv);
	virtual ULONG   __stdcall AddRef();
	virtual ULONG   __stdcall Release();

	// Interface IClassFactory
	virtual HRESULT __stdcall CreateInstance(IUnknown* pUnknownOuter,
		const IID& iid,
		void** ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);

	// Constructor
	CFactory() : m_cRef(1) {trace("Factory constructor");}

	// Destructor
	~CFactory() { trace("Class factory:\t\tDestroy self."); }

private:
	long m_cRef;
};

//
// Class factory IUnknown implementation
//
HRESULT __stdcall CFactory::QueryInterface(const IID& iid, void** ppv)
{
	trace("query interface factory");
	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG __stdcall CFactory::AddRef()
{
	trace("add ref factory");
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CFactory::Release()
{
	trace("release ref factory");
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

//
// IClassFactory implementation
//
HRESULT __stdcall CFactory::CreateInstance(IUnknown* pUnknownOuter,
	const IID& iid,
	void** ppv)
{
	trace("Class factory:\t\tCreate component.");

	// Cannot aggregate.
	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	// Create component.
	CEquationSolver* pEq = new CEquationSolver;
	if (pEq == NULL)
	{
		return E_OUTOFMEMORY;
	}
	pEq->AddRef();
	// Get the requested interface.
	HRESULT hr = pEq->QueryInterface(iid, ppv);

	// Release the IUnknown pointer.
	// (If QueryInterface failed, component will delete itself.)
	pEq->Release();
	return hr;
}

// LockServer
HRESULT __stdcall CFactory::LockServer(BOOL bLock)
{
	if (bLock)
	{
		InterlockedIncrement(&g_cServerLocks);
	}
	else
	{
		InterlockedDecrement(&g_cServerLocks);
	}
	return S_OK;
}

///////////////////////////////////////////////////////////
//
// Exported functions
//

//
// Can DLL unload now?
//
STDAPI DllCanUnloadNow()
{
	if ((g_cComponents == 0) && (g_cServerLocks == 0))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

//
// Get class factory
//
STDAPI DllGetClassObject(const CLSID& clsid,
	const IID& iid,
	void** ppv)
{
	trace("DllGetClassObject:\tCreate class factory.");

	// Can we create this component?
	if (clsid != CLSID_EquationsComponent)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	// Create class factory.
	CFactory* pFactory = new CFactory;  // No AddRef in constructor
	if (pFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	// Get requested interface.
	HRESULT hr = pFactory->QueryInterface(iid, ppv);
	pFactory->Release();

	return hr;
}

//
// Server registration
//
STDAPI DllRegisterServer()
{
	return RegisterServer(g_hModule,
		CLSID_EquationsComponent,
		g_szFriendlyName,
		g_szVerIndProgID,
		g_szProgID);
}


//
// Server unregistration
//
STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_EquationsComponent,
		g_szVerIndProgID,
		g_szProgID);
}

///////////////////////////////////////////////////////////
//
// DLL module information
//
BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD dwReason,
	void* lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HMODULE)hModule;
	}
	return TRUE;
}