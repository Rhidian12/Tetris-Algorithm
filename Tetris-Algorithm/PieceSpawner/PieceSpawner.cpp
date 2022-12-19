#include "PieceSpawner.h"

#include <SceneManager/SceneManager.h>
#include <Scene/Scene.h>
#include <GameObject/GameObject.h>
#include <Input/InputManager/InputManager.h>

#include "../Board/Board.h"
#include "../Tetromino/Tetromino.h"
#include "../TetrisScene/TetrisScene.h"

PieceSpawner::PieceSpawner(Integrian2D::GameObject* pOwner)
	: Component{ pOwner }
	, m_pBoard{}
	, m_PreviousPiece{ -1 }
{
	srand(static_cast<unsigned int>(time(nullptr)));
}

Integrian2D::Component* PieceSpawner::Clone(Integrian2D::GameObject* pOwner) noexcept
{
	return new PieceSpawner{ pOwner };
}

void PieceSpawner::Start()
{
	using namespace Integrian2D;

	m_pBoard = SceneManager::GetInstance()->GetActiveScene()->GetGameObject("Board")->GetComponentByType<Board>();

	InputManager::GetInstance()->AddCommand
	(
		GameInput{ KeyboardInput::Space },
		new SpawnPiece{ SceneManager::GetInstance()->GetActiveScene(), this },
		State::OnRelease
	);
}

void PieceSpawner::Update()
{
	using namespace Integrian2D;
}

Tetromino* PieceSpawner::GenerateNewPiece()
{
	using namespace Integrian2D;

	const int8_t firstRoll{ rand() % 8 };
	int8_t acceptedRoll{ firstRoll };

	if (firstRoll == m_PreviousPiece || firstRoll == 7)
	{
		const int8_t secondRoll{ rand() % 7 };
		acceptedRoll = secondRoll;
	}

	m_PreviousPiece = acceptedRoll;

	GameObject* pTetromino{ new GameObject{} };
	Tetromino* pPiece{ new Tetromino{ pTetromino, static_cast<TetrominoShape>(acceptedRoll), m_pBoard->GetStartPos() } };
	pTetromino->AddComponent(pPiece);
	static_cast<TetrisScene*>(Integrian2D::SceneManager::GetInstance()->GetActiveScene())->SafeAdd(pTetromino);

	m_pBoard->SetPiece(pPiece);

	return pPiece;
}
