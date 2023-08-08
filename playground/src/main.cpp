#include "playground.hpp"

class World
{
public:
	struct Texel
	{
		using ID = short;
		static const ID Empty = -1;

		ID id;
		std::wstring glyph;

		Texel() :
			id(-1),
			glyph(L" ")
		{

		}

		Texel(const spk::JSON::Object& p_object) :
			id(static_cast<short>(p_object[L"ID"].as<long>())),
			glyph(p_object[L"Glyph"].as<std::wstring>())
		{

		}
	};

	class Chunk
	{
	public:
		static const int Size = 32;
		static const int Height = 32;

	private:
		spk::Vector2Int _position;
		Texel::ID _content[Size][Height];

	public:
		Chunk(const spk::Vector2Int& p_chunkPosition = spk::Vector2Int(0, 0)) :
			_position(p_chunkPosition)
		{
			for (size_t x = 0; x < Size; x++)
			{
				for (size_t y = 0; y < Height; y++)
				{
					_content[x][y] = Texel::Empty;
				}
			}
		}

		spk::Vector2Int convertAbsoluteToRelativePosition(const spk::Vector2Int& p_absolutePosition) const
		{
			return (spk::Vector2Int(spk::positiveModulo(p_absolutePosition.x, Size), spk::positiveModulo(p_absolutePosition.y, Height)));
		}

		spk::Vector2Int convertRelativeToAbsolutePosition(const spk::Vector2Int& p_relativePosition) const
		{
			return (spk::Vector2Int(p_relativePosition.x + _position.x * Size, p_relativePosition.y + _position.y * Height));
		}

		const spk::Vector2Int& position() const
		{
			return (_position);
		}

		void setContent(const Texel::ID& p_value, const int& p_x, const int& p_y)
		{
			if (p_x < 0 || p_x >= Size ||
				p_y < 0 || p_y >= Height)
				spk::throwException(L"Can't set content [" + std::to_wstring(p_x) + L"][" + std::to_wstring(p_y) + L"] inside a chunk : Maybe need a conversion to relative position");
			_content[p_x][p_y] = p_value;
		}

		void setContent(const Texel::ID& p_value, const spk::Vector2Int& p_position)
		{
			setContent(p_value, p_position.x, p_position.y);
		}

		const Texel::ID& content(const int& p_x, const int& p_y) const
		{
			if (p_x < 0 || p_x >= Size ||
				p_y < 0 || p_y >= Height)
				spk::throwException(L"Can't access content [" + std::to_wstring(p_x) + L"][" + std::to_wstring(p_y) + L"] inside a chunk : Maybe need a conversion to relative position");
			return (_content[p_x][p_y]);
		}

		const Texel::ID& content(const spk::Vector2Int& p_position) const
		{
			return (content(p_position.x, p_position.y));
		}
	};

	static spk::Vector2Int convertWorldToChunkPosition(const int& p_x, const int& p_y)
	{
		return (spk::Vector2Int(
					std::floor(static_cast<float>(p_x) / static_cast<int>(Chunk::Size)),
					std::floor(static_cast<float>(p_y) / static_cast<int>(Chunk::Height))
				));
	}
	static spk::Vector2Int convertWorldToChunkPosition(const spk::Vector2Int& p_worldPosition)
	{
		return (convertWorldToChunkPosition(p_worldPosition.x, p_worldPosition.y));
	}

	static spk::Vector2Int convertChunkToWorldPosition(const int& p_x, const int& p_y)
	{
		return (spk::Vector2Int(
					p_x * Chunk::Size,
					p_y * Chunk::Height
				));
	}
	static spk::Vector2Int convertChunkToWorldPosition(const spk::Vector2Int& p_worldPosition)
	{
		return (convertChunkToWorldPosition(p_worldPosition.x, p_worldPosition.y));
	}

	struct Biome
	{
		static inline float radianceRange = 10;
		struct Ground
		{
			struct Stratum
			{
				struct Component
				{
					Texel::ID id;
					float percent;

					Component() :
						id(Texel::Empty),
						percent(0.0f)
					{

					}

					Component(const spk::JSON::Object& p_object) :
						id(static_cast<Texel::ID>(p_object[L"ID"].as<long>())),
						percent(static_cast<float>(p_object[L"Percent"].as<double>()))
					{
					}
				};

				float minimalDepth;
				float maximalDepth;
				std::vector<Component> composition;

				Stratum()
				{

				}

				Stratum(const spk::JSON::Object& p_object) :
					minimalDepth(static_cast<float>(p_object[L"MinimalDepth"].as<double>())),
					maximalDepth(static_cast<float>(p_object[L"MaximalDepth"].as<double>()))
				{
					const spk::JSON::Object& p_compositionObject = p_object[L"Composition"];
					for (size_t i = 0; i < p_compositionObject.size(); i++)
					{
						composition.push_back(Component(p_compositionObject[i]));
					}
				}

				const Texel::ID& generate(float p_localPercent) const
				{
					for (size_t i = 0; i < composition.size(); i++)
					{
						if (p_localPercent <= composition[i].percent)
							return (composition[i].id);
						p_localPercent -= composition[i].percent;
					}
					spk::throwException(L"Can't generate stratum value : Wrong percent total in stratum");
					return (Texel::Empty);
				}
			};

			spk::PseudoRandomGenerator<float> randomGenerator;
			std::vector<Stratum> stratums;

			Ground()
			{

			}

			Ground(const spk::JSON::Object& p_object)
			{
				const spk::JSON::Object& p_stratumArray = p_object[L"Stratums"];
				for (size_t i = 0; i < p_stratumArray.size(); i++)
				{
					stratums.push_back(Stratum(p_stratumArray[i]));
				}
			}

			const Texel::ID& value(const float& p_groundLevel, const spk::Vector2Int& p_texelPosition) const
			{
				float texelRelativeDepth = p_groundLevel - p_texelPosition.y;
				if (texelRelativeDepth < 0)
					return (Texel::Empty);
				for (size_t i = 0; i < stratums.size(); i++)
				{
					const Stratum& tmpStratum = stratums[i];

					if (texelRelativeDepth >= tmpStratum.minimalDepth && 
						texelRelativeDepth <= tmpStratum.maximalDepth)
					{
						return (tmpStratum.generate(randomGenerator.sample(p_texelPosition)));
					}
				}
				return (Texel::Empty);
			}
		};

		std::wstring name;
		spk::Perlin2D selectionPerlin;
		spk::Perlin1D heightPerlin;
		Ground ground;

		Biome()
		{

		}

		Biome(const spk::JSON::Object& p_object) :
			name(p_object[L"Name"].as<std::wstring>()),
			selectionPerlin(p_object[L"SelectionPerlin"]),
			heightPerlin(p_object[L"HeightPerlin"]),
			ground(p_object[L"Ground"])
		{

		}

		const Texel::ID& computeGroundTexel(const float& p_depth, const spk::Vector2Int& p_texelPosition) const
		{
			return (ground.value(p_depth, p_texelPosition));
		}
	};

	class Generator
	{
	private:
		struct ChunkData
		{
			float groundLevel[Chunk::Size];

			ChunkData()
			{
				for (size_t i = 0; i < Chunk::Size; i++)
				{
					groundLevel[i] = std::numeric_limits<float>::min();
				}
			}
		};
		struct ComputationData
		{
			Biome* biomes[Chunk::Size][Chunk::Height];
		};

		std::map<int, ChunkData> _chunkDatas;
		std::map<spk::Vector2Int, Chunk> _preloadedChunks;

		std::vector<Biome> _biomes;

		void _loadBiomes(const spk::JSON::Object& p_object)
		{
			for (size_t i = 0; i < p_object.size(); i++)
			{
				_biomes.push_back(Biome(p_object[i]));
			}
		}

		Biome* _selectBiome(const spk::Vector2Int& p_texelPosition)
		{
			Biome* selectedBiome = &(_biomes[0]);
			float selectedWeight = _biomes[0].selectionPerlin.sample(p_texelPosition);
			for (size_t i = 1; i < _biomes.size(); i++)
			{
				float currentWeight = _biomes[i].selectionPerlin.sample(p_texelPosition);
				if (currentWeight >= selectedWeight)
				{
					selectedBiome = &(_biomes[i]);
					selectedWeight = currentWeight;
				}
			}
			return (selectedBiome);
		}

		void _generateChunkBiome(Chunk& p_chunk, ComputationData& p_computationData)
		{
			for (int x = 0; x < Chunk::Size; x++)
			{
				for (int y = 0; y < Chunk::Height; y++)
				{
					spk::Vector2Int texelPosition = World::convertChunkToWorldPosition(p_chunk.position()) + spk::Vector2Int(x, y);

					p_computationData.biomes[x][y] = _selectBiome(texelPosition);
				}
			}
		}

		void _generateChunkHeight(Chunk& p_chunk, ChunkData& p_chunkData, ComputationData& p_computationData)
		{
			for (int x = 0; x < Chunk::Size; x++)
			{
				int texelRow = p_chunk.position().x * Chunk::Size + x;

				if (p_chunkData.groundLevel[x] == std::numeric_limits<float>::min())
				{
					float groundLevel = p_computationData.biomes[x][0]->heightPerlin.sample(static_cast<float>(texelRow));

					p_chunkData.groundLevel[x] = groundLevel;
				}
			}
		}

		void _generateChunkGround(Chunk& p_chunk, ChunkData& p_chunkData, ComputationData& p_computationData)
		{
			_generateChunkBiome(p_chunk, p_computationData);

			_generateChunkHeight(p_chunk, p_chunkData, p_computationData);

			for (int x = 0; x < Chunk::Size; x++)
			{
				for (int y = 0; y < Chunk::Height; y++)
				{
					spk::Vector2Int texelPosition = World::convertChunkToWorldPosition(p_chunk.position()) + spk::Vector2Int(x, y);

					const World::Texel::ID& texelID = p_computationData.biomes[x][y]->computeGroundTexel(p_chunkData.groundLevel[x], texelPosition);
					p_chunk.setContent(texelID, x, y);
				}
			}
		}

	public:
		Generator()
		{

		}

		Generator(const spk::JSON::Object& p_object)
		{
			Biome::radianceRange = static_cast<float>(p_object[L"BiomeRadianceRange"].as<double>());
			_loadBiomes(p_object[L"Biomes"]);
		}

		Chunk generate(const spk::Vector2Int& p_chunkPosition)
		{
			Chunk result;
			
			if (_preloadedChunks.contains(p_chunkPosition) == false)
			{
				ChunkData& chunkData = _chunkDatas[p_chunkPosition.x];
				ComputationData computationData = ComputationData();

				result = Chunk(p_chunkPosition);
				_generateChunkGround(result, chunkData, computationData);
			}
			else
			{
				result = std::move(_preloadedChunks[p_chunkPosition]);
				_preloadedChunks.erase(p_chunkPosition);
			}

			return (result);
		}
	};

	void _loadTexels(const spk::JSON::Object& p_object)
	{
		for (size_t i = 0; i < p_object.size(); i++)
		{
			Texel newTexel(p_object[i]);

			if (_texels.contains(newTexel.id) == true)
				spk::throwException(L"Texel [" + std::to_wstring(newTexel.id) + L"] already loaded in world configuration file");

			_texels[newTexel.id] = newTexel;
		}
	}
private:
	Generator _generator;

	static inline const Texel EmptyTexel = Texel();
	std::map<Texel::ID, Texel> _texels;
	std::map<spk::Vector2Int, Chunk> _chunks;

public:
	World(const spk::JSON::File& p_worldConfigurationFile) :
		_generator(p_worldConfigurationFile[L"Generator"])
	{
		_loadTexels(p_worldConfigurationFile[L"Texels"]);
	}

	const Texel& texel(const Texel::ID& p_texelID) const
	{
		if (_texels.contains(p_texelID) == false)
			return (EmptyTexel);
		return (_texels.at(p_texelID));
	}

	bool contains(const spk::Vector2Int& p_chunkPosition) const
	{
		return (_chunks.contains(p_chunkPosition));
	}

	Chunk& requestChunk(const spk::Vector2Int& p_chunkPosition)
	{
		if (contains(p_chunkPosition) == false)
		{
			_chunks[p_chunkPosition] = _generator.generate(p_chunkPosition);
		}
		return (_chunks.at(p_chunkPosition));
	}

	const Chunk& chunk(const spk::Vector2Int& p_chunkPosition) const
	{
		if (contains(p_chunkPosition) == false)
			spk::throwException(L"Chunk [" + std::to_wstring(p_chunkPosition.x) + L" / " + std::to_wstring(p_chunkPosition.y) + L"] doesn't exist : May need to instanciate it first by calling requestChunk on it");
		return (_chunks.at(p_chunkPosition));
	}
};

int main()
{
	World world = World(spk::JSON::File(L"worldConfigurationFile.json"));


	std::wfstream output(L"output.txt", std::ios_base::out);

	spk::Vector2Int mapSize = spk::Vector2Int(40 * World::Chunk::Size, 4 * World::Chunk::Height);
	spk::Vector2Int start = spk::Vector2Int(-20 * World::Chunk::Size , -2 * World::Chunk::Height);
	spk::Vector2Int end = spk::Vector2Int(start.x + mapSize.x, start.y + mapSize.y);

	for (int y = end.y; y >= start.y; y--)
	{
		for (int x = start.x; x < end.x; x++)
		{
			spk::Vector2Int absolutePosition(x, y);
			spk::Vector2Int chunkPosition = World::convertWorldToChunkPosition(absolutePosition);

			const World::Chunk& chunk = world.requestChunk(chunkPosition);
			spk::Vector2Int relativePosition = chunk.convertAbsoluteToRelativePosition(absolutePosition);

			output << world.texel(chunk.content(relativePosition)).glyph;
		}
		output << std::endl;
	}

	output.close();

	return (0);
};