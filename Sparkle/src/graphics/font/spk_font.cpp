#include "graphics/spk_font.hpp"

namespace spk
{
	Font::Font(const std::filesystem::path &p_path)
	{
		std::ifstream file(p_path, std::ios::binary | std::ios::ate);
		if (!file.is_open())
		{
			spk::throwException(L"Failed to open font file [" + p_path.wstring() + L"].");
		}

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		_fontData.resize(static_cast<size_t>(size));
		if (!file.read(reinterpret_cast<char *>(_fontData.data()), size))
		{
			spk::throwException(L"Failed to read font file [" + p_path.wstring() + L"].");
		}

		_fontConfiguration = Configuration(p_path.wstring(), _fontData);
	}

	std::shared_ptr<Font::Atlas> Font::atlas(const size_t &p_fontSize, const size_t &p_outlineSize) const
	{
		return (this->operator[](Key(p_fontSize, p_outlineSize)));
	}

	std::shared_ptr<Font::Atlas> Font::atlas(const size_t &p_fontSize, const size_t &p_outlineSize)
	{
		return (this->operator[](Key(p_fontSize, p_outlineSize)));
	}

	std::shared_ptr<Font::Atlas> Font::atlas(const Font::Key &p_fontAtlasKey) const
	{
		return (this->operator[](p_fontAtlasKey));
	}

	std::shared_ptr<Font::Atlas> Font::atlas(const Font::Key &p_fontAtlasKey)
	{
		return (this->operator[](p_fontAtlasKey));
	}

	std::shared_ptr<Font::Atlas> Font::operator[](const Font::Key &p_fontAtlasKey) const
	{
		if (_fontAtlas.contains(p_fontAtlasKey) == false)
			spk::throwException(L"Atlas cannot be instancied inside a const Font object");
		return _fontAtlas.at(p_fontAtlasKey);
	}

	std::shared_ptr<Font::Atlas> Font::operator[](const Font::Key &p_fontAtlasKey)
	{

		if (_fontAtlas.contains(p_fontAtlasKey) == false)
		{
			_fontAtlas.emplace(p_fontAtlasKey, std::make_shared<Atlas>(_fontData, _fontConfiguration, p_fontAtlasKey));
		}
		return _fontAtlas[p_fontAtlasKey];
	}
}