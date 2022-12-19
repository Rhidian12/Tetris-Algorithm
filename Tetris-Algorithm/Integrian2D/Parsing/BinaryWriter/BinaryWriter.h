#pragma once

#include "../../Integrian2D_API.h"

#include <string>
#include <fstream>
#include <type_traits>

#include "../../Utils/Utils.h"

namespace Integrian2D
{
	/* This is a RAII writer to write data to binary files */
	class BinaryWriter final
	{
	public:
		/* Opens the file with path fileName */
		INTEGRIAN2D_API BinaryWriter(const std::string& filePath);
		INTEGRIAN2D_API ~BinaryWriter();

		/* Open the binary file
		   This happens automatically on creation of the writer */
		INTEGRIAN2D_API void Open() noexcept;

		/* Open a new binary file */
		INTEGRIAN2D_API void OpenNewFile(const std::string& newFile) noexcept;

		/* Close the binary file
		   This happens automatically on destruction of the writer */
		INTEGRIAN2D_API void Close() noexcept;

		/* Read data from the binary file
		   If the requested data is not POD, this function might result in undefined behaviour */
		template<typename Data>
		void Write(const Data& data) noexcept;

	private:
		std::fstream m_File;
		std::string m_FileName;
	};

	template<typename Data>
	void BinaryWriter::Write(const Data& data) noexcept
	{
		ASSERT(m_File.is_open(), "BinaryWriter could not open the file!");

		typedef std::remove_const_t<std::remove_reference_t<decltype(data)>> removeCVData;

		static_assert(!std::is_same_v<removeCVData, char[]>, "String literals are not supported! Only strings!");

		if constexpr (std::is_same_v<removeCVData, std::string>)
		{
			const size_t size{ data.size() };
			m_File.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
			m_File.write(data.c_str(), data.size());
		}
		else
			m_File.write(reinterpret_cast<const char*>(&data), sizeof(Data));
	}
}