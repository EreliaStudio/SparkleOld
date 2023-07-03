#pragma once 

#include <memory>
#include <stdexcept>

namespace spk
{
	template <typename TType>
	class Singleton
	{
	protected:
		Singleton()
		{

		}

		static inline std::shared_ptr<TType> _instance = nullptr;

	public:
		template <typename... Args>
		static constexpr std::shared_ptr<TType> instanciate(Args &&...args)
		{
			if (_instance != nullptr)
				throw std::runtime_error("Can't instanciate an already instancied singleton");
			_instance = std::shared_ptr<TType>(new TType (std::forward<Args>(args)...));
			return _instance;
		}

		static constexpr std::shared_ptr<TType> instance()
		{
			return _instance;
		}

		static void release()
		{
			_instance.reset();
		}
	};
}