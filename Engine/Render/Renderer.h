#pragma once

#include "Math/Vector2.h"
#include "Math/Color.h"
#include <Windows.h>
#include <vector>

namespace Wanted
{
	class ScreenBuffer;

	class WANTED_API Renderer
	{
		struct Frame
		{
			Frame(int bufferCount);
			~Frame();

			void Clear(const Vector2& screenSize);

			CHAR_INFO* charInfoArray = nullptr;
			int* sortingOrderArray = nullptr;
		};

		struct RenderCommand
		{
			const char* text = nullptr;

			Vector2 position;

			Color color = Color::White;

			int sortingOrder = 0;
		};

	public:
		Renderer(const Vector2& screenSize);
		~Renderer();

		void Draw();

		void Submit(
			const char* text,
			const Vector2& position,
			Color color = Color::White,
			int sortingOrder = 0
		);

		static Renderer& Get();

	private:
		void Clear();

		void Present();

		ScreenBuffer* GetCurrentBuffer();

	private:
		Vector2 screenSize;

		Frame* frame = nullptr;

		ScreenBuffer* screenBuffers[2] = {};

		int currentBufferIndex = 0;

		std::vector<RenderCommand> renderQueue;

		static Renderer* instance;
	};
}

