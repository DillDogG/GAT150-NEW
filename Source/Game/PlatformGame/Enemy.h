#pragma once
#include "Framework/Framework.h"

namespace kiko {
	class Enemy : public kiko::Actor {
	public:
		CLASS_DECLARATION(Enemy);

		bool Initialize() override;
		void Update(float dt) override;
		void OnCollisionEnter(Actor* other) override;
		void OnCollisionExit(Actor* other) override;
		float GetHealth() { return m_health; }

	private:
		float speed = 0;
		float m_health = 0;
		int groundCount = 0;

		float m_jumpHeight = 0;
		//int m_missileCount = 0;
		//float m_missileRate = 0;
		//float m_missileTimer = 0;
		//float m_adrenaline = 0;
		float m_immuneTime = 0;
		float m_immuneTimer = 0;
		//float m_powerTimer = 0;
		//
		//bool m_shield = false;
		//bool m_multi = false;

		PhysicsComponent* m_physicsComponent = nullptr;
		SpriteComponent* m_spriteComponent = nullptr;
	};
}