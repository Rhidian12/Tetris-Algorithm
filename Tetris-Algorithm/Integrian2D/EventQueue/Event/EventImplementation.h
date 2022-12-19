#pragma once

#include "../../Integrian2D_API.h"

#include <string>

namespace Integrian2D
{
	class INTEGRIAN2D_API IEventData
	{
	public:
		virtual ~IEventData() = default;
	};

	template<typename Type>
	class EventData final : public IEventData
	{
	public:
		EventData(const Type& data)
			: m_Data{ data }
		{}

		const Type& GetData() const noexcept
		{
			return m_Data;
		}

	private:
		Type m_Data;
	};

	class INTEGRIAN2D_API EventImplementation final
	{
	public:
		EventImplementation(const char* eventName);

		template<typename Type>
		EventImplementation(const char* eventName, const Type& data);

		const char* GetEvent() const noexcept;

		template<typename Type>
		const Type& GetData() const noexcept;

	private:
		const char* m_Event;
		IEventData* m_pData;
	};

	template<typename Type>
	EventImplementation::EventImplementation(const char* eventName, const Type& data)
		: m_Event{ eventName }
		, m_pData{ new EventData{ data } }
	{}

	template<typename Type>
	const Type& EventImplementation::GetData() const noexcept
	{
		return static_cast<EventData<Type>*>(m_pData)->GetData();
	}
}