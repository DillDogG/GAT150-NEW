#pragma once
#include "Framework/Framework.h"

namespace kiko {
	class Player : public kiko::Actor {
	public:
		CLASS_DECLARATION(Player);
		/* Player(float speed, float turnRate, const Transform& transform) :
			Actor{ transform },
			speed{ speed },
			m_turnRate{ turnRate }
		{
			m_fireRate = 1;
			m_fireTimer = m_fireRate;
			m_missileCount = 5;
			m_missileRate = 0.5f;
			m_missileTimer = m_missileRate;
			m_adrenaline = 50;
			m_immuneTime = 2.5f;
			m_immuneTimer = m_immuneTime;
			m_health = 2;
		} */

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