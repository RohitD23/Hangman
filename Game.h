#pragma once
#include "Texture.h"
#include "Data.h"
#include "Button.h"
#include <vector>
#include <fstream>
#include <sstream>

extern Screen_Options current_option;

class Game {
public:
	Game();

	~Game();

	bool load_game_media();

	void render();

	void free();

	bool CheckTextureLoaded();
	bool Check_number_of_inputs();

	void handle_event(SDL_Event& e);

	Button& Get_passButton() { return passButton; }
	
	int Get_no_of_passes() { return no_of_passes; }

private:
	Texture Hangman_start;
	Texture Hangman_main;

	Texture QuestionTextTexture;
	std::vector <Texture> player_answer_text_texture;

	Texture ResultImageTexture;
	Texture ResultTextTexture;

	Texture timeTextTexture;

	Texture NoOfQuestionsRemaining;

	const static int TOTAL_HANGMAN_MAIN_SPRITES = 6;
	const static int TOTAL_RESULT_SPRITES = 3;
	const static int Sprite_Height = 400;
	const static int Sprite_Width = 350;

	std::vector <Riddle> riddles;
	std::vector <int> already_choose_question;
	std::vector <char> player_answer;

	int number_of_dashes;
	int number_of_inputs;

	int lifes_remaining;

	int hangman_sprite;
	bool render_next_question;

	uint32_t startTime;
	uint32_t pausedTime;
	uint32_t total_time;
	int check_time;

	Button passButton;
	int no_of_passes;

	Button resumeButton;
	Button restartButton;
	Button exitButton;

	which_screen_to_render currentScreen;

	SDL_Rect currentHangmanSprite[TOTAL_HANGMAN_MAIN_SPRITES];
	SDL_Rect currentResultSprite[TOTAL_RESULT_SPRITES];

	bool loadRiddles();

	int ChooseQuestion();
	int LoadQuestion();

	bool CheckPlayerAnswer();
	void LoadResultTextTexture();

	void renderQuestion();
	void renderResult();
	void renderPauseScreen();

	void checkTime();
	bool checkGameOver();

	void PassQuestion();

	bool LoadButtons();

};
