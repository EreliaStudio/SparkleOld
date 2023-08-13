#pragma once 

#include <memory>
#include <stdexcept>

namespace spk
{
	/**
	 * @brief Singleton class template.
	 *
	 * This class template provides a base implementation for creating singleton objects.
	 * It ensures that only one instance of the object is created and provides methods to access and release the instance.
	 *
	 * @tparam TType The type of the singleton object.
	 */
	template <typename TType>
	class Singleton
	{
	protected:
		/**
		 * @brief Protected constructor.
		 * This constructor is protected to prevent direct instantiation of the singleton class.
		 */
		Singleton()
		{
		}

		static inline std::shared_ptr<TType> _instance = nullptr; /**< The shared pointer to the singleton instance. */

	public:
		/**
		 * @brief Instantiate the singleton object.
		 * This template function creates a new instance of the singleton object if it doesn't exist.
		 * If an instance already exists, it returns the existing instance.
		 *
		 * @tparam Args The types of the arguments to be passed to the constructor of the singleton object.
		 * @param args The arguments to be passed to the constructor of the singleton object.
		 * @return The shared pointer to the singleton instance.
		 */
		template <typename... Args>
		static constexpr std::shared_ptr<TType> instanciate(Args &&...args)
		{
			if (_instance != nullptr)
				throw std::runtime_error("Can't instanciate an already instancied singleton");
			
			_instance = std::shared_ptr<TType>(new TType(std::forward<Args>(args)...));
			return _instance;
		}

		/**
		 * @brief Get the singleton instance.
		 * This function returns the shared pointer to the singleton instance.
		 *
		 * @return The shared pointer to the singleton instance.
		 */
		static constexpr std::shared_ptr<TType> instance()
		{
			return _instance;
		}

		/**
		 * @brief Get the singleton instance.
		 * This function returns the shared pointer to the singleton instance.
		 *
		 * @return The shared pointer to the singleton instance.
		 */
		static constexpr const std::shared_ptr<const TType> c_instance()
		{
			return _instance;
		}

		/**
		 * @brief Release the singleton instance.
		 *
		 * This function releases the singleton instance by resetting the shared pointer to nullptr.
		 */
		static void release()
		{
			_instance.reset();
		}
	};
}
