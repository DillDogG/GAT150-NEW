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
		void OnCollision(Actor* other) override;
		int GetHealth() { return m_health; }
		int GetMissile() { return m_missileCount; }
		float GetAdrenaline() { return m_adrenaline; }

	private:
		float speed = 0;
		float m_turnRate = 0;
		int m_health = 0;

		float m_fireRate = 1;
		float m_fireTimer = 0;
		int m_missileCount = 5;
		float m_missileRate = 0;
		float m_missileTimer = 0;
		float m_adrenaline = 50;
		float m_immuneTime = 2.5f;
		float m_immuneTimer = 0;
		float m_powerTimer = 0;

		bool m_shield = false;
		bool m_multi = false;

		PhysicsComponent* m_physicsComponent = nullptr;
		SpriteComponent* m_spriteComponent = nullptr;
	};
}