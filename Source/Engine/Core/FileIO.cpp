#include "FileIO.h"
#include "Logger.h"
#include <fstream>
#include <iostream>

namespace kiko {
	std::string getFilePath() { return std::filesystem::current_path().string(); }

	bool setFilePath(const std::filesystem::path& path) {
		std::error_code ec;
		std::filesystem::current_path(path, ec);
		return ec.value() == 0;
	}

	std::string getFileName(const std::filesystem::path& path) {
		return path.filename().string();
	}

	bool fileExists(const std::filesystem::path& path) { return std::filesystem::exists(path); }

	bool getFileSize(const std::filesystem::path& path, size_t& size) {
		std::error_code ec;
		size = std::filesystem::file_size(path, ec);

		return ec.value() == 0;
	}

	bool readFile(const std::filesystem::path& path, std::string& buffer) {
		if (!fileExists(path)) {
			WARNING_LOG("file not loaded: " + path.string());
			return false;
		}

		// get file size and set buffer size
		size_t size;
		if (!getFileSize(path, size)) return false;
		buffer.resize(size);

		// read file into buffer
		std::ifstream fstream(path);
		fstream.read(buffer.data(), size);
		fstream.close();

		return true;
	}
}
