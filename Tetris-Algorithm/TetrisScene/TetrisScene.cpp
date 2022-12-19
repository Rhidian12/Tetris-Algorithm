#include "TetrisScene.h"

#include <GameObject/GameObject.h>
#include <TextureManager/TextureManager.h>
#include <Texture/Texture.h>

#include "../Board/Board.h"
#include "../FrameCounter/FrameCounter.h"
#include "../PieceSpawner/PieceSpawner.h"

TetrisScene::TetrisScene(const std::string& name)
	: Scene{ name.c_str() }
	, m_ObjectsToAdd{}
{
}

void TetrisScene::Start()
{
	using namespace Integrian2D;

	// TextureManager::GetInstance()->AddTexture("I-Tetromino", new Texture{ "Resources/I.png" });
	// TextureManager::GetInstance()->AddTexture("J-Tetromino", new Texture{ "Resources/J.png" });
	// TextureManager::GetInstance()->AddTexture("L-Tetromino", new Texture{ "Resources/L.png" });
	// TextureManager::GetInstance()->AddTexture("O-Tetromino", new Texture{ "Resources/O.png" });
	// TextureManager::GetInstance()->AddTexture("S-Tetromino", new Texture{ "Resources/S.png" });
	// TextureManager::GetInstance()->AddTexture("Z-Tetromino", new Texture{ "Resources/Z.png" });
	// TextureManager::GetInstance()->AddTexture("T-Tetromino", new Texture{ "Resources/T.png" });

	GameObject* pFrameCounter{ new GameObject{} };
	pFrameCounter->AddComponent(new FrameCounter{ pFrameCounter });
	AddGameObject("FrameCounter", pFrameCounter);

	GameObject* pBoard{ new GameObject{} };
	pBoard->AddComponent(new Board{ pBoard });
	AddGameObject("Board", pBoard);

	GameObject* pPieceSpawner{ new GameObject{} };
	pPieceSpawner->AddComponent(new PieceSpawner{ pPieceSpawner });
	AddGameObject("PieceSpawner", pPieceSpawner);
}

void TetrisScene::LateUpdate()
{
	for (Integrian2D::GameObject* pG : m_ObjectsToAdd)
		AddGameObject("SafeObject", pG);

	m_ObjectsToAdd.clear();
}

void TetrisScene::SafeAdd(Integrian2D::GameObject* pG)
{
	m_ObjectsToAdd.push_back(pG);
}
