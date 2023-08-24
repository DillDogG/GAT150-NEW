#include "Astroid.h"
#include "Framework/Framework.h"
#include "SpaceGame.h"
#include "Weapon.h"
#include "Renderer/Renderer.h"
#include "Framework/Emitter.h"

namespace kiko {
	CLASS_DEFINITION(Astroid);
	bool Astroid::Initialize() {
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

	void Astroid::Update(float dt) {
		Actor::Update(dt);
		kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
		m_physicsComponent->ApplyForce(forward * speed);
		transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());
	}

	void Astroid::OnCollisionEnter(Actor* other) {
		if (other->tag == "pWeapon" && tag != "pAstroid") {
			destroyed = true;
			kiko::EmitterData data;
			data.burst = true;
			data.burstCount = 100;
			data.spawnRate = 200;
			data.angle = 0.0f;
			data.angleRange = kiko::Pi;
			data.lifetimeMin = 0.25f;
			data.lifetimeMax = 0.5f;
			data.speedMin = 50;
			data.speedMax = 250;
			data.damping = 0.5f;
			data.color = kiko::Color{ 1, 1, 1, 1 };
			auto emitter = std::make_unique<kiko::Emitter>(transform, data);
			emitter->lifespan = 0.75f;
			m_scene->Add(std::move(emitter));
			for (int i = 0; i < m_size; i++) {
				//std::unique_ptr<Astroid> astroid = std::make_unique<Astroid>((speed + 150.0f), transform);
				//astroid->transform.rotation = kiko::randomf((other->transform.rotation * 0.5f), (other->transform.rotation * 1.5f));
				//astroid->tag = "pAstroid";
				//// create componenets
				//std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
				//component->m_texture = GET_RESOURCE(kiko::Texture, "Astroid Small.png", kiko::g_renderer);
				//astroid->AddComponent(std::move(component));
				//auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
				//collisionComponent->m_radius = 30.f;
				//astroid->AddComponent(std::move(collisionComponent));
				//m_scene->Add(std::move(astroid));
				auto astroid = INSTANTIATE(Astroid, "AstroidSmallP");
				astroid->transform = { this->transform.position, this->transform.rotation, 1 };
				astroid->Initialize();
				m_scene->Add(std::move(astroid));
			}
		}
		if (other->tag == "eWeapon" && tag != "eAstroid") {
			destroyed = true;
			kiko::EmitterData data;
			data.burst = true;
			data.burstCount = 100;
			data.spawnRate = 200;
			data.angle = 0.0f;
			data.angleRange = kiko::Pi;
			data.lifetimeMin = 0.25f;
			data.lifetimeMax = 0.5f;
			data.speedMin = 50;
			data.speedMax = 250;
			data.damping = 0.5f;
			data.color = kiko::Color{ 1, 1, 1, 1 };
			auto emitter = std::make_unique<kiko::Emitter>(transform, data);
			emitter->lifespan = 0.75f;
			m_scene->Add(std::move(emitter));
			for (int i = 0; i < m_size; i++) {
				//std::unique_ptr<Astroid> astroid = std::make_unique<Astroid>((speed + 150.0f), transform);
				//astroid->transform.rotation = kiko::randomf((other->transform.rotation * 0.5f), (other->transform.rotation * 1.5f));
				//astroid->tag = "eAstroid";
				//// create componenets
				//std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
				//component->m_texture = GET_RESOURCE(kiko::Texture, "Astroid Small.png", kiko::g_renderer);
				//astroid->AddComponent(std::move(component));
				//auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
				//collisionComponent->m_radius = 30.f;
				//astroid->AddComponent(std::move(collisionComponent));
				//m_scene->Add(std::move(astroid));
				auto astroid = INSTANTIATE(Astroid, "AstroidSmallE");
				astroid->transform = { this->transform.position, this->transform.rotation, 1 };
				astroid->Initialize();
				m_scene->Add(std::move(astroid));
			}
		}
		if (other->tag == "pWeapon" && tag == "eAstroid") {
			destroyed = true;
			kiko::EmitterData data;
			data.burst = true;
			data.burstCount = 100;
			data.spawnRate = 200;
			data.angle = 0.0f;
			data.angleRange = kiko::Pi;
			data.lifetimeMin = 0.25f;
			data.lifetimeMax = 0.5f;
			data.speedMin = 50;
			data.speedMax = 250;
			data.damping = 0.5f;
			data.color = kiko::Color{ 1, 1, 1, 1 };
			auto emitter = std::make_unique<kiko::Emitter>(transform, data);
			emitter->lifespan = 0.75f;
			m_scene->Add(std::move(emitter));
		}
		if (other->tag == "eWeapon" && tag == "pAstroid") {
			destroyed = true;
			kiko::EmitterData data;
			data.burst = true;
			data.burstCount = 100;
			data.spawnRate = 200;
			data.angle = 0.0f;
			data.angleRange = kiko::Pi;
			data.lifetimeMin = 0.25f;
			data.lifetimeMax = 0.5f;
			data.speedMin = 50;
			data.speedMax = 250;
			data.damping = 0.5f;
			data.color = kiko::Color{ 1, 1, 1, 1 };
			auto emitter = std::make_unique<kiko::Emitter>(transform, data);
			emitter->lifespan = 0.75f;
			m_scene->Add(std::move(emitter));
		}
	}

	void Astroid::Read(const json_t& value) {
		Actor::Read(value);
		READ_DATA(value, speed);
		READ_NAME_DATA(value, "size", m_size);
	}
}