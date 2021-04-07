#pragma once
#include "Texture.h"
#include "Data.h"
#include <sstream>

extern Screen_Options current_option;

class GameOver {
public:

	~GameOver();

	bool load_game_over_media();

	void render();

	void free();

	bool CheckTextureLoaded();

	void handle_event(SDL_Event& e);

private:
	Texture GameOverLostTexture;
	Texture GameOverWonTexture;
	Texture GameOverTextTexture;
};