#pragma once
#include "Framework/Framework.h"

namespace kiko {
	class Astroid : public kiko::Actor {
	public:
		CLASS_DECLARATION(Astroid);
		bool Initialize() override;
		void Update(float dt) override;
		void OnCollision(Actor* other) override;
	private:
		float speed = 0;
		int m_size = 0;
		PhysicsComponent* m_physicsComponent = nullptr;
		SpriteComponent* m_spriteComponent = nullptr;
	};
}