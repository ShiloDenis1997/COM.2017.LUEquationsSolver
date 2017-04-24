#pragma once
#include <Unknwn.h>

struct INondelegatingUnknown
{
	virtual HRESULT __stdcall
		NondelegatingQueryInterface(const IID&, void**) = 0;
	virtual ULONG __stdcall NondelegatingAddRef() = 0;
	virtual ULONG __stdcall NondelegatingRelease() = 0;
};