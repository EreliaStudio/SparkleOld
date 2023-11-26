#pragma once

#include <vector>
#include "math/spk_vector2.hpp"
#include "math/spk_vector3.hpp"

namespace spk
{
	class ColorMesh
	{
	public:
		struct Vertex
		{
			spk::Vector3 pos;
			spk::Color color;
			spk::Vector3 normal;

			Vertex();
			Vertex(const spk::Vector3& p_pos, const spk::Color& p_color, const spk::Vector3& p_normal);

			friend std::wostream& operator << (std::wostream& p_os, const Vertex& p_vertex)
			{
				p_os << "Pos : " << p_vertex.pos << " - Color : " << p_vertex.color << " - Normal : " << p_vertex.normal;
				return (p_os);
			}
		};

	private:
		std::vector<spk::Vector3> _points;
		std::vector<spk::Color> _colors;
		std::vector<spk::Vector3> _normals;

		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indexes;

		bool _needUpdate = false;

	public:
		ColorMesh();

		std::vector<spk::Vector3>& points();
		const std::vector<spk::Vector3>& points() const;

		std::vector<spk::Vector2>& uvs();
		const std::vector<spk::Vector2>& uvs() const;

		std::vector<spk::Vector3>& normals();
		const std::vector<spk::Vector3>& normals() const;
		
		const std::vector<Vertex>& vertices() const;
		const std::vector<unsigned int> indexes() const;

		void addVertex(const size_t& p_pointIndex, const size_t& p_uvIndex, const size_t& p_normalIndex);
		void addFace(const unsigned int& p_vertexIndexA, const unsigned int& p_vertexIndexB, const unsigned int& p_vertexIndexC);

		/**
		 * Top side exemple :
		 *  Z
		 *  |   A       B
		 *  |    *-----*
		 *  |    |     |
		 *  |    |     |
		 *  |    |     |
		 *  |    *-----*
		 *  |   D       C
		 *  |
		 *  o---------------X
		*/
		void addFace(const unsigned int& p_vertexIndexA, const unsigned int& p_vertexIndexB, const unsigned int& p_vertexIndexC, const unsigned int& p_vertexIndexD);

		bool needUpdate() const;
		void setNeedUpdateFlag(bool p_state);
		void resetNeedUpdateFlag();
	};
}