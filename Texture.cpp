#include "Texture.h"

Texture::Texture()
	:texture(nullptr), tHeight(0), tWidth(0) { }

Texture::Texture(std::string textureText, SDL_Color textColor)
	: texture(nullptr), tHeight(0), tWidth(0)
{
	loadFromRenderedText(textureText, textColor, 500);
}

Texture::~Texture() {
	free();
}

void Texture::free() {

	if (texture != nullptr) {
		SDL_DestroyTexture(texture);

		texture = nullptr;
		tHeight = 0;
		tWidth = 0;
	}
}

bool Texture::load_media_from_file(std::string path) {
	free();

	SDL_Texture* newTexture = nullptr;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == nullptr) {
		std::cout << "Enable to load surface SDL ERROR : " << IMG_GetError() << std::endl;
	}
	else {

		newTexture = SDL_CreateTextureFromSurface(GameWindow.Get_Renderer(), loadedSurface);

		if (newTexture == nullptr) {
			std::cout << "Enable to load texture SDL ERROR : " << SDL_GetError() << std::endl;
		}
		else {
			tHeight = loadedSurface->h;
			tWidth = loadedSurface->w;
		}

		SDL_FreeSurface(loadedSurface);
	}

	texture = newTexture;

	return (texture != nullptr);
}

void Texture::renderTexture(SDL_Rect* clip, int x, int y) const {

	SDL_Rect destination{ x, y, tWidth, tHeight };

	if (clip != nullptr) {
		destination.w = clip->w;
		destination.h = clip->h;
	}

	SDL_RenderCopy(GameWindow.Get_Renderer(), texture, clip, &destination);

}

bool Texture::loadFromRenderedText(std::string textureText, SDL_Color fgtextColor, int wrap)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(*(GameWindow.Get_Font()), textureText.c_str(), fgtextColor, wrap);
	if (textSurface == nullptr)
	{
		std::cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
	}
	else
	{
		//Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(GameWindow.Get_Renderer(), textSurface);
		if (texture == nullptr)
		{
			std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
		}
		else
		{
			//Get image dimensions
			tWidth = textSurface->w;
			tHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return texture != NULL;
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture
	SDL_SetTextureColorMod(texture, red, green, blue);
}