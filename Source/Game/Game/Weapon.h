#pragma once
#include "Framework/Actor.h"
#include "Framework/Framework.h"

namespace kiko {
	class Weapon : Actor {
	public:
		/* Weapon(float speed, const kiko::Transform& transform) :
			Actor{ transform },
			speed{ speed }
		{ lifespan = 2.0f; } */
		bool Initialize() override;
		void Update(float dt) override;
		//void OnCollision(Actor* other) override;
		void Read(const json_t& value);
	private:
		float speed = 0;
		kiko::PhysicsComponent* m_physicsComponent = nullptr;
	};
}