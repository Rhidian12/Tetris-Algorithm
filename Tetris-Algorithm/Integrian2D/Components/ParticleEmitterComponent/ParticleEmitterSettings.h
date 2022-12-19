#pragma once

#include "../../Integrian2D_API.h"
#include "../../Math/TypeDefines.h"

namespace Integrian2D
{
	enum class EmitterShape
	{
		Circle,
		Cone
	};

	template<EmitterShape Shape>
	struct INTEGRIAN2D_API ParticleEmitterSettings final
	{
#pragma region Always Present
		/* The minimum size each particle can be when it is spawned */
		float minimumSpawnSize{ 0.1f };

		/* The maximum size each particle can be when it is spawned */
		float maximumSpawnSize{ 2.f };

		/* The minimum lifetime of each particle, measured in seconds */
		float minimumTime{ 1.f };

		/* The maximum lifetime of each particle, measured in seconds */
		float maximumTime{ 2.f };

		/* The starting velocity of the particles, this should be a normalized vector
			Note that the particles will deviate from this velocity */
		Vector2f startingVelocity{};

		/* The minimum radius that the particles are spawned in */
		float minimumEmitterRange{ 0.f };

		/* The maximum radius that the particles are spawned in */
		float maximumEmitterRange{ 10.f };

		/* The percentual minimum change in size during the particle's lifetime */
		float minimumSizeGrow{ -1.f };

		/* The percentual maximum change in size during the particle's lifetime */
		float maximumSizeGrow{ 1.f };

		/* The interval between each spawned particle (ie how many seconds per particle spawn) */
		float spawnInterval{ 1.f };

		/* The maximum amount of particles to spawn */
		int maximumNumberOfParticles{ 50 };

		/* The minimum speed of each particle */
		float minimumParticleSpeed{ 20.f };

		/* The maximum speed of each particle */
		float maximumParticleSpeed{ 50.f };

		/* The colour of each particle */
		RGBColour colour{ Colours::White };
#pragma endregion

#pragma region SFINAE
		template<EmitterShape _Shape = Shape>
		struct INTEGRIAN2D_API Type {};

		template<>
		struct INTEGRIAN2D_API Type<EmitterShape::Cone>
		{
			/* The negative angle of the cone */
			float coneNegativeAngle{ -15.f };

			/* The negative angle of the cone */
			float conePositiveAngle{ 15.f };

			/* The direction the cone is pointing at */
			Vector2f coneDirection{ 0.f, 1.f };
		};
#pragma endregion

		Type<Shape> shapeSpecificData{};
	};

	using ConeEmitterSettings = ParticleEmitterSettings<EmitterShape::Cone>;
	using CircleEmitterSettings = ParticleEmitterSettings<EmitterShape::Circle>;
}