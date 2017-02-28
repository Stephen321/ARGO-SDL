#include "UISystem.h"

#include "TransformComponent.h"
#include "SpriteComponent.h"

UISystem::UISystem(float updateRate)
	: System(updateRate)
{
}


UISystem::~UISystem()
{
}

void UISystem::Initialize(SDL_Renderer* renderer)
{
	_renderer = renderer;

	// Text
	TTF_Init();
	_fontSize = 32;
	_font = TTF_OpenFont("Media\\Fonts\\font.ttf", _fontSize);

	_interactiveTextTexture = std::vector<SDL_Texture*>();
	_interactiveTextRectangle = std::vector<SDL_Rect>();

	_displayTextTexture = std::vector<SDL_Texture*>();
	_displayTextRectangle = std::vector<SDL_Rect>();

	//TTF_CloseFont(_font); // Free Font Memory
}

void UISystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				// Transform to screen
				TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
				SpriteComponent* sprite = static_cast<SpriteComponent*>(e->GetComponent(Component::Type::Sprite));

				SDL_Rect scaledRect;
				if (transform != nullptr)
				{
					scaledRect = transform->rect;
					scaledRect.w *= transform->scaleX;
					scaledRect.h *= transform->scaleY;
					scaledRect.x -= transform->origin.x * transform->scaleX;
					scaledRect.y -= transform->origin.y * transform->scaleY;
				}

				if (transform != nullptr && sprite != nullptr)
				{
					//Render to screen
					SDL_RenderCopyEx(_renderer, sprite->texture, &sprite->sourceRect, &scaledRect, 
						transform->angle, &transform->origin, SDL_FLIP_NONE);
				}

				// Draw Text
				for (int i = 0; i < _interactiveTextTexture.size(); i++)
				{
					SDL_RenderCopy(_renderer, _interactiveTextTexture[i], NULL, &_interactiveTextRectangle[i]);
				}

				for (int i = 0; i < _displayTextTexture.size(); i++)
				{
					SDL_RenderCopy(_renderer, _displayTextTexture[i], NULL, &_displayTextRectangle[i]);
				}
			}
		}
	}
}

void UISystem::DeleteText()
{
	for (int i = 0; i < _interactiveTextTexture.size(); i++)
	{
		SDL_DestroyTexture(_interactiveTextTexture[i]);
	}
	_interactiveTextTexture.clear();

	_interactiveTextRectangle.clear();
}

void UISystem::CreateText(std::string message, int x, int y)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_interactiveTextTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_interactiveTextTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_interactiveTextRectangle.push_back(textRectangle);

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _interactiveTextTexture.back(), NULL, &_interactiveTextRectangle.back());
}

void UISystem::CreateTextAtOrigin(std::string message, int x, int y)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_interactiveTextTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_interactiveTextTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x - width / 2;
	textRectangle.y = y - height / 2;
	textRectangle.w = width;
	textRectangle.h = height;
	_interactiveTextRectangle.push_back(textRectangle);

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _interactiveTextTexture.back(), NULL, &_interactiveTextRectangle.back());
}

void UISystem::CreateTextAtCenter(std::string message, int x, int y)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_interactiveTextTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_interactiveTextTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x - width / 2;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_interactiveTextRectangle.push_back(textRectangle);

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _interactiveTextTexture.back(), NULL, &_interactiveTextRectangle.back());
}

void UISystem::CreateTextColoured(std::string message, int x, int y, Uint8 r, Uint8 b, Uint8 g, Uint8 a)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ r, g, b, a });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_interactiveTextTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_interactiveTextTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_interactiveTextRectangle.push_back(textRectangle);

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _interactiveTextTexture.back(), NULL, &_interactiveTextRectangle.back());
}

void UISystem::UpdateText(std::string message, int index)
{
	// Destroy Previous Image
	SDL_DestroyTexture(_interactiveTextTexture[index]);

	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_interactiveTextTexture[index] = textTexture;

	int width, height;
	SDL_QueryTexture(_interactiveTextTexture[index], NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = _interactiveTextRectangle[index].x;
	textRectangle.y = _interactiveTextRectangle[index].y;
	textRectangle.w = width;
	textRectangle.h = height;
	_interactiveTextRectangle[index] = textRectangle;

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _interactiveTextTexture[index], NULL, &_interactiveTextRectangle[index]);
}

void UISystem::UpdateTextAtCenter(std::string message, int index)
{
	// Destroy Previous Image
	SDL_DestroyTexture(_interactiveTextTexture[index]);

	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_interactiveTextTexture[index] = textTexture;

	int width, height;
	SDL_QueryTexture(_interactiveTextTexture[index], NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = (_interactiveTextRectangle[index].x + _interactiveTextRectangle[index].w / 2) - width / 2;
	textRectangle.y = _interactiveTextRectangle[index].y;
	textRectangle.w = width;
	textRectangle.h = height;
	_interactiveTextRectangle[index] = textRectangle;

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _interactiveTextTexture[index], NULL, &_interactiveTextRectangle[index]);
}

void UISystem::UpdateTextColoured(std::string message, int index, Uint8 r, Uint8 b, Uint8 g, Uint8 a)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ r, g, b, a });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_interactiveTextTexture[index] = textTexture;

	int width, height;
	SDL_QueryTexture(_interactiveTextTexture[index], NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = _interactiveTextRectangle[index].x;
	textRectangle.y = _interactiveTextRectangle[index].y;
	textRectangle.w = width;
	textRectangle.h = height;
	_interactiveTextRectangle[index] = textRectangle;

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _interactiveTextTexture[index], NULL, &_interactiveTextRectangle[index]);
}

void UISystem::CreateDisplayText(std::string message, int x, int y)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_displayTextTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_displayTextTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x - width / 2;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_displayTextRectangle.push_back(textRectangle);

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _displayTextTexture.back(), NULL, &_displayTextRectangle.back());
}

void UISystem::CreateDisplayTextColoured(std::string message, int x, int y, Uint8 r, Uint8 b, Uint8 g, Uint8 a)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ r, g, b, a });

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, surface);
	_displayTextTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_displayTextTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x - width / 2;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_displayTextRectangle.push_back(textRectangle);

	SDL_FreeSurface(surface);
	SDL_RenderCopy(_renderer, _displayTextTexture.back(), NULL, &_displayTextRectangle.back());
}


const std::vector<SDL_Rect>& UISystem::GetDisplayTextRectangle() const
{
	return _displayTextRectangle;
}
const std::vector<SDL_Rect>& UISystem::GetInteractiveTextRectangle() const
{
	return _interactiveTextRectangle;
}
