#include "AnimationSystem.h"

#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "SpriteComponent.h"
#include "StatusEffectComponent.h"

#include "ConstHolder.h"

AnimationSystem::AnimationSystem()
{
}


AnimationSystem::~AnimationSystem()
{
}

void AnimationSystem::Initialize()
{

}

void AnimationSystem::Process(float dt)
{
	System::Process(dt);

	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
				AnimationComponent* animation = static_cast<AnimationComponent*>(e->GetComponent(Component::Type::Animation));

				if (animation->state != AnimationComponent::State::Bumping && animation->state != AnimationComponent::State::Staggered)
				{
					// Movement
					float currentVelocity = sqrt(physics->xVelocity * physics->xVelocity + physics->yVelocity * physics->yVelocity);

					if (currentVelocity < 1)
					{
						animation->state = AnimationComponent::State::Idle;
					}

					else
					{
						if (currentVelocity > MAX_PLAYER_VELOCITY * 0.7f)
						{
							animation->state = AnimationComponent::State::Fast;
						}

						else if (currentVelocity > MAX_PLAYER_VELOCITY * 0.4f)
						{
							animation->state = AnimationComponent::State::Medium;
						}

						else
						{
							animation->state = AnimationComponent::State::Slow;
						}
					}

					if (physics->xDir != 0 && physics->yDir != 0)
					{
						animation->state = AnimationComponent::State::Turning;
					}
				}

				// Blink Counter
				animation->counter += dt;

				// Bump
				if (animation->state == AnimationComponent::State::Bumping)
				{
					animation->bumpingCounter += dt;
				}

				FSM(animation, e);
			}
		}
	}
}

void AnimationSystem::FSM(AnimationComponent*& animation, Entity* e)
{
	SpriteComponent* spriteComponent = static_cast<SpriteComponent*>(e->GetComponent(Component::Type::Sprite));

	switch (animation->state)
	{
	case AnimationComponent::State::Idle:
	{
		spriteComponent->sourceRect.y = (int)animation->state * spriteComponent->sourceRect.h;

		if (animation->counter > 2)
		{
			spriteComponent->sourceRect.x = spriteComponent->sourceRect.w;
		}

		if (animation->counter > 2.2)
		{
			spriteComponent->sourceRect.x = 0;
			animation->counter -= 2.2;
		}

		break;
	}
	case AnimationComponent::State::Slow:
	{
		spriteComponent->sourceRect.y = (int)animation->state * spriteComponent->sourceRect.h;

		if (animation->counter > 2)
		{
			spriteComponent->sourceRect.x = spriteComponent->sourceRect.w;
		}

		if (animation->counter > 2.2)
		{
			spriteComponent->sourceRect.x = 0;
			animation->counter -= 2.2;
		}

		break;
	}
	case AnimationComponent::State::Medium:
	{
		spriteComponent->sourceRect.y = (int)animation->state * spriteComponent->sourceRect.h;

		if (animation->counter > 2)
		{
			spriteComponent->sourceRect.x = spriteComponent->sourceRect.w;
		}

		if (animation->counter > 2.2)
		{
			spriteComponent->sourceRect.x = 0;
			animation->counter -= 2.2;
		}

		break;
	}
	case AnimationComponent::State::Fast:
	{
		spriteComponent->sourceRect.y = (int)animation->state * spriteComponent->sourceRect.h;

		if (animation->counter > 2)
		{
			spriteComponent->sourceRect.x = spriteComponent->sourceRect.w;
		}

		if (animation->counter > 2.2)
		{
			spriteComponent->sourceRect.x = 0;
			animation->counter -= 2.2;
		}

		break;
	}
	case AnimationComponent::State::Turning:
	{
		spriteComponent->sourceRect.y = (int)animation->state * spriteComponent->sourceRect.h;

		if (animation->counter > 2)
		{
			spriteComponent->sourceRect.x = spriteComponent->sourceRect.w;
		}

		if (animation->counter > 2.2)
		{
			spriteComponent->sourceRect.x = 0;
			animation->counter -= 2.2;
		}

		break;
	}
	case AnimationComponent::State::Bumping:
	{
		spriteComponent->sourceRect.y = (int)animation->state * spriteComponent->sourceRect.h;

		if (animation->counter > 2)
		{
			spriteComponent->sourceRect.x = spriteComponent->sourceRect.w;
		}

		if (animation->counter > 2.2)
		{
			spriteComponent->sourceRect.x = 0;
			animation->counter -= 2.2;
		}

		if (animation->bumpingCounter > 0.2f)
		{
			animation->state = AnimationComponent::State::Idle;
			animation->bumpingCounter -= 0.2f;
		}

		break;
	}
	case AnimationComponent::State::Staggered:
	{
		spriteComponent->sourceRect.y = (int)animation->state * spriteComponent->sourceRect.h;

		if (animation->counter > 2)
		{
			spriteComponent->sourceRect.x = spriteComponent->sourceRect.w;
		}

		if (animation->counter > 2.2)
		{
			spriteComponent->sourceRect.x = 0;
			animation->counter -= 2.2;
		}

		StatusEffectComponent* status = static_cast<StatusEffectComponent*>(e->GetComponent(Component::Type::StatusEffect));
		if (!status->staggered)
		{
			animation->state = AnimationComponent::State::Idle;
		}

		break;
	}

	default:
		break;
	}
}

// entity id
// entity state
// state chack
// state set