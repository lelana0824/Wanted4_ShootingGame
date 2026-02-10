#include "Item.h"
#include <string>

Item::Item(Vector2& position, int bulletCount, FireDirectionType fireDirection)
	:super("3", position, Color::Yellow), bulletCount(bulletCount), fireDirection(fireDirection)
{

	disappearTimer.SetTargetTime(3.0f);
	std::string mainImage = std::to_string(bulletCount);

	if (bulletCount == 1)
	{
		switch (fireDirection)
		{
			case FireDirectionType::Top:
				mainImage = "¡è";
				break;
			case FireDirectionType::Side:
				mainImage = "¡ç¡æ";
				break;
			case FireDirectionType::All:
				mainImage = "AD";
				break;
			default:
				break;
			}
	}

	
	std::string temp = std::string("||") + mainImage + "||";
	const char* newImage = temp.c_str();
	ChangeImage(newImage);
}

void Item::Tick(float deltatime)
{
	disappearTimer.Tick(deltatime);

	if (disappearTimer.IsTimeOut())
	{
		Destroy();
	}
}
