#pragma once
#include "Framework/Framework.h"

namespace kiko {
	class Item : public kiko::Actor {
	public:
		CLASS_DECLARATION(Item);
		bool Initialize() override;
		void Update(float dt) override;
		void OnCollisionEnter(Actor* other) override;
	private:
		PhysicsComponent* m_physicsComponent = nullptr;
		SpriteComponent* m_spriteComponent = nullptr;
	};
}