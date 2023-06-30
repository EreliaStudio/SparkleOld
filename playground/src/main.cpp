#include "playground.hpp"

class File
{
private:
	spk::JSON::Object _root;

public:
	File()
	{

	}

	File(const std::wstring& p_filePath)
	{
		load(p_filePath);
	}

	void load(const std::wstring& p_filePath)
	{

	}

	const spk::JSON::Object& operator [] (const std::wstring& p_key) const
	{
		return (_root[p_key]);
	}

	const spk::JSON::Object& operator[](size_t p_index) const
	{
		return (_root[p_index]);
	}

	template<typename TType>
	const TType& as()
	{
		return (_root.as<TType>());
	}
};

int main()
{
	

	return (0);
}