#pragma once

#include "graphics/spk_texture.hpp"
#include "graphics/spk_pipeline.hpp"

namespace spk
{
    class Font
    {
    public:
        enum class OutlineType
        {
            Pixelized,
            SharpEdge,
            Standard,
            Manhattan
        };

    public:
        struct Configuration
        {
        private:
            std::wstring _fileName;
            size_t _nbGlyph;

            size_t _countNbChar(const std::vector<uint8_t> &p_fontData);

        public:
            Configuration();
            Configuration(const std::wstring &p_fileName, const std::vector<uint8_t> &p_fontData);

            const std::wstring &fileName() const;
            const size_t &nbGlyph() const;
        };

    public:
        struct Key
        {
            size_t fontSize;
            size_t outlineSize;
            size_t outlineSizeSquared;
            OutlineType outlineType;

            Key(const size_t &p_fontSize, const size_t &p_outlineSize, const OutlineType &p_outlineType = OutlineType::Standard);
            bool operator<(const Key &p_other) const;
        };

        class Atlas
        {
        public:
            struct GlyphData
            {
                spk::Vector2 uvs[4];
                spk::Vector2Int position[4];
                spk::Vector2Int step;
            };

        private:
            Configuration _fontConfiguration;
            std::vector<GlyphData> _glyphDatas;
            Texture _texture;

            void _applyOutline(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const Key &p_key);

        public:
            Atlas();
            Atlas(const std::vector<uint8_t> &p_fontData, const Configuration &p_fontConfiguration, const Key &p_key);

            const GlyphData &glyph(const wchar_t &p_char) const;
            const Texture &texture() const;
        };

    private:
        std::vector<uint8_t> _fontData;
        Configuration _fontConfiguration;
        std::map<Key, std::shared_ptr<Atlas>> _fontAtlas;

    public:
        Font(const std::filesystem::path &p_path);

        std::shared_ptr<Atlas> atlas(const size_t &p_fontSize, const size_t &p_outlineSize) const;
        std::shared_ptr<Atlas> atlas(const size_t &p_fontSize, const size_t &p_outlineSize);

        std::shared_ptr<Atlas> atlas(const Key &p_fontAtlasKey) const;
        std::shared_ptr<Atlas> atlas(const Key &p_fontAtlasKey);

        std::shared_ptr<Atlas> operator[](const Key &p_fontAtlasKey) const;
        std::shared_ptr<Atlas> operator[](const Key &p_fontAtlasKey);
    };
}