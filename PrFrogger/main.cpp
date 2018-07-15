#include <SFML/Graphics.hpp>
#include <iostream>

#define WIDTH_MAP 15

using namespace sf;

String tileMap = "s1111s111s1111s11s1s";

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

	void update() {

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

	}

};

int main()
{
	RenderWindow window(VideoMode(480, 640), "Practic Frogger");

	Image mapImage;
	mapImage.loadFromFile("images/Map.bmp");
	Texture mapTexture;
	mapTexture.loadFromImage(mapImage);
	Sprite sMap;
	sMap.setTexture(mapTexture);
	
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

	
		
		std::cout << frog.getSprite().getPosition().x << " " << frog.getSprite().getPosition().y << std::endl;
		window.clear();
		int h = 0;
		for (char tile : tileMap) {
			if (tile == 's') {
				sMap.setTextureRect(IntRect(0, 0, 32, 32));
			}
			if (tile == '1') {
				sMap.setTextureRect(IntRect(32, 0, 32, 32));
			}

			for (int i = 0; i < WIDTH_MAP; i++) {
				sMap.setPosition(i * 32, h * 32);
				window.draw(sMap);
			}
			h++;
		}
		frog.update();
		window.draw(frog.getSprite());
		window.display();
	}

	return 0;
}