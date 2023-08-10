#include "graphics/spk_mesh.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
	Mesh::Mesh()
	{

	}
	
	void Mesh::clear()
	{
		_isBaked = false;

		_vertices.clear();
		_uvs.clear();
		_normals.clear();

		_faces.clear();

		_computedVertices.clear();
		_computedUvs.clear();
		_computedNormals.clear();
		_computedIndexes.clear(); 
	}

	/**
	 * @brief Return the baking state of the Mesh. 
	 */
	bool Mesh::isBaked() const
	{
		return (_isBaked);
	}

	/**
	 * @brief Bake the Mesh, to fill its computedVector.
	*/
	void Mesh::bake()
	{
		for (size_t i = 0; i < _faces.size(); i++)
		{
			if (_faces[i].content.size() != 3 && _faces[i].content.size() != 4)
				spk::throwException(L"A face must contain 3 or 4 points : Unable to bake such face.");

			size_t indexBaseValue = _computedVertices.size();

			for (size_t j = 0; j < _faces[i].content.size(); j++)
			{
				_computedVertices.push_back(_vertices[_faces[i].content[j].vertex]);
				_computedUvs.push_back(_uvs[_faces[i].content[j].uv]);
				_computedNormals.push_back(_normals[_faces[i].content[j].normal]);
			}

			const size_t indexesValues[6] = {
				0, 1, 2, //First triangle
				0, 2, 3};//Second triangle

			for (size_t j = 0; j < (_faces[i].content.size() == 3 ? 3 : 6); j++)
			{
				_computedIndexes.push_back(indexesValues[j] + indexBaseValue);
			}
		}
	}

	void Mesh::computeNormals()
	{
    	_normals.clear();

		for (size_t i = 0; i < _faces.size(); i++)
		{
			if (_faces[i].content.size() != 3 && _faces[i].content.size() != 4)
				spk::throwException(L"A face contain an unexpected amount of point : Unable to compute a face normal.");

			for (size_t j = 0; j < _faces[i].content.size(); j++)
			{
				_faces[i].content[j].normal = _normals.size();
			}			

			size_t indexA = _faces[i].content[0].vertex;
			size_t indexB = _faces[i].content[1].vertex;
			size_t indexC = _faces[i].content[2].vertex;

			spk::Vector3 edge1 = _vertices[indexB] - _vertices[indexA];
			spk::Vector3 edge2 = _vertices[indexC] - _vertices[indexA];

			_normals.push_back(edge1.cross(edge2).normalize());

		}

		_isBaked = false;
	}

	void Mesh::addVertices(const std::vector<spk::Vector3> &p_vertices)
	{
		_vertices.insert(_vertices.end(), p_vertices.begin(), p_vertices.end());
	}

	void Mesh::addVertex(const spk::Vector3 &p_vertex)
	{
		_vertices.push_back(p_vertex);
	}

	void Mesh::addUVs(const std::vector<spk::Vector2> &p_uvs)
	{
		_uvs.insert(_uvs.end(), p_uvs.begin(), p_uvs.end());
	}

	void Mesh::addUV(const spk::Vector2 &p_uv)
	{
		_uvs.push_back(p_uv);
	}

	void Mesh::addNormals(const std::vector<spk::Vector3> &p_normals)
	{
		_normals.insert(_normals.end(), p_normals.begin(), p_normals.end());
	}

	void Mesh::addNormal(const spk::Vector3 &p_normal)
	{
		_normals.push_back(p_normal);
	}

	/**
	 * @brief Insert a new face into the Mesh.
	 * @param p_face The new face to add.
	*/
	void Mesh::addFace(const Mesh::Face& p_face)
	{
		_faces.push_back(p_face);
	}

	const std::vector<spk::Vector3> &Mesh::vertices() const
	{
		return (_vertices);
	}

	const std::vector<spk::Vector2> &Mesh::uvs() const
	{
		return (_uvs);
	}

	const std::vector<spk::Vector3> &Mesh::normals() const
	{
		return (_normals);
	}

	const std::vector<Mesh::Face>& Mesh::faces() const
	{
		return (_faces);
	}

	const std::vector<spk::Vector3> &Mesh::computedVertices() const
	{
		return (_computedVertices);
	}

	const std::vector<spk::Vector2> &Mesh::computedUvs() const
	{
		return (_computedUvs);
	}

	const std::vector<spk::Vector3> &Mesh::computedNormals() const
	{
		return (_computedNormals);
	}

	const std::vector<size_t> &Mesh::computedIndexes() const
	{
		return (_computedIndexes);
	}
}