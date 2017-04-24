#include "AgCAggregateComponent.h"
#include "IEquationSolver.h"
#include "Guids.cpp"

void compTrace(const char* msg) { std::cout << msg << std::endl; }

AgCAggregateComponent::AgCAggregateComponent()
{
	compTrace("AComponent: aggregate Component ctor");
	m_cRef = 1;
}

HRESULT AgCAggregateComponent::QueryInterface(const IID& riid, void** ppvObject)
{
	compTrace("AComponent: aggregate component query interface");
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
		if (m_pUnknownInner == nullptr)
			compTrace("AComponent: OH NO! It's null reference");
		return m_pUnknownInner->QueryInterface(riid, ppvObject);
	}
	else
	{
		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(this)->AddRef();
	return S_OK;
}

ULONG AgCAggregateComponent::AddRef()
{
	compTrace("AComponent: aggregate Add ref");
	return InterlockedIncrement(&m_cRef);
}

ULONG AgCAggregateComponent::Release()
{
	compTrace("AComponent: aggregate Release");
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		compTrace("AComponent: aggregate Deleting component");
		delete this;
		return 0;
	}

	return m_cRef;
}

void AgCAggregateComponent::WhoAmI() const
{
	compTrace("AComponent: I am a component which aggregates equations component");
}

HRESULT AgCAggregateComponent::Init()
{
	compTrace("AComponent: AComponent: aggregate component init");
	IUnknown* pUnknownOuter = this;

	compTrace("AComponent: call CoCreateInstance");
	HRESULT hr = CoCreateInstance(CLSID_EquationsComponent,
		pUnknownOuter,
		CLSCTX_INPROC_SERVER,
		IID_IUnknown,
		(void**)&m_pUnknownInner);
	if (FAILED(hr))
	{
		compTrace("AComponent: failed to create aggregated component");
		return E_FAIL;
	}

	compTrace("AComponent: Get the IEquationSolver interface from the inner component.");
	hr = m_pUnknownInner->QueryInterface(IID_IEquationSolver, (void**)&m_pEquationSolver); //@N
	if (FAILED(hr))
	{
		compTrace("AComponent: Inner component does not support interface IEquationSolver.");
		compTrace("AComponent: Call release");
		m_pUnknownInner->Release();
		return E_FAIL;
	}

	// We need to release the reference count added to the
	// outer component in the above call.  So call Release
	// on the pointer you passed to CoCreateInstance.
	compTrace("AComponent: call release");
	pUnknownOuter->Release();
	return S_OK;
}

AgCAggregateComponent::~AgCAggregateComponent()
{
	compTrace("AComponent: aggregate Component destructor");
	::InterlockedDecrement(&g_cComponents);
	compTrace("AComponent: aggregate Destroy self.");

	// Prevent recursive destruction on next AddRef/Release pair.
	m_cRef = 1;

	// Counter the pUnknownOuter->Release in the Init method.
	IUnknown* pUnknownOuter = this;
	pUnknownOuter->AddRef();

	// Properly release the pointer; there might be per-interface
	// reference counts.
	m_pEquationSolver->Release();

	// Release contained component.
	if (m_pUnknownInner != NULL)   // @N
	{
		m_pUnknownInner->Release();
	}
}