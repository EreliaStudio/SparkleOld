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
		class IOBuffer : public std::wstringbuf
		{
		private:
			static inline std::recursive_mutex _mutex;
			static inline size_t _maximumPrefixSize = 0;
			std::wstring _prefix;
			std::wostream& _outputStream;

		public:
			IOBuffer(std::wostream& p_outputStream, const std::wstring& p_prefix) :
				_outputStream(p_outputStream),
				_prefix(L"")
			{
				setPrefix(p_prefix);
			}

			~IOBuffer()
			{
				if (pbase() != pptr())
				{
					flush();
				}
			}

			virtual int sync()
			{
				flush();
				return 0;
			}

			void flush()
			{
				_mutex.lock();
				if (_prefix.size() != 0)
				{
					_outputStream << L"[" << std::wstring(_maximumPrefixSize - _prefix.size(), ' ') << _prefix << L"] - ";
				}
				_outputStream << str();
				str(L"");
				_outputStream.flush();
				_mutex.unlock();
			}

			void setPrefix(std::wstring p_prefix)
			{
				_prefix = p_prefix;
				_mutex.lock();
				if (_prefix.size() > _maximumPrefixSize)
				{
					_maximumPrefixSize = _prefix.size();
				}
				_mutex.unlock();
			}
		};

	public:
		IOBuffer buffer;

	public:
		IOStream(std::wostream& p_outputStream, std::wstring p_prefix = L"") :
			std::wostream(&buffer),
			buffer(p_outputStream, p_prefix)
		{
			std::locale::global(std::locale(""));
			std::iostream::sync_with_stdio(false);
			std::locale::global(std::locale("C"));
		}

		void setPrefix(const std::wstring& p_prefix)
		{
			buffer.setPrefix(p_prefix);
		}
	};

	extern thread_local spk::IOStream cout;
	extern thread_local spk::IOStream cerr;
}

#ifndef DEBUG_LINE
#define DEBUG_LINE() spk::cout << __FUNCTION__ << "::" << __LINE__ << std::endl
#endif