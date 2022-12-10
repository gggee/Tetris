#pragma once
#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;
#define RND(min, max)  (rand()%((max)-(min)+1)+(min))
#define SIZECUBE 34
using Type = typename int; //рабочий тип для программы
const int L = 20; //высота
const int W = 15; //ширина


class Game_Tetris //класс для игры
{
private:
	Type** Mas;
	Type Cube;
	Type Length;//длина
	Type Width;//ширина

public:
	//Game_Tetris() {}
	Game_Tetris(): Length(L), Width(W), Cube (SIZECUBE)
	{
		this->Mas = new Type * [L];
		for (Type i = 0; i < L; ++i)
		{
			this->Mas[i] = new Type[W];
		}
	}
	//Game_Tetris(Type** mas, Type l, Type w, Type cube) : Mas(mas), Length(l), Width(w), Cube (cube) {}
	Game_Tetris(Type length, Type width, Type cube)
	{
		setL_W(length, width);
		setCube(cube);
	}
	Game_Tetris(Type l, Type w)
	{
		setL_W(l,w);
	}
	~Game_Tetris() { Delete(); }; //деструктор класса

	//getter
	Type getLength() { return Length; }
	Type getWidth() { return Width; }
	Type getCube() { return Cube; }
	//setter
	void setCube(Type cube)
	{
		this->Cube = cube;
	}
	void setL_W(Type length, Type width)
	{
		Delete();
		this->Mas = new Type * [Length];
		for (Type i = 0; i < Length; ++i) //Чтобы создать массив с данной высотой
		{
			this->Mas[i] = new Type[Width];
		}
		this->Width = width;
		this->Length = length;
	}

	void Mas_null()
	{
		for (Type i = 0; i < this->getLength(); i++) 
		{
			for (Type j = 0; j < this->getWidth(); j++) 
			{ 
				this->Mas[i][j] = 0; 
			} 
		}
	}

	//удалить массив
	void Delete()
	{
		if (this->Mas == nullptr)
		{
			return;
		}
		for (Type i = 0; i < this->getLength(); i++)
		{
			delete[] this->Mas[i];
		}
		delete[] this->Mas;
	}

	//вставка эл.
	void Insert(Type l, Type w, Type elem)
	{
		Mas[l][w] = elem;
	}

	void Copy(Type elem1, Type elem2, Type elem_1, Type elem_2) 
	{
		if (CheckMas(elem1, elem2) == false || CheckMas(elem_1, elem_2) == false)
		{ 
			return;
		}
		this->Mas[elem1][elem2] = this->Mas[elem_1][elem_2];
	}

	//наличие элемента в массиве
	bool CheckMas(Type elem1, Type elem2)
	{
		//if (this->CheckMas(elem1, elem2) == -1)
		if (this->Border(elem1, elem2)== -1)
		{
			return false;
		}
		return true;
	}

	//выход за границу Mas
	Type Border(Type elem1, Type elem2)
	{
		if (elem1 > this->getLength() || elem2 > this->getWidth() || elem1 < 0 || elem2 < 0)
		{
			return -1;
		}
		return this->Mas[elem1][elem2];
	}

	//если не пустой = проигрыш
	bool GameEnd()
	{
		for (size_t i = 0; i < this->getWidth(); i++)
		{
			if (Border(0, i) != 0 || Border(1, i) != 0)
			{
				return false;
			}
		}
		return true;
	}

	//вывод
	void Print(sf::Sprite& tiles, sf::RenderWindow& window)
		// Sprite (sfml) представление текстуры
		//RenderWindow (sfml) для 2D-рисования
	{
		for (Type i = 0; i < this->getLength(); i++)
		{
			for (Type j = 0; j < this->getWidth(); j++)
			{
				if (Border(i, j) !=0) 
				{
					tiles.setTextureRect(sf::IntRect(this->Border(i, j) * this->getCube(), 0,
						this->getCube(), this->getCube())); //расположенный в точке (Border) размером getCube
					tiles.setPosition(j * this->getCube(), i * this->getCube()); //setPosition(sfml) для позиции
					window.draw(tiles); //draw (sfml) для рисование
				}
			}
		}
	}
};

Type Shapes[7][4] =
{
	1,3,5,7,
	2,4,5,7,
	3,5,4,6,
	3,5,4,7,
	2,3,5,7,
	3,5,7,6,
	2,3,4,5,
};

//для фигур
class Shapes_:virtual public Game_Tetris 
{
private:
	Type H; //horizontal-горизонтальная ось
	Type V; //vertical-вертикальная ось
public:
	Shapes_ (): H(), V() {}
	//Shapes_(Type v, Type h): H(h), V(v) {}
	~Shapes_() = default;

	//getter
	Type getH() { return H; }
	Type getV() { return V; }

	//setter
	void setH(Type h) { this->H = h; }
	void setV(Type v) { this->V = v; }
	void setH_V(Type h, Type v) 
	{
		this->H = h;
		this->V = v;
	}
	
	void Shapes_Print(sf::Sprite& tiles, sf::RenderWindow& window, Type colorNum, Game_Tetris& Pole);

} mas1[4], mas2[4];

void Shapes_::Shapes_Print(sf::Sprite& tiles, sf::RenderWindow& window, Type color, Game_Tetris& Pole) 
{
	for (Type i = 0; i < 4; i++) 
	{ 
		tiles.setTextureRect(sf::IntRect(color * Pole.getCube(), 0, Pole.getCube(), Pole.getCube())); 
		tiles.setPosition(mas1[i].getH() * Pole.getCube(), mas1[i].getV() * Pole.getCube()); 
		window.draw(tiles); 
	} 
}

//движение
class Move : virtual public Game_Tetris
{
private:
	Type Crd;// сокрщ. coordinates - координаты нашей фигуры
	Type Color;
	double Speed; //скорость
	bool Rotation; //вращение
	double time;
	bool add;
public:
	Move() : Crd(), Color(), Speed(), Rotation(false),add(true), time() {}
	Move(Type crd, Type color, double speed, bool rtn,double time, bool add)
	{
		setCrd(crd); setColor(color); setSpeed(speed); setRtn(rtn); setTime(time); setAdd(add);
	}/*Crd(crd), Color(color), Speed(speed), Rotation(rtn),time(time), add(add) */
	//getter
	Type getCrd() { return Crd; }
	Type getColor() { return Color; }
	double getSpeed() { return Speed; }
	double getTime() { return time; }
	bool getRtn() { return Rotation; }
	bool getAdd() { return add; }
	//setter
	void setCrd(Type crd)
	{
		this->Crd = crd;
	}
	void setColor(Type color)
	{
		this->Color = color;
	}
	void setSpeed(double speed)
	{
		this->Speed = speed;
	}
	void setTime(double time)
	{
		this->time = time;
	}
	void setRtn(bool rtn)
	{
		this->Rotation = rtn;
	}
	void setAdd(bool Add)
	{
		this->add = Add;
	}
	void setAll(Type crd, Type color, bool rtn, bool add, double time,  double speed)
	{
		setCrd(crd); setColor(color); setSpeed(speed); setRtn(rtn); setTime(time); setAdd(add);
	}
};

