#pragma once
#include "Window.h"
#include <string>

extern Window GameWindow;

class Texture {

public:
	Texture();
	Texture(std::string textureText, SDL_Color textColor);

	~Texture();

	void free();

	bool load_media_from_file(std::string path);

	void renderTexture(SDL_Rect* clip, int x, int y) const;

	bool loadFromRenderedText(std::string textureText, SDL_Color fgtextColor, int wrap);

	SDL_Texture* Get_Texture() const { return texture; }

	int Get_Width() { return tWidth; }
	int Get_Height() { return tHeight; }

	void setColor(Uint8 red, Uint8 green, Uint8 blue);

private:
	SDL_Texture* texture;
	int tHeight;
	int tWidth;
};
