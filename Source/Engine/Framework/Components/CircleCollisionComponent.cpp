#include "CircleCollisionComponent.h"
#include "Framework/Actor.h"

namespace kiko {
    bool kiko::CircleCollisionComponent::CheckCollision(CollisionComponent* collision) {
        float distance = m_owner->m_transform.position.Distance(collision->m_owner->m_transform.position);
        float radius = m_radius + collision->m_radius;

        return (distance <= radius);
    }
    void CircleCollisionComponent::Update(float dt) {
    }
}