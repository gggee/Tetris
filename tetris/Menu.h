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

// ����� ��� ������� ����: ��������, ����������,
//   ���� ������� �� ���������
#define COLOR_ACTIVE     \
          CREATE_COLOR(COLOR::blue, COLOR::white)
#define COLOR_NONACTIVE  \
          CREATE_COLOR(COLOR::green, COLOR::black)
#define COLOR_DEFAULT    \
          CREATE_COLOR(COLOR::black, COLOR::white)

// ����� �������� ����, ������ 1.1
class Menu
{
public:
	using Container = typename vector<string>;
	using Iter = typename Container::iterator;
private:
	Container  Items;    // ������ ������� ����
	int  Row, Col; // �������� ���������� ����
	int  Height, Width; // + ������� ���� ����
	int  cActive;  // ���� ��������� ������ ����: ���� ���� � ��������
	int  cNonActive; // ���� ����������� ������ ����: ���� ���� � ��������
	int  cDefault; // ���� ������� �� ���������
	int  Select;   // ������ ��������� ������ ���� ������ ���� ����
	int  StartItem; // + ������ ������� �������� � ���� ������ ����
public:
	Menu() : Row(), Col(), Height(10), Width(20),
		Select(), StartItem(),
		cActive(COLOR_ACTIVE),
		cNonActive(COLOR_NONACTIVE),
		cDefault(COLOR_DEFAULT)
	{}
	Menu(const Container& items) : Menu() {
		this->Items = items; // ����������� ������� ���� � ������
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

	// ������ ������ ���� �� �����
	int Show(int select = 0) {
		// ��������� ��������� ������ ���� ��� ��������� ������
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
		// ���� ����� ��������� ����������
		this->Select = (select >= 0 && select < getSize()) ?
			select : 0;
		CursorHide(false);
		bool DrawItems = true;
		// ������� �-��� ������ � ���������� ����
		while (true) {
			if (DrawItems)
			{
				// ���� ������ ������� ���� �� �����
				Iter it = begin(Items);
				advance(it, StartItem); // it += StartItem
				for (int i = 0; i < Height && it != end(Items); ++it, ++i)
				{
					SetColor((i == Select) ? cActive : cNonActive);
					SetPos(Row + i, Col); // �����. ������ ���� �� ������
					string str = *it;
					str.resize(Width, ' ');
					cout << str; // ����� ������ ������ ���� �� �����
				}
				// ��������������� ���� ������� �� ���������
				SetColor(cDefault);
				DrawItems = false;
			}
			// ��������� ������� ������
			if (_kbhit())
			{
				int key = _getch();
				if (key == _KEY::CURSOR1 || key == CURSOR2) {
					// ���� ���� ������ ������ ���������� ��������
					//  �������� ��� ������� �������
					key = _getch();
				}
				// ����� ������ ����������
				while (_kbhit()) { _getch(); }
				// ����������� �� ����� ����������
				switch (key) {
					// �����
				case _KEY::UP: case _KEY::LEFT: case 'w': case 'W':
					if (Select > 0) { Select--; }
					else
					{
						if (StartItem > 0) { StartItem--; }
						else { // ������������
							Select = Height - 1;
							StartItem = getSize() - Select;
						}
					}
					DrawItems = true;
					break;

					// ����
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

				case _KEY::ENTER: // ����� ������
					CursorHide(true, 20); // �������� ������ �� ������ � 20%
					return this->Select; // ���������� ������ ��������� ������ ����

				case _KEY::ESC:   // ����� �� ������
					CursorHide(true, 20);
					return -1;      // �� ������������ ������

				} // switch(key)
			} // if(_kbhit())
		} // while(true)
	} // Show(int select);

	int Show(int row, int col, int select = 0) {
		// ����������� ���������� ������ ����
		this->Row = row, this->Col = col;
		return Show(select);
	}

}; // class Menu
