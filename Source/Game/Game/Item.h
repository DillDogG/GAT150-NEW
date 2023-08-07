#pragma once
#include "Framework/Actor.h"

class Item : public kiko::Actor {
public:
	Item(const kiko::Transform& transform, float lifeSpan) :
		Actor{ transform }
	{
		m_lifespan = lifeSpan;
	}
	void Update(float dt) override;
	void OnCollision(Actor* other) override;
};