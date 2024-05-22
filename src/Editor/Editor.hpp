#pragma once

namespace mm
{

class Editor
{
public:
	static void Init() { s_instance = new Editor(); }
	static void DeInit() { delete s_instance; }
	static Editor& Get() { return *s_instance; }

	void OnUpdate(float deltaTime);
	void OnUIRender();

private:
	static Editor* s_instance;
};

}

