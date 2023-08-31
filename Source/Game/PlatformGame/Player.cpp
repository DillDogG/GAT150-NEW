#include "Player.h"
//#include "Renderer/Texture.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Framework/Framework.h"
#include "Attack.h"
//#include "Framework/Components/EnginePhysicsComponent.h"
#include "Core/Logger.h"
//#include "Core/Math/MathUtls.h"

namespace kiko {
	CLASS_DEFINITION(Player);
	bool Player::Initialize() {
		Actor::Initialize();
		m_physicsComponent = GetComponent<kiko::PhysicsComponent>();
		m_spriteAnimComponent = GetComponent<SpriteAnimComponent>();
		return true;
	}

	void Player::Update(float dt) {
		Actor::Update(dt);
		vec2 velocity = this->m_physicsComponent->m_velocity;
		bool onGround = (groundCount > 0);
		float dir = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) dir = -1;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) dir = 1;

		kiko::vec2 forward = kiko::vec2{ 1, 0 };
		if (dir) {
			velocity.x += speed * dir * dt;
			velocity.x = Clamp(velocity.x, -maxSpeed, maxSpeed);
			m_physicsComponent->SetVelocity(velocity);
			//m_physicsComponent->ApplyForce(forward * speed * dir);
		}
		// check if position is off screen, if so wrap the position and set physics component to new position
		if ((transform.position.x < 0 || transform.position.x >(float)kiko::g_renderer.GetWidth()) || (transform.position.y < 0 || transform.position.y >(float)kiko::g_renderer.GetHeight())) {
			transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
			transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());

			m_physicsComponent->SetPosition(transform.position);
		}
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE) && onGround) {
			vec2 up = vec2{ 0, -1 };
			m_physicsComponent->SetVelocity(velocity + (up * m_jumpHeight));
			m_jumpTimer = m_jumpTime;
		}
		m_immuneTimer -= dt;
		if (dir != 0) m_spriteAnimComponent->flipH = (dir < 0);
		if (animate) {
			if (velocity.y < -2) { m_spriteAnimComponent->SetSequence("fall"); }
			else if (velocity.y > 2) { m_spriteAnimComponent->SetSequence("jump"); }
			else if (std::fabs(velocity.x) > 0.2f) { m_spriteAnimComponent->SetSequence("walk"); }
			else if (std::fabs(velocity.x) > 0.7f) { m_spriteAnimComponent->SetSequence("run"); }
			else m_spriteAnimComponent->SetSequence("idle");
		}
		if (m_health < 1) {
			destroyed = true;
			kiko::EventManager::Instance().DispatchEvent("OnPlayerDead", 0);
			auto actor = INSTANTIATE(Actor, "PlayerBody");
			actor->transform = { this->transform.position, this->transform.rotation, 1.75f };
			actor->Initialize();
			m_scene->Add(std::move(actor));
		}

		if (m_animationTime <= 0) { animate = true; }

		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_LSHIFT)) {
			m_dashDuration = 0.5f;
			m_spriteAnimComponent->SetSequence("dash");
			animate = false;
			m_physicsComponent->SetVelocity(velocity * 2);
			m_animationTime = 0.5f;
		}

		if (m_attackTimer <= 0) {
			if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_Q)){
				if (!this->m_spriteAnimComponent->flipH) {
					auto attack = INSTANTIATE(Attack, "pAttackL");
					attack->transform = { vec2{this->transform.position.x + 15, this->transform.position.y}, this->transform.rotation, 1 };
					attack->Initialize();
					m_scene->Add(std::move(attack));
				} else {
					auto attack = INSTANTIATE(Attack, "pAttackLFlipped");
					attack->transform = { vec2{this->transform.position.x - 15, this->transform.position.y}, this->transform.rotation, 1 };
					attack->Initialize();
					m_scene->Add(std::move(attack));
				}
				
				m_attackTimer = 0.25f;
				m_spriteAnimComponent->SetSequence("attackL");
				animate = false;
				m_animationTime = 0.25f;
			}
			else if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_E)) {
				if (!this->m_spriteAnimComponent->flipH) {
					auto attack = INSTANTIATE(Attack, "pAttackM");
					attack->transform = { vec2{this->transform.position.x + 5, this->transform.position.y}, this->transform.rotation, 1.5 };
					attack->Initialize();
					m_scene->Add(std::move(attack));
				}
				else {
					auto attack = INSTANTIATE(Attack, "pAttackMFlipped");
					attack->transform = { vec2{this->transform.position.x - 5, this->transform.position.y}, this->transform.rotation, 1.5 };
					attack->Initialize();
					m_scene->Add(std::move(attack));
				}
				m_attackTimer = 0.5f;
				m_spriteAnimComponent->SetSequence("attackM");
				animate = false;
				m_animationTime = 0.5f;
			}
			else if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_H)) {
				if (!this->m_spriteAnimComponent->flipH) {
					auto attack = INSTANTIATE(Attack, "pAttackH");
					attack->transform = { vec2{this->transform.position.x + 5, this->transform.position.y}, this->transform.rotation, 2 };
					attack->Initialize();
					m_scene->Add(std::move(attack));
				}
				else {
					auto attack = INSTANTIATE(Attack, "pAttackHFlipped");
					attack->transform = { vec2{this->transform.position.x - 5, this->transform.position.y}, this->transform.rotation, 2 };
					attack->Initialize();
					m_scene->Add(std::move(attack));
				}
				m_attackTimer = 1;
				m_spriteAnimComponent->SetSequence("attackH");
				animate = false;
				m_animationTime = 1;
			}
		}
		m_attackTimer -= dt;
		m_jumpTimer -= dt;
		m_animationTime -= dt;
		m_dashDuration -= dt;
		
	}

	void Player::OnCollisionEnter(Actor* other) {
		if (m_immuneTimer <= 0) {
			if (other->tag == "Enemy") {
				m_health -= 15;
				m_immuneTimer = 2.5f;
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
		READ_DATA(value, maxSpeed);
		READ_NAME_DATA(value, "health", m_health);
		READ_NAME_DATA(value, "jump", m_jumpHeight);
		READ_NAME_DATA(value, "jumpTime", m_jumpTime);
	}
}