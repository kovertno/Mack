#ifndef TEXTURE_UTILS_H
#define TEXTURE_UTILS_H

#include <vector>
#include <string>

namespace TextureUtils {
	unsigned int LoadCubemap(std::vector<std::string>& faces);
}

#endif