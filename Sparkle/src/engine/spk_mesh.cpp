#include "engine/spk_mesh.hpp"

namespace spk
{
	Mesh::Vertex::Vertex() :
		pos(0),
		uv(0),
		normal(0)
	{

	}

	Mesh::Vertex::Vertex(const spk::Vector3& p_pos, const spk::Vector2& p_uv, const spk::Vector3& p_normal) :
		pos(p_pos),
		uv(p_uv),
		normal(p_normal)
	{

	}

	Mesh::Mesh()
	{

	}

	std::vector<spk::Vector3>& Mesh::points()
	{
		return (_points);
	}
	
	const std::vector<spk::Vector3>& Mesh::points() const
	{
		return (_points);
	}

	std::vector<spk::Vector2>& Mesh::uvs()
	{
		return (_uvs);
	}

	const std::vector<spk::Vector2>& Mesh::uvs() const
	{
		return (_uvs);
	}

	std::vector<spk::Vector3>& Mesh::normals()
	{
		return (_normals);
	}

	const std::vector<spk::Vector3>& Mesh::normals() const
	{
		return (_normals);
	}

	const std::vector<Mesh::Vertex>& Mesh::vertices() const
	{
		return (_vertices);
	}

	const std::vector<unsigned int> Mesh::indexes() const
	{
		return (_indexes);
	}

	void Mesh::addVertex(const size_t& p_pointIndex, const size_t& p_uvIndex, const size_t& p_normalIndex)
	{
		if (_points.size() <= p_pointIndex)
			spk::throwException(L"Invalid point index : requested point [" + std::to_wstring(p_pointIndex) + L"] over [" + std::to_wstring(_points.size()) + L"] stored points");
		if (_uvs.size() <= p_uvIndex)
			spk::throwException(L"Invalid uv index : requested uv [" + std::to_wstring(p_uvIndex) + L"] over [" + std::to_wstring(_points.size()) + L"] stored uvs");
		if (_normals.size() <= p_normalIndex)
			spk::throwException(L"Invalid normal index : requested normal [" + std::to_wstring(p_normalIndex) + L"] over [" + std::to_wstring(_normals.size()) + L"] stored normals");

		_vertices.emplace_back(_points[p_pointIndex], _uvs[p_uvIndex], _normals[p_normalIndex]);
	}

	void Mesh::addFace(const unsigned int& p_vertexIndexA, const unsigned int& p_vertexIndexB, const unsigned int& p_vertexIndexC)
	{
		if (_vertices.size() <= p_vertexIndexA)
			spk::throwException(L"Invalid vertex index : requested vertex [" + std::to_wstring(p_vertexIndexA) + L"] over [" + std::to_wstring(_vertices.size()) + L"] stored vertices");
		
		if (_vertices.size() <= p_vertexIndexB)
			spk::throwException(L"Invalid vertex index : requested vertex [" + std::to_wstring(p_vertexIndexB) + L"] over [" + std::to_wstring(_vertices.size()) + L"] stored vertices");
		
		if (_vertices.size() <= p_vertexIndexC)
			spk::throwException(L"Invalid vertex index : requested vertex [" + std::to_wstring(p_vertexIndexC) + L"] over [" + std::to_wstring(_vertices.size()) + L"] stored vertices");
		
		_indexes.emplace_back(p_vertexIndexA);        
		_indexes.emplace_back(p_vertexIndexB);        
		_indexes.emplace_back(p_vertexIndexC);   

		_needUpdate = true;     
	}

	void Mesh::addFace(const unsigned int& p_vertexIndexA, const unsigned int& p_vertexIndexB, const unsigned int& p_vertexIndexC, const unsigned int& p_vertexIndexD)
	{
		if (_vertices.size() <= p_vertexIndexA)
			spk::throwException(L"Invalid vertex index : requested vertex [" + std::to_wstring(p_vertexIndexA) + L"] over [" + std::to_wstring(_vertices.size()) + L"] stored vertices");
		
		if (_vertices.size() <= p_vertexIndexB)
			spk::throwException(L"Invalid vertex index : requested vertex [" + std::to_wstring(p_vertexIndexB) + L"] over [" + std::to_wstring(_vertices.size()) + L"] stored vertices");
		
		if (_vertices.size() <= p_vertexIndexC)
			spk::throwException(L"Invalid vertex index : requested vertex [" + std::to_wstring(p_vertexIndexC) + L"] over [" + std::to_wstring(_vertices.size()) + L"] stored vertices");
		
		if (_vertices.size() <= p_vertexIndexD)
			spk::throwException(L"Invalid vertex index : requested vertex [" + std::to_wstring(p_vertexIndexD) + L"] over [" + std::to_wstring(_vertices.size()) + L"] stored vertices");
		
		_indexes.emplace_back(p_vertexIndexA);        
		_indexes.emplace_back(p_vertexIndexB);        
		_indexes.emplace_back(p_vertexIndexC);        
		_indexes.emplace_back(p_vertexIndexA);        
		_indexes.emplace_back(p_vertexIndexC);        
		_indexes.emplace_back(p_vertexIndexD);    
		
		_needUpdate = true;    
	}

	bool Mesh::needUpdate() const
	{
		return (_needUpdate);
	}

	void Mesh::setNeedUpdateFlag(bool p_state)
	{
		_needUpdate = p_state;
	}
	
	void Mesh::resetNeedUpdateFlag()
	{
		_needUpdate = false;
	}
}