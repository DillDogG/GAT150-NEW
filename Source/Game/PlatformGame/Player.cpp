#include "Player.h"
//#include "Renderer/Texture.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
//#include "Framework/Components/EnginePhysicsComponent.h"
#include "Core/Logger.h"

namespace kiko {
	CLASS_DEFINITION(Player);
	bool Player::Initialize() {
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

	void Player::Update(float dt) {
		Actor::Update(dt);
		float dir = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) dir = -1;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) dir = 1;

		kiko::vec2 forward = kiko::vec2{ 1, 0 };

		auto physicsComponent = GetComponent<kiko::PhysicsComponent>();
		physicsComponent->ApplyForce(forward * speed * dir);
		transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());
		bool onGround = (groundCount > 0);
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE) && onGround) {
			vec2 up = vec2{ 0, -1 };
			m_physicsComponent->SetVelocity(up * m_jumpHeight);
		}
		m_immuneTimer -= dt;

		if (m_health < 1) {
			destroyed = true;
			kiko::EventManager::Instance().DispatchEvent("OnPlayerDead", 0);
		}
	}

	void Player::OnCollisionEnter(Actor* other) {
		if (m_immuneTimer <= 0) {
			if (other->tag == "Enemy") {
				m_health--;
				m_immuneTimer = m_immuneTime;
			}
		}
		if (other->tag == "Ground") groundCount++;
	}

	void Player::OnCollisionExit(Actor* other) {
		if (other->tag == "Ground") groundCount--;
	}

	void Player::Read(const json_t& value) {
		Actor::Read(value);

		READ_DATA(value, speed);
		READ_NAME_DATA(value, "health", m_health);
		READ_NAME_DATA(value, "jump", m_jumpHeight);
		READ_NAME_DATA(value, "immunetime", m_immuneTime);
	}
}