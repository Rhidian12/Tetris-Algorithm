#pragma once

#include "../Integrian2D_API.h"
#include "../Utils/Utils.h"
#include "Point.h"
#include "Vector.h"
#include "../Logger/Logger.h"

#include <utility>

namespace Integrian2D
{
	/* A Matrix(for example a 3x3 Matrix) looks like this internally:
	   [ A B C ]
	   [ D E F ]
	   [ G H I ] */

	/* A list of available operators:
	   Assume Type is the templated type given to the Matrix 

	   Vector<Columns, Type> operator[](const int row)

	   Type operator()(const int row, const int column)

	   Matrix<Rows, Columns, Type> operator+(const Matrix&, const Matrix&)
	   Matrix<Rows, Columns, Type> operator-(const Matrix&, const Matrix&)

	   Matrix<Rows, Columns, Type> operator*(const Matrix&, const Matrix&)
	   Matrix<Rows, Columns, Type> operator*(const Matrix&, const Type&)
	   Matrix<Rows, Columns, Type> operator*(const Matrix&, const Point&)

	   Matrix<Rows, Columns, Type> operator/(const Matrix&, const Matrix&)
	   Matrix<Rows, Columns, Type> operator/(const Matrix&, const Type&)
	   Matrix<Rows, Columns, Type> operator/(const Matrix&, const Point&)

	   */

	/* A list of available functions:
	   int GetAmountOfRowsInMatrix(const Matrix<Rows, Columns, Type>& m)
	   =>	returns the amount of rows in the provided matrix

	   int GetAmountOfColumnsInMatrix(const Matrix<Rows, Columns, Type>&)
	   =>	returns the amount of columns in the provided matrix

	   static Matrix<Rows, Columns, Type> GetIdentityMatrix()
	   =>	A static function to create an Identity Matrix. 
	   =>	Example call: Matrix3f matrix{ GetIdentityMatrix<3, 3, float>() };

	   Matrix<Rows, Columns, Type> GetMatrixCofactor(const Matrix<Rows, Columns, Type>& m, const int rowToIgnore, const int colToIgnore, const int length)
	   =>	returns the cofactor of a matrix. This function is primarily called by GetAdjointMatrix() and GetDeterminantOfMatrix() internally

	   Type GetDeterminantOfMatrix(const Matrix<Rows, Columns, Type>& m, const int length)
	   =>	returns the determinant of a matrix. This function is primarily called by GetAdjointMatrix() internally
	   =>	if you wish to call this function manually, provide the amount of Rows or Columns as the second parameter

	   Matrix<Columns, Rows, Type> TransposeMatrix(const Matrix<Rows, Columns, Type>& m)
	   =>	returns the transposed matrix of the provided matrix.

	   Matrix<Columns, Rows, Type> GetAdjointMatrix(const Matrix<Columns, Rows, Type>& m)
	   =>	returns the adjoint matrix of the provided matrix. This function is primarily called by GetInverseMatrix() internally

	   Matrix<Rows, Columns, Type> GetInverseMatrix(const Matrix<Rows, Columns, Type>& m)
	   =>	returns the inverse matrix of the provided matrix.
	*/
	template<int Rows, int Columns, typename Type>
	struct INTEGRIAN2D_API Matrix
	{
#pragma region Constructors
		Matrix<Rows, Columns, Type>()
		{
			for (int r{}; r < Rows; ++r)
				for (int c{}; c < Columns; ++c)
					data[r][c] = static_cast<Type>(0.f);
		}
#pragma endregion

#pragma region Rule Of 5
		Matrix<Rows, Columns, Type>(const Matrix<Rows, Columns, Type>& other) noexcept
		{
			for (int r{}; r < Rows; ++r)
				for (int c{}; c < Columns; ++c)
					data[r][c] = other.data[r][c];
		}
		Matrix<Rows, Columns, Type>(Matrix<Rows, Columns, Type>&& other) noexcept
		{
			for (int r{}; r < Rows; ++r)
				for (int c{}; c < Columns; ++c)
					data[r][c] = std::move(other.data[r][c]);
		}
		Matrix<Rows, Columns, Type>& operator=(const Matrix<Rows, Columns, Type>& other) noexcept
		{
			for (int r{}; r < Rows; ++r)
				for (int c{}; c < Columns; ++c)
					data[r][c] = other.data[r][c];

			return *this;
		}
		Matrix<Rows, Columns, Type>& operator=(Matrix<Rows, Columns, Type>&& other) noexcept
		{
			for (int r{}; r < Rows; ++r)
				for (int c{}; c < Columns; ++c)
					data[r][c] = std::move(other.data[r][c]);

			return *this;
		}
#pragma endregion

#pragma region Data
		Type data[Rows][Columns];
#pragma endregion

#pragma region Member Operators
		const Vector<Columns, Type>& operator[](const int r) const noexcept
		{
			ASSERT(r < Rows, "Matrix::operator[] > Index is out of bounds!");
			return *(reinterpret_cast<Vector<Columns, Type>*>(data[r]));
		}

		Vector<Columns, Type>& operator[](const int r) noexcept
		{
			ASSERT(r < Rows, "Matrix::operator[] > Index is out of bounds!");
			return *(reinterpret_cast<Vector<Columns, Type>*>(data[r]));
		}

		const Type& operator()(const int r, const int c) const noexcept
		{
			ASSERT((r < Rows&& c < Columns), "Matrix::operator() > Indices are out of bounds!");
			return data[r][c];
		}

		Type& operator()(const int r, const int c) noexcept
		{
			ASSERT((r < Rows&& c < Columns), "Matrix::operator() > Indices are out of bounds!");
			return data[r][c];
		}
#pragma endregion
	};

#pragma region Arithmetic Operators
	template<int Rows, int Columns, typename Type>
	Matrix<Rows, Columns, Type> operator+(const Matrix<Rows, Columns, Type>& lhs, const Matrix<Rows, Columns, Type>& rhs) noexcept
	{
		Matrix<Rows, Columns, Type> matrix{};

		for (int r{}; r < Rows; ++r)
			for (int c{}; c < Columns; ++c)
				matrix.data[r][c] = lhs.data[r][c] + rhs.data[r][c];

		return matrix;
	}

	template<int Rows, int Columns, typename Type>
	Matrix<Rows, Columns, Type> operator-(const Matrix<Rows, Columns, Type>& lhs, const Matrix<Rows, Columns, Type>& rhs) noexcept
	{
		Matrix<Rows, Columns, Type> matrix{};

		for (int r{}; r < Rows; ++r)
			for (int c{}; c < Columns; ++c)
				matrix.data[r][c] = lhs.data[r][c] - rhs.data[r][c];

		return matrix;
	}

	template<int RowsM1, int ColumnsM1, int RowsM2, int ColumnsM2, typename Type>
	Matrix<RowsM1, ColumnsM1, Type> operator*(const Matrix<RowsM1, ColumnsM1, Type>& lhs, const Matrix<RowsM2, ColumnsM2, Type>& rhs) noexcept
	{
		static_assert(ColumnsM1 == RowsM2, "Matrix::operator*() > Matrix<Rows1, Columns1> * Matrix<Rows2, Columns2>. Columns1 and Rows2 must be equal!");

		Matrix<RowsM1, ColumnsM2, Type> matrix{};

		constexpr int maxMatrixLength{ ColumnsM1 };

		for (int row{}; row < RowsM1; ++row)
			for (int col{}; col < ColumnsM2; ++col)
				for (int i{}; i < maxMatrixLength; ++i)
					matrix.data[row][col] += lhs.data[row][i] * rhs.data[i][col];

		return matrix;
	}

	template<int Rows, int Columns, typename Type>
	Matrix<Rows, Columns, Type> operator*(const Matrix<Rows, Columns, Type>& lhs, const Type& rhs) noexcept
	{
		Matrix<Rows, Columns, Type> matrix{};

		for (int row{}; row < Rows; ++row)
			for (int col{}; col < Columns; ++col)
				matrix.data[row][col] += lhs.data[row][col] * rhs;

		return matrix;
	}

	template<int Rows, int Columns, int P, typename Type>
	Point<P, Type> operator*(const Matrix<Rows, Columns, Type>& lhs, const Point<P, Type>& rhs) noexcept
	{
		static_assert(Columns == P, "Matrix::operator*() > Amount of Columns and Point must be equal");

		Point<P, Type> point{};

		for (int row{}; row < Rows; ++row)
			for (int col{}; col < Columns; ++col)
				point[row] += lhs.data[row][col] * rhs[row];

		return point;
	}

	template<int RowsM1, int ColumnsM1, int RowsM2, int ColumnsM2, typename Type>
	Matrix<RowsM1, ColumnsM1, Type> operator/(const Matrix<RowsM1, ColumnsM1, Type>& lhs, const Matrix<RowsM2, ColumnsM2, Type>& rhs) noexcept
	{
		static_assert(RowsM2 == ColumnsM2, "Matrix::operator/() > The divider must be square!");
		static_assert(ColumnsM1 == RowsM2, "Matrix::operator/() > Matrix<Rows1, Columns1> * Matrix<Rows2, Columns2>. Columns1 and Rows2 must be equal!");

		// this is pain
		// it has cost me blood, sweat and tears to make this
		// if you think it is wrong, congratulations
		// it is right, cunt

		return lhs * GetInverseMatrix(rhs);
	}

	template<int Rows, int Columns, typename Type>
	Matrix<Rows, Columns, Type> operator/(const Matrix<Rows, Columns, Type>& lhs, const Type& rhs) noexcept
	{
		Matrix<Rows, Columns, Type> matrix{};

		for (int row{}; row < Rows; ++row)
			for (int col{}; col < Columns; ++col)
				matrix.data[row][col] += lhs.data[row][col] / rhs;

		return matrix;
	}

	template<int Rows, int Columns, int P, typename Type>
	Matrix<Rows, Columns, Type> operator/(const Matrix<Rows, Columns, Type>& lhs, const Point<P, Type>& rhs) noexcept
	{
		static_assert(Columns == P, "Matrix::operator/() > Amount of Columns and Point must be equal");

		Point<P, Type> point{};

		for (int row{}; row < Rows; ++row)
			for (int col{}; col < Columns; ++col)
				point[row] += lhs.data[row][col] * rhs[row];

		return point;
	}
#pragma endregion

	// == Useful Non-Member Functions ==
	template<int Rows, int Columns, typename Type>
	constexpr int GetAmountOfRowsInMatrix(const Matrix<Rows, Columns, Type>&) noexcept
	{
		return Rows;
	}

	template<int Rows, int Columns, typename Type>
	constexpr int GetAmountOfColumnsInMatrix(const Matrix<Rows, Columns, Type>&) noexcept
	{
		return Columns;
	}

	template<int Rows, int Columns, typename Type>
	Matrix<Rows, Columns, Type> GetIdentityMatrix() noexcept
	{
		static_assert(Rows == Columns, "Identity Matrices must be square!");

		Matrix<Rows, Columns, Type> matrix{};

		for (int i{}; i < Rows; ++i)
			matrix.data[i][i] = static_cast<Type>(1.f);

		return matrix;
	}

	template<int Rows, int Columns, typename Type>
	Matrix<Rows, Columns, Type> GetMatrixCofactor(const Matrix<Rows, Columns, Type>& m, const int rowToIgnore, const int colToIgnore, const int length) noexcept
	{
		static_assert(Rows == Columns, "GetMatrixCofactor() > Matrix must be square!");

		int rowCounter{}, columnCounter{};
		Matrix<Rows, Columns, Type> matrix{};

		for (int r{}; r < length; ++r)
		{
			for (int c{}; c < length; ++c)
			{
				if (r != rowToIgnore && c != colToIgnore)
				{
					matrix.data[rowCounter][columnCounter++] = m.data[r][c];

					if (columnCounter == length - 1)
					{
						columnCounter = 0;
						++rowCounter;
					}
				}
			}
		}

		return matrix;
	}

	template<int Rows, int Columns, typename Type>
	Type GetDeterminantOfMatrix(const Matrix<Rows, Columns, Type>& m, const int length) noexcept
	{
		static_assert(Rows == Columns, "GetDeterminant() > Matrix must be square!");

		if (length == 1)
			return m.data[0][0];
		else if (length == 2)
			return (m.data[0][0] * m.data[1][1]) - (m.data[0][1] * m.data[1][0]); // ad - bc
		else
		{
			Type determinant{};
			int sign{ 1 };

			for (int i{}; i < length; ++i)
			{
				Matrix<Rows, Columns, Type> matrix{ GetMatrixCofactor<Rows, Columns, Type>(m, 0, i, length) };
				determinant += sign * m.data[0][i] * GetDeterminantOfMatrix<Rows, Columns, Type>(matrix, length - 1);
				sign = -sign;
			}

			return determinant;
		}
	}

	template<int Rows, int Columns, typename Type>
	Matrix<Columns, Rows, Type> TransposeMatrix(const Matrix<Rows, Columns, Type>& m) noexcept
	{
		Matrix<Columns, Rows, Type> matrix{};

		for (int r{}; r < Rows; ++r)
			for (int c{}; c < Columns; ++c)
				matrix.data[c][r] = m.data[r][c];

		return matrix;
	}

	template<int Rows, int Columns, typename Type>
	Matrix<Columns, Rows, Type> GetAdjointMatrix(const Matrix<Columns, Rows, Type>& m) noexcept
	{
		static_assert(Rows == Columns, "Matrix::GetAdjointMatrix() > Matrix must be square!");

		Matrix<Columns, Rows, Type> matrix{};

		if constexpr (Rows == 1)
			matrix[0][0] = 1;
		else
		{
			int sign{ 1 };

			for (int r{}; r < Rows; ++r)
			{
				for (int c{}; c < Columns; ++c)
				{
					Matrix<Columns, Rows, Type> cofactorMatrix{ GetMatrixCofactor(m, r, c, Rows) };

					sign = ((r + c) % 2 == 0) ? 1 : -1;
					
					matrix.data[c][r] = sign * GetDeterminantOfMatrix(cofactorMatrix, Rows - 1);
				}
			}
		}

		return matrix;
	}

	template<int Rows, int Columns, typename Type>
	Matrix<Rows, Columns, Type> GetInverseMatrix(const Matrix<Rows, Columns, Type>& m) noexcept
	{
		static_assert(Rows == Columns, "Matrix::GetInverseMatrix() > Matrix must be square!");

		const Type determinant{ GetDeterminantOfMatrix(m, Rows) };

		if (Utils::AreEqual(determinant, static_cast<Type>(0.f)))
		{
			Logger::LogError("Matrix::GetInverseMatrix() > Determinant is zero, no inverse matrix exists! Returning Identity Matrix!");
			return GetIdentityMatrix<Rows, Columns, Type>();
		}

		Matrix<Rows, Columns, Type> adjointMatrix{ GetAdjointMatrix(m) };

		Matrix<Rows, Columns, Type> inverseMatrix{};

		for (int r{}; r < Rows; ++r)
			for (int c{}; c < Columns; ++c)
				inverseMatrix.data[r][c] = adjointMatrix.data[r][c] / determinant;

		return inverseMatrix;
	}
}