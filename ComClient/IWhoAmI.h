#pragma once
#include <Unknwn.h>

interface IWhoAmI : IUnknown
{
	virtual ~IWhoAmI()
	{
	}

	virtual void WhoAmI() const = 0;
};