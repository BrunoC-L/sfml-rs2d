#include "fileEditor.h"

namespace editor {
	FileEditor::FileEditor(VChunk chunk, VTile pos, int wall) : wall(wall), chunk(chunk), pos(pos) {
		fileName = getEditorTileFileName();
		read();
	}

	void FileEditor::read() {
		std::ifstream reader(fileName, std::ios::in);
		std::string str;
		std::getline(reader, str);
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
		auto s = getChangesFileName(false);
		auto g = getChangesFileName(true);
		std::ofstream changesWriter(getChangesFileName(false), std::ios::out); // create changes file
		auto v = changesWriter.is_open();
		changesWriter.close();
		std::remove(getChangesFileName(true).c_str()); // remove changes done file
		JSON content;
		content["wall"] = std::to_string(wall);
		for (int i = 0; i < SIZE; ++i) {
			content[StrObjectTypes[i]] = "[]";
			for (const auto& v : values[i])
				content[StrObjectTypes[i]].push(JSON('"' + v + '"'));
		}
		auto str = content.objectAsString();

		std::ofstream writer(fileName, std::ios::out);
		auto u = writer.is_open();
		writer.write(str.c_str(), str.length());
		writer.close();
	}

	void FileEditor::save(const std::vector<std::pair<editor::FileEditor::ObjectType, std::string>>& all) {
		for (int type = 0; type < ObjectType::SIZE; ++type) {
			values[type].clear();
			for (const auto& value : all) {
				if (value.first != type)
					continue;
				add(ObjectType(type), value.second);
			}
		}
		save();
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
			std::to_string(int(chunk.x)) + "-" + std::to_string(int(chunk.y)) + "-" + std::to_string(int(chunk.z)) + "/" +
			std::to_string(int(pos.x)) + "-" + std::to_string(int(pos.y)) + ".json";
	}

	std::string FileEditor::getChangesFileName(bool done) const {
		return "../../../assets/editor/changes" + std::string(done ? " done/" : "/") +
			std::to_string(int(chunk.x)) + " " + std::to_string(int(chunk.y)) + " " + std::to_string(int(pos.z)) + " " +
			std::to_string(int(pos.x)) + " " + std::to_string(int(pos.y));
	}
}
