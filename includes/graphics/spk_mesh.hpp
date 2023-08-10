#pragma once

#include <vector>
#include <filesystem>
#include "math/spk_vector3.hpp"


namespace spk
{
	/**
	* @brief Represents a 3D object, and provides methods to insert and access its contents.
	* Additionnaly, it provide a method to bake normals based on indexes definition.
	*/
	class Mesh
	{
	public:
		class Face
		{
		public:
			struct Point
			{
				size_t vertex;
				size_t normal;
				size_t uv;
			};

			std::vector<Point> content;
		};

	private:
		bool _isBaked = false;

		std::vector<spk::Vector3> _vertices; /**< A vector storing every vertices composing the object. */
		std::vector<spk::Vector2> _uvs; /**< A vector storing every uv coordinates composing the object. */
		std::vector<spk::Vector3> _normals; /**< A vector storing every normals composing the object. */

		std::vector<Face> _faces; /**< A vector storing every face of this object. */

		std::vector<spk::Vector3> _computedVertices; /**< A vector storing vertices composing the object's faces. */
		std::vector<spk::Vector2> _computedUvs; /**< A vector storing uv coordinates composing the object's faces. */
		std::vector<spk::Vector3> _computedNormals; /**< A vector storing normals composing the object's faces. */
		std::vector<size_t> _computedIndexes; /**< A vector storing normals composing the object's faces. */

		/**
		 * @brief Insert a new index into the Mesh.
		 * @param p_index The index to add.
		*/
		void _addIndex(const size_t& p_index);
		
	public:
		/**
		 * @brief Constructor for the Mesh class, with no indexes, vertices and normals.
		 */
		Mesh();

		/**
		 * @brief Remove every information inside this Mesh.
		*/
		void clear();

		/**
		 * @brief Return the baking state of the Mesh. 
 		 */
		bool isBaked() const;

		/**
		 * @brief Bake the Mesh, to fill its computedVector.
		*/
		void bake();

		/**
		 * @brief Recalculate the normals of this object, using the vertices indexes provided by the user inside the faces vector.
		 * @note This method will erase the previsouly stored normals.
		*/
		void computeNormals();

		/**
		 * @brief Insert a list of vertices into the Mesh.
		 * @param p_vertices A vector containing a list of vertices to add.
		*/
		void addVertices(const std::vector<spk::Vector3>& p_vertices);

		/**
		 * @brief Insert a new vertex into the Mesh.
		 * @param p_vertex The vertex to add.
		*/
		void addVertex(const spk::Vector3& p_vertex);

		/**
		 * @brief Insert a list of uvs into the Mesh.
		 * @param p_uvs A vector containing a list of uvs to add.
		*/
		void addUVs(const std::vector<spk::Vector2>& p_uvs);

		/**
		 * @brief Insert a new uv coordinate into the Mesh.
		 * @param p_uv The uv coordinate to add.
		*/
		void addUV(const spk::Vector2& p_uv);

		/**
		 * @brief Insert a list of normals into the Mesh.
		 * @param p_normals A vector containing a list of normals to add.
		*/
		void addNormals(const std::vector<spk::Vector3>& p_normals);

		/**
		 * @brief Insert a new normal into the Mesh.
		 * @param p_normal The normal to add.
		*/
		void addNormal(const spk::Vector3& p_normal);

		/**
		 * @brief Insert a new face into the Mesh.
		 * @param p_face The new face to add.
		*/
		void addFace(const Face& p_face);

		/**
		 * @brief Return a reference of the vertices contained inside the mesh.
		*/
		const std::vector<spk::Vector3>& vertices() const;

		/**
		 * @brief Return a reference of the uv coordinates contained inside the mesh.
		*/
		const std::vector<spk::Vector2>& uvs() const;

		/**
		 * @brief Return a reference of the normals contained inside the mesh.
		*/
		const std::vector<spk::Vector3>& normals() const;

		/**
		 * @brief Return a reference of the faces contained inside the mesh.
		*/
		const std::vector<Face>& faces() const;

		/**
		 * @brief Return a reference of the computed vertices calculated based on the faces provided to the mesh.
		*/
		const std::vector<spk::Vector3>& computedVertices() const;
		/**
		 * @brief Return a reference of the computed uvs calculated based on the faces provided to the mesh.
		*/
		const std::vector<spk::Vector2>& computedUvs() const;
		/**
		 * @brief Return a reference of the computed normals calculated based on the faces provided to the mesh.
		*/
		const std::vector<spk::Vector3>& computedNormals() const;

		/**
		 * @brief Return a reference of the computed indexes calculated based on the faces provided to the mesh.
		*/
		const std::vector<size_t>& computedIndexes() const;
	};
}