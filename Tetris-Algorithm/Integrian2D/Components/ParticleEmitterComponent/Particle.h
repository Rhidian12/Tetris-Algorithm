#pragma once

#include "../../Integrian2D_API.h"
#include "../../Math/TypeDefines.h"
#include "../../Utils/Utils.h"
#include "ParticleEmitterSettings.h"

namespace Integrian2D
{
	class Texture;

	class Particle final
	{
	public:
		template<EmitterShape Shape>
		INTEGRIAN2D_API void Initialize(const Point2f& _position, Texture* const pTexture, const ParticleEmitterSettings<Shape>& settings) noexcept;

		INTEGRIAN2D_API void Update() noexcept;
		INTEGRIAN2D_API void Render() const noexcept;

		INTEGRIAN2D_API bool GetIsActive() const noexcept;

	private:
		Point2f m_Position;
		RGBColour m_Colour;
		Texture* m_pTexture;
		Vector2f m_Velocity;

		float m_CurrentSize;
		float m_InitialSize;
		float m_CurrentTime;
		float m_MaximumTime;
		float m_SizeGrow; /* Percentual grow during the particle's lifetime (< 0 == shrink, 0 > == grow) */
		float m_Rotation;
		float m_Speed;
		bool m_IsActive;
	};

	template<EmitterShape Shape>
	void Particle::Initialize(const Point2f& _position, Texture* const pTexture, const ParticleEmitterSettings<Shape>& settings) noexcept
	{
		/* Set the particle to true */
		m_IsActive = true;

		/* Set position */
		m_Position = _position;

		/* Set texture */
		m_pTexture = pTexture;

		/* Set lifetime */
		m_MaximumTime = Utils::RandomNumber(settings.minimumTime, settings.maximumTime);
		m_CurrentTime = m_MaximumTime;

		/* Set size */
		m_InitialSize = Utils::RandomNumber(settings.minimumSpawnSize, settings.maximumSpawnSize);
		m_CurrentSize = m_InitialSize;

		/* Set size grow */
		m_SizeGrow = Utils::RandomNumber(settings.minimumSizeGrow, settings.maximumSizeGrow);

		/* Set rotation */
		m_Rotation = static_cast<float>(Utils::RandomNumber(-Utils::PI, Utils::PI));

		/* Set speed */
		m_Speed = Utils::RandomNumber(settings.minimumParticleSpeed, settings.maximumParticleSpeed);

		/* Move particle in a random direction a random distance away from starting point */
		const Vector2f randomDirection{ Utils::RandomNumber(-1.f, 1.f), Utils::RandomNumber(-1.f, 1.f) };
		const float randomDistance{ Utils::RandomNumber(settings.minimumEmitterRange, settings.maximumEmitterRange) };
		m_Position += Point2f{ randomDirection * randomDistance };

		/* Set velocity and rotate it with the rotation */
		const float c{ cosf(m_Rotation) };
		const float s{ sinf(m_Rotation) };

		m_Velocity.x = settings.startingVelocity.x * c - settings.startingVelocity.y * s;
		m_Velocity.y = settings.startingVelocity.x * s + settings.startingVelocity.y * c;
	}
}