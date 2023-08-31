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
		/* vec2 dir = 0;
		kiko::vec2 forward = kiko::vec2{ 1, 0 };
		Player* player = m_scene->GetActor<Player>();
		if (player) {
			dir = (player->transform.position + 45) - transform.position;
			dir.x = speed * dir.x;
			dir.x = Clamp(dir.x, -maxSpeed, maxSpeed);
			m_physicsComponent->SetVelocity(dir);
			
		} */

		kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
		float angle = 0;
		Player* player = m_scene->GetActor<Player>();
		if (player) {
			Vector2 direction = player->transform.position - transform.position;
			float turnAngle = vec2::SignedAngle(forward, direction.Normalized());
			//transform.rotation += turnAngle * 3 * dt;
			m_physicsComponent->ApplyTorque(turnAngle * 0.3f);

			angle = vec2::Angle(forward, direction.Normalized());

		}
		m_physicsComponent->ApplyForce(forward * speed);
		//auto physicsComponent = GetComponent<kiko::PhysicsComponent>();
		// check if position is off screen, if so wrap the position and set physics component to new position
		if ((transform.position.x < 0 || transform.position.x >(float)kiko::g_renderer.GetWidth()) || (transform.position.y < 0 || transform.position.y >(float)kiko::g_renderer.GetHeight())) {
			transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
			transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());

			m_physicsComponent->SetPosition(transform.position);
		}
		bool onGround = (groundCount > 0);
		m_immuneTimer -= dt;

		if (m_health < 1) {
			destroyed = true;
		}
	}

	void Enemy::OnCollisionEnter(Actor* other) {
		if (m_immuneTimer <= 0) {
			if (other->tag == "pAttackL") {
				m_health -= 15;
				m_immuneTimer = m_immuneTime;
			}
			else if (other->tag == "pAttackM") {
				m_health -= 30;
				m_immuneTimer = m_immuneTime;
			}
			else if (other->tag == "pAttackH") {
				m_health -= 45;
				m_immuneTimer = m_immuneTime;
			}
			else if (other->tag == "Coin") {
				float cSpeed = other->GetComponent<kiko::PhysicsComponent>()->m_velocity.x;
				if (cSpeed > 5.0f) {
					m_health -= cSpeed * 2;
					INFO_LOG("COIN CRIT! DEALT " << cSpeed * 3 << " DAMAGE");
					m_immuneTimer = m_immuneTime;
				}
			}
		}
	}

	void Enemy::OnCollisionExit(Actor* other) {
	}

	void Enemy::Read(const json_t& value) {
		Actor::Read(value);

		READ_DATA(value, speed);
		READ_DATA(value, maxSpeed);
		READ_NAME_DATA(value, "health", m_health);
		READ_NAME_DATA(value, "jump", m_jumpHeight);
		READ_NAME_DATA(value, "immunetime", m_immuneTime);
	}
}