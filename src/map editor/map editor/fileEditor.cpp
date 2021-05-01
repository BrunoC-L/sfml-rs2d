#include "fileEditor.h"

namespace editor {
	FileEditor::FileEditor(VTile p) : pos(p) {
		int cx = int(p.x / TILES_PER_CHUNK);
		int cy = int(p.y / TILES_PER_CHUNK);
		pos.x  = int(pos.x) % TILES_PER_CHUNK;
		pos.y  = int(pos.y) % TILES_PER_CHUNK;
		fileName = getEditorTileFileName();
		read();
	}

	void FileEditor::read() {
		std::ifstream reader(fileName, std::ios::in);
		std::string str;
		reader >> str;
		reader.close();
		if (str.length()) {
			JSON content(str);
			for (int i = 0; i < SIZE; ++i) {
				for (const auto& child : content[StrObjectTypes[i]].children)
					values[i].push_back(child.asString());
			}
		}
	}

	void FileEditor::save() {
		std::ofstream changesWriter(getChangesFileName(), std::ios::out);
		JSON content;
		for (int i = 0; i < SIZE; ++i) {
			content[StrObjectTypes[i]] = "[]";
			for (const auto& v : values[i])
				content[StrObjectTypes[i]].push(JSON(v));
		}
		auto str = content.objectAsString();

		std::ofstream writer(fileName, std::ios::out);
		writer.write(str.c_str(), str.length());
		writer.close();
	}

	void FileEditor::add(ObjectType ty, std::string value) {
		values[ty].push_back(value);
	}

	void FileEditor::remove(ObjectType ty, std::string value) {
		auto it = std::find(values[ty].begin(), values[ty].end(), value);
		if (it == values[ty].end())
			throw std::exception(("trying to remove absent value from property " + std::to_string(ty) + " value = " + value).c_str());
		values[ty].erase(it);
	}

	std::string FileEditor::getEditorTileFileName() const {
		return "../../../assets/editor/data/chunks/" +
			std::to_string(chunk.x) + "-" + std::to_string(chunk.y) + "-" + std::to_string(pos.z) + "/" +
			std::to_string(pos.x) + "-" + std::to_string(pos.y) + ".txt";
	}

	std::string FileEditor::getChangesFileName() const {
		return "../../../assets/editor/changes/" +
			std::to_string(int(chunk.x)) + " " + std::to_string(int(chunk.y)) + " " + std::to_string(int(pos.z)) + " " +
			std::to_string(int(pos.x)) + " " + std::to_string(int(pos.y));
	}
}
