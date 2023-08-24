#include "Weapon.h"
#include "Renderer/Renderer.h"

namespace kiko {
	CLASS_DEFINITION(Weapon);
	bool Weapon::Initialize() {
		Actor::Initialize();
		
		m_physicsComponent = GetComponent<PhysicsComponent>();

		auto collisionComponent = GetComponent<kiko::CollisionComponent>();
		if (collisionComponent) {
			
		}
		return true;
	}

	void Weapon::Update(float dt) {
		Actor::Update(dt);
		kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
		m_physicsComponent->SetVelocity(forward * speed);
		transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());
	}

	void Weapon::OnCollisionEnter(Actor* other) {
		if (tag == "pWeapon" && other->tag == "Enemy") destroyed = true;
		if (tag == "eWeapon" && other->tag == "Player") destroyed = true;
		if (tag == "eWeapon" && other->tag == "pWeapon") destroyed = true;
		if (tag == "pWeapon" && other->tag == "eWeapon") destroyed = true;
		if (other->tag == "Astroid") destroyed = true;
	}

	void Weapon::Read(const json_t& value) {
		Actor::Read(value);
		READ_DATA(value, speed);
	}
}