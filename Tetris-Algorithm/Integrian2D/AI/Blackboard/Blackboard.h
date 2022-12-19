#pragma once

#include "../../Utils/Utils.h"

#include <unordered_map>
#include <string>
#include <memory>

namespace Integrian2D
{
	class IBlackboardData
	{
	public:
		virtual ~IBlackboardData() = default;
	};

	template<typename Type>
	class BlackboardData final : public IBlackboardData
	{
	public:
		BlackboardData<Type>(const Type& data)
			: m_Data{ data }
		{}

		void ChangeData(const Type& data) noexcept
		{
			m_Data = data;
		}

		Type GetData() const noexcept
		{
			return m_Data;
		}

	private:
		Type m_Data;
	};

	class Blackboard final
	{
	public:
		template<typename Type>
		void AddData(const std::string& id, const Type& data) noexcept
		{
			m_pData.insert(std::make_pair(id, std::make_shared<BlackboardData<Type>>(data)));
		}

		template<typename Type>
		Type GetData(const std::string& id) const noexcept
		{
			auto it{ m_pData.find(id) };

			ASSERT(it != m_pData.cend(), std::string{ "Blackboard::GetData() > the string with id" } + id + " was not found!");
			
			return std::static_pointer_cast<BlackboardData<Type>>(it->second)->GetData();
		}

		template<typename Type>
		void ChangeData(const std::string& id, const Type& data) noexcept
		{
			auto it{ m_pData.find(id) };
			
			ASSERT(it != m_pData.cend(), std::string{ "Blackboard::ChangeData() > the string with id" } + id + " was not found!");

			std::static_pointer_cast<BlackboardData<Type>>(it->second)->ChangeData(data);
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<IBlackboardData>> m_pData;
	};
}