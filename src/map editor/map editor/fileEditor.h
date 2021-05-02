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
			MONSTER,
			NPC,
			OBJECT,
			ITEM,
			SIZE
		};
		const std::string StrObjectTypes[SIZE] = {
			"monster",
			"npc",
			"object",
			"item",
		};
		std::string fileName;
		std::vector<std::string> values[SIZE];
		VTile pos;
		VChunk chunk;
		int wall;
		FileEditor(VChunk chunk, VTile pos, int wall);
		std::string getEditorTileFileName() const;
		std::string getChangesFileName(bool done) const;
		void read();
		void save();
		void save(const std::vector<std::pair<editor::FileEditor::ObjectType, std::string>>& all);
		void add(ObjectType ty, std::string value);
		void remove(ObjectType ty, std::string value);
	};
}
