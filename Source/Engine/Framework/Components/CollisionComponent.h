#pragma once
#include "Component.h"

namespace kiko {
	class CollisionComponent : public Component {
	public:
		virtual bool CheckCollision(CollisionComponent* Component) { return false; }
	public:
		float m_radius = 0;
	};
}