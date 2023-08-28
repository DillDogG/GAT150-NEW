#include "Enemy.h"
#include "Player.h"
//#include "Renderer/Texture.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
//#include "Framework/Components/EnginePhysicsComponent.h"
#include "Core/Logger.h"

namespace kiko {
	CLASS_DEFINITION(Enemy);
	bool Enemy::Initialize() {
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

	void Enemy::Update(float dt) {
		Actor::Update(dt);
		vec2 dir = 0;
		kiko::vec2 forward = kiko::vec2{ 1, 0 };
		Player* player = m_scene->GetActor<Player>();
		if (player) {
			dir = player->transform.position - transform.position;
		}


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

		//if (m_health < 1) {
		//	destroyed = true;
		//	kiko::EventManager::Instance().DispatchEvent("OnPlayerDead", 0);
		//}
	}

	void Enemy::OnCollisionEnter(Actor* other) {
		if (m_immuneTimer <= 0) {
			if (other->tag == "Player") {
				m_health--;
				m_immuneTimer = m_immuneTime;
			}
		}
		if (other->tag == "Ground") groundCount++;
	}

	void Enemy::OnCollisionExit(Actor* other) {
		if (other->tag == "Ground") groundCount--;
	}

	void Enemy::Read(const json_t& value) {
		Actor::Read(value);

		READ_DATA(value, speed);
		READ_NAME_DATA(value, "health", m_health);
		READ_NAME_DATA(value, "jump", m_jumpHeight);
		READ_NAME_DATA(value, "immunetime", m_immuneTime);
	}
}