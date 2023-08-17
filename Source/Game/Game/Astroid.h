#pragma once
#include "Framework/Actor.h"

class Astroid : public kiko::Actor {
public:
	Astroid(float speed, const kiko::Transform& transform) :
		Actor{ transform },
		speed{ speed } {
		m_size = 0;
	}
	Astroid(float speed, const kiko::Transform& transform, int size) :
		Actor{ transform },
		speed{ speed },
		m_size{ size } {}
	void Update(float dt) override;
	void OnCollision(Actor* other) override;
private:
	float speed = 0;
	int m_size = 0;
};