#include "Coordinator.h"

inline bool intersects(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	return !(x1 + width1 <= x2 ||
		x2 + width2 <= x1 ||
		y1 + height1 <= y2 ||
		y2 + height2 <= y1);
}

void RenderByLevelSystem::update(Tmpl8::Surface* screen)
{
	for (const auto& entity : entities)
	{
		const auto& transform = Coordinator::Get().getComponent<Transform>(entity);
		const auto& renderable = Coordinator::Get().getComponent<Renderable>(entity);
		const auto& level = Coordinator::Get().getComponent<Level>(entity);

		if(Map::currentLevel == level.neededLevel)
			sprites[renderable.spriteKey]->DrawScaled(static_cast<int>(transform.px), static_cast<int>(transform.py), transform.sx, transform.sy, screen, renderable.flipped);
	}
}

void AlwaysRenderSystem::update(Tmpl8::Surface* screen)
{
	for (const auto& entity : entities)
	{
		const auto& transform = Coordinator::Get().getComponent<Transform>(entity);
		const auto& renderable = Coordinator::Get().getComponent<Renderable>(entity);
		sprites[renderable.spriteKey]->DrawScaled(static_cast<int>(transform.px), static_cast<int>(transform.py), transform.sx, transform.sy, screen, renderable.flipped);
	}
}

//void EntityCollisionSystem::update(float deltaTime)
//{
//	for (auto& entity1 : entities)
//	{
//		auto& transform1 = Coordinator::Get().getComponent<Transform>(entity1);
//		auto& physics1 = Coordinator::Get().getComponent<Physics>(entity1);
//		auto& collider1 = Coordinator::Get().getComponent<Collider>(entity1);
//		
//		for (auto& entity2 : entities)
//		{
//			auto& transform2 = Coordinator::Get().getComponent<Transform>(entity2);	
//			auto& collider2 = Coordinator::Get().getComponent<Collider>(entity2);
//
//			float newPositionX = transform1.px + physics1.velocity.x;
//			if (intersects(newPositionX + collider1.offset.x, transform1.py + collider1.offset.y, collider1.sx, collider1.sy,
//				transform2.px + collider2.offset.x, transform2.py + collider2.offset.y, collider2.sx, collider2.sy))
//			{
//				transform1.px = newPositionX;
//			}
//			else
//				physics1.velocity.x = 0.f;
//
//			float newPositionY = transform1.py + physics1.velocity.y;
//			if (intersects(transform1.px + collider1.offset.x, newPositionY + collider1.offset.y, collider1.sx, collider1.sy,
//				transform2.px + collider2.offset.x, transform2.py + collider2.offset.y, collider2.sx, collider2.sy))
//			{
//				transform1.py = newPositionY;
//			}
//			else
//				physics1.velocity.y = 0.f;
//		}	
//	}
//}

void CollisionSystem::update(float deltaTime)
{
	for (auto& entity : Coordinator::Get().getEntities<>())
	{
		if (Coordinator::Get().hasComponent<Physics>(entity) && Coordinator::Get().hasComponent<ManagePlayerInputs>(entity))
		{
			auto& transform = Coordinator::Get().getComponent<Transform>(entity);
			auto& physics = Coordinator::Get().getComponent<Physics>(entity);
			auto& collider = Coordinator::Get().getComponent<Collider>(entity);

			float newPositionX = transform.px + physics.velocity.x;
			float newPositionY = transform.py + physics.velocity.y;

			bool isCollisionX = false;
			bool isCollisionY = false;

			for (const auto& entity1 : entities)
			{
				if (entity != entity1)
				{
					if (Coordinator::Get().hasComponent<Level>(entity1) && !Coordinator::Get().hasComponent<LevelTrigger>(entity1))
					{
						auto& transform1 = Coordinator::Get().getComponent<Transform>(entity1);
						auto& collider1 = Coordinator::Get().getComponent<Collider>(entity1);
						auto& level1 = Coordinator::Get().getComponent<Level>(entity1);

						if (level1.neededLevel == Map::currentLevel)
						{

							
							if (intersects(newPositionX + collider.offset.x, transform.py + collider.offset.y, collider.sx, collider.sy,
								transform1.px + collider1.offset.x, transform1.py + collider1.offset.y, collider1.sx, collider1.sy))
							{
								
								isCollisionX = true;
							}


							
							if (intersects(transform.px + collider.offset.x, newPositionY + collider.offset.y, collider.sx, collider.sy,
								transform1.px + collider1.offset.x, transform1.py + collider1.offset.y, collider1.sx, collider1.sy))
							{
								
								isCollisionY = true;
							}
						}
					}
				}
			}

			if (!isCollisionX)
				transform.px = newPositionX;
			if(!isCollisionY)
				transform.py = newPositionY;
		}
	}
}

void HandlePlayerInputSystem::update(float deltaTime)
{
	for (auto& entity : entities)
	{
		auto& transform = Coordinator::Get().getComponent<Transform>(entity);
		auto& physics = Coordinator::Get().getComponent<Physics>(entity);
		
		if(isKeyDown('a'))
			physics.velocity.x += deltaTime * -1.f * physics.acceleration.x;
		if (isKeyDown('d'))
			physics.velocity.x += deltaTime * 1.f * physics.acceleration.x;
		if (isKeyDown('w'))
			physics.velocity.y += deltaTime * -1.f * physics.acceleration.y;
		if (isKeyDown('s'))
			physics.velocity.y += deltaTime * 1.f * physics.acceleration.y;
		if (!isKeyDown('a') && !isKeyDown('d'))
			physics.velocity.x -= physics.velocity.x * physics.friction.x * deltaTime;
		if(!isKeyDown('w') && !isKeyDown('s'))
			physics.velocity.y -= physics.velocity.y * physics.friction.y * deltaTime;

		physics.velocity.x = std::clamp(physics.velocity.x, -physics.maxVelocity.x, physics.maxVelocity.x);
		physics.velocity.y = std::clamp(physics.velocity.y, -physics.maxVelocity.y, physics.maxVelocity.y);

		if (std::abs(physics.velocity.x) <= 0.01f) physics.velocity.x = 0;
		if (std::abs(physics.velocity.y) <= 0.01f) physics.velocity.y = 0;
	}
}
 
bool HandlePlayerInputSystem::isKeyDown(int key)
{
	return buttons.find(key) != buttons.end();
}

void LevelTriggerSystem::update(float deltaTime)
{
	for (auto& trigger: entities)
	{
		auto& level = Coordinator::Get().getComponent<Level>(trigger);
		if (level.neededLevel == Map::currentLevel)
		{
			auto& triggerTransform = Coordinator::Get().getComponent<Transform>(trigger);
			auto& collider = Coordinator::Get().getComponent<Collider>(trigger);
			auto& levelTrigger = Coordinator::Get().getComponent<LevelTrigger>(trigger);

			auto& playerTransform = Coordinator::Get().getComponent<Transform>(player);
			auto& playerCollider = Coordinator::Get().getComponent<Collider>(player);


			if (intersects(playerTransform.px + playerCollider.offset.x, playerTransform.py + playerCollider.offset.y, playerCollider.sx, playerCollider.sy,
				triggerTransform.px + collider.offset.x, triggerTransform.py + collider.offset.y, collider.sx, collider.sy))
			{
				Map::currentLevel = levelTrigger.nextLevel;
				playerTransform.px = levelTrigger.nx;
				playerTransform.py = levelTrigger.ny;
				break;
			}
		}
	}
}

void LevelTriggerSystem::createLevelTrigger(Transform ts, Collider cl, Level l, LevelTrigger lt)
{
	auto entity = Coordinator::Get().createEntity();
	Coordinator::Get().addComponent<Transform>(entity, ts);
	Coordinator::Get().addComponent<Collider>(entity, cl);
	Coordinator::Get().addComponent<Level>(entity, l);
	Coordinator::Get().addComponent<LevelTrigger>(entity, lt);
}
