#pragma once

#include <sstream>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

namespace spk
{
	/**
	 * @brief An output stream with a custom prefix, suitable for logging or debugging.
	 * @note unicode is supported.
	 */
	class IOStream : public std::wostream
	{
	private:
		/**
		 * @brief Custom buffer to support prefix output and thread safety.
		 */
		class IOBuffer : public std::wstringbuf
		{
		private:
			static inline std::recursive_mutex _mutex;   ///< Mutex for thread safety.
			static inline size_t _maximumPrefixSize = 0; ///< Maximum size of prefix string.
			std::wstring _prefix;						///< Prefix for current instance.
			std::wostream &_outputStream;				///< Reference to output stream.

		public:
			/**
			 * @brief Constructor.
			 * @param p_outputStream Reference to the output stream.
			 * @param p_prefix Prefix string for the stream.
			 */
			IOBuffer(std::wostream& p_outputStream, const std::wstring& p_prefix) :
				_outputStream(p_outputStream),
				_prefix(L"")
			{
				setPrefix(p_prefix);
			}

			/**
			 * @brief Destructor. If the buffer is not empty, it will be flushed.
			 */
			~IOBuffer()
			{
				std::lock_guard<std::recursive_mutex> lock(_mutex);
				if (pbase() != pptr())
				{
					flush();
				}
			}

			/**
			 * @brief Synchronizes the buffer with the output.
			 * @return 0 on success.
			 */
			virtual int sync()

			{
				std::lock_guard<std::recursive_mutex> lock(_mutex);
				flush();
				return 0;
			}

			/**
			 * @brief Flushes the buffer to output.
			 */
			void flush()
			{
				std::lock_guard<std::recursive_mutex> lock(_mutex);

				if (_prefix.size() != 0)
				{
					_outputStream << L"[" << std::wstring(_maximumPrefixSize - _prefix.size(), ' ') << _prefix << L"] - ";
				}
				_outputStream << str();
				str(L"");
				_outputStream.flush();
			}

			/**
			 * @brief Changes the prefix string.
			 * @param p_prefix The new prefix string.
			 */
			void setPrefix(std::wstring p_prefix)
			{
				std::lock_guard<std::recursive_mutex> lock(_mutex);

				_prefix = p_prefix;
				if (_prefix.size() > _maximumPrefixSize)
				{
					_maximumPrefixSize = _prefix.size();
				}
			}
		};

	public:
		IOBuffer buffer; ///< Buffer instance associated with this output stream.

	public:
		/**
		 * @brief Constructor.
		 * @param p_outputStream Reference to the output stream.
		 * @param p_prefix Prefix string for the stream.
		 */
		IOStream(std::wostream &p_outputStream, std::wstring p_prefix = L"") : std::wostream(&buffer),
																			   buffer(p_outputStream, p_prefix)
		{
			std::locale::global(std::locale(""));
			std::ios_base::sync_with_stdio(false);
			std::locale::global(std::locale("C"));
		}

		/**
		 * @brief Sets the prefix of the buffer.
		 * @param p_prefix The new prefix string.
		 */
		void setPrefix(const std::wstring &p_prefix)
		{
			buffer.setPrefix(p_prefix);
		}
	};
  
	/// @brief Thread-local instances of IOStream for standard output and error output.
	extern thread_local spk::IOStream cout;
	extern thread_local spk::IOStream cerr;
}

#ifndef DEBUG_LINE
#define DEBUG_LINE() spk::cout << __FUNCTION__ << "::" << __LINE__ << std::endl
#endif