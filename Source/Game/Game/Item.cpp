#include "Item.h"

namespace kiko {
	CLASS_DEFINITION(Item);
	bool Item::Initialize() {
		Actor::Initialize();
		m_physicsComponent = GetComponent<kiko::PhysicsComponent>();
		auto collisionComponent = GetComponent<kiko::CollisionComponent>();
		if (collisionComponent) {
			auto renderComponent = GetComponent<kiko::RenderComponent>();
			if (renderComponent) {
				float scale = transform.scale;
				collisionComponent->m_radius = GetComponent<kiko::RenderComponent>()->GetRadius() * scale;
			}
		}
		return true;
	}

	void Item::Update(float dt) {
		Actor::Update(dt);
		transform.rotation -= 0.03125f;

	}

	void Item::OnCollision(Actor* other) {
		if (other->tag == "Player") destroyed = true;
	}

	void Item::Read(const json_t& value) {
		Actor::Read(value);
	}
}