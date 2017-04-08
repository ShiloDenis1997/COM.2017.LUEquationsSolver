#pragma once
#include <Unknwn.h>

interface IEquationPrinter : IUnknown
{
	virtual ~IEquationPrinter()
	{
	}

	virtual void PrintLMatrix() const = 0;
	virtual void PrintUMatrix() const = 0;
};

DEFINE_GUID(IID_IEquationPrinter,
	0x6568d656, 0x3161, 0x4708,
	0x8f, 0x91, 0xcb, 0x40, 0x72, 0xd7, 0xf8, 0x51);