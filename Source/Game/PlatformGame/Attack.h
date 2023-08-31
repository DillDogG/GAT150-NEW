#pragma once
#include "Framework/Framework.h"

namespace kiko {
	class Attack : public kiko::Actor {
	public:
		CLASS_DECLARATION(Attack);
		bool Initialize() override;
		void Update(float dt) override;
		void OnCollisionEnter(Actor* other) override;
		//void Read(const json_t& value);
	private:
		float damage = 0;
		kiko::PhysicsComponent* m_physicsComponent = nullptr;
	};
}