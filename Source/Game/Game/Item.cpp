#include "Item.h"

void Item::Update(float dt) {
	Actor::Update(dt);
	m_transform.rotation -= 0.03125f;

}

void Item::OnCollision(Actor* other) {
	if (other->m_tag == "Player") m_destroyed = true;
}
