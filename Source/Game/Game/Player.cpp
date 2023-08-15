#include "Player.h"
#include "Weapon.h"
#include "SpaceGame.h"
#include "Framework/Framework.h"
#include "Renderer/Texture.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Core/Logger.h"

bool Player::Initialize() {
	Actor::Initialize();
	m_physicsComponent = GetComponent<kiko::PhysicsComponent>();
	auto collisionComponent = GetComponent<kiko::CollisionComponent>();
	if (collisionComponent) {
		auto renderComponent = GetComponent<kiko::RenderComponent>();
		if (renderComponent) {
			float scale = m_transform.scale;
			collisionComponent->m_radius = GetComponent<kiko::RenderComponent>()->GetRadius() * scale;
		}
	}
	return true;
}

void Player::Update(float dt) {
	Actor::Update(dt);
	float rotate = 0;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
	m_transform.rotation += rotate * m_turnRate * kiko::g_time.GetDeltaTime();

	float thrust = 0;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;

	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(m_transform.rotation);
	//AddForce(forward * m_speed * thrust);

	auto physicsComponent = GetComponent<kiko::PhysicsComponent>();
	physicsComponent->ApplyForce(forward * m_speed * thrust);
	//m_transform.position += forward * m_speed * thrust * kiko::g_time.GetDeltaTime();
	m_transform.position.x = kiko::Wrap(m_transform.position.x, (float)kiko::g_renderer.GetWidth());
	m_transform.position.y = kiko::Wrap(m_transform.position.y, (float)kiko::g_renderer.GetHeight());
	if (m_fireTimer < 0) {
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && m_multi) {
			kiko::Transform transform { m_transform.position, m_transform.rotation, 1 };
			std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, m_transform);
			weapon->m_tag = "pWeapon";
			std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
			component->m_texture = GET_RESOURCE(kiko::Texture, "Bullet.png", kiko::g_renderer);
			weapon->AddComponent(std::move(component));
			auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 15.f;
			weapon->AddComponent(std::move(collisionComponent));
			weapon->Initialize();
			m_scene->Add(std::move(weapon));
			weapon = std::make_unique<Weapon>(400.0f, m_transform);
			weapon->m_transform.rotation += 0.15f;
			weapon->m_tag = "pWeapon";
			component = std::make_unique<kiko::SpriteComponent>();
			component->m_texture = GET_RESOURCE(kiko::Texture, "Bullet.png", kiko::g_renderer);
			weapon->AddComponent(std::move(component));
			collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 15.f;
			weapon->AddComponent(std::move(collisionComponent));
			weapon->Initialize();
			m_scene->Add(std::move(weapon));
			weapon = std::make_unique<Weapon>(400.0f, m_transform);
			weapon->m_transform.rotation -= 0.15f;
			weapon->m_tag = "pWeapon";
			component = std::make_unique<kiko::SpriteComponent>();
			component->m_texture = GET_RESOURCE(kiko::Texture, "Bullet.png", kiko::g_renderer);
			weapon->AddComponent(std::move(component));
			collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 15.f;
			weapon->AddComponent(std::move(collisionComponent));
			weapon->Initialize();
			m_scene->Add(std::move(weapon));
			m_fireTimer = m_fireRate;
		} else if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE)) {
			kiko::Transform transform { m_transform.position, m_transform.rotation, 1 };
			std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, m_transform);
			weapon->m_tag = "pWeapon";
			std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
			component->m_texture = GET_RESOURCE(kiko::Texture, "Bullet.png", kiko::g_renderer);
			weapon->AddComponent(std::move(component));
			auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 15.f;
			weapon->AddComponent(std::move(collisionComponent));
			weapon->Initialize();
			m_scene->Add(std::move(weapon));
			m_fireTimer = m_fireRate;
		}
	} else {
		m_fireTimer -= dt;
	}
	m_immuneTimer -= dt;
	
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_LSHIFT) && m_adrenaline > 0) {
		kiko::g_time.SetTimeScale(0.5f);
		m_adrenaline -= (kiko::g_time.GetUnscaledDeltaTime() * 5);
	} else kiko::g_time.SetTimeScale(1.0f);

	if (m_missileTimer < 0 && m_missileCount > 0) {
		if (kiko::g_inputSystem.GetMouseButtonDown(0) == 1) {
			kiko::EmitterData data;
			data.burst = true;
			data.burstCount = 100;
			data.spawnRate = 200;
			data.angle = 0;
			data.angleRange = kiko::Pi;
			data.lifetimeMin = 0.25f;
			data.lifetimeMax = 0.5f;
			data.speedMin = 25;
			data.speedMax = 150;
			data.damping = 0.5f;
			data.color = kiko::Color{ 1, 0, 0, 1 };
			kiko::Transform transformer{ { kiko::g_inputSystem.GetMousePosition() }, 0, 1 };
			auto emitter = std::make_unique<kiko::Emitter>(transformer, data);
			emitter->m_lifespan = 0.5f;
			emitter->m_tag = "pWeapon";
			auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 30.f;
			emitter->AddComponent(std::move(collisionComponent));
			emitter->Initialize();
			m_scene->Add(std::move(emitter));
			m_missileCount--;
			m_missileTimer = m_missileRate;
		}
	} else {
		m_missileTimer -= dt;
	}

	if (m_powerTimer < 0) {
		m_fireRate = 1;
		m_shield = false;
		m_multi = false;
	} else {
		m_powerTimer -= dt;
	}
	if (m_adrenaline < 50) m_adrenaline += kiko::g_time.GetUnscaledDeltaTime();

	if (m_health < 1) {
		m_game->SetLives(m_game->GetLives() - 1);
		dynamic_cast<SpaceGame*>(m_game)->SetState(SpaceGame::eState::PlayerDeadStart);
		m_destroyed = true;
	}
}

void Player::OnCollision(Actor* other) {
	if (m_immuneTimer <= 0 && !m_shield) {
		if (other->m_tag == "eWeapon" || other->m_tag == "Enemy" || other->m_tag == "eAstroid") {
			m_health--;
			m_immuneTimer = m_immuneTime;
		}
	}
	if (other->m_tag == "Rapid") {
		m_fireRate = 0.25f;
		m_powerTimer = 15;
		INFO_LOG("Rapid")
	}
	if (other->m_tag == "Multi") {
		m_powerTimer = 15;
		m_multi = true;
		INFO_LOG("Multi-Shot")
	}
	if (other->m_tag == "Shield") {
		m_powerTimer = 15;
		m_shield = true;
		INFO_LOG("Shield")
	}
	if (other->m_tag == "Missile") {
		m_missileCount++;
		INFO_LOG("Missile Pickup")
	}
	if (other->m_tag == "Health") {
		m_health++;
		if (m_health > 5) m_health = 5;
		INFO_LOG("Health Pickup")
	}
}
