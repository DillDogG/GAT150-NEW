#include "Astroid.h"
#include "Framework/Framework.h"
#include "SpaceGame.h"
#include "Weapon.h"
#include "Renderer/Renderer.h"
#include "Framework/Emitter.h"

void Astroid::Update(float dt) {
	Actor::Update(dt);
	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
	transform.position += forward * speed * kiko::g_time.GetDeltaTime();
	transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
	transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());
}

void Astroid::OnCollision(Actor* other) {
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
			std::unique_ptr<Astroid> astroid = std::make_unique<Astroid>((speed + 150.0f), transform);
			astroid->transform.rotation = kiko::randomf((other->transform.rotation * 0.5f), (other->transform.rotation * 1.5f));
			astroid->tag = "pAstroid";
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
			std::unique_ptr<Astroid> astroid = std::make_unique<Astroid>((speed + 150.0f), transform);
			astroid->transform.rotation = kiko::randomf((other->transform.rotation * 0.5f), (other->transform.rotation * 1.5f));
			astroid->tag = "eAstroid";
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