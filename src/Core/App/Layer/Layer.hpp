#pragma once

namespace mm
{
	class Application;

	class Layer
	{
	public:
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnUIRender() {}
		virtual	~Layer() {}
	};
}


