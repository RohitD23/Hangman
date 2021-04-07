#include "MainMenu.h"

MainMenu::~MainMenu() {
	free();
}

void MainMenu::render(bool& frame_loop, int& frame) {

	SDL_SetRenderDrawColor(GameWindow.Get_Renderer(), 0, 0, 0xAA, 0xFF);
	SDL_RenderClear(GameWindow.Get_Renderer());

	SDL_Rect* currentClip = &intro_animation[frame / 500];

	SDL_Rect intro_text_outline = { GameWindow.Get_Width() / 2, (GameWindow.Get_Height() / 2) - 100, 400, 250 };
	SDL_Rect intro_text_inline = { (GameWindow.Get_Width() / 2) + 5, ((GameWindow.Get_Height() / 2) + 5) - 100, 390, 240 };

	introTexture.renderTexture(currentClip, (GameWindow.Get_Width() / 2) - 400, (GameWindow.Get_Height() / 2) - 200);
	intro_text_texture.renderTexture(nullptr, (GameWindow.Get_Width() / 2) + 90, (GameWindow.Get_Height() / 2) - 160);

	SDL_SetRenderDrawColor(GameWindow.Get_Renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(GameWindow.Get_Renderer(), &intro_text_outline);

	SDL_SetRenderDrawColor(GameWindow.Get_Renderer(), 0, 0, 0xAA, 0xFF);
	SDL_RenderFillRect(GameWindow.Get_Renderer(), &intro_text_inline);

	int y = -50;
	for (auto& text : menu_text_texture) {
		std::get<0>(text).renderTexture(nullptr, (GameWindow.Get_Width() / 2) + 100, (GameWindow.Get_Height() / 2) + y);
		y += 50;
	}

	//Update screen
	SDL_RenderPresent(GameWindow.Get_Renderer());

	if (!frame_loop)
		frame++;
	else
		frame--;

	//Cycle animation
	if (frame / 500 >= INTRO_ANIMATION_FRAMES) {
		frame = 2000;
		frame_loop = true;
	}
	if (frame <= 0) {
		frame = 500;
		frame_loop = false;
	}
}

void MainMenu::down_key_menu_selection(const SDL_Color& textColor, const std::string each_option_text[][2]) {

	for (int i{ 0 }; i < TOTAL_MENU_TEXT_TEXTURE; i++) {

		if (std::get<1>(menu_text_texture[i]) == true) {

			if (i == TOTAL_MENU_TEXT_TEXTURE - 1) {

				std::get<1>(menu_text_texture[0]) = true;
				std::get<1>(menu_text_texture[TOTAL_MENU_TEXT_TEXTURE - 1]) = false;

				if (!std::get<0>(menu_text_texture[0]).loadFromRenderedText("#Play", textColor, 500))
				{
					std::cout << "Failed to render text texture!" << std::endl;
				}

				if (!std::get<0>(menu_text_texture[2]).loadFromRenderedText("Exit", textColor, 500))
				{
					std::cout << "Failed to render text texture!" << std::endl;
				}

				break;
			}
			else {

				std::get<1>(menu_text_texture[i]) = false;
				std::get<1>(menu_text_texture[i + 1]) = true;

				if (!std::get<0>(menu_text_texture[i]).loadFromRenderedText(each_option_text[i][1], textColor, 500))
				{
					std::cout << "Failed to render text texture!" << std::endl;
				}

				if (!std::get<0>(menu_text_texture[i + 1]).loadFromRenderedText(each_option_text[i + 1][0], textColor, 500))
				{
					std::cout << "Failed to render text texture!" << std::endl;
				}

				break;
			}
		}

	}
}

void MainMenu::up_key_menu_selection(const SDL_Color& textColor, const std::string each_option_text[][2]) {

	for (int i{ 0 }; i < TOTAL_MENU_TEXT_TEXTURE; i++) {

		if (std::get<1>(menu_text_texture[i]) == true) {

			if (i == 0) {

				std::get<1>(menu_text_texture[0]) = false;
				std::get<1>(menu_text_texture[TOTAL_MENU_TEXT_TEXTURE - 1]) = true;

				if (!std::get<0>(menu_text_texture[0]).loadFromRenderedText("Play", textColor, 500))
				{
					std::cout << "Failed to render text texture!" << std::endl;
				}

				if (!std::get<0>(menu_text_texture[2]).loadFromRenderedText("#Exit", textColor, 500))
				{
					std::cout << "Failed to render text texture!" << std::endl;
				}

				break;
			}
			else {

				std::get<1>(menu_text_texture[i]) = false;
				std::get<1>(menu_text_texture[i - 1]) = true;

				if (!std::get<0>(menu_text_texture[i]).loadFromRenderedText(each_option_text[i][1], textColor, 500))
				{
					std::cout << "Failed to render text texture!" << std::endl;
				}

				if (!std::get<0>(menu_text_texture[i - 1]).loadFromRenderedText(each_option_text[i - 1][0], textColor, 500))
				{
					std::cout << "Failed to render text texture!" << std::endl;
				}

				break;
			}
		}

	}
}

void MainMenu::menu_selection(SDL_Event& e) {

	SDL_Color textColor = { 0xFF, 0xFF, 0xFF };

	std::string each_option_text[TOTAL_MENU_TEXT_TEXTURE][2] = { {"#Play", "Play"},
		{"#Rules", "Rules"}, {"#Exit", "Exit"} };

	*(GameWindow.Get_Font()) = TTF_OpenFont("src/LTBulletin-Medium.ttf", 40);

	if (e.type == SDL_KEYDOWN) {

		switch (e.key.keysym.sym) {

		case SDLK_DOWN:
		case SDLK_s:

			down_key_menu_selection(textColor, each_option_text);
			break;

		case SDLK_UP:
		case SDLK_w:

			up_key_menu_selection(textColor, each_option_text);
			break;
		}
	}

}

void MainMenu::load_menu_texture(const SDL_Color& textColor) {

	*(GameWindow.Get_Font()) = TTF_OpenFont("src/LTBulletin-Medium.ttf", 40);

	std::get<1>(menu_text_texture[0]) = true;
	std::get<1>(menu_text_texture[1]) = false;
	std::get<1>(menu_text_texture[2]) = false;

	if (!std::get<0>(menu_text_texture[0]).loadFromRenderedText("#Play", textColor, 500))
		std::cout << "Failed to render text texture!" << std::endl;

	if (!std::get<0>(menu_text_texture[1]).loadFromRenderedText("Rules", textColor, 500))
		std::cout << "Failed to render text texture!" << std::endl;

	if (!std::get<0>(menu_text_texture[2]).loadFromRenderedText("Exit", textColor, 500))
		std::cout << "Failed to render text texture!" << std::endl;

}

bool MainMenu::load_Menu_Menu_Media() {

	bool success = true;

	if (!introTexture.load_media_from_file("src/Hangman_intro.png")) {
		std::cout << "Failed to load Hangman_intro.png" << std::endl;
		success = false;
	}

	else {
		int x_offset = 0;

		for (SDL_Rect& s : intro_animation) {
			s.x = x_offset;
			s.y = 0;
			s.w = 350;
			s.h = 400;

			x_offset += 350;
		}

		*(GameWindow.Get_Font()) = TTF_OpenFont("src/LTBulletin-Medium.ttf", 55);

		if (*(GameWindow.Get_Font()) == nullptr) {
			std::cout << "Failed to load lazy font! SDL_ttf Error: " << TTF_GetError() << std::endl;
			success = false;
		}

		else {
			//Render text
			SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
			if (!intro_text_texture.loadFromRenderedText("HANGMAN", textColor, 500)) {
				std::cout << "Failed to render text texture!" << std::endl;
				success = false;
			}

			load_menu_texture(textColor);
		}
	}

	return success;
}

void MainMenu::free() {

	introTexture.free();
	intro_text_texture.free();

	for (auto& s : menu_text_texture) {
		std::get<0>(s).free();
		std::get<1>(s) = false;
	}

	for (SDL_Rect& s : intro_animation) {
		s.x = 0;
		s.y = 0;
		s.w = 0;
		s.h = 0;
	}
}

bool MainMenu::CheckTextureLoaded() {

	bool textureLoaded = true;

	if (introTexture.Get_Texture() == nullptr && intro_text_texture.Get_Texture() == nullptr)
		textureLoaded = false;

	return textureLoaded;
}