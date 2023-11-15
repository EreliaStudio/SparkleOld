#include "engine/spk_mesh_primitive.hpp"

namespace spk
{
	Cube::Cube()
	{
		points().push_back(spk::Vector3(-0.5f, -0.5f, -0.5f));
		points().push_back(spk::Vector3( 0.5f, -0.5f, -0.5f));
		points().push_back(spk::Vector3( 0.5f,  0.5f, -0.5f));
		points().push_back(spk::Vector3(-0.5f,  0.5f, -0.5f));
		points().push_back(spk::Vector3(-0.5f, -0.5f,  0.5f));
		points().push_back(spk::Vector3( 0.5f, -0.5f,  0.5f));
		points().push_back(spk::Vector3( 0.5f,  0.5f,  0.5f));
		points().push_back(spk::Vector3(-0.5f,  0.5f,  0.5f));

		uvs().push_back(spk::Vector2(0.0f, 0.0f));
		uvs().push_back(spk::Vector2(1.0f, 0.0f));
		uvs().push_back(spk::Vector2(1.0f, 1.0f));
		uvs().push_back(spk::Vector2(0.0f, 1.0f));

		normals().push_back(spk::Vector3( 0.0f,  0.0f, -1.0f));
		normals().push_back(spk::Vector3( 0.0f,  0.0f,  1.0f));
		normals().push_back(spk::Vector3( 0.0f, -1.0f,  0.0f));
		normals().push_back(spk::Vector3( 0.0f,  1.0f,  0.0f));
		normals().push_back(spk::Vector3(-1.0f,  0.0f,  0.0f));
		normals().push_back(spk::Vector3( 1.0f,  0.0f,  0.0f));

		addVertex(0, 0, 0); addVertex(1, 1, 0); addVertex(2, 2, 0); addVertex(3, 3, 0);
		addFace(0, 1, 2, 3);

		addVertex(4, 0, 1); addVertex(7, 1, 1); addVertex(6, 2, 1); addVertex(5, 3, 1);
		addFace(4, 5, 6, 7);

		addVertex(0, 0, 2); addVertex(4, 1, 2); addVertex(5, 2, 2); addVertex(1, 3, 2);
		addFace(8, 9, 10, 11);

		addVertex(1, 0, 3); addVertex(5, 1, 3); addVertex(6, 2, 3); addVertex(2, 3, 3);
		addFace(12, 13, 14, 15);

		addVertex(2, 0, 4); addVertex(6, 1, 4); addVertex(7, 2, 4); addVertex(3, 3, 4);
		addFace(16, 17, 18 ,19);

		addVertex(4, 0, 5); addVertex(0, 1, 5); addVertex(3, 2, 5); addVertex(7, 3, 5);
		addFace(20, 21, 22, 23);
	}
}