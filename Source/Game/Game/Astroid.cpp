#include "Astroid.h"
#include "Framework/Framework.h"
#include "SpaceGame.h"
#include "Weapon.h"
#include "Renderer/Renderer.h"
#include "Framework/Emitter.h"

void Astroid::Update(float dt) {
	Actor::Update(dt);
	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(m_transform.rotation);
	m_transform.position += forward * m_speed * kiko::g_time.GetDeltaTime();
	m_transform.position.x = kiko::Wrap(m_transform.position.x, (float)kiko::g_renderer.GetWidth());
	m_transform.position.y = kiko::Wrap(m_transform.position.y, (float)kiko::g_renderer.GetHeight());
}

void Astroid::OnCollision(Actor* other) {
	if (other->m_tag == "pWeapon" && m_tag != "pAstroid") {
		m_destroyed = true;
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
		auto emitter = std::make_unique<kiko::Emitter>(m_transform, data);
		emitter->m_lifespan = 0.75f;
		m_scene->Add(std::move(emitter));
		for (int i = 0; i < m_size; i++) {
			std::unique_ptr<Astroid> astroid = std::make_unique<Astroid>((m_speed + 150.0f), m_transform);
			astroid->m_transform.rotation = kiko::randomf((other->m_transform.rotation * 0.5f), (other->m_transform.rotation * 1.5f));
			astroid->m_tag = "pAstroid";
			// create componenets
			std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
			component->m_texture = GET_RESOURCE(kiko::Texture, "Astroid Small.png", kiko::g_renderer);
			astroid->AddComponent(std::move(component));
			auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 30.f;
			astroid->AddComponent(std::move(collisionComponent));
			m_scene->Add(std::move(astroid));
		}
	}
	if (other->m_tag == "eWeapon" && m_tag != "eAstroid") {
		m_destroyed = true;
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
		auto emitter = std::make_unique<kiko::Emitter>(m_transform, data);
		emitter->m_lifespan = 0.75f;
		m_scene->Add(std::move(emitter));
		for (int i = 0; i < m_size; i++) {
			std::unique_ptr<Astroid> astroid = std::make_unique<Astroid>((m_speed + 150.0f), m_transform);
			astroid->m_transform.rotation = kiko::randomf((other->m_transform.rotation * 0.5f), (other->m_transform.rotation * 1.5f));
			astroid->m_tag = "eAstroid";
			// create componenets
			std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
			component->m_texture = GET_RESOURCE(kiko::Texture, "Astroid Small.png", kiko::g_renderer);
			astroid->AddComponent(std::move(component));
			auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 30.f;
			astroid->AddComponent(std::move(collisionComponent));
			m_scene->Add(std::move(astroid));
		}
	}
	if (other->m_tag == "pWeapon" && m_tag == "eAstroid") {
		m_destroyed = true;
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
		auto emitter = std::make_unique<kiko::Emitter>(m_transform, data);
		emitter->m_lifespan = 0.75f;
		m_scene->Add(std::move(emitter));
	}
	if (other->m_tag == "eWeapon" && m_tag == "pAstroid") {
		m_destroyed = true;
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
		auto emitter = std::make_unique<kiko::Emitter>(m_transform, data);
		emitter->m_lifespan = 0.75f;
		m_scene->Add(std::move(emitter));
	}
}