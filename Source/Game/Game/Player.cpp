#include "Player.h"
#include "Weapon.h"
#include "SpaceGame.h"
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
			float scale = transform.scale;
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
	transform.rotation += rotate * m_turnRate * kiko::g_time.GetDeltaTime();

	float thrust = 0;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;

	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
	//AddForce(forward * speed * thrust);

	auto physicsComponent = GetComponent<kiko::PhysicsComponent>();
	physicsComponent->ApplyForce(forward * speed * thrust);
	//transform.position += forward * speed * thrust * kiko::g_time.GetDeltaTime();
	transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
	transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());
	if (m_fireTimer < 0) {
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && m_multi) {
			auto weapon = INSTANTIATE(Weapon, "RocketP");
			weapon->transform = { transform.position, transform.rotation, 1 };
			weapon->Initialize();
			m_scene->Add(std::move(weapon));
			weapon = INSTANTIATE(Weapon, "RocketP");
			weapon->transform = { transform.position, transform.rotation, 1 };
			weapon->Initialize();
			m_scene->Add(std::move(weapon));
			weapon = INSTANTIATE(Weapon, "RocketP");
			weapon->transform = { transform.position, transform.rotation, 1 };
			weapon->Initialize();
			m_scene->Add(std::move(weapon));
			m_fireTimer = m_fireRate;
		} else if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE)) {
			auto weapon = INSTANTIATE(Weapon, "RocketP");
			weapon->transform = { this->transform.position, this->transform.rotation, 1 };
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
			emitter->lifespan = 0.5f;
			emitter->tag = "pWeapon";
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
		destroyed = true;
	}
}

void Player::OnCollision(Actor* other) {
	if (m_immuneTimer <= 0 && !m_shield) {
		if (other->tag == "eWeapon" || other->tag == "Enemy" || other->tag == "eAstroid") {
			m_health--;
			m_immuneTimer = m_immuneTime;
		}
	}
	if (other->tag == "Rapid") {
		m_fireRate = 0.25f;
		m_powerTimer = 15;
		INFO_LOG("Rapid")
	}
	if (other->tag == "Multi") {
		m_powerTimer = 15;
		m_multi = true;
		INFO_LOG("Multi-Shot")
	}
	if (other->tag == "Shield") {
		m_powerTimer = 15;
		m_shield = true;
		INFO_LOG("Shield")
	}
	if (other->tag == "Missile") {
		m_missileCount++;
		INFO_LOG("Missile Pickup")
	}
	if (other->tag == "Health") {
		m_health++;
		if (m_health > 5) m_health = 5;
		INFO_LOG("Health Pickup")
	}
}
