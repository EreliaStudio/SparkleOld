#pragma once

#include "design_pattern/spk_singleton.hpp"

namespace spk
{
	template <typename TContextType>
	class ContextManager : public spk::Singleton<ContextManager<TContextType>>
	{
		friend class spk::Singleton<ContextManager<TContextType>>;
		friend class ReadWriteAccessor;
		friend class ReadOnlyAccessor;

	public:
		class ReadWriteAccessor
		{
		public:
			static TContextType &get()
			{
				if (ContextManager::instance() == nullptr)
					throw std::runtime_error("Context manager not initialized");
				return (*ContextManager::instance()->_readWriteContext);
			}

			static void swap()
			{
				if (ContextManager::instance() == nullptr)
					throw std::runtime_error("Context manager not initialized");
				ContextManager::instance()->swapReadWriteContext();
			}
		};

		class ReadOnlyAccessor
		{
		public:
			static const TContextType &get()
			{
				if (ContextManager::instance() == nullptr)
					throw std::runtime_error("Context manager not initialized");
				return (*ContextManager::instance()->_readOnlyContext);
			}
			
			static void swap()
			{
				if (ContextManager::instance() == nullptr)
					throw std::runtime_error("Context manager not initialized");
				ContextManager::instance()->swapReadOnlyContext();
			}
		};

	protected:
		std::recursive_mutex _bufferAccessMutex;
		std::unique_ptr<TContextType> _readWriteContext;
		std::unique_ptr<TContextType> _readOnlyContext;
		std::unique_ptr<TContextType> _intermediaryBuffer;
		bool _isSwapRequested;


		template <typename... Args>
		ContextManager(Args &&...p_args) :
			_readWriteContext(std::make_unique<TContextType>(std::forward<Args>(p_args)...)),
			_readOnlyContext(std::make_unique<TContextType>(std::forward<Args>(p_args)...)),
			_intermediaryBuffer(std::make_unique<TContextType>(std::forward<Args>(p_args)...)), _isSwapRequested(false)
		{
			
		}

		void swapReadWriteContext()
		{
			std::lock_guard<std::recursive_mutex> lock(_bufferAccessMutex);
			
			_readWriteContext.swap(_intermediaryBuffer);
			_isSwapRequested = true;
		}

		void swapReadOnlyContext()
		{
			std::lock_guard<std::recursive_mutex> lock(_bufferAccessMutex);

			_readOnlyContext.swap(_intermediaryBuffer);
			_isSwapRequested = false;
		}

	public:
		ContextManager(const ContextManager&) = delete;
		ContextManager& operator=(const ContextManager&) = delete;
	};
}