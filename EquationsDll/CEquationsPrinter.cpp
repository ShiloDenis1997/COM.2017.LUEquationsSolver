#include "CEquationsPrinter.h"

void printTrace(const char* msg) { std::cout << msg << std::endl; }

void CEquationsPrinter::PrintLMatrix() const
{
	if (owner->m_LUmatr == nullptr)
	{
		std::cout << TEXT("No matrix provided") << std::endl;
	}

	for (int i = 0; i < owner->m_N; i++)
	{
		for (int j = 0; j < owner->m_N; j++)
		{
			if (j > i)
			{
				std::cout << 0 << " ";
			}
			else if (j < i)
			{
				std::cout << owner->m_LUmatr[i][j] << " ";
			}
			else
			{
				std::cout << "1 ";
			}
		}

		std::cout << std::endl;
	}
}

void CEquationsPrinter::PrintUMatrix() const
{
	if (owner->m_LUmatr == nullptr)
	{
		std::cout << TEXT("No matrix provided") << std::endl;
	}

	for (int i = 0; i < owner->m_N; i++)
	{
		for (int j = 0; j < owner->m_N; j++)
		{
			if (j < i)
			{
				std::cout << 0 << " ";
			}
			else if (j >= i)
			{
				std::cout << owner->m_LUmatr[i][j] << " ";
			}
		}

		std::cout << std::endl;
	}
}

CEquationsPrinter::CEquationsPrinter(CEquationSolver* ownerComp)
{
	printTrace("Printer interface ctor");
	owner = ownerComp;
	m_cRef = 1;
	owner->AddRef();
}

CEquationsPrinter::~CEquationsPrinter()
{
	printTrace("Printer destroying");
	owner->Release();
}

HRESULT CEquationsPrinter::QueryInterface(const IID& riid, void** ppvObject)
{
	printTrace("component query interface");
	if (riid != IID_IEquationPrinter)
	{
		return owner->QueryInterface(riid, ppvObject);
	}
	else
	{
		*ppvObject = static_cast<IEquationPrinter*>(this);
	}
	
	reinterpret_cast<IUnknown*>(this)->AddRef();
	return S_OK;
}

ULONG CEquationsPrinter::AddRef()
{
	printTrace("Add ref printer");
	owner->AddRef();
	return InterlockedIncrement(&m_cRef);
}

ULONG CEquationsPrinter::Release()
{
	printTrace("Release printer");
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		printTrace("Deleting printer part");
		owner->printerIface = nullptr;
		delete this;
		return 0;
	}
	owner->Release();
	return m_cRef;
}