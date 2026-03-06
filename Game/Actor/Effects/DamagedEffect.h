#pragma once
#include "Actor/Actor.h"
#include "Util/Util.h"
#include "Util/Timer.h"


using namespace Wanted;

class DamagedEffect : public Actor
{
	RTTI_DECLARATIONS(DamagedEffect, Actor)

		// 애니메이션 이펙트 프레임 구조체.
		struct EffectFrame
	{
		EffectFrame(
			const char* frame,
			float playTime = 0.05f,
			Color color = Color::Red)
			: playTime(playTime), color(color)
		{
			// 문자열 설정.
			size_t length = strlen(frame) + 1;
			this->frame = new char[length];
			strcpy_s(this->frame, length, frame);
		}

		~EffectFrame()
		{
			SafeDeleteArray(frame);
		}

		// 문자열 변수 (화면에 보여줄 문자열).
		char* frame = nullptr;

		// 재생 시간.
		float playTime = 0.0f;

		// 색상.
		Color color = Color::White;
	};

public:
	DamagedEffect(const Vector2& position, const Vector2& moveDirection, float moveSpeed = 1.0f);
	~DamagedEffect();

	virtual void Tick(float deltaTime) override;

private:

	// 시퀀스 문자열 수(배열 원소 수).
	int effectSequenceCount = 0;

	// 현재 보여지는 시퀀스 인덱스.
	int currentSequenceIndex = 0;

	Vector2 moveDirection;

	// 애니메이션 재생에 사용할 타이머.
	// 시퀀스 사이에 시간 계산용.
	Timer timer;

	float moveSpeed = 5.0f;
	float xPosition = 0.0f;
	float yPosition = 0.0f;

	// 효과 재생에 사용할 문자열 시퀀스 (일종의 애니메이션 프레임).
	DamagedEffect::EffectFrame sequence[12] = {
		DamagedEffect::EffectFrame("*", 1.3f, Color::Red),
		DamagedEffect::EffectFrame("*", 1.3f, Color::Blue),
		DamagedEffect::EffectFrame("*", 1.3f, Color::Green),
		DamagedEffect::EffectFrame("*", 1.3f, Color::Red),
		DamagedEffect::EffectFrame("*", 1.3f, Color::Red),
		DamagedEffect::EffectFrame("*", 1.3f, Color::Blue),
		DamagedEffect::EffectFrame("*", 1.3f, Color::Green),
		DamagedEffect::EffectFrame("*", 1.3f, Color::Red),
		DamagedEffect::EffectFrame("*", 1.3f, Color::Red),
		DamagedEffect::EffectFrame("*", 1.3f, Color::Blue),
		DamagedEffect::EffectFrame("*", 1.3f, Color::Green),
		DamagedEffect::EffectFrame("*", 1.3f, Color::Red),
	};
};

