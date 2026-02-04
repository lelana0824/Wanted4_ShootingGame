#pragma once

#include <vector>
#include "Common/RTTI.h"

namespace Wanted {
	class  Actor;

	// 담당임무: 레벨에 있는 모든 액터 관리.
	class WANTED_API Level : public RTTI
	{
		RTTI_DECLARATIONS(Level, RTTI)
	public:
		Level();
		virtual ~Level();

		// 게임 플레이 이벤트
		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		void AddNewActor(Actor* newActor);

		// 액터 추가제거 처리함수
		void ProcessAddAndDestroyActors();

	protected:
		std::vector<Actor*> actors;

		// 실행 중에 추가 요청된 액터의 배열.
		std::vector<Actor*> addRequestedActors;
	};

}

