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
		class StorageBufferLayout
		{
		private:
		public:
			StorageBufferLayout()
			{
			}

			void treat(const InstructionSet::Instruction &p_instruction)
			{
				spk::cout << "Parsing StorageBuffer instruction [" << spk::to_wstring(p_instruction.code) << "]" << std::endl;
			}
		};

		class PushConstantLayout
		{
		private:
		public:
			PushConstantLayout()
			{
			}

			void treat(const InstructionSet::Instruction &p_instruction)
			{
				spk::cout << "Parsing PushConstant instruction [" << spk::to_wstring(p_instruction.code) << "]" << std::endl;
			}
		};

		class UniformBlockLayout
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

				// Overloading the < operator for std::map
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

		private:
			Mode _mode;

		public:
			UniformBlockLayout()
			{
			}

			void treat(const InstructionSet::Instruction &p_instruction)
			{
				spk::cout << "Parsing UniformBlock instruction [" << spk::to_wstring(p_instruction.code) << "]" << std::endl;
			}
		};

		class OutputBufferLayout
		{
		private:
		public:
			OutputBufferLayout()
			{
			}

			void treat(const InstructionSet::Instruction &p_instruction)
			{
				spk::cout << "Parsing OutputBuffer instruction [" << spk::to_wstring(p_instruction.code) << "]" << std::endl;
			}
		};

	private:
		int _vertexTypeMask = static_cast<int>(InstructionSet::Instruction::Type::Version) |
							  static_cast<int>(InstructionSet::Instruction::Type::PushConstant) |
							  static_cast<int>(InstructionSet::Instruction::Type::SamplerUniform) |
							  static_cast<int>(InstructionSet::Instruction::Type::StorageBuffer) |
							  static_cast<int>(InstructionSet::Instruction::Type::UniformBlock);

		int _fragmentTypeMask = static_cast<int>(InstructionSet::Instruction::Type::Version) |
								static_cast<int>(InstructionSet::Instruction::Type::PushConstant) |
								static_cast<int>(InstructionSet::Instruction::Type::SamplerUniform) |
								static_cast<int>(InstructionSet::Instruction::Type::OutputBuffer) |
								static_cast<int>(InstructionSet::Instruction::Type::UniformBlock);

		StorageBufferLayout _storageBufferLayout;
		OutputBufferLayout _outputBufferLayout;
		PushConstantLayout _pushConstantLayout;
		static std::map<UniformBlockLayout::Key, UniformBlockLayout> _uniformBlocks;
		static std::map<std::wstring, UniformBlockLayout::Key> _uniformBlockKeys;

		void _parseVersion(const InstructionSet::Instruction& p_instruction)
		{

		}
		
		void _parseStorageBuffer(const InstructionSet::Instruction& p_instruction)
		{

		}
		
		void _parseOutputBuffer(const InstructionSet::Instruction& p_instruction)
		{

		}
		
		void _parsePushConstant(const InstructionSet::Instruction& p_instruction)
		{

		}
		
		void _parseUniformBlock(const InstructionSet::Instruction& p_instruction)
		{

		}
		
		void _parseStructure(const InstructionSet::Instruction& p_instruction)
		{

		}
		
		void _parseFunction(const InstructionSet::Instruction& p_instruction)
		{

		}
		
		void _parseError(const InstructionSet::Instruction& p_instruction)
		{

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
			for (const auto& instruction : p_instructionSet.instructions())
			{
				if (p_typeMask & static_cast<int>(instruction.type))
				{
					_parseInstruction(instruction);
				}
			}
		}

	public:
		Configuration(const Input &p_vertexInput, const Input &p_fragmentInput)
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