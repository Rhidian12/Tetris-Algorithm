#pragma once

#include "../Utils/Utils.h"
#include "../Point/Point.h"
#include "../Tetromino/Tetromino.h"

#include <Components/Component/Component.h>

#include <string> /* std::string */
#include <array> /* std::array */
#include <queue> /* std::queue */

class TetrisAlgorithm final : public Integrian2D::Component
{
public:
	TetrisAlgorithm(Integrian2D::GameObject* pOwner);

	virtual Integrian2D::Component* Clone(Integrian2D::GameObject* pOwner) noexcept override;

	virtual void Start() override;

	void OnUpdate(const uint64_t currentFrame);

	__NODISCARD bool IsExecutingBestMove() const;

private:
	struct MoveToExecute final
	{
		std::vector<Integrian2D::Point2f> TargetPos;
		uint8_t TargetRotation;
	};

	void FindCurrentPiece();
	void CalculateBestMove();
	void ExecuteBestMove(const uint64_t currentFrame);
	void CalculateClicksToExecute();
	__NODISCARD int8_t EvaluatePosition(const std::vector<Integrian2D::Point2f>& points) const;

	__NODISCARD uint8_t GetRowIndex(const uint8_t index) const;
	__NODISCARD uint8_t GetColumnIndex(const uint8_t index) const;

	/* Algorithm Parameters */
	inline constexpr static int8_t m_ClearLineScore{ static_cast<int8_t>(150) };
	inline constexpr static int8_t m_HoleScore{ static_cast<int8_t>(-50) };
	inline constexpr static int8_t m_BumpinessScore{ static_cast<int8_t>(-30) };
	inline constexpr static int8_t m_AggregateHeightScore{ static_cast<int8_t>(-40) };

	/* Program Information */
	bool m_IsBestMoveCalculated;
	class Board* m_pBoard;
	MoveToExecute m_BestMove;
	bool m_IsExecutingBestMove;
	std::queue<Point> m_ClicksToExecute;

	/* Tetris Information */
	Tetromino* m_pCurrentPiece;
};