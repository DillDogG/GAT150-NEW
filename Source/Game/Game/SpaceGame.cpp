#include "SpaceGame.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Astroid.h"
#include "Framework/Framework.h"
#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"

bool SpaceGame::Initialize() {
	//font and text initialize
	m_font = GET_RESOURCE(kiko::Font, "Freshman.ttf", 24);
	m_scoreText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_titleText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_livesText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_healthText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_missileText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_adrenalineText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_gameOverText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_diedText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_levelText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_levelCompleteText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));


	//initialize audio
	kiko::g_audioSystem.AddAudio("thrusters", "Thrusters.wav");
	kiko::g_audioSystem.AddAudio("explosion", "Explosion.wav");
	kiko::g_audioSystem.AddAudio("laser", "Laser_Shoot.wav");

	//creates window
	kiko::g_renderer.CreateWindow("GAT150", 800, 600);

	//makes
	m_scene = std::make_unique<kiko::Scene>();

	

	return true;
}

void SpaceGame::Shutdown() {
}

void SpaceGame::Update(float dt) {
	switch (m_state) {
	case eState::Title:
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE)) {
			m_state = eState::StartGame;
			m_endless = false;
		}
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_E)) {
			m_state = eState::StartGame;
			m_endless = true;
			m_spawnTime = 1.5f;
		}
		break;
	case eState::StartGame:
		m_score = 0;
		m_lives = 3;
		m_state = eState::StartLevel;
		currentLevel = 1;
		break;
	case eState::StartLevel:
		m_scene->RemoveAll(); {
			// create player
			std::unique_ptr<Player> player = std::make_unique<Player>(200.0f, kiko::Pi, kiko::Transform{ {400, 300}, 1.2f });
			player->m_tag = "Player";
			player->m_game = this;
			//player->SetDamping(0.9f);
			// create components
			/* Generating player with a model 
			std::unique_ptr<kiko::ModelRenderComponent> component = std::make_unique<kiko::ModelRenderComponent>();
			component->m_model = kiko::g_resources.Get<kiko::Model>("Ship.txt", kiko::g_renderer);
			player->AddComponent(std::move(component)); */
			/* Generating player with a sprite */
			auto component = CREATE_CLASS(SpriteComponent)
			component->m_texture = GET_RESOURCE(kiko::Texture, "PlayerShip.png", kiko::g_renderer);
			player->AddComponent(std::move(component)); 

			auto physicsComponent = CREATE_CLASS(EnginePhysicsComponent)
			physicsComponent->m_damping = 0.9f;
			player->AddComponent(std::move(physicsComponent));

			auto collisionComponent = CREATE_CLASS(CircleCollisionComponent);
			collisionComponent->m_radius = 30.f;
			player->AddComponent(std::move(collisionComponent));

			player->Initialize();
			m_scene->Add(std::move(player));
			enemiesSpawned = 0;
			if (m_endless) m_state = eState::Endless;
			else m_state = eState::Game;
		}
		break;
	case eState::Game:
		m_spawnTimer += dt;
		if (m_spawnTimer >= m_spawnTime) {
			m_spawnTimer = 0;
			std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(150.0f, kiko::Pi, kiko::Transform{{ (float)kiko::random(800), (float)kiko::random(600) }, kiko::randomf(3) });
			enemy->m_tag = "Enemy";
			enemy->m_game = this;
			// create componenets
			std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
			component->m_texture = GET_RESOURCE(kiko::Texture, "EnemyShip.png", kiko::g_renderer);
			enemy->AddComponent(std::move(component));
			auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 30.f;
			enemy->AddComponent(std::move(collisionComponent));
			enemy->Initialize();
			m_scene->Add(std::move(enemy));
			enemiesSpawned++;
			if (enemiesSpawned % 5 == 3) {
				std::unique_ptr<Item> item = std::make_unique<Item>(kiko::Transform{{ (float)kiko::random(800), (float)kiko::random(600) }, 1.5f }, kiko::randomf(3.0f, 7.0f));
				component = std::make_unique<kiko::SpriteComponent>();
				component->m_texture = GET_RESOURCE(kiko::Texture, "Box.png", kiko::g_renderer);
				item->AddComponent(std::move(component));
				switch (kiko::random(5)) {
				case 0:
					item->m_tag = "Health";
					break;
				case 1:
					item->m_tag = "Rapid";
					break;
				case 2:
					item->m_tag = "Missile";
					break;
				case 3:
					item->m_tag = "Shield";
					break;
				case 4:
					item->m_tag = "Multi";
					break;
				}
				item->m_game = this;
				auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
				collisionComponent->m_radius = 30.f;
				item->AddComponent(std::move(collisionComponent));
				item->Initialize();
				m_scene->Add(std::move(item));
			}
			if (enemiesSpawned % 7 == 1) {
				int aSize = kiko::random(2, 7);
				std::unique_ptr<Astroid> astroid = std::make_unique<Astroid>(150.0f, kiko::Transform{{ (float)kiko::random(800), (float)kiko::random(600) }, kiko::randomf(3) }, aSize);
				astroid->m_tag = "Astroid";
				astroid->m_game = this;
				// create componenets
				std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
				if (aSize > 4) component->m_texture = GET_RESOURCE(kiko::Texture, "Astroid Medium.png", kiko::g_renderer);
				else component->m_texture = GET_RESOURCE(kiko::Texture,"Astroid Large.png", kiko::g_renderer);
				astroid->AddComponent(std::move(component));
				auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
				collisionComponent->m_radius = 30.f;
				astroid->AddComponent(std::move(collisionComponent));
				astroid->Initialize();
				m_scene->Add(std::move(astroid));
			}
		}
		switch (currentLevel) {
		case 1:
			if (enemiesSpawned >= enemiesR1) {
				currentLevel++;
				m_state = eState::LevelCompleteStart;
				m_spawnTime = 5;
			}
			break;
		case 2:
			if (enemiesSpawned >= enemiesR2) {
				currentLevel++;
				m_state = eState::LevelCompleteStart;
				m_spawnTime = 4;
			}
			break;
		case 3:
			if (enemiesSpawned >= enemiesR3) {
				currentLevel++;
				m_state = eState::LevelCompleteStart;
				m_spawnTime = 3;
			}
			break;
		case 4:
			if (enemiesSpawned >= enemiesR4) {
				currentLevel++;
				m_state = eState::LevelCompleteStart;
				m_spawnTime = 2;
			}
			break;
		case 5:
			if (enemiesSpawned >= enemiesR5) {
				currentLevel = 1;
				m_state = eState::LevelCompleteStart;
				m_spawnTime = 1.5f;
			}
			break;
		}
		break;
	case eState::LevelCompleteStart:
		m_stateTimer = 3;
		m_state = eState::LevelComplete;
		break;
	case eState::LevelComplete:
		if (m_stateTimer < 0) {
			m_state = eState::StartLevel;
		} else {
			m_stateTimer -= dt;
		}
		break;
	case eState::PlayerDeadStart:
		m_stateTimer = 3;
		m_state = eState::PlayerDead;
		break;
	case eState::PlayerDead:
		if (m_stateTimer < 0) {
			m_state = eState::StartLevel;
			if (m_lives == 0) m_state = eState::GameOver;
		} else {
			m_stateTimer -= dt;
		}
		break;
	case eState::GameOverStart:
		m_stateTimer = 5;
		m_state = eState::GameOver;
		break;
	case eState::GameOver:
		if (m_stateTimer < 0) {
			m_state = eState::Title;
		} else {
			m_stateTimer -= dt;
		}
		break;
	case eState::Endless:
		m_spawnTimer += dt;
		if (m_spawnTimer >= m_spawnTime) {
			m_spawnTimer = 0;
			std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(150.0f, kiko::Pi, kiko::Transform{{ (float)kiko::random(800), (float)kiko::random(600) }, kiko::randomf(3) });
			enemy->m_tag = "Enemy";
			enemy->m_game = this;
			// create componenets
			std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
			component->m_texture = GET_RESOURCE(kiko::Texture, "EnemyShip.png", kiko::g_renderer);
			enemy->AddComponent(std::move(component));
			auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 30.f;
			enemy->AddComponent(std::move(collisionComponent));
			m_scene->Add(std::move(enemy));
			enemiesSpawned++;
			if (enemiesSpawned % 6 == 3) {
				std::unique_ptr<Item> item = std::make_unique<Item>(kiko::Transform{{ (float)kiko::random(800), (float)kiko::random(600) }, kiko::randomf(3), 0.75f }, kiko::randomf(3.0f, 7.0f));
				component = std::make_unique<kiko::SpriteComponent>();
				component->m_texture = GET_RESOURCE(kiko::Texture, "Box.png", kiko::g_renderer);
				item->AddComponent(std::move(component));
				switch (kiko::random(5)) {
				case 0:
					item->m_tag = "Health";
					break;
				case 1:
					item->m_tag = "Rapid";
					break;
				case 2:
					item->m_tag = "Missile";
					break;
				case 3:
					item->m_tag = "Shield";
					break;
				case 4:
					item->m_tag = "Multi";
					break;
				}
				item->m_game = this;
				auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
				collisionComponent->m_radius = 30.f;
				item->AddComponent(std::move(collisionComponent));
				item->Initialize();
				m_scene->Add(std::move(item));
			}
			if (enemiesSpawned % 8 == 1) {
				int aSize = kiko::random(2, 7);
				std::unique_ptr<Astroid> astroid = std::make_unique<Astroid>(150.0f, kiko::Transform{{ (float)kiko::random(800), (float)kiko::random(600) }, kiko::randomf(3) }, aSize);
				astroid->m_tag = "Astroid";
				astroid->m_game = this;
				// create componenets
				std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
				if (aSize > 4) component->m_texture = GET_RESOURCE(kiko::Texture, "Astroid Medium.png", kiko::g_renderer);
				else component->m_texture = GET_RESOURCE(kiko::Texture, "Astroid Large.png", kiko::g_renderer);
				astroid->AddComponent(std::move(component));
				auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
				collisionComponent->m_radius = 30.f;
				astroid->AddComponent(std::move(collisionComponent));
				astroid->Initialize();
				m_scene->Add(std::move(astroid));
			}
		}
		break;
	}
	Player* player = m_scene->GetActor<Player>();
	if (player) {
		m_healthText->Create(kiko::g_renderer, "HEALTH: " + std::to_string(player->GetHealth()), {1, 1, 1, 1});
		m_missileText->Create(kiko::g_renderer, "MISSILES: " + std::to_string(player->GetMissile()), {1, 1, 1, 1});
		m_adrenalineText->Create(kiko::g_renderer, "ADRENALINE: " + std::to_string(player->GetAdrenaline()), {1, 1, 1, 1});
	}
	m_titleText->Create(kiko::g_renderer, "ASTROIDS! PRESS SPACE TO BEGIN", { 1, 1, 1, 1 });
	m_levelCompleteText->Create(kiko::g_renderer, "LEVEL COMPLETE! STARTING NEXT LEVEL", { 1, 1, 1, 1 });
	m_scoreText->Create(kiko::g_renderer, "SCORE: " + std::to_string(m_score), {1, 1, 1, 1});
	m_livesText->Create(kiko::g_renderer, "LIVES: " + std::to_string(m_lives), {1, 1, 1, 1});
	m_gameOverText->Create(kiko::g_renderer, "GAME OVER", kiko::Color{ 1, 1, 1, 1 });
	m_diedText->Create(kiko::g_renderer, "YOU DIED", kiko::Color{ 1, 1, 1, 1 });
	m_levelText->Create(kiko::g_renderer, "LEVEL: " + std::to_string(currentLevel), kiko::Color{ 1, 1, 1, 1});
	m_scene->Update(dt);
}

void SpaceGame::Draw(kiko::Renderer& renderer) {
	if (m_state == eState::Title) m_titleText->Draw(renderer, 200, 250);
	if (m_state == eState::LevelComplete) m_levelCompleteText->Draw(renderer, 200, 250);
	if (m_state == eState::PlayerDead) m_diedText->Draw(renderer, 200, 250);
	if (m_state == eState::GameOver) m_gameOverText->Draw(renderer, 200, 250);
	if (m_state == eState::Game) m_levelText->Draw(renderer, 550, 20);
	m_scoreText->Draw(renderer, 20, 20);
	m_livesText->Draw(renderer, 20, 50);
	m_healthText->Draw(renderer, 20, 80);
	m_missileText->Draw(renderer, 550, 50);
	m_adrenalineText->Draw(renderer, 550, 80);
	m_scene->Draw(renderer);
}
