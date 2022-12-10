#include "Game.h"
#define RND(min, max)  (rand()%((max)-(min)+1)+(min))

void menu(RenderWindow& window)
{
	//загружаем для меню изображения
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("images/play.png"); //начало
	menuTexture2.loadFromFile("images/about3.png");//setting
	menuTexture3.loadFromFile("images/exit3.png");//exit
	aboutTexture.loadFromFile("images/bluemoon.png"); //фон для setting
	menuBackground.loadFromFile("images/BackMenu2.png"); //фон для menu
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
	bool isMenu = 1; 
	int menuNum = 0; //какой выбрали
	menu1.setPosition(50, 30); //позиции
	menu2.setPosition(50, 100);
	menu3.setPosition(50, 170);
	menuBg.setPosition(0, 0);

	while (isMenu) 
	{
		menu1.setColor(Color::White); //задаеи цвет
		menu2.setColor(Color::White);
		menu3.setColor(Color::White);
		menuNum = 0;
		window.clear(Color(129, 181, 221));

		if (IntRect(100, 30, 300, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Green); menuNum = 1; }
		if (IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Green); menuNum = 2; }
		if (IntRect(100, 150, 300, 50).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Green); menuNum = 3; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) isMenu = false;//если нажали первую кнопку, то выходим из меню 
			if (menuNum == 2) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
			if (menuNum == 3) { window.close(); isMenu = false; }

		}

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);
		
		window.display();
	}
}

bool Pole_(Game_Tetris& Pole)
{
	for (Type i = 0; i < 4; i++)
	{
		if (mas1[i].getH() < 0 || mas1[i].getH() >= Pole.getWidth() || mas1[i].getV() >= Pole.getLength())
		{
			return false;
		}
		else if (Pole.Border(mas1[i].getV(), mas1[i].getH()) != 0)
		{
			return false;
		}
	}
	return true;
}

void forCopy(Type c = 0)
{
	if (c == 1)
	{
		for (Type i = 0; i < 4; i++)
		{
			mas1[i] = mas2[i];
			mas1[i].setV(mas1[i].getV() + 1);
		}
	}
	else
	{
		for (Type i = 0; i < 4; i++)
		{
			mas1[i] = mas2[i];
		}
	}
}


void StartGame()
{
	sf::Texture Obj;
	Move Move_c;
	sf::Sprite Cube(Obj);
	Game_Tetris Pole;
	sf::Clock clock;
	float timer = 0;

	Move_c.setAll(0, 1, false, true, 0, 0.5);
	Pole.setL_W(L, W);
	Pole.setCube(SIZECUBE);
	Pole.Mas_null();
	//srand(time(0));
	
	RenderWindow window(sf::VideoMode(Pole.getWidth() * Pole.getCube(), Pole.getLength() * Pole.getCube()),
		"Game: Tetris");
	menu(window); //функция меню
	Texture t1, t2, t3;
	
	bool a;
	a = t1.loadFromFile("./images/tiles.png");
	a = t2.loadFromFile("./images/backg.png");
	a = t3.loadFromFile("./images/Over.png");
	
	Sprite s(t1), background(t2), frame(t3);

	while (window.isOpen()) //открыть
	{
		if (Pole.GameEnd() == false)
		{
			while (window.isOpen())
			{
				sf::Event newEvent;
				while (window.pollEvent(newEvent))
				{
					if (newEvent.type == sf::Event::Closed)
					{
						window.close();
					}
					else if (newEvent.type == sf::Event::KeyPressed)
					{
						if (newEvent.key.code == sf::Keyboard::Escape || newEvent.key.code == sf::Keyboard::Tab || newEvent.key.code == sf::Keyboard::Enter)
						{
							window.close();
						}
					}
				}
				window.clear(Color::Blue);
				window.draw(frame);
				window.display();
			}
		}

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		Move_c.setTime(Move_c.getTime() + time);
		Event event;
		//движенние игры
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				window.close();
			}
			else if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Up)
				{
					Move_c.setRtn(true);
				}
				else if (event.key.code == Keyboard::Right)
				{

					Move_c.setCrd(1);
				}
				else if (event.key.code == Keyboard::Left)
				{
					Move_c.setCrd(-1);
				}
				else if (event.key.code == Keyboard::Escape)
				{
					window.close();
				}
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			Move_c.setSpeed(0.05);
		}

		for (Type i = 0; i < 4; i++)
		{
			mas2[i] = mas1[i];
			mas1[i].setH(mas1[i].getH() + Move_c.getCrd());
		}


		if (!Pole_(Pole))
		{
			forCopy();
		}

		if (Move_c.getRtn() != 0)
		{
			for (Type i = 0; i < 4; i++)
			{
				mas1[i].setH_V(mas1[1].getH() - (mas1[i].getV() - mas1[1].getV()), mas1[1].getV() +
					(mas1[i].getH() - mas1[1].getH()));
			}
			if (!Pole_(Pole))
			{
				forCopy();
			}
		}

		Type temp = RND(0, 6);
		if (Move_c.getTime() > Move_c.getSpeed())
		{
			forCopy(1);

			if (!Pole_(Pole))
			{
				for (Type i = 0; i < 4; i++)
				{
					Pole.Insert(mas2[i].getV(), mas2[i].getH(), Move_c.getColor());
				}
				Move_c.setColor(1 + temp);
				for (Type i = 0; i < 4; i++)
				{
					mas1[i].setH_V(Shapes[temp][i] % 2, Shapes[temp][i] / 2);
				}
			}
			Move_c.setTime(0);
		}

		if (Move_c.getAdd() != 0)
		{
			if (mas1[0].getH() == 0)
			{
				for (Type i = 0; i < 4; i++)
				{
					mas1[i].setH_V(Shapes[temp][i] % 2, Shapes[temp][i] / 2);
				}
			}
			Move_c.setAdd(false);
		}


		Type Temp = Pole.getLength() - 1;
		for (Type i = Pole.getLength() - 1; i > 0; i--)
		{
			Type count = 0;
			for (Type j = 0; j < Pole.getWidth(); j++)
			{
				if (Pole.Border(i, j) != 0)
				{
					count++;
				}
				Pole.Copy(Temp, j, i, j);
			}

			if (count < Pole.getWidth())
			{
				Temp--;
			}
		}

		window.clear(Color::White);
		window.clear();
		window.draw(background);
		Move_c.setSpeed(0.3);
		Move_c.setCrd(0);
		Move_c.setRtn(false);

		Pole.Print(Cube, window);

		for (Type i = 0; i < 4; i++)
		{
			Cube.setTextureRect(sf::IntRect(Move_c.getColor() * Pole.getCube(), 0, Pole.getCube(), Pole.getCube()));
			Cube.setPosition(mas1[i].getH() * Pole.getCube(), mas1[i].getV() * Pole.getCube());
			window.draw(Cube);
		}
		
		
		/*for (int i = 0; i < 4; i++)
		{
			s.setTextureRect(IntRect(Move_c.getColor() * 18, 0, 18, 18));
			s.setPosition(mas1[i].getH() * 18, mas2[i].getV() * 20);
			s.move(28, 31);
			window.draw(s);
		}*/

		window.display();
	}
	return;
}