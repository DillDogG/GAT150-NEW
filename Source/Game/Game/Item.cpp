#include "Item.h"

void Item::Update(float dt) {
	Actor::Update(dt);
	transform.rotation -= 0.03125f;

}

void Item::OnCollision(Actor* other) {
	if (other->tag == "Player") destroyed = true;
}
