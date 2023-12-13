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
			std::wostream& _outputStream;				///< Reference to output stream.

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
			 * @brief Returns the size of the maximum prefix used.
			 * @return The size of the maximum prefix.
			 */
			size_t prefixSize() const 
			{
				return (_maximumPrefixSize);
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

			void resetPrefix()
			{
				std::lock_guard<std::recursive_mutex> lock(_mutex);
				_prefix= L"";
				_maximumPrefixSize = 0;
			}

			void lock()
			{
				_mutex.lock();
			}

			void unlock()
			{
				_mutex.unlock();
			}
		};

	private:
		IOBuffer buffer; ///< Buffer instance associated with this output stream.

		static inline bool _localeUpdated = false; ///< Whether the global locale has been updated.

		void _setUnicodeLocale()
		{
			// if (_localeUpdated == false)
			// {
			// 	std::locale::global(std::locale("C"));
			// 	std::ios_base::sync_with_stdio(false);
			// 	std::locale::global(std::locale("C"));
			// 	_localeUpdated = true;
			// }
		}

	public:
		/**
		 * @brief Constructor.
		 * @param p_outputStream Reference to the output stream.
		 * @param p_prefix Prefix string for the stream.
		 */
		IOStream(std::wostream& p_outputStream, std::wstring p_prefix = L"") :
			std::wostream(&buffer),
			buffer(p_outputStream, p_prefix)
		{
			_setUnicodeLocale();
		}

		/**
		 * @brief Sets the prefix of the buffer.
		 * @param p_prefix The new prefix string.
		 */
		void setPrefix(const std::wstring& p_prefix)
		{
			buffer.setPrefix(p_prefix);
		}

		void resetPrefix()
		{
			buffer.resetPrefix();
		}

		/**
		 * @brief Returns the size of the maximum prefix used.
		 * @return The size of the maximum prefix.
		 */
		size_t prefixSize() const 
		{
			return (buffer.prefixSize());
		}

		/**
		 * @brief Locks the stream's buffer, ensuring thread safety.
		 *
		 * This method prevents simultaneous access to the stream's buffer from different threads.
		 * It effectively provides a mechanism to ensure that any modifications to the buffer
		 * (such as appending output or changing the prefix) are done in a thread-safe manner.
		 */
		void lock()
		{
			buffer.lock();
		}
		
		/**
		 * @brief Unlocks the stream's buffer, allowing other threads to access it.
		 *
		 * This method ends the period of exclusive access that began with a call to `lock()`.
		 * After a call to `unlock()`, the buffer can be accessed by the next thread that successfully locks it.
		 */
		void unlock()
		{
			buffer.unlock();
		}
	};

	/// @brief Thread-local instances of IOStream for standard output.
	extern thread_local spk::IOStream cout;
	/// @brief Thread-local instances of IOStream for error output.
	extern thread_local spk::IOStream cerr;
}

#ifndef DEBUG_LINE
#define DEBUG_LINE() spk::cout << __FUNCTION__ << " - line: " << __LINE__ << std::endl
#endif