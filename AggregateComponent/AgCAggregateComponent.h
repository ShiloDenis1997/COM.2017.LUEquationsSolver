#pragma once
#include <Unknwn.h>
#include "IWhoAmI.h"
#include "AggregateComponent.h"
#include <iostream>

class AgCAggregateComponent : public IWhoAmI
{
public:
	AgCAggregateComponent();
	~AgCAggregateComponent() override;
	HRESULT __stdcall QueryInterface(const IID& riid, void** ppvObject) override;
	ULONG __stdcall AddRef() override;
	ULONG __stdcall Release() override;
	void WhoAmI() const override;
private:
	long m_cRef;
};