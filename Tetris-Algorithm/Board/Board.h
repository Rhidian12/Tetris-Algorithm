#pragma once

#include <Components/Component/Component.h>

#include "../Utils/Utils.h"

#include <array> /* std::array */
#include <vector> /* std::vector */

class Board final : public Integrian2D::Component
{
public:
	Board(Integrian2D::GameObject* pOwner);

	virtual Component* Clone(Integrian2D::GameObject* pOwner) noexcept override;

	virtual void Start() override;
#ifdef _DEBUG
	virtual void Render() const override;
	virtual void Update() override;
#endif

	void OnFrameUpdate(const uint64_t currentFrame);

	void Remove(const std::vector<Integrian2D::Point2f>& points);
	void Add(const std::vector<Integrian2D::Point2f>& points);
	void SetPiece(class Tetromino* pPiece);

	__NODISCARD bool IsCoordinateOccupied(const Integrian2D::Point2f& coord) const;
	__NODISCARD bool IsAnyRowComplete(const std::vector<Integrian2D::Point2f>& points);
	__NODISCARD bool IsRowComplete(const uint8_t row) const;
	__NODISCARD uint8_t GetNrOfHoles() const;
	__NODISCARD uint8_t GetNewNrOfHoles(const std::vector<Integrian2D::Point2f>& points);
	__NODISCARD uint8_t GetBumpiness() const;
	__NODISCARD uint8_t GetNewBumpiness(const std::vector<Integrian2D::Point2f>& points);
	__NODISCARD uint8_t GetColHeight(const uint8_t col) const;
	__NODISCARD uint8_t GetAggregateHeight() const;
	__NODISCARD uint8_t GetNewAggregateHeight(const std::vector<Integrian2D::Point2f>& points);
	__NODISCARD const std::array<bool, g_NrOfBoard>& GetBoardState() const;
	__NODISCARD class Tetromino* const GetCurrentPiece() const;
	__NODISCARD Integrian2D::Point2f GetStartPos() const;

private:
	void SetBoardState();
	uint8_t ConvertPosToIndex(Integrian2D::Point2f p) const;
#ifdef _DEBUG
	void DebugBoardState() const;
#endif

	/* Board Information */
	std::array<bool, g_NrOfBoard> m_BoardState;
	std::vector<class Tetromino*> m_Tetrominos;
	class Tetromino* m_pCurrentPiece;
};