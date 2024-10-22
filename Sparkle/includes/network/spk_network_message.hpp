#pragma once

#include <stdint.h>
#include "data_structure/spk_data_buffer.hpp"

namespace spk::Network
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
		/**
		 * \brief Represents the header of a Message object.
		 */
		class Header
		{
		public:
            using Type = int32_t; /**< Type of the message */
            using EmiterID = long long; /**< ID of the emiter */
			friend class Message;

		private:
			Type _type; /**< Identifier of the message */
			uint32_t _size; /**< Size of the message */
			mutable EmiterID _emiter; /**< Emitter of the message */

		public:
            /**
             * @brief Constructs a new Header with specified ID.
             *
             * @param p_type Identifier for the new header.
             */
            template <typename TMessageType = Type>
			Header(TMessageType p_type = 0) :
                _type(static_cast<Type>(p_type)),
                _size(0)
            {

            }

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
			const Type& type() const
            {
                return (_type);
            }

            /**
             * @brief Returns the ID of the header.
             *
             * @return ID of the header.
             */
            template <typename TMessageType>
			const TMessageType typeAs() const
            {
                return (static_cast<TMessageType>(_type));
            }

            /**
             * @brief Sets the type of the header.
             *
             * @param p_type New type for the header.
             */
            template <typename TMessageType>
			void setType(TMessageType p_type)
            {
                _type = static_cast<Type>(p_type);
            }

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

		using Type = Header::Type; ///< Redirection of the Type describe in Header class

	private:
		Header _header; /**< Header of the message */
		DataBuffer _content; /**< Content of the message */
	
	public:
        /**
         * @brief Constructs a new Message with specified ID.
         *
         * @param p_type Identifier for the new message.
         */
        template <typename TMessageType = Type>
		Message(TMessageType p_type = 0) : _header(p_type)
        {

        }

        /**
         * @brief Creates an answer message with the specified ID.
         *
         * @param p_type ID of the answer message.
         * @return A new message.
         */
		Message createAwnser(Header::Type p_type) const;

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
         * @brief Check if the message is empty or if there is still data to read.
         * 
         * @return Return true if there is no data left inside the message, false otherwise.
        */
        bool empty() const;

		/**
		 * @brief Resize the amount of data stored inside the message.
		 *
		 * @param p_newSize The number of bytes required inside the message.
		 */
		void resize(const size_t &p_newSize);

        /**
         * @brief Skip a certain number of bytes inside the message
		 *
		 * @param p_number The number of bytes to skip.
         */
		void skip(const size_t &p_number);

        /**
         * @brief Clear the content of the message.
         * @note The message will have a size of zero after this method is called.
         * You may want to check the "reset" method.
         */
		void clear();

        /**
         * @brief Reset the bookmark inside the message, allowing you to re-read it from start.
        */
		void reset();

		/**
		 * @brief Retrieve data of a specific type from the buffer.
		 *
		 * @tparam OutputType The type of data to retrieve.
		 * @return The retrieved data.
		 */
		template <typename OutputType>
		OutputType get() const
		{
			return (_content.get<OutputType>());
		}

		/**
		 * @brief Edit the buffer at a specific offset with input data.
		 *
		 * @tparam InputType The type of input data.
		 * @param p_offset Offset in the buffer where the data should be written.
		 * @param p_input The data to write into the buffer.
		 */
		template <typename InputType>
		void edit(const size_t& p_offset, const InputType& p_input)
		{
			_content.edit(p_offset, p_input);
		}


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