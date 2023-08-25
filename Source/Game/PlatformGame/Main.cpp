#include "Core/Core.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "PlatformGame.h"
//#include "Core/Logger.h"
#include "Physics/PhysicsSystem.h"
#include <iostream>
#include <thread>
#include <array>
#include <map>
#include <functional>
// TO USE, DROPDOWN ON LOCAL WINDOWS DEBUGGER CONFIGURE STARTUP PROJECTS AND SELECT THE STARTUP
using namespace std;

int main(int argc, char* argv[]) {
	INFO_LOG("Loaded Program");

#pragma region Initialization
	kiko::MemoryTracker::Initialize();
	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets/platformer");

	kiko::g_renderer.Initialize();

	kiko::g_inputSystem.Initialize();
	kiko::g_audioSystem.Initialize();
	kiko::PhysicsSystem::Instance().Initialize();

	unique_ptr<PlatformGame> game = make_unique<PlatformGame>();
	game->Initialize();


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