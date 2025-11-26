#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
using namespace sf;
using namespace std;




struct Bullet {
	CircleShape  shape;

	float greitisx;
	float greitisy;
};





int main() {

	RenderWindow window(VideoMode(1000, 900), "Mano zaidimas");
	window.setFramerateLimit(60);

	//statuas zaidmo
	enum zaidimostat { menu, game, galas };
	zaidimostat state = menu;

	//meniu 
	Texture texmeniu;
	texmeniu.loadFromFile("assets/meniu.png");
	Sprite meniu(texmeniu);
	meniu.setPosition(-10, -50);

	//start button

	Texture texstartbutton;
	texstartbutton.loadFromFile("assets/start.png");
	Sprite start(texstartbutton);
	start.setPosition(200, 300);

	//exit button 
	Texture exitbutton;
	exitbutton.loadFromFile("assets/exit.png");
	Sprite exitbtn(exitbutton);
	exitbtn.setPosition(300, 500);









	//fonas
	Texture texBackground;
	texBackground.loadFromFile("assets/background.png");
	Sprite background(texBackground);
	background.setPosition(0, 0);


	//sirdeles
	Texture texHeart;
	texHeart.loadFromFile("assets/heart.png");
	Sprite heart(texHeart);
	heart.setScale(0.05f, 0.05f);







	// playeris 1
	Texture texPlayer;
	texPlayer.loadFromFile("assets/player.png");
	Sprite player(texPlayer);
	player.setPosition(400, 400);
	player.setScale(0.07f, 0.07f);

	//playeris 2
	Texture texPlayer2;
	texPlayer2.loadFromFile("assets/player2.png");
	Sprite player2(texPlayer2);
	player2.setPosition(400, 300);

	//kulkos


	vector<Bullet> bullets;
	Vector2f lastDir = { 1, 0 };
	Vector2f lastDir2 = { 1,0 };


	//monstras

	Texture texmonster;
	texmonster.loadFromFile("assets/enemy.png");
	Sprite monster(texmonster);
	monster.setScale(0.1f, 0.1f);
	monster.setPosition(300, 300);

	int monsterhp = 5;
	bool monsterDead = false;
	

	Clock respawnTimer;



	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {

			if (event.type == Event::Closed)
				window.close();
		}

		//koks yra dabar fonas/statusas

		if (state == menu) {

			window.clear();
			window.draw(meniu);
			window.draw(start);
			window.draw(exitbtn);
			window.display();

			if (Mouse::isButtonPressed(Mouse::Left)) {

				Vector2i mp = Mouse::getPosition(window);

				if (start.getGlobalBounds().contains(mp.x, mp.y))
					state = game;

				if (exitbtn.getGlobalBounds().contains(mp.x, mp.y))
					window.close();
			}

			continue;

		}



		// 1 playerio judejimas
		float greitis = 5.0f;
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			player.move(-greitis, 0);
			lastDir = { -1, 0 };
		}
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			player.move(greitis, 0);
			lastDir = { 1, 0 };
		}
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			player.move(0, -greitis);
			lastDir = { 0, -1 };
		}
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			player.move(0, greitis);
			lastDir = { 0, 1 };
		}
		// ribos 1 playeris 
		Vector2f pos = player.getPosition();
		Vector2f size(player.getGlobalBounds().width, player.getGlobalBounds().height);
		if (pos.x < 0) {
			player.setPosition(0, pos.y);
		}
		if (pos.y < 0) {
			player.setPosition(pos.x, 0);
		}
		if (pos.x + size.x > 1000) {
			player.setPosition(1000 - size.x, pos.y);
		}
		if (pos.y + size.y > 900) {
			player.setPosition(pos.x, 900 - size.y);
		}

		//playerio 2 judejimas 
		float greitis2 = 5.0f;
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			player2.move(-greitis2, 0);
			lastDir2 = { -1, 0 };
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			player2.move(greitis2, 0);
			lastDir2 = { 1, 0 };
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			player2.move(0, -greitis2);
			lastDir2 = { 0, -1 };
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			player2.move(0, greitis2);
			lastDir2 = { 0, 1 };
		}

		if (monsterDead && respawnTimer.getElapsedTime().asSeconds() > 3) {

			// random vieta
			float x = rand() % 900 + 50;
			float y = rand() % 800 + 50;

			monster.setPosition(x, y);
			monsterhp = 5;
			monsterDead = false;
		}


		//monstro judejimas link artimiausio playerio
		Vector2f mpos = monster.getPosition();
		Vector2f p1 = player.getPosition();
		Vector2f p2 = player2.getPosition();

		float dist1 = sqrt(pow(mpos.x - p1.x, 2) + pow(mpos.y - p1.y, 2));
		float dist2 = sqrt(pow(mpos.x - p2.x, 2) + pow(mpos.y - p2.y, 2));

		Vector2f target;
		if (dist1 < dist2)
			target = p1;
		else
			target = p2;

		Vector2f dir = target - mpos;
		float length = sqrt(dir.x * dir.x + dir.y * dir.y);
		if (length != 0) {
			dir /= length;
		}
		float monstergreitis = 2.0f;

		monster.move(dir.x * monstergreitis, dir.y * monstergreitis);

		// ribos 2 playeris 
		Vector2f pos2 = player2.getPosition();
		Vector2f size2(player2.getGlobalBounds().width, player2.getGlobalBounds().height);
		if (pos2.x < 0) {
			player2.setPosition(0, pos2.y);
		}
		if (pos2.y < 0) {
			player2.setPosition(pos2.x, 0);
		}
		if (pos2.x + size2.x > 1000) {
			player2.setPosition(1000 - size2.x, pos2.y);
		}
		if (pos2.y + size2.y > 900) {
			player2.setPosition(pos2.x, 900 - size2.y);
		}

		//saudimas player 1 
		static bool saudimas = true;
		if (Keyboard::isKeyPressed(Keyboard::V)) {
			if (saudimas) {
				Bullet b;
				b.shape.setRadius(8);
				b.shape.setFillColor(Color::Yellow);

				b.shape.setPosition(
					player.getPosition().x + size.x / 2,
					player.getPosition().y + size.y / 2
				);

				b.greitisx = lastDir.x * 12;
				b.greitisy = lastDir.y * 12;

				bullets.push_back(b);
				saudimas = false;
			}

		}
		else 
			saudimas = true;
		
	
			// saudimas playeris2
			static bool saudimas2 = true;
			if (Keyboard::isKeyPressed(Keyboard::B)) {
				if (saudimas2) {
					Bullet b;
					b.shape.setRadius(8);
					b.shape.setFillColor(Color::Red);

					b.shape.setPosition(
						player2.getPosition().x + size2.x / 2,
						player2.getPosition().y + size2.y / 2
					);

					b.greitisx = lastDir2.x * 12;
					b.greitisy = lastDir2.y * 12;

					bullets.push_back(b);
					saudimas2 = false;
				}

			}
			else 
				saudimas2 = true;
			

			for (int i = 0; i < bullets.size(); i++) { 

				if (!monsterDead && bullets[i].shape.getGlobalBounds().intersects(monster.getGlobalBounds()))
				{
					monsterhp--;          // sumažinam hp
					bullets.erase(bullets.begin() + i);
					i--;

					// monstras mirė
					if (monsterhp <= 0)
					{
						monsterDead = true;
						monster.setPosition(-5000, -5000); // paslepiam
						respawnTimer.restart();            // pradedam laiko skaičiavimą
					}

					continue;
				}

				bullets[i].shape.move(bullets[i].greitisx, bullets[i].greitisy);

				// Jei kulka išeina iš ekrano — ištrinam
				if (bullets[i].shape.getPosition().x < 0 ||
					bullets[i].shape.getPosition().x > 1000 ||
					bullets[i].shape.getPosition().y < 0 ||
					bullets[i].shape.getPosition().y > 900)
				{
					bullets.erase(bullets.begin() + i);
					i--;
				}
			}

			



			window.clear();
			window.draw(meniu);
			window.draw(background);
			window.draw(player2);
			window.draw(player);
			window.draw(monster);

			for (auto& b : bullets) {
				window.draw(b.shape);
			}

			//for (int i = 0; i < 5; i++) {
				//heart.setPosition(20 + i * 50, 10);  // sirdeles 
			//	window.draw(heart);
			//}
			window.display();


		}
	}





