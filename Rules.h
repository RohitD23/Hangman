#pragma once
#include "Texture.h"
#include "Data.h"
#include <fstream>
#include <sstream>
#include <string>

extern Screen_Options current_option;

class Rules {

public:
	~Rules();

	void free();

	void render();

	bool load_rules();

	bool CheckTextureLoaded();

	void handle_event(SDL_Event* e);

private:
	Texture rulesTexture;
};

