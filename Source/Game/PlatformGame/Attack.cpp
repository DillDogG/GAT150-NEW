#include "Attack.h"
#include "Renderer/Renderer.h"

namespace kiko {
	CLASS_DEFINITION(Attack);
	bool Attack::Initialize() {
		Actor::Initialize();
		m_physicsComponent = GetComponent<PhysicsComponent>();

		//auto collisionComponent = GetComponent<kiko::CollisionComponent>();
		return true;
	}

	void Attack::Update(float dt) {
		Actor::Update(dt);
	}

	void Attack::OnCollisionEnter(Actor* other) {
		
	}

	void Attack::Read(const json_t& value) {
		Actor::Read(value);
		READ_DATA(value, damage);
	}
}