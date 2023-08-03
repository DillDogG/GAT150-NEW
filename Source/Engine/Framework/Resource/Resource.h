#pragma once
#include <string>

namespace kiko {
	class Resource {
	public:
		virtual ~Resource() = default;

		virtual bool Create(std::string filename, ...) = 0;
	private:

	};
}