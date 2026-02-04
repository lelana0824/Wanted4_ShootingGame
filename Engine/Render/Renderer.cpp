#include "Renderer.h"
#include "ScreenBuffer.h"
#include "Util/Util.h"

namespace Wanted {
	Renderer::Frame::Frame(int bufferCount)
	{
		charInfoArray = new CHAR_INFO[bufferCount];
		memset(charInfoArray, 0, sizeof(CHAR_INFO) * bufferCount);

		sortingOrderArray = new int[bufferCount];
		memset(sortingOrderArray, 0, sizeof(int) * bufferCount);
	}

	Renderer::Frame::~Frame()
	{
		SafeDeleteArray(charInfoArray);
		SafeDeleteArray(sortingOrderArray);
	}

	void Renderer::Frame::Clear(const Vector2& screenSize)
	{
		const int width = screenSize.x;
		const int height = screenSize.y;

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				const int index = (y * width) + x;

				CHAR_INFO& info = charInfoArray[index];
				info.Char.AsciiChar = ' ';
				info.Attributes = 0;

				sortingOrderArray[index] = -1;
			}
		}
	}

	// --------------- Frame ------------ //

	Renderer* Renderer::instance = nullptr;

	Renderer::Renderer(const Vector2& screenSize)
		:screenSize(screenSize)
	{
		instance = this;

		const int bufferCount = screenSize.x * screenSize.y;
		frame = new Frame(bufferCount);

		frame->Clear(screenSize);

		screenBuffers[0] = new ScreenBuffer(screenSize);
		screenBuffers[0]->Clear();

		screenBuffers[1] = new ScreenBuffer(screenSize);
		screenBuffers[1]->Clear();

		Present();
	}

	Renderer::~Renderer()
	{
		SafeDelete(frame);
		for (ScreenBuffer*& buffer : screenBuffers)
		{
			SafeDelete(buffer);
		}
	}

	void Renderer::Draw()
	{
		// 화면 지우기.
		Clear();

		// 전제조건: 레벨의 모든 액터가 렌더러에 Submit을 완료.
		// 렌더큐 순회하면서 프레임 채우기.
		for (const RenderCommand& command : renderQueue)
		{
			if (!command.text)
			{
				continue;
			}

			if (command.position.y < 0
				|| command.position.y >= screenSize.y)
			{
				continue;
			}

			// 화면에 그릴 문자열 길이.
			const int length = static_cast<int>(strlen(command.text));

			if (length <= 0)
			{
				continue;
			}

			const int startX = command.position.x;
			const int endX = startX + length - 1;

			if (endX < 0 || startX >= screenSize.x)
			{
				continue;
			}

			// 시작 위치.
			const int visibleStart = startX < 0 ? 0 : startX;
			const int visibleEnd
				= endX >= screenSize.x ? screenSize.x - 1 : endX;

			// 문자열 설정.
			for (int x = visibleStart; x <= visibleEnd; ++x)
			{
				// 문자열 안의 문자 인덱스.
				const int sourceIndex = x - startX;

				// 프레임 (2차원 문자 배열) 인덱스.
				const int index
					= (command.position.y * screenSize.x) + x;

				// 그리기 우선순위 비교
				if (frame->sortingOrderArray[index]
			> command.sortingOrder)
				{
					continue;
				}

				// 데이터 기록.
				frame->charInfoArray[index].Char.AsciiChar
					= command.text[sourceIndex];
				frame->charInfoArray[index].Attributes
					= (WORD)(command.color);

				// 우선순위 업데이트
				frame->sortingOrderArray[index]
					= command.sortingOrder;
			}
		}
		
		// 그리기
		GetCurrentBuffer()->Draw(frame->charInfoArray);

		// 버퍼 교환
		Present();

		// 렌더큐 비우기.
		renderQueue.clear();
	}

	Renderer& Renderer::Get()
	{
		if (!instance)
		{
			MessageBoxA(
				nullptr,
				"Renderer::Get() - instance is null",
				"Error",
				MB_OK
			);

			__debugbreak();
		}

		return *instance;
	}

	void Renderer::Clear()
	{
		// 화면 지우기
		// 1. 프레임(2차원 배열 데이터) 지우기
		frame->Clear(screenSize);

		// 2. 콘솔 버퍼 지우기.
		GetCurrentBuffer()->Clear();
	}

	void Renderer::Submit(
		const char* text, 
		const Vector2& position,
		Color color,
		int sortingOrder)
	{
		// 렌더 데이터 생성 후 큐에 추가.
		RenderCommand command = {};
		command.text = text;
		command.position = position;
		command.color = color;
		command.sortingOrder = sortingOrder;

		renderQueue.emplace_back(command);
	}

	void Renderer::Present()
	{
		// 버퍼 교환.
		SetConsoleActiveScreenBuffer(
			GetCurrentBuffer()->GetBuffer()
		);

		// 인덱스 교체.
		currentBufferIndex = 1 - currentBufferIndex;
	}

	ScreenBuffer* Renderer::GetCurrentBuffer()
	{
		return screenBuffers[currentBufferIndex];
	}
}