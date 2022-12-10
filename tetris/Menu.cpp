#include "Menu.h"
#include "Game.h"

Menu::Container menuMain =
{
  "Start",
  "Settings",
  "About",
  "Exit",
};

int main()
{
	Menu MainMenu(menuMain, 4, 10, 4, 5);  // главное меню
	int select = 0;
	int isExit = false;
	CLS;
	while (!isExit)
	{
		select = MainMenu.Show(select);
		switch (select)
		{
		case 0:
			SetPos(0, 5);
			cout << "Select 1 menu item - Start game";
			break;
		case 1:
			SetPos(0, 5);
			cout << "Select 2 menu item - Continue";
			break;
		case 2:
			break;
		case 3:
			break;
		case -1:
			SetPos(0, 5);
			cout << "Good bye...";
			isExit = true;
			PAUSE;
			break;
		}

	}

}
