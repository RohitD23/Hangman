#include "Game.h"

#define ANSWER_SIZE riddles.at(number_of_dashes).answer.size()

Game::Game()
	:number_of_dashes(0), number_of_inputs(0), render_next_question(true), hangman_sprite(-1), no_of_passes(0),
	 check_time(1), total_time(30), startTime(0), pausedTime(0), lifes_remaining(6), currentScreen(QUESTION_SCREEN) {

	for (auto& sprite : currentHangmanSprite) {
		sprite.x = 0;
		sprite.y = 0;
		sprite.w = 0;
		sprite.h = 0;
	}

	for (auto& sprite : currentResultSprite) {
		sprite.x = 0;
		sprite.y = 0;
		sprite.w = 0;
		sprite.h = 0;
	}

}

Game::~Game() {
	free();
}

void Game::free() {

	Hangman_start.free();
	Hangman_main.free();
	QuestionTextTexture.free();

	ResultImageTexture.free();
	ResultTextTexture.free();

	timeTextTexture.free();

	NoOfQuestionsRemaining.free();

	passButton.free();
	no_of_passes = 0;

	resumeButton.free();
	restartButton.free();
	exitButton.free();

	riddles.clear();
	already_choose_question.clear();
	player_answer.clear();

	currentScreen = QUESTION_SCREEN;

	number_of_dashes = 0;
	number_of_inputs = 0;
	
	lifes_remaining = 6;

	hangman_sprite = -1;
	render_next_question = true;

	startTime = 0;
	pausedTime = 0;
	total_time = 30;
	check_time = 1;

	for (auto& s : player_answer_text_texture)
		s.free();

	for (auto& sprite : currentHangmanSprite) {
		sprite.x = 0;
		sprite.y = 0;
		sprite.w = 0;
		sprite.h = 0;
	}

	for (auto& sprite : currentResultSprite) {
		sprite.x = 0;
		sprite.y = 0;
		sprite.w = 0;
		sprite.h = 0;
	}

}

bool Game::load_game_media() {

	bool success = true;

	already_choose_question.reserve(10);

	if (!Hangman_start.load_media_from_file("src/Hangman_start.png")) {
		std::cout << "Failed to load Hangman_start.png" << std::endl;
		success = false;
	}

	if (!Hangman_main.load_media_from_file("src/Hangman_main.png")) {
		std::cout << "Failed to load Hangman_main.png" << std::endl;
		success = false;
	}

	if (!ResultImageTexture.load_media_from_file("src/Right_or_wrong.png")) {
		std::cout << "Failed to load Right_or_wrong.png" << std::endl;
		success = false;
	}

	int x_offset = 0;
	int y_offset = 0;

	int sprite_position_condition = 0;

	for (auto& sprite : currentHangmanSprite) {
		sprite.x = x_offset;
		sprite.y = y_offset;
		sprite.w = Sprite_Width;
		sprite.h = Sprite_Height;

		sprite_position_condition++;

		x_offset += 350;

		if (sprite_position_condition == 3)
			x_offset = 0;

		if (sprite_position_condition == 3)
			y_offset += 400;
	}

	x_offset = 0;

	for (auto& sprite : currentResultSprite) {
		sprite.x = x_offset;
		sprite.y = 0;
		sprite.w = 250;
		sprite.h = 250;

		x_offset += 250;
	}

	success = loadRiddles();
	success = LoadButtons();

	return success;
}

void Game::render() {

	if (currentScreen == QUESTION_SCREEN)
		renderQuestion();
	else if (currentScreen == RESULT_SCREEN)
		renderResult();
	else if (currentScreen == PAUSE_SCREEN)
		renderPauseScreen();

	SDL_RenderPresent(GameWindow.Get_Renderer());
}

void Game::renderQuestion() {

	if (checkGameOver())
		return;

	if (passButton.Get_isButtonPressed() && no_of_passes < 3)
		PassQuestion();

	//Rectangle initilization
	SDL_Rect answer_space = { (GameWindow.Get_Width() / 2) + 50, GameWindow.Get_Height() / 2, 40, 5 };
	SDL_Rect question_text_outline = { (GameWindow.Get_Width() / 2) - 95, (GameWindow.Get_Height() / 2) + 40, 540, QuestionTextTexture.Get_Height() + 20 };
	SDL_Rect question_text_inline = { (GameWindow.Get_Width() / 2) - 90, (GameWindow.Get_Height() / 2) + 45, 530, QuestionTextTexture.Get_Height() + 10 };

	//Load Question
	if (render_next_question) {
		number_of_dashes = LoadQuestion();
		render_next_question = false;
	}

	//Set Renderer
	SDL_SetRenderDrawColor(GameWindow.Get_Renderer(), 0, 0, 0xAA, 0xFF);
	SDL_RenderClear(GameWindow.Get_Renderer());

	//Render outline Rectangle around question
	SDL_SetRenderDrawColor(GameWindow.Get_Renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(GameWindow.Get_Renderer(), &question_text_outline);

	//Render inline Rectangle around question
	SDL_SetRenderDrawColor(GameWindow.Get_Renderer(), 0, 0, 0xAA, 0xFF);
	SDL_RenderFillRect(GameWindow.Get_Renderer(), &question_text_inline);

	//Render Hangman Image
	if (hangman_sprite == -1)
		Hangman_start.renderTexture(nullptr, GameWindow.Get_Width() / 2 - 500, GameWindow.Get_Height() / 2 - 200);

	else if (hangman_sprite < TOTAL_HANGMAN_MAIN_SPRITES - 1) {

		SDL_Rect* currentSprite = &currentHangmanSprite[hangman_sprite];

		Hangman_main.renderTexture(currentSprite, GameWindow.Get_Width() / 2 - 500, GameWindow.Get_Height() / 2 - 200);
	}

	//Render Question 
	QuestionTextTexture.renderTexture(nullptr, (GameWindow.Get_Width() / 2) - 70, (GameWindow.Get_Height() / 2) + 50);

	checkTime();

	//Render Answer text
	{
		int x_offset = 8;
		int y_offset = 40;
		for (const auto& s : player_answer_text_texture) {
			s.renderTexture(nullptr, (GameWindow.Get_Width() / 2 + 50) + x_offset, (GameWindow.Get_Height() / 2) - y_offset);
			x_offset += 50;
		}
	}

	//Render answer dashes
	SDL_SetRenderDrawColor(GameWindow.Get_Renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	for (int i{ 0 }; i < ANSWER_SIZE; i++) {
		SDL_RenderFillRect(GameWindow.Get_Renderer(), &answer_space);
		answer_space.x += 50;
	}

	//Loading No of Questions remianing and Chances remaining text
	std::stringstream resultString;
	resultString.str("");

	resultString << "Number of Question answered: " << already_choose_question.size() - 1
		<< "\nChances remaining: " << lifes_remaining;

	if (!NoOfQuestionsRemaining.loadFromRenderedText(resultString.str(), { 0xFF,0xFF,0xFF }, 500))
		std::cout << "Failed to load NoOfQuestionsRemaining" << std::endl;

	//Render No of Questions remianing and Chances remaining text
	NoOfQuestionsRemaining.renderTexture(nullptr, Window::SCREEN_WIDTH - NoOfQuestionsRemaining.Get_Width(), 0);

	//Render button
	passButton.render((GameWindow.Get_Width() / 2) + 153, (GameWindow.Get_Height() / 2) - 135);
}

void Game::renderResult() {
	LoadResultTextTexture();

	SDL_SetRenderDrawColor(GameWindow.Get_Renderer(), 0, 0, 0xAA, 0xFF);
	SDL_RenderClear(GameWindow.Get_Renderer());

	if (CheckPlayerAnswer()) {

		SDL_Rect* currentSprite = &currentResultSprite[0];

		ResultImageTexture.renderTexture(currentSprite, (GameWindow.Get_Width() / 2) - 100, GameWindow.Get_Height() / 2 - 200);
		ResultTextTexture.renderTexture(nullptr, (GameWindow.Get_Width() / 2) - 200, (GameWindow.Get_Height() / 2) + 60);
	}
	else {

		SDL_Rect* currentSprite;

		if (check_time < 0)
			currentSprite = &currentResultSprite[2];
		else
			currentSprite = &currentResultSprite[1];

		ResultImageTexture.renderTexture(currentSprite, GameWindow.Get_Width() / 2 - 100, GameWindow.Get_Height() / 2 - 200);
		ResultTextTexture.renderTexture(nullptr, (GameWindow.Get_Width() / 2) - 150, (GameWindow.Get_Height() / 2) + 60);

	}

}

void Game::renderPauseScreen() {

	if (exitButton.Get_isButtonPressed())
		current_option = EXIT;

	else if (restartButton.Get_isButtonPressed())
		free();

	else if (resumeButton.Get_isButtonPressed()) {
		startTime = SDL_GetTicks() - pausedTime;
		resumeButton.Set_isButtonPressed(false);
		currentScreen = QUESTION_SCREEN;
	}

	SDL_SetRenderDrawColor(GameWindow.Get_Renderer(), 0, 0, 0xAA, 0xFF);
	SDL_RenderClear(GameWindow.Get_Renderer());

	resumeButton.render((GameWindow.Get_Width() - resumeButton.Get_Width()) / 2 , (GameWindow.Get_Height() / 2) - 137);

	restartButton.render((GameWindow.Get_Width() - restartButton.Get_Width()) / 2, (GameWindow.Get_Height() / 2) - 67);

	exitButton.render((GameWindow.Get_Width() - exitButton.Get_Width()) / 2, (GameWindow.Get_Height() / 2) + 3);

	SDL_RenderPresent(GameWindow.Get_Renderer());
}

bool Game::loadRiddles() {
	riddles.reserve(27);

	Riddle riddle;
	bool success = true;

	std::ifstream in_file{ "src/Hangman_riddles.dat" };

	if (!in_file) {
		std::cerr << "Hangman riddles open error" << std::endl;
		success = false;
	}

	while (std::getline(in_file, riddle.question)) {

		std::getline(in_file, riddle.answer);
		riddles.push_back(riddle);
	}

	in_file.close();

	return success;
}

int Game::ChooseQuestion() {
	srand(SDL_GetTicks());

	int question_number;
	bool isQuestionChoosed = false;

	while (!isQuestionChoosed) {

		question_number = rand() % 27;

		if (!already_choose_question.empty()) {

			for (const int& s : already_choose_question) {
				if (s != question_number)
					isQuestionChoosed = true;
				else {
					isQuestionChoosed = false;
					break;
				}
			}
		}
		else {
			break;
		}
	}

	already_choose_question.push_back(question_number);

	player_answer.clear();
	player_answer_text_texture.clear();

	player_answer.reserve(riddles.at(question_number).answer.size());
	player_answer_text_texture.reserve(riddles.at(question_number).answer.size());

	return question_number;
}

bool Game::CheckTextureLoaded() {

	bool textureLoaded = true;

	if (Hangman_main.Get_Texture() == nullptr && Hangman_start.Get_Texture() == nullptr)
		textureLoaded = false;

	return textureLoaded;
}

int Game::LoadQuestion() {
	bool success = true;

	int question_number = ChooseQuestion();
	SDL_Color text_color = { 0xFF, 0xFF, 0xFF };

	if (!QuestionTextTexture.loadFromRenderedText(riddles.at(question_number).question, text_color, 500)) {
		std::cout << "Failed to load question text" << std::endl;
	}

	return question_number;
}

void Game::handle_event(SDL_Event& e) {

	SDL_Color text_color = { 0xFF,0xFF,0xFF };

	if (currentScreen == QUESTION_SCREEN) {
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE) {

			if (!player_answer.empty()) {
				player_answer.pop_back();
				player_answer_text_texture.pop_back();
				number_of_inputs--;
			}

		}

		else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
			currentScreen = PAUSE_SCREEN;
			pausedTime = SDL_GetTicks() - startTime;
		}

		else if (player_answer.size() == ANSWER_SIZE && (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)) {
			currentScreen = RESULT_SCREEN;
		}

		else if (e.type == SDL_KEYDOWN && (Check_number_of_inputs() || player_answer.empty())) {

			switch (e.key.keysym.sym) {

			case SDLK_a:
				player_answer.push_back('a');
				player_answer_text_texture.emplace_back("A", text_color);
				number_of_inputs++;
				break;

			case SDLK_b:
				player_answer.push_back('b');
				player_answer_text_texture.emplace_back("B", text_color);
				number_of_inputs++;
				break;

			case SDLK_c:
				player_answer.push_back('c');
				player_answer_text_texture.emplace_back("C", text_color);
				number_of_inputs++;
				break;

			case SDLK_d:
				player_answer.push_back('d');
				player_answer_text_texture.emplace_back("D", text_color);
				number_of_inputs++;
				break;

			case SDLK_e:
				player_answer.push_back('e');
				player_answer_text_texture.emplace_back("E", text_color);
				number_of_inputs++;
				break;

			case SDLK_f:
				player_answer.push_back('f');
				player_answer_text_texture.emplace_back("F", text_color);
				number_of_inputs++;
				break;

			case SDLK_g:
				player_answer.push_back('g');
				player_answer_text_texture.emplace_back("G", text_color);
				number_of_inputs++;
				break;

			case SDLK_h:
				player_answer.push_back('h');
				player_answer_text_texture.emplace_back("H", text_color);
				number_of_inputs++;
				break;

			case SDLK_i:
				player_answer.push_back('i');
				player_answer_text_texture.emplace_back("I", text_color);
				number_of_inputs++;
				break;

			case SDLK_j:
				player_answer.push_back('j');
				player_answer_text_texture.emplace_back("J", text_color);
				number_of_inputs++;
				break;

			case SDLK_k:
				player_answer.push_back('k');
				player_answer_text_texture.emplace_back("K", text_color);
				number_of_inputs++;
				break;

			case SDLK_l:
				player_answer.push_back('l');
				player_answer_text_texture.emplace_back("L", text_color);
				number_of_inputs++;
				break;

			case SDLK_m:
				player_answer.push_back('m');
				player_answer_text_texture.emplace_back("M", text_color);
				number_of_inputs++;
				break;

			case SDLK_n:
				player_answer.push_back('n');
				player_answer_text_texture.emplace_back("N", text_color);
				number_of_inputs++;
				break;

			case SDLK_o:
				player_answer.push_back('o');
				player_answer_text_texture.emplace_back("O", text_color);
				number_of_inputs++;
				break;

			case SDLK_p:
				player_answer.push_back('p');
				player_answer_text_texture.emplace_back("P", text_color);
				number_of_inputs++;
				break;

			case SDLK_q:
				player_answer.push_back('q');
				player_answer_text_texture.emplace_back("Q", text_color);
				number_of_inputs++;
				break;

			case SDLK_r:
				player_answer.push_back('r');
				player_answer_text_texture.emplace_back("R", text_color);
				number_of_inputs++;
				break;

			case SDLK_s:
				player_answer.push_back('s');
				player_answer_text_texture.emplace_back("S", text_color);
				number_of_inputs++;
				break;

			case SDLK_t:
				player_answer.push_back('t');
				player_answer_text_texture.emplace_back("T", text_color);
				number_of_inputs++;
				break;

			case SDLK_u:
				player_answer.push_back('u');
				player_answer_text_texture.emplace_back("U", text_color);
				number_of_inputs++;
				break;

			case SDLK_v:
				player_answer.push_back('v');
				player_answer_text_texture.emplace_back("V", text_color);
				number_of_inputs++;
				break;

			case SDLK_w:
				player_answer.push_back('w');
				player_answer_text_texture.emplace_back("W", text_color);
				number_of_inputs++;
				break;

			case SDLK_x:
				player_answer.push_back('x');
				player_answer_text_texture.emplace_back("X", text_color);
				number_of_inputs++;
				break;

			case SDLK_y:
				player_answer.push_back('y');
				player_answer_text_texture.emplace_back("Y", text_color);
				number_of_inputs++;
				break;

			case SDLK_z:
				player_answer.push_back('z');
				player_answer_text_texture.emplace_back("Z", text_color);
				number_of_inputs++;
				break;


			}
		}
	}

	else if (currentScreen == RESULT_SCREEN) {

		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
			currentScreen = QUESTION_SCREEN;

			render_next_question = true;
			number_of_inputs = 0;
			check_time = 1;
			startTime = 0;

			if (!CheckPlayerAnswer()) {
				hangman_sprite++;
				lifes_remaining--;
			}
		}
	}

	else if (currentScreen == PAUSE_SCREEN) {
		
		restartButton.handleEvent(&e);
		resumeButton.handleEvent(&e);
		exitButton.handleEvent(&e);

	}
}

bool Game::Check_number_of_inputs() {
	if (number_of_inputs < ANSWER_SIZE)
		return true;
	else
		return false;
}

void Game::LoadResultTextTexture() {

	std::stringstream resultStream;
	resultStream.str("");

	SDL_Color textColor = { 0xFF, 0xFF, 0xFF };

	if (CheckPlayerAnswer()) {
		resultStream << "Congratulation!! Your answer is right.\n\nPress Enter to continue";

		if (!ResultTextTexture.loadFromRenderedText(resultStream.str(), textColor, 500))
			std::cout << "Failed to load result texture" << std::endl;
	}
	else if (check_time < 0) {
		resultStream << "Sorry!! you ran out of time.\n\nPress Enter to continue";

		if (!ResultTextTexture.loadFromRenderedText(resultStream.str(), textColor, 500))
			std::cout << "Failed to load result texture" << std::endl;
	}
	else {
		resultStream << "Your answer is wrong. \nThe correct answer is: " << riddles.at(number_of_dashes).answer
			<< "\n\nPress Enter to continue";

		if (!ResultTextTexture.loadFromRenderedText(resultStream.str(), textColor, 500))
			std::cout << "Failed to load result texture" << std::endl;
	}

}

bool Game::CheckPlayerAnswer() {

	bool isAnswerRight = false;

	for (int i{ 0 }; i < ANSWER_SIZE; i++) {

		if (check_time < 0 || player_answer.empty()) {
			isAnswerRight = false;
			break;
		}

		else if (player_answer.at(i) == riddles.at(number_of_dashes).answer.at(i))
			isAnswerRight = true;

		else {
			isAnswerRight = false;
			break;
		}
	}

	return isAnswerRight;
}

void Game::checkTime() {
	if (startTime == 0)
		startTime = SDL_GetTicks();

	std::stringstream timeText;
	timeText.str("");

	check_time = total_time - ((SDL_GetTicks() - startTime) / 1000);
	timeText << "Time remaning : " << check_time;

	if (check_time >= 0) {
		timeTextTexture.loadFromRenderedText(timeText.str(), { 0xFF, 0xFF, 0xFF }, 500);
		timeTextTexture.renderTexture(nullptr, (GameWindow.Get_Width() / 2) + 50, (GameWindow.Get_Height() / 2) - 200);
	}
	else
		currentScreen = RESULT_SCREEN;

}

bool Game::checkGameOver() {

	if (lifes_remaining <= 0) {
		current_option = GAME_LOST_SCREEN;
		return true;
	}
	else if (already_choose_question.size() == 16) {
		current_option = GAME_WON_SCREEN;
		return true;
	}

	return false;
}

void Game::PassQuestion() {
	render_next_question = true;
	number_of_inputs = 0;
	check_time = 1;
	startTime = 0;

	no_of_passes++;

	passButton.Set_isButtonPressed(false);

	if (no_of_passes == 3) {
		passButton.load_lock_image();
		passButton.set_color(0, 0, 170, 255);
	}
}

bool Game::LoadButtons() {

	bool success = true;

	passButton.Set_BUTTON_WIDTH_AND_HEIGHT(100, 50);

	if (!passButton.load_button_media("PASS", (GameWindow.Get_Width() / 2) + 140, (GameWindow.Get_Height() / 2) - 140)) {
		std::cout << "Failed to load button media" << std::endl;
		success = false;
	}

	resumeButton.Set_BUTTON_WIDTH_AND_HEIGHT(150, 50);

	if (!resumeButton.load_button_media("RESUME", ((GameWindow.Get_Width() - resumeButton.Get_Width()) / 2) - 75, 
										(GameWindow.Get_Height() / 2) - 140))
	{
		std::cout << "Failed to load button media" << std::endl;
		success = false;
	}

	restartButton.Set_BUTTON_WIDTH_AND_HEIGHT(150, 50);

	if(!restartButton.load_button_media("RESTART", ((GameWindow.Get_Width() - restartButton.Get_Width()) / 2) - 75,
										(GameWindow.Get_Height() / 2) - 70)) 
	{
		std::cout << "Failed to load button media" << std::endl;
		success = false;
	}

	exitButton.Set_BUTTON_WIDTH_AND_HEIGHT(150, 50);

	if (!exitButton.load_button_media("EXIT", ((GameWindow.Get_Width() - exitButton.Get_Width()) / 2) - 75, 
												(GameWindow.Get_Height() / 2))) 
	{
		std::cout << "Failed to load button media" << std::endl;
		success = false;
	}

	return success;
}

