#pragma once
#include "BaseService.h"

class NAME_EXPORT AntRanService : public BaseService
{
public:
	AntRanService();
	~AntRanService();
private:
	// Inherited via BaseService
	virtual void OnStart() override;
	virtual void OnPause() override;
	virtual void OnContinue() override;
	virtual void OnStop() override;
	virtual void OnShutdown() override;
};

