#pragma once



class Timer
{
public:
	Timer(float targetTime = 1.0f);

	//  타이머 업데이트 함수.
	void Tick(float deltaTIme);

	void Reset();

	bool IsTimeOut() const;

	void SetTargetTime(float newTargetTime);

private:
	float elapsedTime = 0.0f;
	float targetTime = 0.0f;
};

