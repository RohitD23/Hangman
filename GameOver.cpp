#include "GameOver.h"

GameOver::~GameOver() { free(); }

void GameOver::free() {

	GameOverLostTexture.free();
	GameOverWonTexture.free();
	GameOverTextTexture.free();
}

bool GameOver::CheckTextureLoaded() {
	bool textureLoaded = true;

	if (GameOverLostTexture.Get_Texture() == nullptr && GameOverWonTexture.Get_Texture() == nullptr)
		textureLoaded = false;

	return textureLoaded;
}

bool GameOver::load_game_over_media() {

	bool success = true;

	if (!GameOverLostTexture.load_media_from_file("src/Hangman_Lost.png")) {
		std::cout << "Failed to load Hangman_Lost.png";
		success = false;
	}

	if (!GameOverWonTexture.load_media_from_file("src/Hangman_won.png")) {
		std::cout << "Failed to load Hangman_Lost.png";
		success = false;
	}

	return success;
}

void GameOver::render() {
	SDL_SetRenderDrawColor(GameWindow.Get_Renderer(), 0, 0, 0xAA, 0xFF);
	SDL_RenderClear(GameWindow.Get_Renderer());

	if (current_option == GAME_LOST_SCREEN) {

		GameOverLostTexture.renderTexture(nullptr, GameWindow.Get_Width() / 2 - 400, GameWindow.Get_Height() / 2 - 200);

		std::stringstream resultStream;
		resultStream.str("");

		resultStream << "You killed the stickman.....Maybe you are the monster.\n\nPress 'Enter' to play again.\nPress 'esc' to return to Main Menu.";

		if (!GameOverTextTexture.loadFromRenderedText(resultStream.str(), { 0xFF,0xFF,0xFF }, 500))
			std::cout << "Failed to load GameOverTextTexture" << std::endl;

		GameOverTextTexture.renderTexture(nullptr, GameWindow.Get_Width() / 2, GameWindow.Get_Height() / 2 - 100);
	}
	else if (current_option == GAME_WON_SCREEN) {
		GameOverWonTexture.renderTexture(nullptr, GameWindow.Get_Width() / 2 - 400, GameWindow.Get_Height() / 2 - 200);

		std::stringstream resultStream;
		resultStream.str("");

		resultStream << "Congratulations you saved the stickman.\n\nPress 'Enter' to play again.\nPress 'esc' to return to Main Menu.";

		if (!GameOverTextTexture.loadFromRenderedText(resultStream.str(), { 0xFF,0xFF,0xFF }, 500))
			std::cout << "Failed to load GameOverTextTexture" << std::endl;

		GameOverTextTexture.renderTexture(nullptr, GameWindow.Get_Width() / 2, GameWindow.Get_Height() / 2 - 100);
	}

	SDL_RenderPresent(GameWindow.Get_Renderer());
}

void GameOver::handle_event(SDL_Event& e) {

	if (e.type == SDL_KEYDOWN) {
		
		switch (e.key.keysym.sym) {
		
		case SDLK_RETURN:
			current_option = GAME_SCREEN;
			break;

		case SDLK_ESCAPE:
			current_option = MAIN_MENU_SCREEN;
			break;

		}
	}
}