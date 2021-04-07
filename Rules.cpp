#include "Rules.h"

Rules::~Rules() { free(); }

void Rules::free() {
	rulesTexture.free();
}

bool Rules::load_rules() {

	bool success = true;

	std::fstream file{ "src/Rules.dat" };

	std::stringstream rulesStream;
	rulesStream.str("");

	std::string rules;

	if (!file) {
		std::cerr << "Failed to open rules file" << std::endl;
		success = false;
	}
	else {
		while (std::getline(file, rules))
			rulesStream << rules << "\n";

		if (!rulesTexture.loadFromRenderedText(rulesStream.str(), { 255, 255, 255 }, 1000)) {
			std::cout << "Failed to load Rules Media" << std::endl;
			success = false;
		}
	}

	file.close();

	return success;
}

void Rules::render() {

	SDL_Rect outer_box{ (GameWindow.Get_Width() / 2) - 500, 10, 1000, GameWindow.Get_Height() - 20};
	SDL_Rect inner_box{ (GameWindow.Get_Width() / 2) - 495, 15, 990, GameWindow.Get_Height() - 30 };

	//Set Renderer
	SDL_SetRenderDrawColor(GameWindow.Get_Renderer(), 0, 0, 0xAA, 0xFF);
	SDL_RenderClear(GameWindow.Get_Renderer());

	SDL_SetRenderDrawColor(GameWindow.Get_Renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(GameWindow.Get_Renderer(), &outer_box);

	SDL_SetRenderDrawColor(GameWindow.Get_Renderer(), 0, 0, 0xAA, 0xFF);
	SDL_RenderFillRect(GameWindow.Get_Renderer(), &inner_box);

	rulesTexture.renderTexture(nullptr, (GameWindow.Get_Width() / 2) - 480, 30);

	SDL_RenderPresent(GameWindow.Get_Renderer());
}

void Rules::handle_event(SDL_Event* e) {

	if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE)
		current_option = MAIN_MENU_SCREEN;
}

bool Rules::CheckTextureLoaded() {

	bool textureLoaded = true;

	if (rulesTexture.Get_Texture() == nullptr)
		textureLoaded = false;

	return textureLoaded;
}