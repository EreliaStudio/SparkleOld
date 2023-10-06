#include "sparkle.hpp"

#include <regex>

class AbstractPipeline
{
public:
	class Input
	{
	public:
		class Instruction
		{
		public:
			enum class Type
			{
				Version,
				StorageBuffer,
				OutputBuffer,
				PushConstant,
				SamplerUniform,
				UniformBlock,
				Structure,
				Function,
				Error
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
		std::string _name;
		std::string _code;
		std::vector<Instruction> _instructions;

		void compactify()
		{
			// Extract the first line (usually the #version directive in GLSL)
			std::string firstLine = _code.substr(0, _code.find('\n') + 1);
			std::string restOfTheCode = _code.substr(_code.find('\n') + 1);

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

			_code = firstLine + restOfTheCode;
		}

		void parseInstructions()
		{
			int braceCount = 0;
			bool requiresSemicolon = false;
			std::string newInstruction;

			for (char c : _code)
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
					requiresSemicolon = (
						newInstruction.compare(0, 6, "layout") == 0 ||
						newInstruction.compare(0, 6, "struct") == 0 ||
						newInstruction.compare(0, 7, "uniform") == 0
					);
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
		Input(const std::filesystem::path &p_filePath) : Input(p_filePath.filename().string(), spk::getFileContentAsString(p_filePath))
		{
		}
		Input(const std::string &p_inputName, const std::string &p_inputCode) : _name(p_inputName),
																				_code(p_inputCode)
		{
			compactify();
			parseInstructions();
		}
	};

protected:
	virtual void _loadProgram(const Input &p_vertexInput, const Input &p_fragmentInput) = 0;
	void _loadPipeline(const Input &p_vertexInput, const Input &p_fragmentInput)
	{
	}

public:
};

class Pipeline : public AbstractPipeline
{
private:
	void _loadProgram(const Input &p_vertexInput,
					  const Input &p_fragmentInput) override
	{
	}

public:
	Pipeline(const Input &p_vertexInput, const Input &p_fragmentInput)
	{
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