#include "playground.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include "external_libraries/stb_truetype.h"

namespace spk
{
	class Font
	{
    private:
        struct Configuration
        {
        private:
            std::wstring _fileName;         
            size_t _nbGlyph;

            size_t _countNbChar(const std::vector<uint8_t>& p_fontData)
            {
                size_t result = 0;
                int numFonts = stbtt_GetNumberOfFonts(p_fontData.data());

                if (numFonts == 0) {
                    throw std::runtime_error("No fonts found in the provided font data.");
                }

                for (int fontIndex = 0; fontIndex < numFonts; ++fontIndex) {
                    stbtt_fontinfo fontInfo;

                    if (!stbtt_InitFont(&fontInfo, p_fontData.data(), stbtt_GetFontOffsetForIndex(p_fontData.data(), fontIndex))) {
                        spk::throwException(L"Can't initiate font from file [" + _fileName + L"]");
                    }

                    result += fontInfo.numGlyphs;
                }

                return (result);
            }

        public:
            Configuration() :
                _fileName(L"Uninitialized"),
                _nbGlyph(0)
            {

            }

            Configuration(const std::wstring& p_fileName, const std::vector<uint8_t>& p_fontData) :
                _fileName(p_fileName),
                _nbGlyph(_countNbChar(p_fontData))
            {

            }

            const std::wstring& fileName() const
            {
                return (_fileName);
            }

            const size_t& nbGlyph() const
            {
                return (_nbGlyph);
            }
        };

	public:
		struct Key
		{
			size_t textSize;
			size_t outlineSize;

			Key(const size_t& p_textSize, const size_t& p_outlineSize)
			{
				textSize = p_textSize;
				outlineSize = p_outlineSize;
			}
			bool operator < (const Key& p_other) const
			{
				if (textSize < p_other.textSize)
					return (true);
				if (textSize == p_other.textSize && outlineSize < p_other.outlineSize)
					return (true);
				return (false);
			}
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

		public:
            Atlas()
            {
                
            }

            Atlas(const std::vector<uint8_t>& p_fontData, const Configuration& p_fontConfiguration, const size_t& p_fontSize, const size_t& p_outlineSize)
            {
                std::vector<uint8_t> atlasData;
                spk::Vector2Int atlasSize = spk::Vector2Int(32, 32);
                stbtt_packedchar* charInformation = new stbtt_packedchar[p_fontConfiguration.nbGlyph()];

                while (1) {
                    atlasData.resize(atlasSize.x * atlasSize.y);

                    stbtt_pack_context context;
                    int startingPackError = stbtt_PackBegin(&context, atlasData.data(), atlasSize.x, atlasSize.y, 0, 1, nullptr);
                    if (startingPackError == 0)
                        spk::throwException(L"Failed to start the packing process for font [" + p_fontConfiguration.fileName() + L"] with error [" + std::to_wstring(startingPackError) + L"]");

                    stbtt_PackSetOversampling(&context, 1, 1);
                    int packingFontRange = stbtt_PackFontRange(&context, p_fontData.data(), 0, static_cast<float>(p_fontSize), 0, p_fontConfiguration.nbGlyph(), charInformation);
                    if (packingFontRange == 0)
                    {
                        stbtt_PackEnd(&context);
                        atlasSize.x *= 2;
                        atlasSize.y *= 2;
                    }
                    else {
                        stbtt_PackEnd(&context);
                        break;
                    }
                }

                _glyphDatas.resize(p_fontConfiguration.nbGlyph());

                for (wchar_t i = L' '; i < p_fontConfiguration.nbGlyph(); i++)
                {
                    GlyphData& data = _glyphDatas[i];

                    stbtt_aligned_quad quad;
                    spk::Vector2 quadStep;
                    stbtt_GetPackedQuad(charInformation, atlasSize.x, atlasSize.y, i, &quadStep.x, &quadStep.y, &quad, 1);

                    data.uvs[0] = { quad.s0, quad.t0 };
                    data.uvs[1] = { quad.s1, quad.t0 };
                    data.uvs[2] = { quad.s0, quad.t1 };
                    data.uvs[3] = { quad.s1, quad.t1 };

                    data.position[0] = spk::Vector2Int(quad.x0 * atlasSize.x, quad.y0 * atlasSize.y);
                    data.position[1] = spk::Vector2Int(quad.x1 * atlasSize.x, quad.y0 * atlasSize.y);
                    data.position[2] = spk::Vector2Int(quad.x0 * atlasSize.x, quad.y1 * atlasSize.y);
                    data.position[3] = spk::Vector2Int(quad.x1 * atlasSize.x, quad.y1 * atlasSize.y);

                    data.step = quadStep * atlasSize;
                }

                for (int y = 0; y < atlasSize.y; y++)
                {
                    for (int x = 0; x < atlasSize.x; x++)
                    {
                        size_t index = x + y * atlasSize.x;
                        if (atlasData[index] != 0x00)
                        {
                            atlasData[index] = 0xFF;
                        }
                    }
                }

                std::fstream output("fontResult.txt", std::ios_base::out);
                for (int y = 0; y < atlasSize.y; y++)
                {
                    for (int x = 0; x < atlasSize.x; x++)
                    {
                        size_t index = x + y * atlasSize.x;
                        if (atlasData[index] != 0x00)
                            output << "O";
                        else
                            output << " ";
                    }
                    output << std::endl;
                }
                output << std::endl;
                output.close();

                DEBUG_LINE();
                _texture.uploadToGPU(atlasData.data(), atlasSize,
                    Texture::Format::R, Texture::Filtering::Nearest,
                    Texture::Wrap::Repeat, Texture::Mipmap::Disable);
                DEBUG_LINE();
            }

            const GlyphData& operator << (const wchar_t& p_char) const
            {
                if (_glyphDatas.size() < static_cast<size_t>(p_char))
                    spk::throwException(L"Char [" + std::wstring(1, p_char) + L"](dec : " + std::to_wstring(static_cast<size_t>(p_char)) + L") cannot be rendered : it doesn't exist in Font [" + _fontConfiguration.fileName() + L"]");
                return (_glyphDatas[static_cast<size_t>(p_char)]);
            }

            const Texture& texture() const
            {
                return (_texture);
            }
		};

	private:
        std::vector<uint8_t> _fontData;
        Configuration _fontConfiguration;
		std::map<Key, std::shared_ptr<Atlas>> _fontAtlas;

	public:
		Font(const std::filesystem::path& p_path)
        {
            std::ifstream file(p_path, std::ios::binary | std::ios::ate);
            if (!file.is_open())
            {
                spk::throwException(L"Failed to open font file [" + p_path.wstring() + L"].");
            }

            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);

            _fontData.resize(static_cast<size_t>(size));
            if (!file.read(reinterpret_cast<char*>(_fontData.data()), size))
            {
                spk::throwException(L"Failed to read font file [" + p_path.wstring() + L"].");
            }

            _fontConfiguration = Configuration(p_path.wstring(), _fontData);
        }

		std::shared_ptr<Atlas> atlas(const size_t& p_fontSize, const size_t& p_outlineSize) const
        {
            return (this->operator[](Key(p_fontSize, p_outlineSize)));
        }

		std::shared_ptr<Atlas> atlas(const size_t& p_fontSize, const size_t& p_outlineSize)
        {
            return (this->operator[](Key(p_fontSize, p_outlineSize)));
        }

		std::shared_ptr<Atlas> atlas(const Key& p_fontAtlasKey) const
        {
            return (this->operator[](p_fontAtlasKey));
        }

		std::shared_ptr<Atlas> atlas(const Key& p_fontAtlasKey)
        {
            return (this->operator[](p_fontAtlasKey));
        }

		std::shared_ptr<Atlas> operator [] (const Key& p_fontAtlasKey) const
        {
            if (_fontAtlas.contains(p_fontAtlasKey) == false)
                spk::throwException(L"Atlas cannot be instancied inside a const Font object");
            return _fontAtlas.at(p_fontAtlasKey);
        }

		std::shared_ptr<Atlas> operator [] (const Key& p_fontAtlasKey)
        {
            if (_fontAtlas.contains(p_fontAtlasKey) == false)
            {
                _fontAtlas.emplace(p_fontAtlasKey, std::make_shared<Atlas>(_fontData, _fontConfiguration, p_fontAtlasKey.textSize, p_fontAtlasKey.outlineSize));
            }
            return _fontAtlas[p_fontAtlasKey];
        }
	};
}

class Test : public spk::Widget::Interface
{
private:

    struct Unit
    {
        spk::Vector2 position;
        spk::Vector2 uv;
    };

    spk::Pipeline _pipeline;
    std::shared_ptr<spk::Pipeline::Object> _object;
    spk::Font _font;
    std::shared_ptr<spk::Font::Atlas> _fontAtlas;
    spk::Image _image;

    int value = 0;

    void _onGeometryChange()
    {
        std::vector<Unit> units = {
            {spk::Vector2(-1,  1), spk::Vector2(0, 0)},
            {spk::Vector2( 1,  1), spk::Vector2(1, 0)},
            {spk::Vector2(-1, -1), spk::Vector2(0, 1)},
            {spk::Vector2( 1, -1), spk::Vector2(1, 1)},
        };

        std::vector<unsigned int> indexes = {
            0, 2, 3, 3, 1, 0
        };

        _object->storage().vertices() << units << std::endl;
        _object->storage().indexes() << indexes << std::endl;
    }
    void _onRender()
    {
        if (value == 0)
            _fontAtlas->texture().bind(0);
        else
            _image.bind(0);

        _pipeline.uniform(L"textureID") << 0 << std::endl;
        _object->render();

        
        if (value == 0)
            _fontAtlas->texture().unbind();
        else
            _image.unbind();
    }

    bool _onUpdate()
    {
        if (spk::Keyboard::instance()->inputStatus(spk::Keyboard::A) == spk::InputState::Pressed)
        {
            DEBUG_LINE();
            value = 0;
        }
            
        if (spk::Keyboard::instance()->inputStatus(spk::Keyboard::Z) == spk::InputState::Pressed)
        {
            DEBUG_LINE();
            value = 1;
        }

        return (false);
    }

public:
    Test(const std::wstring &p_name) :
        spk::Widget::Interface(p_name),
        _pipeline(spk::ShaderModule(L"colorShader.vert"), spk::ShaderModule(L"colorShader.frag")),
        _object(_pipeline.createObject()),
        _font(L"Minecraft.ttf"),
        _fontAtlas(_font[spk::Font::Key(25, 0)]),
        _image(L"imageTest.png")
    {

    }

    ~Test()
    {

    }
};

int main()
{
    spk::Application app(L"Playground", 800);
    spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);

    std::shared_ptr<Test> test = app.addRootWidget<Test>(L"MainWidget");
    test->setGeometry(spk::Vector2Int(0, 0), app.size());
    test->activate();

    return (app.run());
}