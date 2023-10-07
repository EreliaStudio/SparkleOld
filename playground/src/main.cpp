#include "sparkle.hpp"

#include <regex>

class AbstractPipeline
{
public:
	struct Input
	{
		std::string name;
		std::string code;

		void checkCodeValidity()
		{
		}

		Input(const std::filesystem::path &p_filePath) : Input(p_filePath.filename().string(), spk::getFileContentAsString(p_filePath))
		{
		}
		Input(const std::string &p_name, const std::string &p_code) : name(p_name),
																	  code(p_code)
		{
			checkCodeValidity();
		}
	};

	class Configuration
	{
	private:
		class InstructionSet
		{
		public:
			class Instruction
			{
			public:
				enum class Type
				{
					Version = 0b0000000000000001,
					StorageBuffer = 0b0000000000000010,
					OutputBuffer = 0b0000000000000100,
					PushConstant = 0b0000000000001000,
					SamplerUniform = 0b0000000000010000,
					UniformBlock = 0b0000000000100000,
					Structure = 0b0000000001000000,
					Function = 0b0000000010000000,
					Error = 0b0000000100000000
				};

				Type type;
				std::string code;

				Instruction(const Type &p_type, const std::string &p_code) : type(p_type),
																			 code(p_code)
				{
				}

				friend std::wostream &operator<<(std::wostream &p_os, const Type &p_type)
				{
					switch (p_type)
					{
					case Instruction::Type::StorageBuffer:
						p_os << L"StorageBuffer";
						break;
					case Instruction::Type::OutputBuffer:
						p_os << L"OutputBuffer";
						break;
					case Instruction::Type::UniformBlock:
						p_os << L"UniformBlock";
						break;
					case Instruction::Type::SamplerUniform:
						p_os << L"SamplerUniform";
						break;
					case Instruction::Type::PushConstant:
						p_os << L"PushConstant";
						break;
					case Instruction::Type::Function:
						p_os << L"Function";
						break;
					case Instruction::Type::Structure:
						p_os << L"Structure";
						break;
					case Instruction::Type::Version:
						p_os << L"Version";
						break;
					case Instruction::Type::Error:
						p_os << L"Error";
						break;
					default:
						p_os << L"Not defined";
						break;
					}
					return (p_os);
				}
			};

		private:
			std::vector<Instruction> _instructions;

			std::string compactify(const std::string &p_code)
			{
				std::string firstLine = p_code.substr(0, p_code.find('\n') + 1);
				std::string restOfTheCode = p_code.substr(p_code.find('\n') + 1);

				std::vector<std::pair<std::regex, std::string>> regexes = {
					std::make_pair(std::regex("//[^\n]*"), ""),	   // Remove single-line comments
					std::make_pair(std::regex("\n|\r"), " "),	   // Remove newlines and carriage returns
					std::make_pair(std::regex("/\\*.*?\\*/"), ""), // Remove multi-line comments
					std::make_pair(std::regex("\t"), " "),		   // Remove tabs and replace with spaces
					std::make_pair(std::regex(" +"), " "),		   // Remove extra spaces
					std::make_pair(std::regex(" *\\= *"), "="),	   // Remove space around equal
				};

				for (auto &regex : regexes)
				{
					restOfTheCode = std::regex_replace(restOfTheCode, std::get<0>(regex), std::get<1>(regex));
				}

				return (firstLine + restOfTheCode);
			}

			void parseInstructions(const std::string &p_code)
			{
				int braceCount = 0;
				bool requiresSemicolon = false;
				std::string newInstruction;

				for (char c : p_code)
				{
					if (c != '\n' && !(newInstruction.empty() && c == ' '))
						newInstruction += c;

					if (c == '{')
					{
						++braceCount;
					}
					else if (c == '}')
					{
						if (braceCount == 0)
							throw std::runtime_error("Unbalanced braces in GLSL code");
						--braceCount;
					}

					if (newInstruction.size() >= 6 && !requiresSemicolon)
					{
						requiresSemicolon = (newInstruction.compare(0, 6, "layout") == 0 ||
											 newInstruction.compare(0, 6, "struct") == 0 ||
											 newInstruction.compare(0, 7, "uniform") == 0);
					}

					if ((c == ';' || ((c == '}' || c == '\n') && !requiresSemicolon)) && braceCount == 0)
					{
						_instructions.emplace_back(determineInstructionType(newInstruction), newInstruction);

						newInstruction.clear();
						requiresSemicolon = false;
					}
				}
			}

			Instruction::Type determineInstructionType(const std::string &p_instruction)
			{
				std::vector<std::pair<std::regex, Instruction::Type>> rules = {
					{std::regex(R"(#version\s+\d+)"), Instruction::Type::Version},
					{std::regex(R"(layout\(location\s*=\s*\d+\)\s+in)"), Instruction::Type::StorageBuffer},
					{std::regex(R"(layout\(location\s*=\s*\d+\)\s+out)"), Instruction::Type::OutputBuffer},
					{std::regex(R"(layout\(push_constant\)\s+uniform)"), Instruction::Type::PushConstant},
					{std::regex(R"(layout\((?:set\s*=\s*\d+,\s*)?binding\s*=\s*\d+\)\s+uniform\s+(?!sampler))"), Instruction::Type::UniformBlock},
					{std::regex(R"(layout\((?:set\s*=\s*\d+,\s*)?binding\s*=\s*\d+\)\s+uniform\s+sampler)"), Instruction::Type::SamplerUniform},
					{std::regex(R"(struct\s+\w+\s*\{[^\}]+\})"), Instruction::Type::Structure},
					{std::regex(R"(\w+\s+\w+\s*\([^)]*\)\s*\{)"), Instruction::Type::Function}};

				for (auto &rule : rules)
				{
					if (std::regex_search(p_instruction, rule.first))
					{
						return rule.second;
					}
				}
				return Instruction::Type::Error;
			}

		public:
			InstructionSet(const Input &p_input)
			{
				parseInstructions(compactify(p_input.code));
			}

			const std::vector<Instruction>& instructions() const
			{
				return (_instructions);
			}
		};

	public:
		struct Data
		{
			enum class Type
			{
				Int,
				UInt,
				Float,
				Structure,
				Error
			};

			Type type;
			size_t format;
			size_t size;

			Data(const Type& p_type = Type::Error, const size_t& p_format = 0, const size_t& p_size = 0) :
				type(p_type),
				format(p_format),
				size(p_size)
			{

			}

			Data(const size_t& p_size) :
				type(Type::Structure),
				format(1),
				size(p_size)
			{

			}
		};

		class StructureLayout
		{
		private:
			std::map<std::string, Data> _structures;
			std::map<std::string, Data> _standaloneStructures;

		public:
			StructureLayout()
			{

			}

			void reset()
			{
				_structures = {
					{"float", 	Data(Data::Type::Float, 1, sizeof(float))},
					{"int", 	Data(Data::Type::Int, 1, sizeof(int))},
					{"uint", 	Data(Data::Type::UInt, 1, sizeof(unsigned int))},
					
					{"vec2", 	Data(Data::Type::Float, 2, sizeof(float))},
					{"ivec2", 	Data(Data::Type::Int, 2, sizeof(int))},
					{"uvec2", 	Data(Data::Type::UInt, 2, sizeof(unsigned int))},
					
					{"vec3", 	Data(Data::Type::Float, 3, sizeof(float))},
					{"ivec3", 	Data(Data::Type::Int, 3, sizeof(int))},
					{"uvec3", 	Data(Data::Type::UInt, 3, sizeof(unsigned int))},
					
					{"vec4", 	Data(Data::Type::Float, 4, sizeof(float))},
					{"ivec4", 	Data(Data::Type::Int, 4, sizeof(int))},
					{"uvec4", 	Data(Data::Type::UInt, 4, sizeof(unsigned int))},
					
					{"mat4", 	Data(Data::Type::Float, 16, sizeof(float))},
				};
				
				_standaloneStructures = {
					{"sampler1D", 		Data(Data::Type::Int, 1, sizeof(int))},
					{"sampler2D", 		Data(Data::Type::Int, 1, sizeof(int))},
					{"sampler3D", 		Data(Data::Type::Int, 1, sizeof(int))},
					{"samplerCube", 	Data(Data::Type::Int, 1, sizeof(int))},
				};
			}

			void treat(const InstructionSet::Instruction &p_instruction)
			{

			}

			const std::map<std::string, Data>& structures() const
			{
				return (_structures);
			}

			const std::map<std::string, Data>& standaloneStructures() const 
			{
				return (_standaloneStructures);
			}
		};

		class ConfigurationLayout
		{
		protected:
			const std::map<std::string, Data>& structures;
			const std::map<std::string, Data>& standaloneStructures;

		public:
			ConfigurationLayout(const std::map<std::string, Data>& p_structures, const std::map<std::string, Data>& p_standaloneStructures) :
				structures(p_structures),
				standaloneStructures(p_standaloneStructures)
			{

			}

			virtual void treat(const InstructionSet::Instruction &p_instruction) = 0;
		};

		class StorageBufferLayout : public ConfigurationLayout
		{
		public:
			struct Field
			{
				Data data;
				size_t location;
				size_t offset;

				Field(const Data& p_data = Data(), const size_t& p_location = 0, const size_t& p_offset = 0) :
					data(p_data),
					location(p_location),
					offset(p_offset)
				{

				}
			};

		private:
			size_t _stride;
			std::vector<Field> _fields;

		public:
			StorageBufferLayout(const std::map<std::string, Data>& p_structures, const std::map<std::string, Data>& p_standaloneStructures) :
				ConfigurationLayout(p_structures, p_standaloneStructures)
			{
			}

			void treat(const InstructionSet::Instruction &p_instruction)
			{
				spk::cout << "Parsing StorageBuffer instruction [" << spk::to_wstring(p_instruction.code) << "]" << std::endl;
			}

			const size_t& stride() const
			{
				return (_stride);
			}

			const std::vector<Field>& fields() const 
			{
				return (_fields);
			}
		};

		class PushConstantLayout : public ConfigurationLayout
		{
		public:
			struct Field
			{
				Data data;
				size_t offset;

				Field(const Data& p_data = Data(), const size_t& p_offset = 0) :
					data(p_data),
					offset(p_offset)
				{

				}
			};

		private:
			size_t _stride;
			std::vector<Field> _fields;

		public:
			PushConstantLayout(const std::map<std::string, Data>& p_structures, const std::map<std::string, Data>& p_standaloneStructures) :
				ConfigurationLayout(p_structures, p_standaloneStructures)
			{
			}

			void treat(const InstructionSet::Instruction &p_instruction)
			{
				spk::cout << "Parsing PushConstant instruction [" << spk::to_wstring(p_instruction.code) << "]" << std::endl;
			}

			const size_t& stride() const
			{
				return (_stride);
			}

			const std::vector<Field>& fields() const 
			{
				return (_fields);
			}
		};

		class UniformBlockLayout : public ConfigurationLayout
		{	
		public:
			class UniformBlock
			{
			public:
				struct Key
				{
					size_t binding;
					size_t set;

					Key(size_t p_set = 0, size_t p_binding = 0)
						: binding(p_binding), set(p_set)
					{
					}

					bool operator<(const Key &p_other) const
					{
						if (binding < p_other.binding)
						{
							return true;
						}
						else if (binding == p_other.binding)
						{
							return set < p_other.set;
						}
						return false;
					}
				};

				enum class Mode
				{
					Block,
					Sampler
				};

				struct Field
				{
					Data data;
					size_t offset;

					Field(const Data& p_data = Data(), const size_t& p_offset = 0) :
						data(p_data),
						offset(p_offset)
					{

					}
				};

			private:
				Mode _mode;
				Key _key;
				size_t _stride;
				std::vector<Field> _fields;

			public:
				UniformBlock()
				{

				}

				void treat(const InstructionSet::Instruction &p_instruction)
				{
					spk::cout << "Parsing UniformBlock instruction [" << spk::to_wstring(p_instruction.code) << "]" << std::endl;
				}

				const Key& key() const
				{
					return (_key);
				}

				const Mode& mode() const
				{
					return (_mode);
				}

				const size_t& stride() const
				{
					return (_stride);
				}

				const std::vector<Field>& fields() const 
				{
					return (_fields);
				}
			};

			static inline std::map<UniformBlock::Key, UniformBlock> uniformBlocks;
			static inline std::map<std::wstring, UniformBlock::Key> uniformBlockKeys;

		private:
			std::vector<UniformBlock::Key> _subscribedUniformBlocks;

		public:
			UniformBlockLayout(const std::map<std::string, Data>& p_structures, const std::map<std::string, Data>& p_standaloneStructures) :
				ConfigurationLayout(p_structures, p_standaloneStructures)
			{

			}

			void treat(const InstructionSet::Instruction &p_instruction)
			{
				UniformBlock newUniformBlock;

				newUniformBlock.treat(p_instruction);

				if (uniformBlocks.contains(newUniformBlock.key()) == true)
				{
					const auto& oldBlock = uniformBlocks.at(newUniformBlock.key());
					if (oldBlock.stride() != newUniformBlock.stride())
					{
						spk::throwException(L"Instruction [" + spk::to_wstring(p_instruction.code) + L"}\n - Uniform block [set = " + std::to_wstring(newUniformBlock.key().set) + L" / binding = " + std::to_wstring(newUniformBlock.key().binding) + L"] already exist and have a different composition");
					}
				}
			}

			const std::vector<UniformBlock::Key>& subscribedUniformBlocks() const 
			{
				return (_subscribedUniformBlocks);
			}
		};

		class OutputBufferLayout : public ConfigurationLayout
		{
		public:
			struct Field
			{
				Data data;
				size_t location;
				size_t offset;

				Field(const Data& p_data = Data(), const size_t& p_location = 0, const size_t& p_offset = 0) :
					data(p_data),
					location(p_location),
					offset(p_offset)
				{

				}
			};

		private:
			size_t _stride;
			std::vector<Field> _fields;

		public:
			OutputBufferLayout(const std::map<std::string, Data>& p_structures, const std::map<std::string, Data>& p_standaloneStructures) :
				ConfigurationLayout(p_structures, p_standaloneStructures)
			{
			}

			void treat(const InstructionSet::Instruction &p_instruction)
			{
				spk::cout << "Parsing OutputBuffer instruction [" << spk::to_wstring(p_instruction.code) << "]" << std::endl;
			}

			const size_t& stride() const
			{
				return (_stride);
			}

			const std::vector<Field>& fields() const 
			{
				return (_fields);
			}
		};

	private:
		int _vertexTypeMask = static_cast<int>(InstructionSet::Instruction::Type::Version) |
							  static_cast<int>(InstructionSet::Instruction::Type::Structure) |
							  static_cast<int>(InstructionSet::Instruction::Type::PushConstant) |
							  static_cast<int>(InstructionSet::Instruction::Type::SamplerUniform) |
							  static_cast<int>(InstructionSet::Instruction::Type::StorageBuffer) |
							  static_cast<int>(InstructionSet::Instruction::Type::UniformBlock);

		int _fragmentTypeMask = static_cast<int>(InstructionSet::Instruction::Type::Version) |
								static_cast<int>(InstructionSet::Instruction::Type::Structure) |
								static_cast<int>(InstructionSet::Instruction::Type::PushConstant) |
								static_cast<int>(InstructionSet::Instruction::Type::SamplerUniform) |
								static_cast<int>(InstructionSet::Instruction::Type::OutputBuffer) |
								static_cast<int>(InstructionSet::Instruction::Type::UniformBlock);

		StructureLayout _structureLayout;
		StorageBufferLayout _storageBufferLayout;
		OutputBufferLayout _outputBufferLayout;
		PushConstantLayout _pushConstantLayout;
		UniformBlockLayout _uniformBlockLayout;

		void _parseVersion(const InstructionSet::Instruction& p_instruction)
		{
			//Do nothing
		}
		
		void _parseStorageBuffer(const InstructionSet::Instruction& p_instruction)
		{
			_storageBufferLayout.treat(p_instruction);
		}
		
		void _parseOutputBuffer(const InstructionSet::Instruction& p_instruction)
		{
			_outputBufferLayout.treat(p_instruction);
		}
		
		void _parsePushConstant(const InstructionSet::Instruction& p_instruction)
		{
			_pushConstantLayout.treat(p_instruction);
		}
		
		void _parseUniformBlock(const InstructionSet::Instruction& p_instruction)
		{
			_uniformBlockLayout.treat(p_instruction);
		}
		
		void _parseStructure(const InstructionSet::Instruction& p_instruction)
		{
			_structureLayout.treat(p_instruction);
		}
		
		void _parseFunction(const InstructionSet::Instruction& p_instruction)
		{
			//Do nothing
		}
		
		void _parseError(const InstructionSet::Instruction& p_instruction)
		{
			spk::throwException(L"A non-recognized instruction detected :\n- [" + spk::to_wstring(p_instruction.code) + L"]");
		}
		
		void _parseInstruction(const InstructionSet::Instruction& p_instruction)
		{
			// Declare an array of member function pointers, make sure the order is correct.
			void (Configuration::*handlerArray[])(const InstructionSet::Instruction&) = {
				&Configuration::_parseVersion,          // 0b0000000000000001
				&Configuration::_parseStorageBuffer,    // 0b0000000000000010
				&Configuration::_parseOutputBuffer,     // 0b0000000000000100
				&Configuration::_parsePushConstant,     // 0b0000000000001000
				&Configuration::_parseUniformBlock,     // 0b0000000000010000
				&Configuration::_parseUniformBlock,     // 0b0000000000100000
				&Configuration::_parseStructure,        // 0b0000000001000000
				&Configuration::_parseFunction,         // 0b0000000010000000
				&Configuration::_parseError             // 0b0000000100000000
			};

			// Calculate the index based on the bitmask
			int index = static_cast<int>(std::log2(static_cast<int>(p_instruction.type)));

			// Call the function
			(this->*(handlerArray[index]))(p_instruction);
		}

		void _parseInstructionSet(const InstructionSet &p_instructionSet, const int &p_typeMask)
		{
			_structureLayout.reset();
			for (const auto& instruction : p_instructionSet.instructions())
			{
				if (p_typeMask & static_cast<int>(instruction.type))
				{
					_parseInstruction(instruction);
				}
			}
		}

	public:
		Configuration(const Input &p_vertexInput, const Input &p_fragmentInput) :
			_structureLayout(),
			_storageBufferLayout(_structureLayout.structures(), _structureLayout.standaloneStructures()),
			_outputBufferLayout(_structureLayout.structures(), _structureLayout.standaloneStructures()),
			_pushConstantLayout(_structureLayout.structures(), _structureLayout.standaloneStructures()),
			_uniformBlockLayout(_structureLayout.structures(), _structureLayout.standaloneStructures())
		{
			_parseInstructionSet(InstructionSet(p_vertexInput), _vertexTypeMask);
			_parseInstructionSet(InstructionSet(p_fragmentInput), _fragmentTypeMask);
		}
	};

protected:
	virtual void _loadProgram(const Configuration &p_configuration, const Input &p_vertexInput, const Input &p_fragmentInput) = 0;

	void _loadPipeline(const Input &p_vertexInput, const Input &p_fragmentInput)
	{
		_loadProgram(Configuration(p_vertexInput, p_fragmentInput), p_vertexInput, p_fragmentInput);
	}

public:
};

class Pipeline : public AbstractPipeline
{
private:
	void _loadProgram(
		const Configuration &p_configuration,
		const Input &p_vertexInput,
		const Input &p_fragmentInput) override
	{
	}

public:
	Pipeline(const Input &p_vertexInput, const Input &p_fragmentInput)
	{
		_loadPipeline(p_vertexInput, p_fragmentInput);
	}
};

class Test : public spk::Widget::Interface
{
private:
	Pipeline _pipeline;

	void _onGeometryChange()
	{
	}

	void _onRender()
	{
	}

	bool _onUpdate()
	{

		return (false);
	}

public:
	Test(const std::wstring &p_name) : spk::Widget::Interface(p_name),
									   _pipeline(Pipeline::Input("colorShader.vert"), Pipeline::Input("colorShader.frag"))
	{
	}
	~Test()
	{
	}
};

int main()
{
	spk::Application app(L"Coucou", 400);
	spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Qwerty);

	std::shared_ptr<Test> test = app.addRootWidget<Test>(L"Test");
	test->setGeometry(spk::Vector2Int(0, 0), spk::Vector2UInt(400, 400));
	test->activate();

	return (app.run());
};