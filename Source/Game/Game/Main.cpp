#include "Core/Core.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"
#include "Player.h"
#include "Enemy.h"
#include "Audio/AudioSystem.h"
#include "SpaceGame.h"
//#include "Core/Logger.h"
#include "Physics/PhysicsSystem.h"
#include <iostream>
#include <thread>
#include <array>
#include <map>
#include <functional>
// TO USE, DROPDOWN ON LOCAL WINDOWS DEBUGGER CONFIGURE STARTUP PROJECTS AND SELECT THE STARTUP
using namespace std;

class Star {
public:
	Star(kiko::vec2 pos, kiko::vec2 vel) : m_pos{ pos }, m_vel{ vel } {}
	void Update() {
		m_pos += m_vel * kiko::g_time.GetDeltaTime();
	}
	void Wrap(float w, float h) {
		if (m_pos.x < 0) {
			m_pos.x = w;
		}
		if (m_pos.x > w) {
			m_pos.x = 0;
		}
		if (m_pos.y < 0) {
			m_pos.y = h;
		}
		if (m_pos.y > h) {
			m_pos.y = 0;
		}
	}
public:
	kiko::vec2 m_pos;
	kiko::vec2 m_vel;
};

union Data {
	int i;
	bool b;
	char str[6];
};

int main(int argc, char* argv[]) {
	INFO_LOG("Loaded Program");
	Data data;
	data.i = 0;
#pragma region Initialization
	kiko::MemoryTracker::Initialize();
	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets/space");

	kiko::g_renderer.Initialize();
	
	kiko::g_inputSystem.Initialize();
	kiko::g_audioSystem.Initialize();
	kiko::PhysicsSystem::Instance().Initialize();

	unique_ptr<SpaceGame> game = make_unique<SpaceGame>();
	game->Initialize();

	
	vector<Star> stars;
	for (int i = 0; i < 1000; i++) {
		kiko::vec2 pos(kiko::vec2((float)kiko::random(kiko::g_renderer.GetWidth()), (float)kiko::random(kiko::g_renderer.GetHeight())));
		kiko::vec2 vel(kiko::vec2((float)kiko::random(100, 200), (float)kiko::random(2)));
		stars.push_back(Star(pos, vel));
	}


	kiko::Transform transform{ {400, 300}, 1.2f, 2 };

	kiko::vec2 position{ 400, 300 };
	float speed = 200;
	constexpr float turnRate = kiko::DegreesToRadians(180);

	kiko::Scene scene;
	
	
#pragma endregion

	bool quit = false;
	while (!quit) {
		kiko::g_time.Tick();
		kiko::g_inputSystem.Update();
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE)) {
			quit = true;
		}
		kiko::PhysicsSystem::Instance().Update(kiko::g_time.GetDeltaTime());
		kiko::g_audioSystem.Update();
		
		float rotate = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
		transform.rotation += rotate * turnRate * kiko::g_time.GetDeltaTime();

		float thrust = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) {
			thrust = 1;
			//if (!kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_W)) {
			kiko::g_audioSystem.PlayOneShot("thrusters");
			//}
		}
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE)) kiko::g_audioSystem.PlayOneShot("laser");

		kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
		transform.position += forward * speed * thrust * kiko::g_time.GetDeltaTime();
		transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());
		
		kiko::g_renderer.SetColor(0, 0, 0, 0);
		kiko::g_renderer.BeginFrame();

		game->Draw(kiko::g_renderer);
		kiko::vec2 vel(2, 1);
		for (auto& star : stars) {
			star.Update();
			star.Wrap((float)kiko::g_renderer.GetWidth(), (float)kiko::g_renderer.GetHeight());
			kiko::g_renderer.SetColor(kiko::random(256), kiko::random(256), kiko::random(256), 255);
			kiko::g_renderer.DrawPoint(star.m_pos.x, star.m_pos.y);
		}

		kiko::g_renderer.SetColor(255, 255, 255, 255);
		game->Update(kiko::g_time.GetDeltaTime());
		kiko::g_particleSystem.Update(kiko::g_time.GetDeltaTime());
		kiko::g_particleSystem.Draw(kiko::g_renderer);
		//text->Draw(kiko::g_renderer, 400, 300);
		
		kiko::g_renderer.EndFrame();

		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	scene.RemoveAll();
	return 0;
}