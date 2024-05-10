#pragma once

#include "Core/App/Application.hpp"
#include "Core/App/EventBus.hpp"
#include "Core/App/Event.hpp"

#include "Core/MM/Math/Transform.hpp"

#include <dexode/EventBus.hpp>

namespace mm
{
	struct ClipboardContent {
		virtual ~ClipboardContent() {};
	};

	class Clipboard
	{
	public:
		static Clipboard& Instance() { return s_instance; }

		void SetContent(std::unique_ptr<ClipboardContent> content) {
			m_content.reset();
			m_content = std::move(content);
		}

		ClipboardContent* GetContent() { return m_content.get(); }

	private:
		Clipboard() {}

	private:
		std::unique_ptr<ClipboardContent> m_content;
		static Clipboard s_instance;
	};
}

