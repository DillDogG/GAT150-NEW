#pragma once
#include "Framework/Framework.h"

namespace kiko {
	class Weapon : public kiko::Actor {
	public:
		CLASS_DECLARATION(Weapon)
		bool Initialize() override;
		void Update(float dt) override;
		void OnCollision(Actor* other) override;
		//void Read(const json_t& value);
	private:
		float speed = 0;
		kiko::PhysicsComponent* m_physicsComponent = nullptr;
	};
}