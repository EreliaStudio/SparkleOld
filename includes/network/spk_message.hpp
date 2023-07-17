#pragma once

#include <stdint.h>
#include "data_structure/spk_data_buffer.hpp"

namespace spk
{
    /**
     * @class Message
     * @brief Class encapsulating a network message.
     *
     * The Message class encapsulates data along with a header. It supports serialization and deserialization of data.
     */
	class Message
	{
		friend class Socket;
	public:
		class Header
		{
		public:
            using Type = int32_t; /**< Type of the message */
            using EmiterID = long long; /**< ID of the emiter */
			friend class Message;

		private:
			Type _id; /**< Identifier of the message */
			uint32_t _size; /**< Size of the message */
			mutable EmiterID _emiter; /**< Emitter of the message */

		public:
            /**
             * @brief Constructs a new Header with specified ID.
             *
             * @param p_id Identifier for the new header.
             */
			Header(Type p_id = 0);

            /**
             * @brief Returns the emitter ID.
             *
             * @return Emitter ID
             */
			const EmiterID& emiterID() const;

            /**
             * @brief Returns the ID of the header.
             *
             * @return ID of the header.
             */
			const Type& id() const;

            /**
             * @brief Sets the type of the header.
             *
             * @param p_id New type for the header.
             */
			void setType(Type p_id);

            /**
             * @brief Sets the emitter ID of the header.
             *
             * @param p_emiterID New emitter ID.
             */
			void setEmiter(EmiterID p_emiterID) const;

            /**
             * @brief Resets the header size.
             *
             * @param p_size New size for the header.
             */
			void reset(uint32_t p_size);
		};

		using Type = Header::Type;

	private:
		Header _header; /**< Header of the message */
		DataBuffer _content; /**< Content of the message */
	
	public:
        /**
         * @brief Constructs a new Message with specified ID.
         *
         * @param p_id Identifier for the new message.
         */
		Message(Header::Type p_id = 0);

        /**
         * @brief Creates an answer message with the specified ID.
         *
         * @param p_id ID of the answer message.
         * @return A new message.
         */
		Message createAwnser(Header::Type p_id) const;

        /**
         * @brief Returns the header of the message.
         *
         * @return Reference to the header of the message.
         */
		Header& header();

        /**
         * @brief Returns the header of the message.
         *
         * @return Constant reference to the header of the message.
         */
		const Header& header() const;

        /**
         * @brief Returns the data of the message.
         *
         * @return Pointer to the data of the message.
         */
		uint8_t* data();

        /**
         * @brief Returns the data of the message.
         *
         * @return Constant pointer to the data of the message.
         */
		const uint8_t* data() const;

        /**
         * @brief Returns the size of the message.
         *
         * @return Size of the message.
         */
		size_t size() const;

        /**
         * @brief Serialization operator.
         *
         * @param p_input Input data to serialize into the message.
         * @return Reference to the message.
         */
		template <typename InputType>
		Message& operator<<(const InputType& p_input)
		{
			_content << p_input;
			_header._size = _content.size();
			return *this;
		}

		/**
         * @brief Deserialization operator.
         *
         * @param p_output Output data to deserialize from the message.
         * @return Constant reference to the message.
         */
		template <typename OutputType>
		const Message& operator>>(OutputType& p_output) const
		{
			_content >> p_output;
			return *this;
		}
	};
}