#pragma once

#include <iterator>

namespace Integrian2D
{
#pragma region Iterator
	template<typename IteratorValue,
		typename IteratorType = std::bidirectional_iterator_tag,
		typename DifferenceType = std::ptrdiff_t,
		typename = std::enable_if_t<std::is_base_of_v<std::input_iterator_tag, IteratorType>>,
		typename = std::enable_if_t<std::is_integral_v<DifferenceType>>>
	class Iterator final
	{
		using IteratorPointerType = IteratorValue*;
		using IteratorReferenceType = IteratorValue&;

	public:
		Iterator(IteratorPointerType pPointer) noexcept
			: m_Pointer{ pPointer }
		{
			static_assert(!std::is_same_v<IteratorType, std::random_access_iterator_tag>, "Integrian2D::Iterator > This class does nut support random access iterator!");
		}

		IteratorReferenceType operator*() const noexcept
		{
			return *m_Pointer;
		}

		IteratorPointerType operator->() noexcept
		{
			return m_Pointer;
		}

		Iterator& operator++() noexcept
		{
			++m_Pointer;
			return *this;
		}

		Iterator operator++(int) noexcept
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		Iterator& operator--() noexcept
		{
			static_assert(std::is_same_v<IteratorType, std::bidirectional_iterator_tag>, "Iterator::operator--() > The iterator type is not a bidirectional iterator");

			--m_Pointer;
			return *this;
		}

		Iterator operator--(int) noexcept
		{
			static_assert(std::is_same_v<IteratorType, std::bidirectional_iterator_tag>, "Iterator::operator--(int) > The iterator type is not a bidirectional iterator");

			Iterator tmp = *this;
			--(*this);
			return tmp;
		}

		bool operator==(const Iterator& other) const noexcept
		{
			return m_Pointer == other.m_Pointer;
		}
		
		bool operator!=(const Iterator& other) const noexcept
		{
			return m_Pointer != other.m_Pointer;
		}

	private:
		IteratorPointerType m_Pointer;
	};
#pragma endregion

#pragma region ConstIterator
	template<typename IteratorValue,
		typename IteratorType = std::bidirectional_iterator_tag,
		typename DifferenceType = std::ptrdiff_t,
		typename = std::enable_if_t<std::is_base_of_v<std::input_iterator_tag, IteratorType>>,
		typename = std::enable_if_t<std::is_integral_v<DifferenceType>>>
	class ConstIterator final
	{
		using IteratorPointerType = const IteratorValue*;
		using IteratorReferenceType = const IteratorValue&;

	public:
		ConstIterator(IteratorPointerType pPointer) noexcept
			: m_Pointer{ pPointer }
		{
			static_assert(!std::is_same_v<IteratorType, std::random_access_iterator_tag>, "Integrian2D::Iterator > This class does nut support random access iterator!");
		}

		IteratorReferenceType operator*() const noexcept
		{
			return *m_Pointer;
		}

		IteratorPointerType operator->() noexcept
		{
			return m_Pointer;
		}

		ConstIterator& operator++() noexcept
		{
			++m_Pointer;
			return *this;
		}

		ConstIterator operator++(int) noexcept
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		ConstIterator& operator--() noexcept
		{
			static_assert(std::is_same_v<IteratorType, std::bidirectional_iterator_tag>, "Iterator::operator--() > The iterator type is not a bidirectional iterator");

			--m_Pointer;
			return *this;
		}

		ConstIterator operator--(int) noexcept
		{
			static_assert(std::is_same_v<IteratorType, std::bidirectional_iterator_tag>, "Iterator::operator--(int) > The iterator type is not a bidirectional iterator");

			Iterator tmp = *this;
			--(*this);
			return tmp;
		}

		bool operator==(const ConstIterator& other) const noexcept
		{
			return m_Pointer == other.m_Pointer;
		}

		bool operator!=(const ConstIterator& other) const noexcept
		{
			return m_Pointer != other.m_Pointer;
		}

	private:
		IteratorPointerType m_Pointer;
	};
#pragma endregion
}