#pragma once

#include <string>
#include "math/spk_vector2.hpp"

namespace spk
{
	/**
	 * @class AbstractFrame
	 * @brief Abstract base class for frames.
	 * 
	 * Provides an interface and some member variables common to all frames.
	 */
	class AbstractFrame
	{
	protected:
		/// Holds the size of the frame as a Vector2UInt.
		spk::Vector2UInt _size;
		/// Holds the size of the frame as a Vector2UInt.
		spk::Vector2UInt _anchor;

		/**
		 * @brief Method to handle frame resizing logic.
		 * 
		 * This method is called when the frame is resized.
		 */
		virtual void _onResize() = 0;

		/**
		 * @brief Method to handle setting the size of the frame.
		 * 
		 * This method is called when the size of the frame is set.
		 */
		virtual void _onSetSize() = 0;

	public:
		/**
		 * @brief Constructor that initializes the frame with a size.
		 * 
		 * @param p_size Initial size of the frame as a Vector2UInt.
		 */
		AbstractFrame(const spk::Vector2UInt& p_size);

		/// Deleted copy constructor to prevent copying.
		AbstractFrame(const AbstractFrame&) = delete;

		/// Deleted copy assignment operator to prevent copying.
		AbstractFrame& operator=(const AbstractFrame&) = delete;

		/// Default move constructor.
		AbstractFrame(AbstractFrame&&) = default;

		/// Default move assignment operator.
		AbstractFrame& operator=(AbstractFrame&&) = default;

		/**
		 * @brief Clear the frame's content.
		 * 
		 * This is a pure virtual function to be implemented by derived classes.
		 */
		virtual void clear() = 0;

		/**
		 * @brief Render the frame's content.
		 * 
		 * This is a pure virtual function to be implemented by derived classes.
		 */
		virtual void render() = 0;

		/**
		 * @brief Resize the frame to a new size.
		 * 
		 * @param p_size The new size to set for the frame.
		 */
		void resize(const spk::Vector2UInt& p_size);

		void place(const spk::Vector2Int& p_anchor);

		/**
		 * @brief Set the frame's size.
		 * 
		 * @param p_size The size to set for the frame.
		 */
		void setSize(const spk::Vector2UInt& p_size);

		/**
		 * @brief Get the current size of the frame.
		 * 
		 * @return A constant reference to the Vector2UInt holding the size.
		 */
		const Vector2UInt &size() const;

		const Vector2Int &anchor() const;
	};
}