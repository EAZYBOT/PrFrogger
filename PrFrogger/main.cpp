#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <list>
#include <iterator>

#define WIDTH_MAP 15
#define HEIGHT_MAP 20

using namespace sf;

String tileMap = "s2121s101s1010s01s0s";

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
		speed = 0.1;
		respTime = 1000;
		this->name = name;
		if (this->name == "Easy") {
			sprite.setTextureRect(IntRect(0, 0, 32, 32));
			pxSize = 32;
			speed = 0.2;
			respTime = 1200;
		}
		if (this->name == "Medium") {
			sprite.setTextureRect(IntRect(32, 0, 64, 32));
			pxSize = 64;
			speed = 0.1;
			respTime = 1800;
		}
		if (this->name == "Hard") {
			sprite.setTextureRect(IntRect(96, 0, 96, 32));
			pxSize = 96;
			speed = 0.05;
			respTime = 6000;
		}
	}

	void update(float &time, RenderWindow &windows) {
		if (lastTime < 0) {
			carsOnline.push_back(sprite);
			lastTime = respTime;
		}
		else {
			lastTime -= time;
		}
		for (std::list<Sprite>::iterator it = carsOnline.begin(); it != carsOnline.end(); it++) {
			(*it).move(dir*speed*time, 0);
			windows.draw(*it);
			if (((*it).getPosition().x < -pxSize) || ((*it).getPosition().x > 480)) {
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
	bool alive;
	bool isPressed[4];

public:
	Frog() {
		image.loadFromFile("images/frog.bmp");
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setPosition(224, 608);
		alive = true;
		for (bool f : isPressed) {
			f = false;
		}
	}

	Sprite getSprite() {
		return sprite;
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

		window.draw(sprite);

	}

};

int main()
{
	RenderWindow window(VideoMode(480, 640), "Practic Frogger", Style::Close);

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
	
	for (int i = 0; i < HEIGHT_MAP; i++) {
		if (tileMap[i] == '0') {
			mapEnemies.push_back(Enemy(enemySprite, -32, i * 32, 1, "Easy"));
		}
		if (tileMap[i] == '1') {
			mapEnemies.push_back(Enemy(enemySprite, -64, i * 32, 1, "Medium"));
		}
		if (tileMap[i] == '2') {
			mapEnemies.push_back(Enemy(enemySprite, -96, i * 32, 1, "Hard"));
		}
	}

	Clock clock;
	float time = 0;
	Frog frog;

	while (window.isOpen())
	{
		time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

	
		
		//std::cout << frog.getSprite().getPosition().x << " " << frog.getSprite().getPosition().y << std::endl;
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
			roadEnemy.update(time, window);
		}
		frog.update(window);
		window.display();
	}

	return 0;
}