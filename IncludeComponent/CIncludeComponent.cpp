#include "CIncludeComponent.h"
#include "Guids.cpp"

void compTrace(const char* msg) { std::cout << msg << std::endl; }

CIncludeComponent::CIncludeComponent()
{
	compTrace("include Component ctor");
	m_cRef = 1;
	m_pIEquationSolver = NULL;
}

HRESULT CIncludeComponent::QueryInterface(const IID& riid, void** ppvObject)
{
	compTrace("component query interface");
	if (riid == IID_IUnknown)
	{
		*ppvObject = static_cast<IWhoAmI*>(this);
	}
	else if (riid == IID_IWhoAmI)
	{
		*ppvObject = static_cast<IWhoAmI*>(this);
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
	reinterpret_cast<IUnknown*>(this)->AddRef();
	return S_OK;
}

ULONG CIncludeComponent::AddRef()
{
	compTrace("include Add ref");
	return InterlockedIncrement(&m_cRef);
}

ULONG CIncludeComponent::Release()
{
	compTrace("include Release");
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		compTrace("include Deleting component");
		delete this;
		return 0;
	}

	return m_cRef;
}

void CIncludeComponent::WhoAmI() const
{
	compTrace("I am a component which includes equations component");
}

bool CIncludeComponent::LoadMatrix(double ** a, int n)
{
	compTrace("include component wrapper load matrix");
	return m_pIEquationSolver->LoadMatrix(a, n);
}

void CIncludeComponent::SolveWithVector(double * b, double * result)
{
	compTrace("include component wrapper solve with vector");
	m_pIEquationSolver->SolveWithVector(b, result);
}

HRESULT CIncludeComponent::Init()
{
	compTrace("Create include component");
	HRESULT hr = ::CoCreateInstance(CLSID_EquationsComponent,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IEquationSolver,
		(void**)&m_pIEquationSolver);
	if (FAILED(hr))
	{
		compTrace("Cannot create include component");
		return E_FAIL;
	}
	else
	{
		return S_OK;
	}
}

CIncludeComponent::~CIncludeComponent()
{
	compTrace("include Component destructor");
	if (m_pIEquationSolver != NULL)
	{
		m_pIEquationSolver->Release();
	}
}