#include "Texture.h"
#include "Renderer.h"
#include "include/SDL_image.h"

namespace kiko {
	Texture::~Texture() {
		if (m_texture) SDL_DestroyTexture(m_texture);
	}
	bool Texture::Load(std::string filename, class Renderer& renderer) {
		SDL_Surface* surface = IMG_Load(filename.c_str());
		if (!surface) {
			return false;
		}
		m_texture = SDL_CreateTextureFromSurface(renderer.m_renderer, surface);
		SDL_FreeSurface(surface);
		if (!m_texture) {
			
			return false;
		}
		return true;
	}
	vec2 Texture::GetSize() {
		SDL_Point point;
		SDL_QueryTexture(m_texture, nullptr, nullptr, &point.x, &point.y);
		return vec2{ (float)point.x, (float)point.y };
	}
	bool Texture::Create(std::string filename, ...)
	{
		return false;
	}
}