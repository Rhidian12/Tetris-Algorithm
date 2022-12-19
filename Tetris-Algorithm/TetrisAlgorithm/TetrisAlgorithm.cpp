#include "TetrisAlgorithm.h"

#include "../Utils/Utils.h"
#include "../Board/Board.h"
#include "../FrameCounter/FrameCounter.h"

#include <GameObject/GameObject.h>
#include <SceneManager/SceneManager.h>
#include <Scene/Scene.h>
#ifdef _DEBUG
#include <iostream> 
#endif

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

TetrisAlgorithm::TetrisAlgorithm(Integrian2D::GameObject* pOwner)
	: Component{ pOwner }
	, m_IsBestMoveCalculated{}
	, m_pBoard{}
	, m_pCurrentPiece{}
	, m_BestMove{}
	, m_IsExecutingBestMove{}
{}

Integrian2D::Component* TetrisAlgorithm::Clone(Integrian2D::GameObject* pOwner) noexcept
{
	return new TetrisAlgorithm{ pOwner };
}

void TetrisAlgorithm::Start()
{
	using namespace Integrian2D;

	m_pBoard = m_pOwner->GetComponentByType<Board>();

	SceneManager::GetInstance()->GetActiveScene()->GetGameObject("FrameCounter")->
		GetComponentByType<FrameCounter>()->GetDelegate().Bind(this, &TetrisAlgorithm::OnUpdate);
}

void TetrisAlgorithm::OnUpdate(const uint64_t currentFrame)
{
	if (m_pCurrentPiece->IsInvalid())
	{
		if (currentFrame % 53u == 0u)
		{
			/* Find our current moving piece */
			FindCurrentPiece();
		}
	}
	else
	{
		if (!m_IsBestMoveCalculated)
		{
			CalculateBestMove();
			CalculateClicksToExecute();

			m_IsBestMoveCalculated = true;
			m_IsExecutingBestMove = true;
		}

		ExecuteBestMove(currentFrame);
	}
}

bool TetrisAlgorithm::IsExecutingBestMove() const
{
	return m_IsExecutingBestMove;
}

void TetrisAlgorithm::FindCurrentPiece()
{
	__ASSERT(m_pBoard != nullptr);

	m_pCurrentPiece = m_pBoard->GetCurrentPiece();
}

void TetrisAlgorithm::CalculateBestMove()
{
	__ASSERT(!m_pCurrentPiece->IsInvalid() && m_pCurrentPiece->GetShape() != TetrominoShape::NONE);

	Tetromino* tempPiece{ static_cast<Tetromino*>(m_pCurrentPiece->Clone(m_pOwner)) };

	/* Move the piece to the left as long as it's possible */
	while (tempPiece->Move(Tetromino::Direction::Left)) {}

	MoveToExecute move{};
	int8_t max{ std::numeric_limits<int8_t>::min() };

	for (uint8_t i{}; i < g_MaxNrOfBlocks; ++i)
	{
		for (uint8_t j{}; j < g_BoardSize.x; ++j)
		{
			/* Move the piece as much down as possible */
			while (tempPiece->Move(Tetromino::Direction::Down)) {}

			const int8_t score{ EvaluatePosition(tempPiece->GetCurrentPosition()) };

			if (score > max)
			{
				max = score;
				move.TargetPos = tempPiece->GetCurrentPosition();
				move.TargetRotation = tempPiece->GetRotation();
			}
		}

		tempPiece->Rotate(Tetromino::Rotation::CW);
	}

	m_BestMove = move;
}

int8_t TetrisAlgorithm::EvaluatePosition(const std::vector<Integrian2D::Point2f>& points) const
{
	int8_t score{};

	/* Greedy Approach: Does it clear any line? */
	if (m_pBoard->IsAnyRowComplete(points))
		score += m_ClearLineScore; /* excellent move, and we really want to do it */

	/* Check for holes */
	if (m_pBoard->GetNewNrOfHoles(points) > 0u)
		score += m_HoleScore;

	/* Check for bumpiness */
	if (m_pBoard->GetNewBumpiness(points) > 0u)
		score += m_BumpinessScore;

	/* Check for the aggregrate height */
	if (m_pBoard->GetNewAggregateHeight(points) > 0u)
		score += m_AggregateHeightScore;

	/* [TODO]: Add the total inputs required for this position to the final score */

	return score;
}

void TetrisAlgorithm::ExecuteBestMove(const uint64_t currentFrame)
{
	__ASSERT(m_IsBestMoveCalculated);

	if (currentFrame % 3u == 0u)
	{
		m_ClicksToExecute.pop();
	}

	if (m_ClicksToExecute.empty())
	{
		m_pCurrentPiece = nullptr;
		m_IsExecutingBestMove = false;
		m_IsBestMoveCalculated = false;
	}
}

uint8_t TetrisAlgorithm::GetRowIndex(const uint8_t index) const
{
	return static_cast<uint8_t>(index / static_cast<uint32_t>(g_BoardSize.x));
}

void TetrisAlgorithm::CalculateClicksToExecute()
{
	/* How many times should we rotate? */
	/* m_pCurrentPiece.GetRotation() is guaranteed to be 0 when this function is called */

	//__ASSERT(m_pCurrentPiece->GetRotation() == 0);

	//const int8_t rotDistance{ (m_BestMove.TargetRotation - m_pCurrentPiece->GetRotation()) };

	//for (int8_t i{}; i < rotDistance; ++i)
	//	m_ClicksToExecute.push(m_RotateRight);

	///* how far we should move horizontally? */
	//const int8_t horDistance{ static_cast<int8_t>((m_BestMove.TargetPos[0] - m_pCurrentPiece->GetCurrentPosition()[0]).x) };

	//if (horDistance < 0)
	//	for (int8_t i{}; i < abs(horDistance); ++i)
	//		m_ClicksToExecute.push(m_LeftCoords);
	//else if (horDistance > 0)
	//	for (int8_t i{}; i < horDistance; ++i)
	//		m_ClicksToExecute.push(m_RightCoords);

	//m_ClicksToExecute.push(m_DownCoords);
}

uint8_t TetrisAlgorithm::GetColumnIndex(const uint8_t index) const
{
	return static_cast<uint8_t>(index % static_cast<uint32_t>(g_BoardSize.x));
}