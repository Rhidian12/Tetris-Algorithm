#pragma once

#include "../../Integrian2D_API.h"

#include <string>
#include <fstream>
#include <type_traits>

#include "../../Utils/Utils.h"

namespace Integrian2D
{
	/* This is a RAII reader to read data from binary files */
	class BinaryReader final
	{
	public:
		/* Opens the file with path fileName */
		INTEGRIAN2D_API BinaryReader(const std::string& fileName);
		INTEGRIAN2D_API ~BinaryReader();

		/* Open the binary file
		   This happens automatically on creation of the reader */
		INTEGRIAN2D_API void Open() noexcept;

		/* Open a new binary file */
		INTEGRIAN2D_API void OpenNewFile(const std::string& newFile) noexcept;

		/* Close the binary file 
		   This happens automatically on destruction of the reader */
		INTEGRIAN2D_API void Close() noexcept;

		/* Read data from the binary file 
		   If the requested data is wrong, this function will result in undefined behaviour */
		template<typename Type>
		Type Read() noexcept;

	private:
		std::fstream m_File;
		std::string m_FileName;
	};

	template<typename Type>
	Type BinaryReader::Read() noexcept
	{
		ASSERT(m_File.is_open(), "BinaryReader::Read() > The file has not been opened!");

		if constexpr (std::is_same_v<Type, std::string>)
		{
			size_t stringSize{};

			m_File.read(reinterpret_cast<char*>(&stringSize), sizeof(size_t));

			std::string data{};
			data.resize(stringSize);

			m_File.read(&data[0], stringSize);

			return data;
		}
		else
		{
			Type data{};

			m_File.read(reinterpret_cast<char*>(&data), sizeof(Type));

			return data;
		}
	}
}

