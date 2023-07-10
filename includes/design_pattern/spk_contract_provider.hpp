#pragma once

#include <functional>
#include <deque>
#include <cstddef>
#include <stdexcept>

namespace spk
{
	/**
	 * @brief Construct a Contract with a callback function and a callbackContainer.
	 * 
	 */
	class ContractProvider
	{
	public:
		using Callback = std::function<void()>; //!< Type of data representing a callback function.
		using CallbackContainer = std::deque<Callback>; //!< Type of container for storing callback functions.

		/**
		 * @brief  Represent the ownership of a callback and allow users to interact with it.
		 */
		class Contract
		{
			friend class ContractProvider;

		private:
			bool _isOriginal = true; //!< Indicates whether the contract is the original one or if it has been moved or delegated.
			CallbackContainer *_callbackOwner; //!< Pointer to the container that stores the callback.
			Callback *_callback; //!< Pointer to the callback function.

			/**
			 * @brief Construct a new Contract object.
			 * 
			 * @param p_callbackOwner The container that store the Callback.
			 * @param p_callback The function to register in the Contract.
			 */
			Contract(CallbackContainer *p_callbackOwner, Callback *p_callback);
			
			/**
			 * @brief Disable the construction of a new Contract object by copy.
			 * @param p_other 
			 */
			Contract(const Contract &p_other) = delete;

			/**
			 *  false if has been resign or constructed by move else it's true.
			 */
			bool isOriginal();

		public:
			/**
			 * @brief Default constructor.
			 * Create a Contract without container and job, as a placeholder for contract provided later on.
			 */
			Contract();
			
			/**
			 * @brief Constructor by move , move a Contract object.
			 * Set isOriginal to false.
			 * @param p_other 
			 */
			Contract(Contract &&p_other);
			
			/**
			* @brief Move assignment operator.
			* 
			* @param p_other The Contract object to move.
			* @return Contract& The moved Contract object.
			*/
			Contract& operator=(Contract &&p_other) noexcept;
		
			/**
			 * @brief Destroy the Contract object.
			 * resign and set isOriginal to false.
			 */
			~Contract();
			
			/**
			 * @brief Modifie the Callback function.
			 * @param p_callback The new callback function.
			 */
			void edit(const Callback& p_callback);
			
			/**
			 * @brief Erase the Contract callback function.
			 */
			void resign();
		};

	private:
	protected:
		/**
		 * @brief Add the callback inside the container and create a Contract representing it.
		 * 
		 * @param p_callbackOwner The container that store the Callback.
		 * @param p_callback The function to register in the Contract.
		 * @return Contract 
		 */
		Contract subscribe(CallbackContainer& p_callbackOwner, const Callback& p_callback);
	};
}
