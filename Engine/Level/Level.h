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

		template <typename T>
		Actor* GetActorBy() {
			// 컴파일 타임에 타입의 고유 ID(size_t)를 추출하여 cpp 함수로 전달
			Actor* foundActor = GetFirstActorOfRTTI(T::TypeIdClass());

			if (foundActor != nullptr)
			{
				// cpp 내부의 Is(size_t) 검사로 타입 안전성이 보장되었으므로 static_cast 사용
				return foundActor;
			}
			return nullptr;
		}

		virtual std::vector<std::vector<int>>& Grid() = 0;

	protected:
		std::vector<Actor*> actors;

		// 실행 중에 추가 요청된 액터의 배열.
		std::vector<Actor*> addRequestedActors;

		// 핵심 검색 로직을 담당하는 비템플릿 함수
		Actor* GetFirstActorOfRTTI(const size_t typeId);

		std::vector<std::vector<int>> grid;
	};

}

