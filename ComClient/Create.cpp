#include <iostream>
#include <unknwn.h>    // Declare IUnknown.
using namespace std;

#include "Create.h"

typedef IUnknown* (*CREATEFUNCPTR)();

IUnknown* CallCreateInstance(LPCWSTR name)
{
	// Load dynamic link library into process.
	HINSTANCE hComponent = ::LoadLibrary(name);
	if (hComponent == nullptr)
	{
		cout << "CallCreateInstance:\tError: Cannot load component." << endl;
		return nullptr;
	}

	// Get address for CreateInstance function.
	CREATEFUNCPTR CreateInstance
		= reinterpret_cast<CREATEFUNCPTR>(::GetProcAddress(hComponent, "CreateInstance"));
	if (CreateInstance == nullptr)
	{
		cout << "CallCreateInstance:\tError: "
			<< "Cannot find CreateInstance function."
			<< endl;
		return nullptr;
	}

	return CreateInstance();
}
