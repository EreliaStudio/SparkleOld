#pragma once

#include "data_structure/spk_data_buffer.hpp"
#include "data_structure/spk_field_map.hpp"
#include "graphics/pipeline/spk_pipeline_basic_functions.hpp"
#include "graphics/pipeline/spk_shader_module.hpp"
#include "graphics/pipeline/spk_shader_layout.hpp"
#include <map>

#include "spk_basic_functions.hpp"

namespace spk
{
	/**
	 * @class AbstractPipeline
	 * @brief Abstract base class for handling different types of shader pipelines.
	 *
	 * AbstractPipeline serves as a base for managing shader programs and their associated uniform blocks,
	 * sampler uniforms, and objects. Derived classes must implement the virtual methods to offer platform-specific
	 * functionalities.
	 */
	class AbstractPipeline
	{
	public:
		/**
		 * @class Object
		 * @brief A class representing a graphical object in a rendering pipeline.
		 *
		 * The Object class encapsulates all the essential components for rendering a graphical object,
		 * including storage buffers for vertices and indexes, as well as push constants for the associated shader.
		 */
		class Object
		{
		public:
			/**
			 * @class Storage
			 * @brief A class to manage storage buffers associated with a shader layout.
			 *
			 * The Storage class serves to manage storage buffers specifically designed for vertices and indexes.
			 * This class encapsulates Buffer objects for vertices and indexes, providing an interface for
			 * managing these Buffer objects.
			 */
			class Storage
			{
			public:
				/**
				 * @class Buffer
				 * @brief A class to handle data buffering operations.
				 *
				 * The Buffer class provides an abstraction for data storage and manipulation
				 * with various methods to interact with the underlying buffer. 
				 */
				class Buffer
				{
				private:
					/// Typedef for wide character output streams
					typedef std::basic_ostream<wchar_t, std::char_traits<wchar_t>> wostream;

					/// Flag to indicate whether the buffer needs to be updated.
					bool _needUpdate;

					/// The name of the buffer for identification.
					std::wstring _name;

					/// The size of each unit in the buffer.
					size_t _unitSize;

					/// The underlying data storage.
					spk::DataBuffer _data;

				public:
					/**
					 * @brief Constructor for the Buffer class.
					 *
					 * @param p_name The name of the buffer.
					 * @param p_unitSize The size of each unit in the buffer.
					 */
					Buffer(const std::wstring &p_name, size_t p_unitSize);

					/**
					 * @brief Clears the buffer.
					 */
					void clear();

					/**
					 * @brief Overload for the insertion operator.
					 *
					 * Inserts a given data item into the buffer.
					 *
					 * @param p_data The data item to be inserted.
					 * @return Reference to the Buffer object.
					 */
					template <typename TType>
					Buffer &operator<<(const TType &p_data)
					{
						if (_unitSize != sizeof(TType))
						{
							spk::throwException(L"Pushing a unit of size [" + std::to_wstring(sizeof(TType)) + L"] into a " + _name + L" buffer of unit size [" + std::to_wstring(_unitSize) + L"]");
						}
						_data << p_data;
						return (*this);
					}

					/**
					 * @brief Overload for the insertion operator for std::vector.
					 *
					 * Appends a vector of data items to the buffer.
					 *
					 * @param p_data The vector of data items to be inserted.
					 * @return Reference to the Buffer object.
					 */
					template <typename TType>
					Buffer &operator<<(const std::vector<TType> &p_data)
					{
						if (_unitSize != sizeof(TType))
						{
							spk::throwException(L"Pushing a unit of size [" + std::to_wstring(sizeof(TType)) + L"] into a " + _name + L" buffer of unit size [" + std::to_wstring(_unitSize) + L"]");
						}
						_data.append(p_data.data(), p_data.size() * sizeof(TType));
						return (*this);
					}

					/**
					 * @brief Overload for insertion operator for wostream manipulators.
					 *
					 * Allows the use of wostream manipulators with the Buffer object.
					 *
					 * @param func The wostream manipulator function.
					 * @return Reference to the Buffer object.
					 */
					Buffer& operator<<(wostream& (*func)(wostream&));

					/**
					 * @brief Sets the update status of the buffer.
					 *
					 * @param p_state The new update state.
					 */
					void setUpdateStatus(bool p_state);

					/**
					 * @brief Checks if the buffer needs to be updated.
					 *
					 * @return True if the buffer needs to be updated, false otherwise.
					 */
					bool needUpdate() const;

					/**
					 * @brief Retrieves the raw data from the buffer.
					 *
					 * @return Pointer to the underlying data.
					 */
					const uint8_t *data() const;

					/**
					 * @brief Retrieves the size of the buffer.
					 *
					 * @return The size of the buffer in bytes.
					 */
					size_t size() const;
				};


			private:
				/// Buffer to hold vertex data.
				Buffer _vertices;

				/// Buffer to hold index data.
				Buffer _indexes;

			public:
				/**
				 * @brief Constructor for the Storage class.
				 *
				 * Initializes the Storage object based on a given StorageBufferLayout from a shader layout.
				 *
				 * @param p_storageBufferLayout Reference to the StorageBufferLayout from a spk::ShaderLayout object.
				 */
				Storage(const spk::ShaderLayout::StorageBufferLayout& p_storageBufferLayout);

				/**
				 * @brief Accessor for the vertex buffer.
				 *
				 * @return Reference to the Buffer object containing vertex data.
				 */
				Buffer &vertices();

				/**
				 * @brief Accessor for the index buffer.
				 *
				 * @return Reference to the Buffer object containing index data.
				 */
				Buffer &indexes();
			};

			/**
			 * @class PushConstants
			 * @brief A class to handle Push Constants within a shader layout.
			 *
			 * The PushConstants class serves as an extension of the spk::FieldMap. It is designed
			 * to work with push constants in the context of a shader layout, allowing for more
			 * streamlined management of these constants.
			 *
			 * @note This class is derived from spk::FieldMap.
			 */
			class PushConstants : public spk::FieldMap
			{
			public:
				/**
				 * @brief Constructor for the PushConstants class.
				 *
				 * Initializes the PushConstants object based on a given PushConstantsLayout from a shader layout.
				 *
				 * @param p_pushConstantsLayout Reference to the PushConstantsLayout from a spk::ShaderLayout object.
				 */
				PushConstants(const spk::ShaderLayout::PushConstantsLayout& p_pushConstantsLayout);
			};

		private:
			/// Pointer to the pipeline that owns this Object.
			AbstractPipeline* _owner;

			/// Storage buffer for holding vertex and index data.
			Storage _storage;

			/// Number of indexes that have been pushed to the index buffer.
			size_t _nbIndexesPushed;

			/// Push constants associated with this Object.
			PushConstants _pushConstants;

			/**
			 * @brief Abstract method for pushing vertex data.
			 *
			 * @param p_data Pointer to the vertex data.
			 * @param p_dataSize Size of the vertex data.
			 */
			virtual void _pushVerticesData(const uint8_t* p_data, size_t p_dataSize) = 0;

			/**
			 * @brief Abstract method for pushing index data.
			 *
			 * @param p_data Pointer to the index data.
			 * @param p_dataSize Size of the index data.
			 */
			virtual void _pushIndexesData(const uint8_t* p_data, size_t p_dataSize) = 0;

			/**
			 * @brief Abstract method for pushing push constants data.
			 *
			 * @param p_data Pointer to the push constants data.
			 * @param p_dataSize Size of the push constants data.
			 */
			virtual void _pushPushConstantsData(const uint8_t* p_data, size_t p_dataSize) = 0;

			/**
			 * @brief Abstract method called during rendering.
			 */
			virtual void _onRender() = 0;

			/// Updates the vertex buffer.
			void _updateVertices();

			/// Updates the index buffer.
			void _updateIndexes();

			/// Updates the push constants.
			void _updatePushConstants();

		public:
			/**
			 * @brief Constructor for the Object class.
			 *
			 * @param p_owner Pointer to the AbstractPipeline that owns this Object.
			 * @param p_storageBufferLayout Reference to the storage buffer layout from a spk::ShaderLayout object.
			 * @param p_pushConstantsLayout Reference to the push constants layout from a spk::ShaderLayout object.
			 */
			Object(AbstractPipeline* p_owner, const spk::ShaderLayout::StorageBufferLayout& p_storageBufferLayout, const spk::ShaderLayout::PushConstantsLayout& p_pushConstantsLayout);

			/**
			 * @brief Executes the rendering of this Object.
			 */
			void render();

			/**
			 * @brief Accessor for the storage buffer associated with this Object.
			 *
			 * @return Reference to the Storage object.
			 */
			Storage& storage();

			/**
			 * @brief Accessor for the push constants associated with this Object.
			 *
			 * @return Reference to the PushConstants object.
			 */
			PushConstants& pushConstants();

			/**
			 * @brief Retrieve the field corresponding to the provided name
			 * @param p_fieldName The name of the targeted field.
			*/
			PushConstants::Field& pushConstants(const std::wstring& p_fieldName);

			/**
			 * @brief Return the number of indexes stored inside the object
			*/
			const size_t& nbIndexes() const;
		};

		/**
		 * @class Uniform
		 * @brief A class to manage uniform blocks within a shader layout.
		 *
		 * The Uniform class serves to manage uniform data blocks associated with a shader layout.
		 * It contains a data buffer to store the actual uniform data and provides a method to push
		 * this data to the associated shader.
		 */
		class Uniform
		{
		public:
			using Key = ShaderLayout::UniformBlockLayout::Key;

		protected:
			/// The name of the uniform block.
			std::wstring _name;

			/// Data buffer to hold the uniform data.
			spk::DataBuffer _data;

			/**
			 * @brief Abstract method to push the uniform data to the shader.
			 */
			virtual void _pushData() = 0;
			virtual void _bindUniform() = 0;

		public:
			/**
			 * @brief Constructor for the Uniform class.
			 *
			 * Initializes the Uniform object based on a given UniformBlockLayout from a shader layout.
			 *
			 * @param p_uniformBlockLayout Reference to the UniformBlockLayout from a ShaderLayout object.
			 */
			Uniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);

			/**
			 * @brief Accessor for the uniform data.
			 *
			 * @return Pointer to the uniform data.
			 */
			const uint8_t *data() const;

			/**
			 * @brief Accessor for the size of the uniform data.
			 *
			 * @return Size of the uniform data.
			 */
			size_t size() const;

			/**
			 * @brief Overloaded << operator to push data into the uniform buffer.
			 *
			 * @param p_data The data to push into the buffer.
			 * @return Reference to this Uniform object.
			 */
			template <typename TType>
			void set(const TType &p_data)
			{
				if (_data.size() != sizeof(TType))
				{
					spk::throwException(L"Pushing an unexpected data with size [" + std::to_wstring(sizeof(TType)) + L"] into a uniform named [" + _name + L"] of size [" + std::to_wstring(_data.size()) + L"]");
				}
				_data.edit(0, p_data);
			}

			/**
			 * @brief Overloaded = operator to push data into the uniform buffer and emit the data.
			 *
			 * @param p_data The data to push into the buffer.
			 * @return Reference to this Uniform object.
			 */
			template <typename TType>
			Uniform& operator = (const TType& p_data)
			{
				*this << p_data;
				return (*this);
			}

			void update();
			void bind();
		};

		/**
		 * @class UniformBlock
		 * @brief A class derived from Uniform to manage uniform blocks with multiple fields.
		 *
		 * The UniformBlock class extends the functionality of the Uniform class by allowing uniform
		 * blocks to contain multiple fields. Each field can be individually updated.
		 */
		class UniformBlock : public Uniform
		{
		public:

			/**
			 * @class Field
			 * @brief Inner class representing an individual field within a UniformBlock.
			 *
			 * The Field class is responsible for managing the data for an individual field
			 * inside a UniformBlock.
			 */
			class Field
			{
			private:
				/// The name of the field.
				std::wstring _name;

				/// Pointer to the UniformBlock that owns this Field.
				UniformBlock *_owner;

				/// Pointer to the data for this Field.
				void* _data;

				/// Offset of this Field within the UniformBlock.
				size_t _offset;

				/// Size of this Field.
				size_t _size;

			public:
				/// Default constructor.
				Field();

				/**
				 * @brief Constructor for the Field class.
				 *
				 * @param p_name Name of the Field.
				 * @param p_owner Pointer to the owning UniformBlock.
				 * @param p_data Pointer to the data.
				 * @param p_offset Offset of the Field in the UniformBlock.
				 * @param p_size Size of the Field.
				 */
				Field(const std::wstring& p_name, UniformBlock *p_owner, uint8_t *p_data, size_t p_offset, size_t p_size);

				/**
				 * @brief Overloaded << operator to push data into the Field.
				 *
				 * @param p_data Data to be pushed.
				 * @return Reference to this Field object.
				 */
				template <typename TType>
				void set(const TType& p_data)
				{
					if (_size != sizeof(TType))
					{
						spk::throwException(L"Incorrect size of data pushed to the field [" + _name + L"].");
					}

					std::memcpy(static_cast<uint8_t*>(_data) + _offset, &p_data, _size);
				}

				template <typename TType>
				Field& operator = (const TType& p_data)
				{
					set(p_data);
					return (*this);
				}
			};

		private:
			/// Map to hold the fields within this UniformBlock.
			std::map<std::wstring, Field> _fields;

		public:
			/**
			 * @brief Constructor for the UniformBlock class.
			 *
			 * @param p_uniformBlockLayout Reference to a UniformBlockLayout from a ShaderLayout object.
			 */
			UniformBlock(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);

			/**
			 * @brief Accessor method to get a Field by its name.
			 *
			 * @param p_fieldName Name of the field to be accessed.
			 * @return Reference to the corresponding Field object.
			 */
			Field &field(const std::wstring &p_fieldName);

			/**
			 * @brief Overloaded [] operator to get a Field by its name.
			 *
			 * @param p_fieldName Name of the field to be accessed.
			 * @return Reference to the corresponding Field object.
			 */
			Field &operator[](const std::wstring &p_fieldName);
		};


		/**
		 * @class SamplerUniform
		 * @brief A class derived from Uniform to manage sampler uniforms.
		 *
		 * The SamplerUniform class extends the functionality of the Uniform class
		 * to provide specialized handling for sampler uniform types.
		 */
		class SamplerUniform : public Uniform
		{
		private:
			void _bindUniform();

		public:
			/**
			 * @brief Constructor for the SamplerUniform class.
			 *
			 * Initializes a SamplerUniform instance with the layout specified
			 * by the UniformBlockLayout from a ShaderLayout object.
			 *
			 * @param p_uniformBlockLayout Reference to a UniformBlockLayout from a ShaderLayout object.
			 */
			SamplerUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);
		};

	protected:
		ShaderLayout _shaderLayout; ///< Layout of the associated shader program.

		/// Static map to hold shared pointers to Uniform objects, keyed by UniformBlockLayout::Key.
		static inline std::map<ShaderLayout::UniformBlockLayout::Key, std::shared_ptr<Uniform>> _uniformMap;

		/// Map to store keys for accessing Uniform objects in _uniformMap, keyed by uniform name.
		std::map<std::wstring, ShaderLayout::UniformBlockLayout::Key> _uniformKeys;

		/**
		 * @brief Pure virtual function to load a shader program.
		 * @param p_shaderLayout The layout of the shader program to load.
		 */
		virtual void _loadProgram(const ShaderLayout &p_shaderLayout) = 0;

		/**
		 * @brief Load a uniform into the pipeline.
		 * @param p_uniformBlockLayout The layout of the uniform block containing the uniform.
		 * @return A shared pointer to the loaded Uniform object.
		 */
		std::shared_ptr<Uniform> _loadUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);

		/**
		 * @brief Load the pipeline configurations.
		 */
		void _loadPipeline();

		/**
		 * @brief Pure virtual function to load a uniform block.
		 * @param p_uniformBlockLayout The layout of the uniform block to load.
		 * @return A shared pointer to the loaded UniformBlock object.
		 */
		virtual std::shared_ptr<UniformBlock> _loadUniformBlock(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) = 0;

		/**
		 * @brief Pure virtual function to load a sampler uniform.
		 * @param p_uniformBlockLayout The layout of the uniform block containing the sampler uniform.
		 * @return A shared pointer to the loaded SamplerUniform object.
		 */
		virtual std::shared_ptr<SamplerUniform> _loadSamplerUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) = 0;

		/**
		 * @brief Pure virtual function to load an Object.
		 * @param p_storageLayout The storage buffer layout for the Object.
		 * @param p_pushConstantsLayout The push constants layout for the Object.
		 * @return A shared pointer to the loaded Object.
		 */
		virtual std::shared_ptr<Object> _loadObject(const ShaderLayout::StorageBufferLayout& p_storageLayout, const ShaderLayout::PushConstantsLayout& p_pushConstantsLayout) = 0;

	public:
		/**
		 * @brief Constructor for AbstractPipeline.
		 * @param p_vertexInput The vertex shader module.
		 * @param p_fragmentInput The fragment shader module.
		 */
		AbstractPipeline(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput);

		/**
		 * @brief Pure virtual function to launch the pipeline.
		 * @param p_nbIndexes The number of indexes to be processed.
		 */
		virtual void launch(const size_t &p_nbIndexes) = 0;

		/**
		 * @brief Pure virtual function to activate the pipeline.
		 */
		virtual void activate() = 0;

		/**
		 * @brief Pure virtual function to deactivate the pipeline.
		 */
		virtual void deactivate() = 0;

		/**
		 * @brief Retrieve a shared pointer to a Uniform object.
		 * @param p_uniformName The name of the uniform.
		 * @return Shared pointer to the corresponding Uniform object.
		 */
		std::shared_ptr<Uniform> uniform(const std::wstring& p_uniformName);

		/**
		 * @brief Retrieve a shared pointer to a Uniform object.
		 * @param p_uniformKey The key of the uniform.
		 * @return Shared pointer to the corresponding Uniform object.
		 */
		static std::shared_ptr<Uniform> uniform(const ShaderLayout::UniformBlockLayout::Key& p_uniformKey);

		static bool contain(const ShaderLayout::UniformBlockLayout::Key& p_uniformKey);

		/**
		 * @brief Create a new Object.
		 * @return A shared pointer to the newly created Object.
		 */
		std::shared_ptr<Object> createObject();
	};
}