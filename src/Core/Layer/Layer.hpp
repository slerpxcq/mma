#pragma once

namespace mm
{
	class Event;

	class Layer
	{
	public:
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnUIRender() {}
		virtual void OnEvent(Event& e) {};
		virtual	~Layer() {}
	};
}


