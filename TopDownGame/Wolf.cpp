#include "Wolf.h"
#include "Map.h"

Wolf::Wolf(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, Map* map) : Entity(sprite, px, py ,sx, sy), map(map)
{
}

Wolf::~Wolf()
{
}

void Wolf::update(float deltaTime)
{
}
