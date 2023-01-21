#include "TetrisAlgorithm.h"
#include "../Utils/Utils.h"

#ifdef _DEBUG
#include <iostream> 
#endif

#include "../Board/Board.h"
#include "../Timer/Timer.h"

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

TetrisAlgorithm::TetrisAlgorithm(Board* pBoard)
	: m_IsBestMoveCalculated{}
	, m_pBoard{ pBoard }
	, m_CurrentPiece{}
	, m_BestMove{}
	, m_IsExecutingBestMove{}
	, m_ClickStart{}
	, m_NrOfPieces{}
	, m_Cooldown{}
	, m_CooldownStart{}
	, m_NrOfLinesCleared{}
	, m_Level{}
	, m_LevelSpeeds{}
{
	constexpr int nrOfLevels{ 30 };
	const int speeds[nrOfLevels]{ 48,43,38,33,28,23,18,13,8,6,5,5,5,4,4,4,3,3,3,2,2,2,2,2,2,2,2,2,2,1 };

	for (int i{}; i < nrOfLevels; ++i)
		m_LevelSpeeds.insert(std::make_pair(i, speeds[i]));
}

#include <fstream>

void TetrisAlgorithm::OnNewPieceSpawned()
{
	const Timepoint t1{ Timer::Now() };

	m_CurrentPiece = Utils::FindCurrentPiece(m_pBoard);

	if (m_CurrentPiece.IsInvalid())
		return;

	OnNewPieceSpawnedImpl();

	CalculateBestMove();
	CalculateClicksToExecute();

	m_IsBestMoveCalculated = true;
	m_IsExecutingBestMove = true;

	const Timepoint t2{ Timer::Now() };

	std::ofstream output{};
	output.open("Measurements.txt", std::ios::app);

	output << m_CurrentPiece.GetShape() << " > " << (t2 - t1).Count<TimeLength::MilliSeconds>() << "\n";
}

void TetrisAlgorithm::Update()
{
	if (m_CurrentPiece.IsInvalid())
		return;

	if (m_IsBestMoveCalculated)
		ExecuteBestMove();
}

int TetrisAlgorithm::GetSpeed() const
{
	return m_LevelSpeeds.at(m_Level);
}

void TetrisAlgorithm::ApplyBestMove()
{
	m_pBoard->Add(m_BestMove.TargetPos);
}

void TetrisAlgorithm::SetCurrentPiece(const Tetromino& tetromino)
{
	m_CurrentPiece = tetromino;
}

bool TetrisAlgorithm::IsExecutingBestMove() const
{
	return m_IsExecutingBestMove;
}

float TetrisAlgorithm::EvaluatePosition(const std::array<Tetris::Point, 4>& points) const
{
	float score{};

	score += m_AggregateHeightWeight * m_pBoard->GetNewAggregateHeight(points);
	score += m_ClearLineWeight * m_pBoard->GetNewNrOfCompletedLines(points);
	score += m_HoleWeight * m_pBoard->GetNewNrOfHoles(points);
	score += m_BumpinessWeight * m_pBoard->GetNewBumpiness(points);

	return score;
}

void TetrisAlgorithm::ExecuteBestMove()
{
	__ASSERT(m_IsBestMoveCalculated);

	const Timer& timer{ Timer::GetInstance() };

	if (m_ClicksToExecute.empty())
	{
		m_IsExecutingBestMove = false;
		m_IsBestMoveCalculated = false;
		m_CurrentPiece.Invalidate();

		OnBestMoveExecuted();

		return;
	}

	const Click& click{ m_ClicksToExecute.front() };

	if (Utils::AreEqual(m_ClickStart.Count(), 0.0))
	{
		m_ClickStart = timer.Now();
		return;
	}

	if (const double dTime{ (timer.Now() - m_ClickStart).Count() };
		dTime >= click.Delay)
	{
		Utils::SendMousePress(click.ClickCoord);
		m_ClicksToExecute.pop();

		m_ClickStart = Timepoint{};
	}
}

int TetrisAlgorithm::GetRowIndex(const int index) const
{
	return static_cast<int>(index / m_BoardSize.x);
}

void TetrisAlgorithm::CalculateClicksToExecute()
{
	/* How many times should we rotate? */
	/* m_CurrentPiece.GetRotation() is guaranteed to be 0 when this function is called */

	__ASSERT(m_CurrentPiece.GetRotation() == 0);

	//if (m_CurrentPiece.GetShape() == TetrominoShape::I)
	//{
	//	__ASSERT(m_CurrentPiece.GetUtmostLeftPiece().x == 3);
	//}
	//else
	//{
	//	__ASSERT(m_CurrentPiece.GetUtmostLeftPiece().x == 4);
	//}

	const int rotDistance{ (m_BestMove.TargetRotation - m_CurrentPiece.GetRotation()) };

	for (int i{}; i < rotDistance; ++i)
	{
		m_ClicksToExecute.push(Click{ m_VirtualPadWindowCoord, 0.0 });
		m_ClicksToExecute.push(Click{ m_RotateRight, 0.0 });

		m_CurrentPiece.Rotate(Tetromino::Rotation::CW);
	}

	const auto getUtmostLeftPiece = [](const std::array<Tetris::Point, 4>& arr)->int
	{
		return std::min_element(arr.cbegin(), arr.cend(), [](const auto& a, const auto& b)
			{
				return a.x < b.x;
			})->x;
	};
	const int bestMoveLeft{ getUtmostLeftPiece(m_BestMove.TargetPos) };

	int counter{};
	if (bestMoveLeft < m_CurrentPiece.GetUtmostLeftPiece().x)
	{
		while (bestMoveLeft != m_CurrentPiece.GetUtmostLeftPiece().x)
		{
			m_ClicksToExecute.push(Click{ m_VirtualPadWindowCoord, 0.0 });
			m_ClicksToExecute.push(Click{ m_LeftCoords, 0.0 });
			m_CurrentPiece.Move(Tetromino::Direction::Left);

			if (++counter > 5)
				break;
		}
	}
	else if (bestMoveLeft > m_CurrentPiece.GetUtmostLeftPiece().x)
	{
		while (bestMoveLeft != m_CurrentPiece.GetUtmostLeftPiece().x)
		{
			m_ClicksToExecute.push(Click{ m_VirtualPadWindowCoord,0.0 });
			m_ClicksToExecute.push(Click{ m_RightCoords,0.0 });
			m_CurrentPiece.Move(Tetromino::Direction::Right);

			if (++counter > 5)
				break;
		}
	}

	m_ClicksToExecute.push(Click{ m_PadsCoords,0.0 });
	m_ClicksToExecute.push(Click{ m_StickyCoords,0.0 });
	m_ClicksToExecute.push(Click{ m_DownCoords,0.0 });

	if (m_Level <= 5)
		m_ClicksToExecute.push(Click{ m_VirtualPadWindowCoord, Timer::GetInstance().GetTimePerFrame() * (20.0 - m_Level) });

	m_ClicksToExecute.push(Click{ m_PadsCoords,0.0 });
	m_ClicksToExecute.push(Click{ m_StickyCoords,0.0 });
}

int TetrisAlgorithm::GetColumnIndex(const int index) const
{
	return static_cast<int>(index % m_BoardSize.x);
}