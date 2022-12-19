#pragma once

#include <Components/Component/Component.h>
#include <Command/Command.h>

class PieceSpawner final : public Integrian2D::Component
{
public:
	PieceSpawner(Integrian2D::GameObject* pOwner);

	virtual Integrian2D::Component* Clone(Integrian2D::GameObject* pOwner) noexcept override;
	virtual void Start() override;
	virtual void Update() override;

	class Tetromino* GenerateNewPiece();

private:
	class Board* m_pBoard;
	int8_t m_PreviousPiece;
};


class SpawnPiece final : public Integrian2D::Command
{
public:
	SpawnPiece(Integrian2D::Scene* const pScene, PieceSpawner* pSpawner)
		: Command{ pScene }
		, m_pSpawner{ pSpawner }
	{}

	virtual void Execute() override
	{
		m_pSpawner->GenerateNewPiece();
	}

private:
	PieceSpawner* m_pSpawner;
};