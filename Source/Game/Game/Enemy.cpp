#include "Enemy.h"
#include "Player.h"
#include "SpaceGame.h"
#include "Weapon.h"
#include "Renderer/Renderer.h"

namespace kiko {
	CLASS_DEFINITION(Enemy);
	bool Enemy::Initialize() {
		Actor::Initialize();
		m_physicsComponent = GetComponent<PhysicsComponent>();
		auto collisionComponent = GetComponent<CollisionComponent>();
		if (collisionComponent) {
			auto renderComponent = GetComponent<RenderComponent>();
			if (renderComponent) {
				float scale = transform.scale;
				collisionComponent->m_radius = GetComponent<RenderComponent>()->GetRadius() * scale;
			}
		}
		return true;
	}
	void Enemy::Update(float dt) {
		Actor::Update(dt);


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
		//transform.position += forward * speed * kiko::g_time.GetDeltaTime();
		transform.position.x = Wrap(transform.position.x, (float)g_renderer.GetWidth());
		transform.position.y = Wrap(transform.position.y, (float)g_renderer.GetHeight());

		if (m_fireTimer < 0 && angle < kiko::DegreesToRadians(30.0f)) {
			auto weapon = INSTANTIATE(Weapon, "RocketE");
			weapon->transform = { this->transform.position, this->transform.rotation, 1 };
			weapon->Initialize();
			m_scene->Add(std::move(weapon));
			m_fireTimer = m_fireRate;
			/* kiko::Transform transform { this->transform.position, this->transform.rotation, 1 };
			std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, transform);
			weapon->tag = "eWeapon";
			std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
			component->m_texture = GET_RESOURCE(kiko::Texture, "Bullet.png", kiko::g_renderer);
			weapon->AddComponent(std::move(component));
			auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 15.f;
			weapon->AddComponent(std::move(collisionComponent));
			weapon->Initialize();
			m_scene->Add(std::move(weapon));
			m_fireTimer = m_fireRate; */
		}
		else {
			m_fireTimer -= dt;
		}
	}

	void Enemy::OnCollisionEnter(Actor* other) {
		if (other->tag == "pWeapon" || other->tag == "Astroid" || other->tag == "pAstroid") {
			if (!destroyed) {
				//m_game->AddPoints(100);
				EventManager::Instance().DispatchEvent("AddPoints", 100);
				destroyed = true;
				EmitterData data;
				data.burst = true;
				data.burstCount = 100;
				data.spawnRate = 200;
				data.angle = 0;
				data.angleRange = Pi;
				data.lifetimeMin = 0.5f;
				data.lifetimeMax = 1.5f;
				data.speedMin = 50;
				data.speedMax = 250;
				data.damping = 0.5f;
				data.color = Color{ 0, 1, 1, 1 };
				auto emitter = std::make_unique<Emitter>(transform, data);
				emitter->lifespan = 1.0f;
				m_scene->Add(std::move(emitter));
			}
		}
	}

	void Enemy::Read(const json_t& value) {
		Actor::Read(value);
		READ_DATA(value, speed);
		READ_NAME_DATA(value, "turnrate", m_turnRate);
		READ_NAME_DATA(value, "firerate", m_fireRate);
	}
}