#pragma once

#include "design_pattern/spk_singleton.hpp"

namespace spk
{
	/**
	 * \class ContextManager
	 * \brief ContextManager is a singleton template class that manages ReadWrite and ReadOnly access to a common context.
	 *
	 * This class provides access to a context object for read and write operations in a thread safe manner. 
	 * It uses double buffering technique, thus ensuring that read operations are not blocked during write operations.
	 *
	 * \tparam TContextType The type of the context object this class is managing.
	 */
	template <typename TContextType>
	class ContextManager : public spk::Singleton<ContextManager<TContextType>>
	{
		friend class spk::Singleton<ContextManager<TContextType>>;
		friend class ReadWriteAccessor;
		friend class ReadOnlyAccessor;

	public:
		/**
		 * \class ReadWriteAccessor
		 * \brief This class provides static methods to access and swap the context object for read and write operations.
		 */
		class ReadWriteAccessor
		{
		public:
			/**
			 * \brief Fetches the context object for read/write operations.
			 * \throws std::runtime_error if the ContextManager singleton instance is not initialized.
			 * \return A reference to the read/write context object.
			 */
			static TContextType &get()
			{
				if (ContextManager::instance() == nullptr)
					throw std::runtime_error("Context manager not initialized");
				return (*ContextManager::instance()->_readWriteContext);
			}
			/**
			 * \brief Swaps the context object used for read/write operations with an intermediary buffer.
			 * \throws std::runtime_error if the ContextManager singleton instance is not initialized.
			 */
			static void swap()
			{
				if (ContextManager::instance() == nullptr)
					throw std::runtime_error("Context manager not initialized");
				ContextManager::instance()->swapReadWriteContext();
			}
		};
		/**
		 * \class ReadOnlyAccessor
		 * \brief This class provides static methods to access and swap the context object for read operations.
		 */
		class ReadOnlyAccessor
		{
		public:
			/**
			 * \brief Fetches the context object for read operations.
			 * \throws std::runtime_error if the ContextManager singleton instance is not initialized.
			 * \return A const reference to the read-only context object.
			 */
			static const TContextType &get()
			{
				if (ContextManager::instance() == nullptr)
					throw std::runtime_error("Context manager not initialized");
				return (*ContextManager::instance()->_readOnlyContext);
			}

			/**
			 * \brief Indicate if a swap is requested for the ReadOnlyBuffer.
			 */
			static bool swapRequested()
			{
				return (*ContextManager::instance()->_isSwapRequested);
			}
	

			/**
			 * \brief Swaps the context object used for read operations with an intermediary buffer.
			 * \throws std::runtime_error if the ContextManager singleton instance is not initialized.
			 */
			static void swap()
			{
				if (ContextManager::instance() == nullptr)
					throw std::runtime_error("Context manager not initialized");
				ContextManager::instance()->swapReadOnlyContext();
			}
		};

	protected:
		std::recursive_mutex _bufferAccessMutex; /*!< Mutex to protect context swapping operations. */
		std::unique_ptr<TContextType> _readWriteContext; /*!< The context object used for read/write operations. */
		std::unique_ptr<TContextType> _readOnlyContext; /*!< The context object used for read operations. */
		std::unique_ptr<TContextType> _intermediaryBuffer; /*!< An intermediary buffer for swapping contexts. */
		bool _isSwapRequested; /*!< Flag to indicate whether a context swap has been requested. */

		/**
		 * \brief Construct a new Context Manager object and initializes the contexts and the intermediary buffer.
		 * \tparam Args Variadic template argument.
		 * \param p_args Arguments for the construction of the context object.
		 */
		template <typename... Args>
		ContextManager(Args &&...p_args) :
			_readWriteContext(std::make_unique<TContextType>(std::forward<Args>(p_args)...)),
			_readOnlyContext(std::make_unique<TContextType>(std::forward<Args>(p_args)...)),
			_intermediaryBuffer(std::make_unique<TContextType>(std::forward<Args>(p_args)...)), _isSwapRequested(false)
		{
			
		}

		/**
		 * \brief Swaps the context object used for read/write operations with an intermediary buffer.
		 * Thread safe due to the usage of std::lock_guard.
		 */
		void swapReadWriteContext()
		{
			std::lock_guard<std::recursive_mutex> lock(_bufferAccessMutex);
			
			_readWriteContext.swap(_intermediaryBuffer);
			_isSwapRequested = true;
		}

		/**
		 * \brief Swaps the context object used for read operations with an intermediary buffer.
		 * Thread safe due to the usage of std::lock_guard.
		 */
		void swapReadOnlyContext()
		{
			if (_isSwapRequested == false)
				throw std::runtime_error("ReadOnlyContext does not need to be swapped");
			
			std::lock_guard<std::recursive_mutex> lock(_bufferAccessMutex);

			_readOnlyContext.swap(_intermediaryBuffer);
			_isSwapRequested = false;
		}

	public:
		ContextManager(const ContextManager&) = delete; /*!< Deleted copy constructor to prevent copy operations. */
		ContextManager& operator=(const ContextManager&) = delete; /*!< Deleted copy assignment operator to prevent copy operations. */
	};
}
