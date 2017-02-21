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

void UISystem::Initialize(SDL_Renderer* renderer, Camera2D::Camera* camera)
{
	_renderer = renderer;
	_camera = camera;

	// Text
	TTF_Init();
	_fontSize = 32;
	_font = TTF_OpenFont("Media\\Fonts\\font.ttf", _fontSize);
	_surface = std::vector<SDL_Surface*>();
	_textTexture = std::vector<SDL_Texture*>();
	_textRectangle = std::vector<SDL_Rect>();

	CreateText("FUCK", 25, 450);
	UpdateText("TEST ", 0);


	//TTF_CloseFont(_font); // Free Font Memory
}

void UISystem::Process(float dt)
{
	test++;


	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				UpdateText("TEST " + std::to_string(test), 0);

				// Transform to screen
				TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
				SpriteComponent* sprite = static_cast<SpriteComponent*>(e->GetComponent(Component::Type::Sprite));

				SDL_Rect scaledRect = transform->rect;
				scaledRect.w *= transform->scaleX;
				scaledRect.h *= transform->scaleY;
				scaledRect.x -= transform->origin.x * transform->scaleX;
				scaledRect.y -= transform->origin.y * transform->scaleY;

				if (transform != nullptr && sprite != nullptr)
				{
					//Render to screen
					SDL_RenderCopyEx(_renderer, sprite->texture, &sprite->sourceRect, &scaledRect, transform->angle, &transform->origin, SDL_FLIP_NONE);
				}

				// Text
				for (int i = 0; i < _textTexture.size(); i++)
				{
					SDL_RenderCopy(_renderer, _textTexture[i], NULL, &_textRectangle[i]);
				}
			}
		}
	}
}

void UISystem::CreateText(std::string message, int x, int y)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });
	_surface.push_back(surface);

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, _surface.back());
	_textTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_textTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_textRectangle.push_back(textRectangle);

	SDL_FreeSurface(_surface.back());
	SDL_RenderCopy(_renderer, _textTexture.back(), NULL, &_textRectangle.back());
}

void UISystem::CreateTextColoured(std::string message, int x, int y, Uint8 r, Uint8 b, Uint8 g, Uint8 a)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ r, g, b, a });
	_surface.push_back(surface);

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, _surface.back());
	_textTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_textTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_textRectangle.push_back(textRectangle);

	SDL_FreeSurface(_surface.back());
	SDL_RenderCopy(_renderer, _textTexture.back(), NULL, &_textRectangle.back());
}

void UISystem::UpdateText(std::string message, int index)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });
	_surface[index] = surface;

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, _surface[index]);
	_textTexture[index] = textTexture;

	int width, height;
	SDL_QueryTexture(_textTexture[index], NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = _textRectangle[index].x;
	textRectangle.y = _textRectangle[index].y;
	textRectangle.w = width;
	textRectangle.h = height;
	_textRectangle[index] = textRectangle;

	SDL_FreeSurface(_surface[index]);
	SDL_RenderCopy(_renderer, _textTexture[index], NULL, &_textRectangle[index]);
	SDL_DestroyTexture(_textTexture[index]);

}
