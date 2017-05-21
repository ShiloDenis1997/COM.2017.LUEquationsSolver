#pragma once
#include <Unknwn.h>

interface IWhoAmI : IUnknown
{
	virtual ~IWhoAmI()
	{
	}

	virtual void WhoAmI() const = 0;
};

DEFINE_GUID(IID_IWhoAmI,
	0x9ae105bf, 0xae86, 0x4c36,
	0xb2, 0xe3, 0x5f, 0x6f, 0x66, 0x5, 0x36, 0x59);
