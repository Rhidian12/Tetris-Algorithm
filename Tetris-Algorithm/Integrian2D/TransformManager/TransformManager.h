#pragma once

#include <vector>
#include <mutex>

namespace Integrian2D
{
	class TransformComponent;

	class TransformManager final
	{
	public:
		~TransformManager() noexcept;

		void UpdateTransforms() noexcept;

		void AddTransformComponent(TransformComponent* const pTransformComponent) noexcept;
		void RemoveTransformComponent(TransformComponent* const pTransformComponent) noexcept;

		void ForceImmediateRecalculation(TransformComponent* const pTransformComponent) noexcept;
		void NotifyRecalculation() noexcept;

	private:
		friend class Scene;

		TransformManager();
		void InformChildren(TransformComponent* const pParent) noexcept;
		void MoveTree(TransformComponent* const pParent) noexcept;
		bool ShouldRecalculate() const noexcept;

		struct TransformComponentNode final
		{
			TransformComponentNode* pPreviousNode;
			TransformComponent* pTransform;
			TransformComponentNode* pNextNode;
			TransformComponentNode* pParent;
		};

		bool m_HasATransformBeenChanged;

		std::vector<TransformComponentNode*> m_pTransformComponents;

		TransformComponentNode* m_pHead;

		std::mutex m_Mutex;
	};
}