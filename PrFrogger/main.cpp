#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <ctime>
#include <sstream>

#define WIDTH_MAP 15
#define HEIGHT_MAP 20
#define CENTER_X 240
#define CENTER_Y 320

using namespace sf;

int gameTime = 0;
String tileMap = "s2121s101s1010s01s0s";
String endgameString;
bool fShow = true;
bool win = false;
bool resetGame = false;
bool reset = false;
Clock timer;

class Enemy {
private:
	Sprite sprite;
	String name;
	int pxSize;
	float speed;
	int dir;
	float respTime;
	std::list<Sprite> carsOnline;
	float lastTime;


public:
	Enemy(Sprite enemySprite, int x, int y, int dir, String name) {
		this->sprite = enemySprite;
		this->sprite.setPosition(x, y);
		this->dir = dir;
		int deltResp = rand() % 1500;
		speed = 0.1;
		respTime = 1000;
		this->name = name;
		if (this->name == "Easy") {
			sprite.setTextureRect(IntRect(0, 0, 32, 32));
			pxSize = 32;
			speed = 0.2;
			respTime = 1000 + deltResp;
		}
		if (this->name == "Medium") {
			sprite.setTextureRect(IntRect(32, 0, 64, 32));
			pxSize = 64;
			speed = 0.1;
			respTime = 2400 + deltResp;
		}
		if (this->name == "Hard") {
			sprite.setTextureRect(IntRect(96, 0, 96, 32));
			pxSize = 96;
			speed = 0.05;
			respTime = 5500 + deltResp;
		}
	}

	void update(float &time, RenderWindow &windows, Sprite frog, bool &alive) {
		if (lastTime < 0) {
			carsOnline.push_back(sprite);
			lastTime = respTime;
		}
		else {
			lastTime -= time;
		}
		for (std::list<Sprite>::iterator it = carsOnline.begin(); it != carsOnline.end(); it++) {
			if ((frog.getPosition().x > (*it).getPosition().x + 3) && (frog.getPosition().x < (*it).getPosition().x + pxSize - 3) ||
				(frog.getPosition().x + 32 >(*it).getPosition().x + 3) && (frog.getPosition().x + 32 < (*it).getPosition().x + pxSize - 3)) {
				if (frog.getPosition().y == (*it).getPosition().y) {
					alive = false;
				}
			}
			(*it).move(dir*speed*time, 0);
			windows.draw(*it);
			if (((*it).getPosition().x < -96) || ((*it).getPosition().x > 480)) {
				it = carsOnline.erase(it);
			}
		}
	}



};


class Frog
{
private:
	Image image;
	Texture texture;
	Sprite sprite;
	bool isPressed[4];

public:
	Frog() {
		image.loadFromFile("images/frog.bmp");
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setPosition(224, 608);
		for (bool &f : isPressed) {
			f = false;
		}
	}

	Sprite getSprite() {
		return sprite;
	}

	void restart() {
		sprite.setPosition(224, 608);
	}

	void update(RenderWindow &window) {

		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			if (isPressed[0] == false) {
				sprite.move(0, -32);
				isPressed[0] = true;
			}
		}
		else {
			isPressed[0] = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			if (isPressed[1] == false) {
				sprite.move(0, 32);
				isPressed[1] = true;
			}	
		}
		else {
			isPressed[1] = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			if (isPressed[2] == false) {
				sprite.move(-32, 0);
				isPressed[2] = true;
			}
		}
		else {
			isPressed[2] = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			if (isPressed[3] == false) {
				sprite.move(32, 0);
				isPressed[3] = true;
			}
		}
		else {
			isPressed[3] = false;
		}

		if (sprite.getPosition().x < 0) {
			sprite.move(32, 0);
		}
		if (sprite.getPosition().y < 0) {
			sprite.move(0, 32);
		}
		if (sprite.getPosition().x > 448) {
			sprite.move(-32, 0);
		}
		if (sprite.getPosition().y > 608) {
			sprite.move(0, -32);
		}

		window.draw(sprite);

	}

};

void menu(RenderWindow & window, Font &font) {
	Texture menuTexture1, menuTexture2;
	menuTexture1.loadFromFile("images/newgame.png");
	menuTexture2.loadFromFile("images/exit.png");
	Sprite menu1(menuTexture1), menu2(menuTexture2);
	Text logo("", font, 100);
	logo.setStyle(Text::Bold);
	logo.setFillColor(Color::Red);

	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(100, CENTER_Y + 150);
	menu2.setPosition(100, CENTER_Y + 200);

	while (isMenu && window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menuNum = 0;
		window.clear(Color(129, 181, 221));

		if (IntRect(100, CENTER_Y + 150, 300, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menuNum = 1; }
		if (IntRect(100, CENTER_Y + 200, 300, 50).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Blue); menuNum = 2; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) isMenu = false;//если нажали первую кнопку, то выходим из меню 
			if (menuNum == 2) { window.close(); isMenu = false; }

		}

		logo.setPosition(CENTER_X - 150, CENTER_Y - 300);
		logo.setString("Practic");
		window.draw(logo);
		logo.setPosition(CENTER_X - 154, CENTER_Y - 210);
		logo.setString("Frogger");
		window.draw(logo);
		window.draw(menu1);
		window.draw(menu2);

		window.display();
	}
}

void updateEnemies(std::vector<Enemy> &mapEnemies, Sprite &enemySprite, RenderWindow &window) {
	mapEnemies.clear();
	float time = 100;
	bool temp = true;
	Sprite tempSprite;
	bool dirF;
	int respX;
	int dir;
	for (int i = 0; i < HEIGHT_MAP; i++) {
		dirF = rand() % 2;
		if (dirF) {
			respX = -96;
			dir = 1;
		}
		else {
			respX = 480;
			dir = -1;
		}
		if (tileMap[i] == '0') {
			mapEnemies.push_back(Enemy(enemySprite, respX, i * 32, dir, "Easy"));
		}
		if (tileMap[i] == '1') {
			mapEnemies.push_back(Enemy(enemySprite, respX, i * 32, dir, "Medium"));
		}
		if (tileMap[i] == '2') {
			mapEnemies.push_back(Enemy(enemySprite, respX, i * 32, dir, "Hard"));
		}

		for (int i = 0; i < 100; i++) {
			for (Enemy &roadEnemy : mapEnemies) {
				roadEnemy.update(time, window, tempSprite, temp);
			}
		}
	}
}

void endgame(Text &endgameText, RenderWindow &window) {
	std::ostringstream endgame_ss;
	endgameText.setFillColor(Color::Red);
	if (fShow) {
		gameTime = timer.getElapsedTime().asSeconds();
		fShow = false;
	}
	if (win) {
		endgameText.setString("Победа!");
		endgameText.setPosition(CENTER_X - 85, CENTER_Y - 130);
	}
	else {
		endgameText.setString("Поражение!");
		endgameText.setPosition(CENTER_X - 125, CENTER_Y - 130);
	}
	endgame_ss << "Время: " << gameTime << "  сек.";
	endgameText.setCharacterSize(50);
	window.draw(endgameText);
	endgameText.setString(endgame_ss.str());
	endgameText.setCharacterSize(36);
	endgameText.setPosition(CENTER_X - 105, CENTER_Y);
	window.draw(endgameText);

	resetGame = false;

	if (IntRect(CENTER_X - 60, CENTER_Y + 85, 120, 45).contains(Mouse::getPosition(window))) { 
		endgameText.setFillColor(Color::Blue); 
		resetGame = true;
	}

	endgameText.setString("Заново");
	endgameText.setCharacterSize(40);
	endgameText.setPosition(CENTER_X - 60, CENTER_Y + 85);
	window.draw(endgameText);

	if (Mouse::isButtonPressed(Mouse::Left))
	{
		if (resetGame) {
			reset = true;
		}
	}
}

int main()
{
	RenderWindow window(VideoMode(480, 640), "Practic Frogger", Style::Close);

	Font font;
	font.loadFromFile("CyrilicOld.ttf");

	menu(window, font);

	Text endgameText("", font, 50);
	endgameText.setStyle(Text::Bold);
	endgameText.setFillColor(Color::Red);

	Image mapImage;
	mapImage.loadFromFile("images/Map.bmp");
	Texture mapTexture;
	mapTexture.loadFromImage(mapImage);
	Sprite sMap;
	sMap.setTexture(mapTexture);

	Image enemyImage;
	enemyImage.loadFromFile("images/Enemies.bmp");
	Texture enemyTexture;
	enemyTexture.loadFromImage(enemyImage);
	Sprite enemySprite;
	enemySprite.setTexture(enemyTexture);

	std::vector<Enemy> mapEnemies;
	
	bool alive = true;

	srand(time(0));

	Clock clock;
	float time = 100;
	Frog frog;
	int gameTime = 0;

	updateEnemies(mapEnemies, enemySprite, window);
	timer.restart();
	while (window.isOpen())
	{
		std::cout << timer.getElapsedTime().asSeconds() << std::endl;
		time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		window.clear();
		int h = 0;
		for (char tile : tileMap) {
			if (tile == 's') {
				sMap.setTextureRect(IntRect(0, 0, 32, 32));
			}
			if (isdigit(tile)) {
				sMap.setTextureRect(IntRect(32, 0, 32, 32));
			}

			for (int i = 0; i < WIDTH_MAP; i++) {
				sMap.setPosition(i * 32, h * 32);
				window.draw(sMap);
			}
			h++;
		}
		for (Enemy &roadEnemy : mapEnemies) {
			roadEnemy.update(time, window, frog.getSprite(), alive);
		}
		if (alive == true) {
			if (win == false) {
				frog.update(window);
			}
		}
		else {
			endgame(endgameText, window);
		}
		if (frog.getSprite().getPosition().y == 0) {
			win = true;
		}
		if (win) {
			endgame(endgameText, window);
			window.draw(frog.getSprite());
		}
		if (reset) {
			updateEnemies(mapEnemies, enemySprite, window);
			timer.restart();
			alive = true;
			fShow = true;
			win = false;
			resetGame = false;
			gameTime = 0;
			frog.restart();
			reset = false;
		}
		window.display();
	}

	return 0;
}