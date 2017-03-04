#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(float updateRate)
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Initialize(SDL_Renderer *& renderer)
{
	_renderer = renderer;
}

void ParticleSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				ParticleComponent* particle = static_cast<ParticleComponent*>(e->GetComponent(Component::Type::Particle));

				for (int j = 0; j < particle->m_particleManagers.size(); ++j)
				{
					ParticleManager * particleManager = &particle->m_particleManagers[j];

					particleManager->update(dt);
					particleManager->render(_renderer);
				}
			}
		}
	}
}