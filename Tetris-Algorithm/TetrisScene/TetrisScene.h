#pragma once

#include <Scene/Scene.h>
#include <vector>

class TetrisScene final : public Integrian2D::Scene
{
public:
	TetrisScene(const std::string& name);

	virtual void Start() override;

	virtual void LateUpdate() override;

	void SafeAdd(Integrian2D::GameObject* pG);

private:
	std::vector<Integrian2D::GameObject*> m_ObjectsToAdd;
};