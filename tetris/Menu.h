/*
  Menu.h
*/
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include "Win10.h"  // SetPos(), SetColor()
using namespace std;

// цвета для пунктов меню: активный, неактивный,
//   цвет консоли по умолчанию
#define COLOR_ACTIVE     \
          CREATE_COLOR(COLOR::blue, COLOR::white)
#define COLOR_NONACTIVE  \
          CREATE_COLOR(COLOR::green, COLOR::black)
#define COLOR_DEFAULT    \
          CREATE_COLOR(COLOR::black, COLOR::white)

// класс оконного меню, версия 1.1
class Menu
{
public:
	using Container = typename vector<string>;
	using Iter = typename Container::iterator;
private:
	Container  Items;    // массив пунктов меню
	int  Row, Col; // экранные координаты меню
	int  Height, Width; // + размеры окна меню
	int  cActive;  // цвет активного пункта меню: цвет фона и символов
	int  cNonActive; // цвет неактивного пункта меню: цвет фона и символов
	int  cDefault; // цвет консоли по умолчанию
	int  Select;   // индекс активного пункта меню внутри окна меню
	int  StartItem; // + индекс первого видимого в окне пункта меню
public:
	Menu() : Row(), Col(), Height(10), Width(20),
		Select(), StartItem(),
		cActive(COLOR_ACTIVE),
		cNonActive(COLOR_NONACTIVE),
		cDefault(COLOR_DEFAULT)
	{}
	Menu(const Container& items) : Menu() {
		this->Items = items; // копирование пунктов меню в объект
	}
	Menu(const Container& items,
		int row, int col,
		int height, int width) : Menu(items)
	{
		this->Row = row, this->Col = col;
		this->Height = height, this->Width = width;
	}
	// get/set
	int getSize()const { return Items.size(); }

	// методы вывода меню на экран
	int Show(int select = 0) {
		// установка активного пункта меню при стартовом выводе
		if (select >= 0 && select < getSize()) {
			if (select < Height) {
				Select = select;
				StartItem = 0;
			}
			else {
				Select = 0;
				StartItem = select;
			}
		}
		else {
			Select = StartItem = 0;
		}
		// тоже самое тернарным оператором
		this->Select = (select >= 0 && select < getSize()) ?
			select : 0;
		CursorHide(false);
		bool DrawItems = true;
		// главная ф-ция вывода и управления меню
		while (true) {
			if (DrawItems)
			{
				// цикл вывода пунктов меню на экран
				Iter it = begin(Items);
				advance(it, StartItem); // it += StartItem
				for (int i = 0; i < Height && it != end(Items); ++it, ++i)
				{
					SetColor((i == Select) ? cActive : cNonActive);
					SetPos(Row + i, Col); // позиц. пункта меню на экране
					string str = *it;
					str.resize(Width, ' ');
					cout << str; // вывод строки пункта меню на экран
				}
				// восстанавливаем цвет консоли по умолчанию
				SetColor(cDefault);
				DrawItems = false;
			}
			// обработка нажатий клавиш
			if (_kbhit())
			{
				int key = _getch();
				if (key == _KEY::CURSOR1 || key == CURSOR2) {
					// если была нажата кнопка управления курсором
					//  забираем код команды курсора
					key = _getch();
				}
				// сброс буфера клавиатуры
				while (_kbhit()) { _getch(); }
				// расшифровка по кодам клавиатуры
				switch (key) {
					// вверх
				case _KEY::UP: case _KEY::LEFT: case 'w': case 'W':
					if (Select > 0) { Select--; }
					else
					{
						if (StartItem > 0) { StartItem--; }
						else { // зацикливание
							Select = Height - 1;
							StartItem = getSize() - Select;
						}
					}
					DrawItems = true;
					break;

					// вниз
				case _KEY::DOWN: case _KEY::RIGHT: case 's': case 'S':
					if (Select < Height - 1) { Select++; }
					else
					{
						if (StartItem < getSize() - Select) {
							StartItem++;
						}
						else {
							Select = 0;
							StartItem = 0;
						}
					}
					DrawItems = true;
					break;

				case _KEY::ENTER: // выбор пункта
					CursorHide(true, 20); // показать курсор на экране в 20%
					return this->Select; // возвращаем индекс активного пункта меню

				case _KEY::ESC:   // отказ от выбора
					CursorHide(true, 20);
					return -1;      // не существующий индекс

				} // switch(key)
			} // if(_kbhit())
		} // while(true)
	} // Show(int select);

	int Show(int row, int col, int select = 0) {
		// настраиваем координаты вывода меню
		this->Row = row, this->Col = col;
		return Show(select);
	}

}; // class Menu
