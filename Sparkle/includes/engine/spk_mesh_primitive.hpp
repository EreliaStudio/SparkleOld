#pragma once

#include "engine/spk_mesh.hpp"

namespace spk
{
	/**
     * @brief Represents a cube mesh in the Sparkle engine.
     *
     * This class extends the Mesh class to specifically represent a 3D cube.
     * It initializes the vertices, normals, UVs, and indices for a standard cube.
     */
	class Cube : public spk::Mesh
	{
	private:

	public:
		/**
         * @brief Constructs a new Cube object.
         *
         * This constructor initializes a cube mesh with predefined vertices, UVs, normals,
         * and face indices appropriate for a cube geometry.
         */
		Cube();
	};
}