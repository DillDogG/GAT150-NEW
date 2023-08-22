#pragma once
#include "Framework/Game.h"
#include "Framework/Event/EventManager.h"
#include "Renderer/Text.h"

class SpaceGame : public kiko::Game, kiko::IEventListener {
public:
	enum eState {
		Title,
		StartGame,
		StartLevel,
		Game,
		LevelCompleteStart,
		LevelComplete,
		PlayerDeadStart,
		PlayerDead,
		GameOverStart,
		GameOver,
		Endless
	};
public:
	virtual bool Initialize() override;
	virtual void Shutdown() override;

	virtual void Update(float dt) override;
	virtual void Draw(kiko::Renderer& renderer) override;

	void SetState(eState state) { m_state = state; }
	void OnAddPoints(const kiko::Event& event);
	void OnPlayerDead(const kiko::Event& event);
private:
	eState m_state = eState::Title;
	float m_spawnTimer = 0;
	float m_spawnTime = 3;

	float m_stateTimer = 0;

	//creates levels
	int enemiesR1 = 5;
	int enemiesR2 = 15;
	int enemiesR3 = 35;
	int enemiesR4 = 55;
	int enemiesR5 = 100;
	int currentLevel = 1;
	int enemiesSpawned = 0;
	bool m_endless = false;

	/* std::shared_ptr<kiko::Font> m_font;
	std::shared_ptr<kiko::Text> m_scoreText;
	std::shared_ptr<kiko::Text> m_titleText;
	std::shared_ptr<kiko::Text> m_livesText;
	std::shared_ptr<kiko::Text> m_healthText;
	std::shared_ptr<kiko::Text> m_gameOverText;
	std::shared_ptr<kiko::Text> m_diedText;
	std::shared_ptr<kiko::Text> m_missileText;
	std::shared_ptr<kiko::Text> m_adrenalineText;
	std::shared_ptr<kiko::Text> m_levelText;
	std::shared_ptr<kiko::Text> m_levelCompleteText; */
};