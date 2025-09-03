#include "Systems.h"

void HandleInput::update(float deltaTime)
{
	for (auto& entity : entities)
	{
		for (const auto& key : buttons)
		{
			/*if (!dialogueSystem->isActive)
			{*/
			auto& inputComponent = coordinator->getComponent<ManageInputs>(entity);
			switch (key)
			{
			case 'a':
				inputComponent.fx--;
				break;
			case 'd':
				inputComponent.fx++;
				break;
			case 'w':
				inputComponent.fy--;
				break;
			case 's':
				inputComponent.fy++;
				break;
			}
		}
	}
}
