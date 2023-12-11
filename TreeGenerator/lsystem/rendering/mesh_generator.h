#ifndef TREE_GENERATOR_LSYSTEM_MESH_GENERATOR_H_
#define TREE_GENERATOR_LSYSTEM_MESH_GENERATOR_H_

#include <vector>

#include <glm/glm.hpp>

#include "../core/lsystem.h"
#include "../../graphics/common/mesh_data.h"
#include "../../graphics/common/transform.h"

namespace tree_generator::lsystem
{
	class MeshGenerator
	{
	public:
		void DefineDraw(const Symbol& symbol, MeshData meshData);
		void DefineMove(const Symbol& symbol);
		void DefineRotate(const Symbol& symbol, glm::vec3 rotation);
		void DefineSave(const Symbol& symbol);
		void DefineRestore(const Symbol& symbol);

		struct MeshGroup
		{
			MeshData mesh;
			std::vector<Transform> instances;
		};
		std::vector<MeshGroup> Generate(const std::vector<Symbol>& symbols) const;
	};
}

#endif  // !TREE_GENERATOR_LSYSTEM_MESH_GENERATOR_H_