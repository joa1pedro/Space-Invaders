#include "Block.h"

void Block::Damage() {
	this->Life -= 1;
}

bool Block::ToBeDestroyed() const
{
	return Life == 1;
}
