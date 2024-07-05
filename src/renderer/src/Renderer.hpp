#pragma once

namespace mm
{

class Renderer : public Singleton<Renderer>
{
public:
	virtual void Startup() override;
	virtual void Shutdown() override;

private:
};

}



