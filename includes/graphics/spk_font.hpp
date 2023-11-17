#pragma once

#include "graphics/spk_texture.hpp"
#include "graphics/spk_pipeline.hpp"

namespace spk
{
    /**
     * @brief Represents a font, which can be used for rendering text.
     * 
     * This class encapsulates the properties and behaviors of a font in a graphical
     * context, providing the necessary functionality for rendering text with different
     * outline styles and alignments.
     */
    class Font
    {
    public:
        /**
         * @brief Pixel value representing a character in the font texture.
         */
        static const uint8_t CHAR_PIXEL = 0xFF;

        /**
         * @brief Pixel value representing empty space in the font texture.
         */
        static const uint8_t EMPTY_PIXEL = 0x00;

        /**
         * @brief Types of outlines available for the font.
         * 
         * This enumeration defines various styles of outlines that can be applied to text,
         * ranging from pixelized to smooth edges.
         */
        enum class OutlineType
        {
            Pixelized,  ///< Outline with a pixelized effect.
            SharpEdge,  ///< Outline with sharp edges.
            Standard,   ///< Standard outline.
            Manhattan   ///< Outline with a Manhattan distance effect.
        };

        friend std::wostream& operator << (std::wostream& p_os, const OutlineType& p_outlineType)
        {
            switch (p_outlineType)
            {
                case OutlineType::Pixelized:
                    p_os << (L"Pixelized");
                    return (p_os);
                case OutlineType::SharpEdge:
                    p_os << (L"SharpEdge");
                    return (p_os);
                case OutlineType::Standard:
                    p_os << (L"Standard");
                    return (p_os);
                case OutlineType::Manhattan:
                    p_os << (L"Manhattan");
                    return (p_os);
            }
            p_os << (L"Unknow");
            return (p_os);
        }
        
        /**
         * @brief Vertical alignment options for text rendering.
         * 
         * This enumeration defines the vertical positioning of text within a given space,
         * such as top, middle, or bottom alignment.
         */
		enum class VerticalAlignment
		{
            Top,    ///< Align text to the top.
            Middle, ///< Align text to the middle.
            Down    ///< Align text to the bottom.
        };

        friend std::wostream& operator << (std::wostream& p_os, const VerticalAlignment& p_verticalAlignment)
        {
            switch (p_verticalAlignment)
            {
                case VerticalAlignment::Top:
                    p_os << (L"Top");
                    return (p_os);
                case VerticalAlignment::Middle:
                    p_os << (L"Middle");
                    return (p_os);
                case VerticalAlignment::Down:
                    p_os << (L"Down");
                    return (p_os);
            }
            p_os << (L"Unknow");
            return (p_os);
        }
        
        /**
         * @brief Horizontal alignment options for text rendering.
         * 
         * This enumeration defines the horizontal positioning of text, which includes left,
         * center, or right alignment.
         */
        enum class HorizontalAlignment
		{
            Left,   ///< Align text to the left.
            Middle, ///< Align text to the center.
            Right   ///< Align text to the right.
        };

        friend std::wostream& operator << (std::wostream& p_os, const HorizontalAlignment& p_horizontalAlignment)
        {
            switch (p_horizontalAlignment)
            {
                case HorizontalAlignment::Left:
                    p_os << (L"Left");
                    return (p_os);
                case HorizontalAlignment::Middle:
                    p_os << (L"Middle");
                    return (p_os);
                case HorizontalAlignment::Right:
                    p_os << (L"Right");
                    return (p_os);
            }
            p_os << (L"Unknow");
            return (p_os);
        }


    public:
        /**
         * @brief Configuration information for the font.
         */
        struct Configuration
        {
        private:
            std::wstring _fileName;            ///< File name of the font.
            std::vector<wchar_t> _validGlyphs; ///< List of valid glyphs in the font.

            /**
             * @brief Compute glyph information from the font data.
             * @param p_fontData Font data to process.
             */
            void _computeGlyphInformation(const std::vector<uint8_t> &p_fontData);

        public:
            Configuration(); ///< Default constructor.
            /**
             * @brief Constructor to initialize the configuration with font data.
             * @param p_fileName Name of the font file.
             * @param p_fontData Data of the font.
             */
            Configuration(const std::wstring &p_fileName, const std::vector<uint8_t> &p_fontData);

            const std::wstring &fileName() const;              ///< Get the font file name.
            const std::vector<wchar_t>& validGlyphs() const;   ///< Get the valid glyphs.
        };

    public:
        /**
         * @brief Key information used for font atlas creation.
         */
        struct Key
        {
            size_t fontSize;            ///< Size of the font.
            size_t outlineSize;         ///< Size of the outline.
            size_t inverseOutlineSize;  ///< Inverse of the outline size.
            size_t outlineSizeSquared;  ///< Outline size squared.
            OutlineType outlineType;    ///< Type of outline to be used.
            std::vector<int> circleIndexes; ///< Indexes for the circle outline.

            /**
             * @brief Constructor to initialize the key with given parameters.
             * @param p_fontSize Size of the font.
             * @param p_outlineSize Size of the outline.
             * @param p_outlineType Type of the outline.
             */
            Key(const size_t &p_fontSize, const size_t &p_outlineSize, const OutlineType &p_outlineType = OutlineType::Standard);
            bool operator<(const Key &p_other) const; ///< Comparison operator.
            void computeCircle(const spk::Vector2Int& p_atlasSize); ///< Compute the circle for the outline.
        };

        /**
         * @brief Represents a font atlas used for rendering characters.
         */
        class Atlas
        {
        public:
            /**
             * @brief Data for each glyph in the atlas.
             */
            struct GlyphData
            {
                spk::Vector2 uvs[4];         ///< UV coordinates for the glyph.
                spk::Vector2Int position[4]; ///< Position of the glyph.
                spk::Vector2Int step;        ///< Step size for rendering the glyph.
            };

            /**
             * @brief Data used for building the font atlas.
             */
            struct BuildData
            {
                std::vector<uint8_t> buffer;       ///< Buffer to store the font atlas.
                spk::Vector2Int size = spk::Vector2Int(32, 32); ///< Size of the atlas.
            };

        private:
            Configuration _fontConfiguration;          ///< Font configuration for the atlas.
            std::vector<GlyphData> _glyphDatas;         ///< List of glyph data.
            Texture _texture;                           ///< Texture for the atlas.

	        /**
             * @brief Compute build data for the font atlas.
             * @param p_fontData Font data to use for building the atlas.
             * @param p_fontConfiguration Configuration of the font.
             * @param p_key Key information for atlas creation.
             * @return BuildData for the atlas.
             */
	        BuildData _computeBuildData(const std::vector<uint8_t> &p_fontData, const Configuration &p_fontConfiguration, const Key &p_key);

        public:
            Atlas(); ///< Default constructor.
            /**
             * @brief Constructor to initialize the atlas with font data, configuration, and key.
             * @param p_fontData Font data to use for building the atlas.
             * @param p_fontConfiguration Configuration of the font.
             * @param p_key Key information for atlas creation.
             */
            Atlas(const std::vector<uint8_t> &p_fontData, const Configuration &p_fontConfiguration, const Key &p_key);

            /**
             * @brief Get glyph data for a specific character.
             * @param p_char Character to get the glyph data for.
             * @return GlyphData for the character.
             */
            const GlyphData &glyph(const wchar_t &p_char) const;

            const Texture &texture() const; ///< Get the texture of the atlas.
        };

    private:
        std::vector<uint8_t> _fontData;                       ///< Font data.
        Configuration _fontConfiguration;                     ///< Font configuration.
        std::map<Key, std::shared_ptr<Atlas>> _fontAtlas;     ///< Map of font atlases.

    public:
        /**
         * @brief Constructor to initialize the font with a file path.
         * @param p_path File path of the font.
         */
        Font(const std::filesystem::path &p_path);

        /**
         * @brief Get a shared pointer to an atlas with specified font size and outline size.
         * @param p_fontSize Font size for the atlas.
         * @param p_outlineSize Outline size for the atlas.
         * @return Shared pointer to the requested atlas.
         */
        std::shared_ptr<Atlas> atlas(const size_t &p_fontSize, const size_t &p_outlineSize) const;

        /**
         * @brief Get or create a shared pointer to an atlas with specified font size and outline size.
         * @param p_fontSize Font size for the atlas.
         * @param p_outlineSize Outline size for the atlas.
         * @return Shared pointer to the requested atlas.
         */
        std::shared_ptr<Atlas> atlas(const size_t &p_fontSize, const size_t &p_outlineSize);

        /**
         * @brief Get a shared pointer to an atlas with a specific key.
         * @param p_fontAtlasKey Key information for atlas creation.
         * @return Shared pointer to the requested atlas.
         */
        std::shared_ptr<Atlas> atlas(const Key &p_fontAtlasKey) const;
        
        /**
         * @brief Get a shared pointer to an atlas with a specific key.
         * @param p_fontAtlasKey Key information for atlas creation.
         * @return Shared pointer to the requested atlas.
         */
        std::shared_ptr<Atlas> atlas(const Key &p_fontAtlasKey);

        /**
         * @brief Get a shared pointer to an atlas with a specific key.
         * @param p_fontAtlasKey Key information for atlas creation.
         * @return Shared pointer to the requested atlas.
         */
        std::shared_ptr<Atlas> operator[](const Key &p_fontAtlasKey) const;
        
        /**
         * @brief Get a shared pointer to an atlas with a specific key.
         * @param p_fontAtlasKey Key information for atlas creation.
         * @return Shared pointer to the requested atlas.
         */
        std::shared_ptr<Atlas> operator[](const Key &p_fontAtlasKey);
    };
}