#include "engine/spk_mesh_primitive.hpp"

namespace spk
{
	Cube::Cube()
	{
		points().push_back(spk::Vector3(-0.5f, -0.5f, -0.5f)); // 0
		points().push_back(spk::Vector3(0.5f, -0.5f, -0.5f)); // 1
		points().push_back(spk::Vector3(0.5f, -0.5f, 0.5f)); // 2
		points().push_back(spk::Vector3(-0.5f, -0.5f, 0.5f)); // 3
		points().push_back(spk::Vector3(-0.5f, 0.5f, -0.5f)); // 4
		points().push_back(spk::Vector3(0.5f, 0.5f, -0.5f)); // 5
		points().push_back(spk::Vector3(0.5f, 0.5f, 0.5f)); // 6
		points().push_back(spk::Vector3(-0.5f, 0.5f, 0.5f)); // 7

        uvs().push_back(spk::Vector2(1, 0));
        uvs().push_back(spk::Vector2(0, 0));
        uvs().push_back(spk::Vector2(0, 1));
        uvs().push_back(spk::Vector2(1, 1));

		normals().push_back(spk::Vector3( 0,  0,  1 ));
		normals().push_back(spk::Vector3( 0,  0, -1 ));
		normals().push_back(spk::Vector3( 0,  1,  0 ));
		normals().push_back(spk::Vector3( 0, -1,  0 ));
		normals().push_back(spk::Vector3( 1,  0,  0 ));
		normals().push_back(spk::Vector3(-1,  0,  0 ));

        addVertex(4, 0, 1); addVertex(5, 1, 1); addVertex(1, 2, 1); addVertex(0, 3, 1);

        addVertex(5, 0, 0); addVertex(6, 1, 0); addVertex(2, 2, 0); addVertex(1, 3, 0);

        addVertex(7, 0, 5); addVertex(4, 1, 5); addVertex(0, 2, 5); addVertex(3, 3, 5);

        addVertex(6, 0, 4); addVertex(7, 1, 4); addVertex(3, 2, 4); addVertex(2, 3, 4);

        addVertex(7, 0, 2); addVertex(6, 1, 2); addVertex(5, 2, 2); addVertex(4, 3, 2);

        addVertex(0, 0, 3); addVertex(1, 1, 3); addVertex(2, 2, 3); addVertex(3, 3, 3);

        addFace(0, 1, 2, 3);
        addFace(4, 5, 6, 7);
        addFace(8, 9, 10, 11);
        addFace(12, 13, 14, 15);
        addFace(16, 17, 18, 19);
        addFace(20, 21, 22, 23);
	}
}