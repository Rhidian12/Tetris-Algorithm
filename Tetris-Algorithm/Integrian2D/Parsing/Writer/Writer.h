#pragma once

#include "../../Integrian2D_API.h"

#include <string>
#include <fstream>
#include <type_traits>

#include "../../Utils/Utils.h"

namespace Integrian2D
{
	/* This is a RAII writer to write data to non-binary files */
	class Writer final
	{
	public:
		/* Opens the non-binary file with path filePath */
		INTEGRIAN2D_API Writer(const std::string& filePath);
		INTEGRIAN2D_API ~Writer();

		/* Open the non-binary file */
		INTEGRIAN2D_API void Open() noexcept;

		/* Open a new non-binary file */
		INTEGRIAN2D_API void OpenNewFile(const std::string& filePath) noexcept;

		/* Close the non-binary file
		   This happens automatically on destruction of the writer */
		INTEGRIAN2D_API void Close() noexcept;

		/* Write data to the non-binary file
		   If the requested data has no operator<<, this function will result in a compiler error */
		template<typename Type>
		void Write(const Type& data, const bool newLine) noexcept;

	private:
		std::ofstream m_File;
		std::string m_FileName;
	};

	template<typename Type>
	void Writer::Write(const Type& data, const bool newLine) noexcept
	{
		ASSERT(m_File.is_open(), "Writer::Write() > File has not been opened!");

		m_File << data;

		if (newLine)
			m_File << '\n';
	}
}