#pragma once

#include "../Integrian2D_API.h"

namespace Integrian2D
{
	class Scene;

	class Command
	{
	public:
		INTEGRIAN2D_API Command(Scene* const pScene);

		INTEGRIAN2D_API virtual ~Command() = default;

		/* When the input related to this command gets triggered, Execute() gets called */
		INTEGRIAN2D_API virtual void Execute() = 0;

		/* Optionally overrideable function for ease-of-use, 
			meaning that this function will never get called internally */
		INTEGRIAN2D_API virtual void Undo() {}

		/* Optionally overrideable function for ease-of-use,
			meaning that this function will never get called internally */
		INTEGRIAN2D_API virtual void Redo() {}

		/* Get the Scene this Command is attached to */
		INTEGRIAN2D_API Scene* const GetScene() const noexcept;

	protected:
		Scene* m_pScene;
	};
}