#pragma once

#include <vector>
#include "math/spk_vector2.hpp"
#include "math/spk_vector3.hpp"

namespace spk
{
	/**
     * @brief Represents a mesh in the Sparkle engine.
     *
     * A mesh is composed of points in 3D, normals, UV coordinates, and indices that define
     * the shape and appearance of a 3D object.
     */
	class Mesh
	{
	public:
		/**
         * @brief Structure representing a vertex in the mesh.
		 * A Vertex is a representation of 3 variables linked together :
		 * - A point in 3D
		 * - An UV
		 * - A normal
         */
		struct Vertex
		{
			spk::Vector3 pos; ///< Position of the vertex.
            spk::Vector2 uv; ///< UV coordinates of the vertex.
            spk::Vector3 normal; ///< Normal vector of the vertex.

            /**
             * @brief Construct a new Vertex object with default values.
             */
            Vertex();

            /**
             * @brief Construct a new Vertex object.
             * 
             * @param p_pos Position of the vertex.
             * @param p_uv UV coordinates of the vertex.
             * @param p_normal Normal vector of the vertex.
             */
            Vertex(const spk::Vector3& p_pos, const spk::Vector2& p_uv, const spk::Vector3& p_normal);

            /**
             * @brief Overload for outputting vertex information to a wide stream.
             * 
             * @param p_os Output stream.
             * @param p_vertex Vertex to output.
             * @return std::wostream& Reference to the output stream.
             */
			friend std::wostream& operator << (std::wostream& p_os, const Vertex& p_vertex)
			{
				p_os << "Pos : " << p_vertex.pos << " - UV : " << p_vertex.uv << " - Normal : " << p_vertex.normal;
				return (p_os);
			}
		};

	private:
		std::vector<spk::Vector3> _points; ///< Collection of points in the mesh.
        std::vector<spk::Vector2> _uvs; ///< Collection of UV coordinates in the mesh.
        std::vector<spk::Vector3> _normals; ///< Collection of normals in the mesh.

        std::vector<Vertex> _vertices; ///< Collection of vertices in the mesh.
        std::vector<unsigned int> _indexes; ///< Collection of indices defining the mesh's faces.

        bool _needUpdate = false; ///< Flag indicating if the mesh needs an update.


	public:
		/**
         * @brief Construct a new Mesh object.
         */
		Mesh();

		/**
		 * @brief Provides access to the mutable points in the mesh.
		 *
		 * This method returns a reference to the vector of 3D points that make up the mesh. 
		 * It allows for direct modification of the mesh's points, which can be used for dynamic 
		 * mesh manipulation.
		 * 
		 * @return std::vector<spk::Vector3>& Reference to the vector of 3D points.
		 */
		std::vector<spk::Vector3>& points();

		/**
		 * @brief Provides access to the immutable points in the mesh.
		 *
		 * This method returns a constant reference to the vector of 3D points. 
		 * It is used for accessing the points without modifying them, typically for 
		 * read-only operations such as rendering.
		 * 
		 * @return const std::vector<spk::Vector3>& Constant reference to the vector of 3D points.
		 */
		const std::vector<spk::Vector3>& points() const;

		/**
		 * @brief Provides access to the mutable UV coordinates in the mesh.
		 *
		 * This method returns a reference to the vector of UV coordinates used for texture mapping. 
		 * It allows for direct modification of the UVs, useful for changing how textures are applied 
		 * to the mesh.
		 * 
		 * @return std::vector<spk::Vector2>& Reference to the vector of UV coordinates.
		 */
		std::vector<spk::Vector2>& uvs();

		/**
		 * @brief Provides access to the immutable UV coordinates in the mesh.
		 *
		 * This method returns a constant reference to the vector of UV coordinates. 
		 * It is used for accessing the UVs without modifying them, typically in texture 
		 * mapping during rendering.
		 * 
		 * @return const std::vector<spk::Vector2>& Constant reference to the vector of UV coordinates.
		 */
		const std::vector<spk::Vector2>& uvs() const;

		/**
		 * @brief Provides access to the mutable normals in the mesh.
		 *
		 * This method returns a reference to the vector of normals. Normals are essential for 
		 * lighting calculations in 3D rendering. Modifying this vector allows for dynamic changes 
		 * to how the mesh interacts with light.
		 * 
		 * @return std::vector<spk::Vector3>& Reference to the vector of normals.
		 */
		std::vector<spk::Vector3>& normals();

		/**
		 * @brief Provides access to the immutable normals in the mesh.
		 *
		 * This method returns a constant reference to the vector of normals, which is used 
		 * in lighting calculations during rendering. It allows for read-only access to the 
		 * normals, ensuring they are not inadvertently modified.
		 * 
		 * @return const std::vector<spk::Vector3>& Constant reference to the vector of normals.
		 */
		const std::vector<spk::Vector3>& normals() const;

		/**
		 * @brief Provides access to the immutable vertices in the mesh.
		 *
		 * This method returns a constant reference to the vector of vertices. Each vertex in 
		 * the vector contains position, UV, and normal data. This method is typically used for 
		 * read-only operations such as rendering.
		 * 
		 * @return const std::vector<Vertex>& Constant reference to the vector of vertices.
		 */
		const std::vector<Vertex>& vertices() const;

		/**
		 * @brief Provides access to the immutable indices in the mesh.
		 *
		 * This method returns a constant reference to the vector of indices. These indices 
		 * define the order in which vertices are connected to form the faces of the mesh. 
		 * This method is primarily used in rendering to draw the mesh correctly.
		 * 
		 * @return const std::vector<unsigned int>& Constant reference to the vector of indices.
		 */
		const std::vector<unsigned int> indexes() const;

		/**
         * @brief Adds a vertex to the mesh.
         * 
         * @param p_pointIndex Index of the point in the mesh.
         * @param p_uvIndex Index of the UV coordinate in the mesh.
         * @param p_normalIndex Index of the normal in the mesh.
         */
        void addVertex(const size_t& p_pointIndex, const size_t& p_uvIndex, const size_t& p_normalIndex);

        /**
         * @brief Adds a triangular face to the mesh.
         * 
         * @param p_vertexIndexA Index of the first vertex of the triangle.
         * @param p_vertexIndexB Index of the second vertex of the triangle.
         * @param p_vertexIndexC Index of the third vertex of the triangle.
         */
        void addFace(const unsigned int& p_vertexIndexA, const unsigned int& p_vertexIndexB, const unsigned int& p_vertexIndexC);

        /**
         * @brief Adds a quadrilateral face to the mesh.
         * 
         * Describes the topology for a face on the mesh, specifying the vertices in 
         * clockwise or counterclockwise order.
		 * 
		 * Top side view exemple :
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
         *
         * @param p_vertexIndexA Index of the first vertex of the quadrilateral.
         * @param p_vertexIndexB Index of the second vertex of the quadrilateral.
         * @param p_vertexIndexC Index of the third vertex of the quadrilateral.
         * @param p_vertexIndexD Index of the fourth vertex of the quadrilateral.
         */
        void addFace(const unsigned int& p_vertexIndexA, const unsigned int& p_vertexIndexB, const unsigned int& p_vertexIndexC, const unsigned int& p_vertexIndexD);

		/**
		 * @brief Checks if the mesh needs to be updated.
		 *
		 * This method is used to determine whether the mesh data has changed and needs
		 * to be updated, for example, in the graphics pipeline.
		 * 
		 * @return bool True if the mesh needs an update, false otherwise.
		 */
		bool needUpdate() const;

		/**
		 * @brief Sets the flag that indicates whether the mesh needs an update.
		 *
		 * This method allows setting the state of the update flag, which can be used
		 * to trigger an update of the mesh data when required.
		 * 
		 * @param p_state The state to set for the update flag (true or false).
		 */
		void setNeedUpdateFlag(bool p_state);

		/**
		 * @brief Resets the update flag for the mesh.
		 *
		 * This method is used to reset the update flag, indicating that the mesh has
		 * been updated and no further updates are currently needed.
		 */
		void resetNeedUpdateFlag();
	};
}