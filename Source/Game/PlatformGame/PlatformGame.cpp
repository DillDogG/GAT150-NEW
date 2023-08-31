#include "PlatformGame.h"
#include "Enemy.h"
#include "Player.h"
#include "Framework/Framework.h"
#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"

bool PlatformGame::Initialize() {
	//font and text initialize
	/* m_font = GET_RESOURCE(kiko::Font, "Freshman.ttf", 24);
	m_scoreText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_titleText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_livesText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_healthText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_missileText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_adrenalineText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_gameOverText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_diedText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_levelText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24));
	m_levelCompleteText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Freshman.ttf", 24)); */


	//initialize audio
	kiko::g_audioSystem.AddAudio("thrusters", "Thrusters.wav");
	kiko::g_audioSystem.AddAudio("explosion", "Explosion.wav");
	kiko::g_audioSystem.AddAudio("laser", "Laser_Shoot.wav");

	//creates window
	kiko::g_renderer.CreateWindow("GAT150", 800, 600);

	//makes scene
	m_scene = std::make_unique<kiko::Scene>();
	m_scene->Load("Scenes/PlatformerScene.json");
	m_scene->Load("Scenes/tilemap.json");
	m_scene->Initialize();

	// add events
	EVENT_SUBSCRIBE("OnPlayerDead", PlatformGame::OnPlayerDead);

	return true;
}

void PlatformGame::Shutdown() {
}

void PlatformGame::Update(float dt) {
	switch (m_state) {
	case eState::Title: {
		m_scene->GetActorByName("PlayerDied")->active = false;
		
		m_state = eState::StartGame;
	}
		break;
	case eState::StartGame: {
		auto actor = INSTANTIATE(Actor, "Crate");
		actor->transform.position = { kiko::random(kiko::g_renderer.GetWidth()), 100 };
		actor->Initialize();
		m_scene->Add(std::move(actor));
		auto coin = INSTANTIATE(Actor, "Coin");
		coin->transform.position = { kiko::random(kiko::g_renderer.GetWidth()), 100 };
		coin->Initialize();
		m_scene->Add(std::move(coin));
		auto player = INSTANTIATE(Player, "Player");
		player->Initialize();
		m_scene->Add(std::move(player));
		auto enemy = INSTANTIATE(Enemy, "Bat");
		enemy->transform.position = { kiko::random(kiko::g_renderer.GetWidth()), 100 };
		enemy->Initialize();
		m_scene->Add(std::move(enemy));
		m_state = eState::StartLevel;
	}
		break;
	case eState::StartLevel:
		m_spawnTimer += dt;
		if (m_spawnTimer >= m_spawnTime) {
			auto enemy = INSTANTIATE(Enemy, "Bat");
			enemy->transform.position = { kiko::random(kiko::g_renderer.GetWidth()), 100 };
			enemy->Initialize();
			m_scene->Add(std::move(enemy));
			m_spawnTimer = 0;
		}
		break;
	case eState::Game:
		
		break;
	case eState::LevelCompleteStart:
		
		break;
	case eState::LevelComplete:
		
		break;
	case eState::PlayerDeadStart:
		m_stateTimer = 3;
		m_scene->GetActorByName("PlayerDied")->active = true;
		m_state = eState::PlayerDead;
		break;
	case eState::PlayerDead:
		if (m_stateTimer < 0) {
			m_state = eState::StartGame;
			m_scene->GetActorByName("PlayerDied")->active = false;
			m_scene->GetActorByName("Bat")->destroyed = true;
			
		}
		else {
			m_stateTimer -= dt;
		}
		break;
	case eState::GameOverStart:
		
		break;
	case eState::GameOver:
		
		break;
	}
	kiko::Player* player = m_scene->GetActor<kiko::Player>();
	if (player) {
		m_scene->GetActorByName("Health")->GetComponent<kiko::TextRenderComponent>()->SetText("HEALTH: " + std::to_string(player->GetHealth()));
	}
	//Player* player = m_scene->GetActor<Player>();
	//if (player) {
		/* m_healthText->Create(kiko::g_renderer, "HEALTH: " + std::to_string(player->GetHealth()), {1, 1, 1, 1});
		m_missileText->Create(kiko::g_renderer, "MISSILES: " + std::to_string(player->GetMissile()), {1, 1, 1, 1});
		m_adrenalineText->Create(kiko::g_renderer, "ADRENALINE: " + std::to_string(player->GetAdrenaline()), {1, 1, 1, 1}); */
		/* m_scene->GetActorByName("Health")->
		m_scene->GetActorByName("Missile")->
		m_scene->GetActorByName("Adrenaline")-> */
		//}
		/* m_scene->GetActorByName("Level")->
		m_scene->GetActorByName("Score")->
		m_scene->GetActorByName("Lives")-> */
		/* m_titleText->Create(kiko::g_renderer, "ASTROIDS! PRESS SPACE TO BEGIN", {1, 1, 1, 1});
		m_levelCompleteText->Create(kiko::g_renderer, "LEVEL COMPLETE! STARTING NEXT LEVEL", { 1, 1, 1, 1 });
		m_scoreText->Create(kiko::g_renderer, "SCORE: " + std::to_string(m_score), {1, 1, 1, 1});
		m_livesText->Create(kiko::g_renderer, "LIVES: " + std::to_string(m_lives), {1, 1, 1, 1});
		m_gameOverText->Create(kiko::g_renderer, "GAME OVER", kiko::Color{ 1, 1, 1, 1 });
		m_diedText->Create(kiko::g_renderer, "YOU DIED", kiko::Color{ 1, 1, 1, 1 });
		m_levelText->Create(kiko::g_renderer, "LEVEL: " + std::to_string(currentLevel), kiko::Color{ 1, 1, 1, 1}); */
	m_scene->Update(dt);
}

void PlatformGame::Draw(kiko::Renderer& renderer) {
	m_scene->Draw(renderer);
	//if (m_state == eState::Title) m_titleText->Draw(renderer, 200, 250);
	//if (m_state == eState::LevelComplete) m_levelCompleteText->Draw(renderer, 200, 250);
	//if (m_state == eState::PlayerDead) m_diedText->Draw(renderer, 200, 250);
	//if (m_state == eState::GameOver) m_gameOverText->Draw(renderer, 200, 250);
	//if (m_state == eState::Game) m_levelText->Draw(renderer, 550, 20);
	//m_scoreText->Draw(renderer, 20, 20);
	//m_livesText->Draw(renderer, 20, 50);
	//m_healthText->Draw(renderer, 20, 80);
	//m_missileText->Draw(renderer, 550, 50);
	//m_adrenalineText->Draw(renderer, 550, 80);
}

void PlatformGame::OnPlayerDead(const kiko::Event& event) {
	m_state = eState::PlayerDeadStart;
}