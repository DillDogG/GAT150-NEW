#include "CircleCollisionComponent.h"
#include "Framework/Actor.h"

namespace kiko {
    CLASS_DEFINITION(CircleCollisionComponent);

    bool CircleCollisionComponent::CheckCollision(CollisionComponent* collision) {
        float distance = m_owner->transform.position.Distance(collision->m_owner->transform.position);
        float radius = m_radius + collision->m_radius;

        return (distance <= radius);
    }
    void CircleCollisionComponent::Update(float dt) {
    }

    void CircleCollisionComponent::Read(const json_t& value) {}
}