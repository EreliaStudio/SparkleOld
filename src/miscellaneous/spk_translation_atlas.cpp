#include "miscellaneous/spk_translation_atlas.hpp"

namespace spk
{
	TranslationAtlas::TranslationAtlas(const std::filesystem::path& p_path)
	{
		load(p_path);
	}


	void TranslationAtlas::load(const std::filesystem::path& p_path)
	{
		spk::JSON::File file;

		try
		{
			file.load(p_path);
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error("TranslationAtlas::load() : " + std::string(e.what()));
		}

		for (const auto& [key, value] : file.root().getObjectElements())
			_translations[key] = value->as<std::wstring>();
	}

	const std::wstring& TranslationAtlas::get(const std::wstring& p_key) const
	{
		if (_translations.count(p_key) == 0)
			return (p_key);
		return (_translations.at(p_key));
	}
}