#pragma once
#include "json.h"
#include <iostream>
#include <fstream>
#include "units.h"

const int TILES_PER_CHUNK = 64;
const int PIXELS_PER_TILE = 32;
const int PIXELS_PER_TILE_ON_IMAGE = 4;

namespace editor {
	class FileEditor {
	public:
		static enum ObjectType {
			RESOURCE,
			MONSTER,
			NPC,
			GAMEOBJECT,
			ITEM,
			WALL,
			SIZE
		};
		const std::string StrObjectTypes[SIZE] = {
			"resource",
			"monster",
			"npc",
			"object",
			"item",
			"wall"
		};
		std::string fileName;
		std::vector<std::string> values[SIZE];
		VTile pos;
		VChunk chunk;
		FileEditor(VTile pos);
		std::string getEditorTileFileName() const;
		std::string getChangesFileName() const;
		void read();
		void save();
		void add(ObjectType ty, std::string value);
		void remove(ObjectType ty, std::string value);
	};
}
