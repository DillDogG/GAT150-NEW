#include "Weapon.h"
#include "Renderer/Renderer.h"

namespace kiko {
	CLASS_DEFINITION(Weapon);
	bool Weapon::Initialize() {
		Actor::Initialize();

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

	void Weapon::Update(float dt) {
		Actor::Update(dt);
		kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
		transform.position += forward * speed * kiko::g_time.GetDeltaTime();
		transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());
	}

	void Weapon::OnCollision(Actor* other) {
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