#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Menu.h"
#include <conio.h>			//getch
#include <stdio.h>
#include <stdlib.h>          // exit, _MAX_PATH
#include <Windows.h>
#include <time.h>
#include <string.h>

using namespace std;

const char KEY_UP = 72;
const char KEY_DOWN = 80;						//определение клавиши "вниз"
const char KEY_LEFT = 75;						//определение клавиши "влево"
const char KEY_RIGHT = 77;						//определение клавиши "вправо"
const char KEY_SPACE = 32;						//определение клавиши "пробел"
const char KEY_ESC = 27;						//определение клавиши "escape"
const char KEY_ENTER = 13;						//определение клавиши "enter"
const char KEY_HOME = 71;
const char KEY_END = 79;

const int SIZEX = 20;				//16
const int SIZEY = 20;				//18
const int countFigures = 7;			//количество фигур тетриса

char glass = 32;				//закрашивание поля 177	32	(нарисовать стакан)
unsigned char block = 254;
unsigned char c = 219;
unsigned char q1 = 181;
unsigned char q2 = 213;

unsigned char Horz = 205;
unsigned char Vert = 186;

int screen[SIZEX][SIZEY] = { 0 };	//размер экрана
int figure[4][4];					//фигура
int toX, toY, score, nextFigure;

int fFigure[countFigures][4][4] =	//инициализация фигур тетриса
{
	{
		{ 1, 1, 0, 0 },			//o
		{ 1, 1, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 1, 0, 0, 0 },			//i
		{ 1, 0, 0, 0 },
		{ 1, 0, 0, 0 },
		{ 1, 0, 0, 0 }
	},
	{
		{ 0, 0, 1, 0 },			//l
		{ 1, 1, 1, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 1, 1, 1, 0 },			//r
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 1, 1, 0 },			//s
		{ 1, 1, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 1, 1, 0, 0 },			//z
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 1, 1, 1, 0 },			//t
		{ 0, 1, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	}
};
bool nextFigureExist = 1;

//прототипы:
void PauseMenuDrawing();
void PauseMenuNavigation();
void MainMenuDrawing();
void MainMenuNavigation(int curItem);						
void Info();
void InfoNavigation();
void StartMenuDrawing();
void StartMenuNavigation();

//для отрисовки элементов, repead
void rep(int count, const char c)			//repeating the block for painting (+ заставка)
{
	for (int i = 0; i <= count; i++)
	{
		cout << c;
	}
}
//tech
void enter(int a)
{
	for (int i = 0; i <= a; i++)
	{
		cout << endl;
	}
}
//для шагающего зайчика
void blinkLeft()
{
	SetColor(Black, Black);
	GotoXY(53, 29);
	rep(1, c);
	
	SetColor(White, Black);
	GotoXY(58, 29);
	rep(1, c);
	Sleep(500);
}
void blinkRight()
{
	SetColor(White, Black);
	GotoXY(53, 29);
	rep(1, c);

	SetColor(Black, Black);
	GotoXY(58, 29);
	rep(1, c);
	Sleep(500);
}
void step(int n)
{
	for (int i = 0; i < n; i++)
	{
		blinkLeft();
		blinkRight();
	}
}
void rabbit()
{
	SetColor(White, Black);		//rabbit
	GotoXY(53, 23);
	rep(1, c);
	GotoXY(54, 24);
	rep(1, c);
	GotoXY(55, 25);
	rep(2, c);
	GotoXY(58, 23);
	rep(1, c);
	GotoXY(57, 24);
	rep(1, c);

	
	GotoXY(50, 26);
	rep(12, c);
	for (int i = 27; i < 29; i++)
	{
		GotoXY(53, i);
		rep(6, c);
	}
	//legs
	GotoXY(53, 29);
	rep(1, c);
	GotoXY(58, 29);
	rep(1, c);
	
	//while (true)
	//{
		//blinkLeft();
		//blinkRight();
	//}
	SetColor(LightGreen, Black);
}

//БЛОК ДЛЯ ЗАСТАВКИ
//рандом для символов падения - в заставке
char randChar()
{
	char letter = rand() % 100;
	return letter;
}
//рандом для длины столбца падения в заставке
int randK()
{
	int k = rand() % 33 + 1;
	return k;
}
//отрисовка столбцов в заставке
void DRAW(int x)
{
	int k = randK();
	for (int i = 0; i < k; i++)
	{
		GotoXY(x, i);
		char j = randChar();
		cout << j << endl;
		Sleep(1);
	}

}
//печать отрисовки столбца в заставке
void DrawTheLetter()
{
	for (int i = 0; i < 50; i++)				// количество столбцов
	{
		DRAW(rand() % 80 + 1);					//по ширине строки
	}

}
//текст в заставке
void Neo()
{
	const int MaxLineLen = 256;

	char line[MaxLineLen];
	char fileName[_MAX_PATH] = "Neo.txt";
	FILE *stream;

	// Открываем файл и проверяем на ошибки
	if ((stream = fopen(fileName, "r")) == NULL)
	{
		exit(1);
	}

	// Читаем файл пока не дойдем до его конца
	while (!feof(stream))
	{
		// Если нормально прочли строку, то выводим ее на экран.
		GotoXY(10, 10);
		if (fgets(line, MaxLineLen, stream) != NULL)

		Sleep(2000);
		system("cls");
		cout << line;
	}

	// Закрываем файл
	fclose(stream);
	Sleep(2000);
	rabbit();
	step(3);
}
//заставка с падающими столбцами
void DrawTheZastavka()
{
	char k = 95;
	char l = 124;
	
	//t
	GotoXY(10, 10);
	rep(0, q1);
	rep(6, c);
	for (int i = 11; i < 15; i++)
	{
		GotoXY(13, i);
		rep(1, c);
	}

	//h
	for (int i = 10; i < 15; i++)
	{
		GotoXY(17, i);
		rep(1, c);
	}
	
	GotoXY(18, 12);
	rep(2, c);
	GotoXY(21, 13);
	rep(0, c);
	GotoXY(21, 14);
	rep(0, c);

	GotoXY(22, 14);
	rep(1, k);

	//e
	for (int i = 10; i < 14; i++)		// |
	{
		GotoXY(23, i);
		rep(0, c);
	}
	GotoXY(24, 10);
	rep(1, c);
	rep(0, q2);
	GotoXY(24, 12);
	rep(1, c);
	GotoXY(24, 14);
	rep(1, c);

	//tetris
	//t
	GotoXY(10, 18);
	rep(0, q1);
	rep(10, c);
	rep(3, k);
	for (int i = 19; i < 24; i++)
	{
		GotoXY(15, i);
		rep(2, c);
	}
	

	//e
	GotoXY(24, 18);
	rep(3, c);
	GotoXY(28, 17);		//__
	rep(1, k);
	GotoXY(24, 21);
	rep(3, c);
	GotoXY(24, 23);
	rep(3, c);

	GotoXY(24, 19);
	rep(1, c);
	GotoXY(24, 20);
	rep(1, c);
	GotoXY(24, 22);
	rep(1, c);

	//t
	GotoXY(30, 18);
	rep(10, c);
	rep(1, k);			//_

	for (int i = 19; i < 24; i++)
	{
		GotoXY(34, i);
		rep(2, c);
	}

	//r
	for (int i = 18; i < 24; i++)			//vert
	{
		GotoXY(43, i);
		rep(1, c);
	}
	
	GotoXY(44, 18);		//кружочек
	rep(2, c);
	GotoXY(47, 19);
	rep(1, c);
	GotoXY(44, 20);
	rep(2, c);

	for (int i = 21; i < 24; i++)		//leg
	{
		GotoXY(47, i);
		rep(1, c);
	}
	GotoXY(49, 23);
	rep(1, k);

	//i
	for (int i = 20; i < 24; i++)
	{
		GotoXY(51, i);
		rep(1, c);
	}
	
	GotoXY(51, 18);
	rep(1, c);
	rep(1, k);

	GotoXY(51, 19);
	rep(0, l);			// |


	//s+
	GotoXY(56, 18);
	rep(3, c);
	GotoXY(55, 19);
	rep(1, c);
	GotoXY(60, 19);
	rep(0, c);
	rep(0, q2);

	GotoXY(56, 20);
	rep(1, c);

	GotoXY(56, 23);
	rep(3, c);
	GotoXY(59, 22);
	rep(1, c);
	GotoXY(55, 22);
	rep(0, c);
	GotoXY(58, 21);
	rep(1, c);


	Sleep(2000);
	DrawTheLetter();

	enter(5);
}
//конец игры
void DrawTheEnd()
{
	ShowCursor(false);
	DrawTheLetter();
	SetColor(LightGreen, Black);
	system("cls");
	char k = 95;
	char l = 124;

	//game
	//g
	GotoXY(8, 8);
	rep(0, q1);
	GotoXY(10, 7);
	rep(3, c);
	GotoXY(11, 8);
	rep(0, c);
	GotoXY(14, 8);
	rep(0, c);
	for (int i = 8; i < 13; i++)
	{
		GotoXY(9, i);
		rep(1, c);
	}
	GotoXY(10, 13);
	rep(3, c);
	GotoXY(13, 11);
	rep(0, c);
	GotoXY(14, 11);
	rep(0, c);
	rep(2, k);	//-
	GotoXY(14, 12);
	rep(0, c);


	//a
	//|
	for (int i = 7; i < 14; i++)
	{
		GotoXY(23, i);
		rep(1, c);
	}
	GotoXY(23, 13);
	rep(1, c); 
	rep(2, k);

	GotoXY(19, 7);
	rep(4, c); 
	for (int i = 8; i < 14; i++)
	{
		GotoXY(18, i);
		rep(1, c);
	}

	GotoXY(20, 11);
	rep(1, c);
	GotoXY(17, 11);
	rep(0, q1);

	//m
	for (int i = 7; i < 14; i++)
	{
		GotoXY(28, i);
		rep(1, c);
	}
	GotoXY(30, 8);
	rep(0, c);
	GotoXY(31, 9);
	rep(0, c);
	GotoXY(32, 10);
	rep(0, c);
	GotoXY(33, 9);
	rep(0, c);
	GotoXY(34, 8);
	rep(1, k);
	for (int i = 7; i < 14; i++)
	{
		GotoXY(35, i);
		rep(1, c);
	}
	GotoXY(37, 6);
	rep(2, k);

	GotoXY(27, 13);
	rep(0, q1);
	GotoXY(37, 7);
	rep(0, q2);
	//e
	for (int i = 7; i < 14; i++)
	{
		GotoXY(40, i);
		rep(1, c);
	}
	GotoXY(42, 7);
	rep(2, c);
	GotoXY(42, 11);
	rep(2, c);
	GotoXY(42, 13);
	rep(2, c);
	rep(0, q2);


	//over
	//o
	for (int i = 21; i < 26; i++)
	{
		GotoXY(32, i);
		rep(1, c);
	}
	GotoXY(34, 20);
	rep(2, c);

	GotoXY(31, 21);
	rep(0, q1);
	
	for (int i = 21; i < 26; i++)
	{
		GotoXY(37, i);
		rep(1, c);
	}
	GotoXY(34, 26);
	rep(2, c);
	GotoXY(39, 25);
	rep(2, k);
//v
	GotoXY(48, 19);
	rep(2, k);
	for (int i = 20; i < 26; i++)
	{
		GotoXY(41, i);
		rep(1, c);
	}
	GotoXY(43, 26);
	rep(2, c);
	for (int i = 20; i < 26; i++)
	{
		GotoXY(46, i);
		rep(1, c);
	}
	
	//e
	for (int i = 20; i < 27; i++)
	{
		GotoXY(50, i);
		rep(1, c);
	}
	GotoXY(52, 20);
	rep(2, c);
	GotoXY(52, 24);
	rep(2, c);
	GotoXY(52, 26);
	rep(2, c);
	rep(2, k);

	//r
	for (int i = 20; i < 27; i++)
	{
		GotoXY(58, i);
		rep(1, c);
	}
	
	GotoXY(60, 20);
	rep(2, c);
	for (int i = 21; i < 23; i++)
	{
		GotoXY(63, i);
		rep(0, c);
	}
	GotoXY(60, 23);
	rep(2, c);
	
	for (int i = 24; i < 27; i++)		//leg
	{
		GotoXY(63, i);
		rep(1, c);
	}
	GotoXY(65, 26);
	rep(0, q2);
	
	Sleep(2000);
	system("cls");
	MainMenuDrawing();
	MainMenuNavigation(1);
}

//блок работы с фигурами
//рандомный выбор фигур
int rnd(int max) //рандомизация
{
	max++;
	return (rand() * max / RAND_MAX);
}
//транспонирование матрицы для разворота, техническая функция
int trans(int x)							//необходима для разворота фигуры
{
	return x = ((x * (-1)) + 3);		//транспонирование матрицы
}
//падение фигур
int moveFigure(int x, int y)							//падение фигуры вниз
{
	if (x < 0)
		return FALSE;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (figure[j][i])
			{
				if ((j + x >= SIZEX) || (i + y >= SIZEY))
					return FALSE;
				if (screen[j + x][i + y])
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}
//поворот фигуры
void rotateFigure() //поворот фигуры
{
	//переворот матрицы
	int temp_figure[4][4];
	int OX = 4;
	int OY = 4;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			temp_figure[j][i] = figure[j][i];
			if (figure[j][i])
			{
				if (i < OX)
					OX = i;
				if (trans(j) < OY)
					OY = trans(j);
			}
			figure[j][i] = 0;
		}

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (temp_figure[trans(i)][j])
				figure[j - OX][i - OY] = 1;

	if (!moveFigure(toX, toY))
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				figure[j][i] = temp_figure[j][i];
}
void printNextFigure()									//напечатать следующую идущую фигуру в поле
{
	GotoXY(SIZEX + 4, 2);
	cout << "Next:";

	for (int i = 0; i < 4; i++)
	{
		GotoXY(SIZEX + 4, i + 4);
		for (int j = 0; j < 4; j++)
		{
			if (fFigure[nextFigure][j][i] == 0)			//если не часть фигуры, то закрасить пробелами, иначе закрасить фигурой
				cout << " ";
			else
				cout << block;
		}
	}
}
//создание фигуры
void createfigure() //создание фигуры
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			figure[j][i] = fFigure[nextFigure][j][i];
	toY = 0;
	toX = SIZEX / 2;

	nextFigure = rnd(countFigures - 1);
	if (nextFigureExist)
		printNextFigure();
}
//удаление линии
void deleteLine() //убить собранную линию
{
	int i, j;
	int cl;

	for (i = SIZEY - 1; i >= 0; i--)
	{
		cl = 1;
		for (j = 0, cl = 1; j < SIZEX; j++)
		{
			if (!screen[j][i])
				cl = 0;
		}

		if (cl)
		{
			GotoXY(0, i);
			for (int k = 0; k < SIZEX; k++)
				cout << "_";
			Sleep(20);

			//поставить заглушку на с фигурой и без
			if (nextFigureExist)
				score += (((i * (-1)) + SIZEY) * 10);
			else if (!nextFigureExist)
				score += (((i * (-1)) + SIZEY) * 20);
			for (int k = i; k > 0; k--)
			{
				for (j = 0; j < SIZEX; j++)
				{
					screen[j][k] = screen[j][k - 1];
				}
			}
			i++;
		}
	}
}

//блок работы с экраном
//прорисовка стакана
void printGlass()
{
	for (int i = 0; i < SIZEX + 1; i++)
	{
		for (int j = 0; j < SIZEY + 1; j++)
		{
			if (i == SIZEX)
				cout << Horz;
			else if ((j == 0 || j == SIZEY) && i <= SIZEX)
				cout << Vert;
			else
				cout << " ";
		}
		cout << endl;
	}
}
//вывод на экран
void print()											//печать
{
	//SetColor(Cyan, Black);
	
	int buff[SIZEX][SIZEY];

	for (int i = 0; i < SIZEY; i++)
		for (int j = 0; j < SIZEX; j++)
			buff[j][i] = screen[j][i];					//вывод из буфера на экран

	for (int i = 0; i < 4; i++)							//вывод фигуры
		for (int j = 0; j < 4; j++)
			if (figure[j][i])
				buff[j + toX][i + toY] = 1;				//если часть фигуры, то в буфере обозначить 1

	GotoXY(0, 0);										//перейти в точку с координатами 0, 0
														//сместила в 1;1

	for (int i = 0; i < SIZEY; i++)
	{
		for (int j = 0; j < SIZEX; j++)
		{

			if (buff[j][i] == 0)
				cout << glass;
			else
				cout << block;
		}
		cout << endl;
	}

	GotoXY(SIZEX + 5, 0); //уйти вбок
	cout << score;
}
//очистка экрана от фигур
void clearTheScreen() //очистка экрана
{
	for (int i = 0; i < SIZEY; i++)
		for (int j = 0; j < SIZEX; j++)
			screen[j][i] = 0;
}

//пауза
int choiceKey()
{
	int c;
	if (_kbhit())										//если нажата клавиша
	{
		if ((c = _getch()) == 224)
			c = _getch();								//если нажата клавиша p (пауза), то нажать любую клавишу для продолжения игры
		return c;
	}
	return FALSE;
}

//игра
void startgame() //игра
{	
	clearTheScreen();
	int c;
	time_t tm;

	system("cls");
	toX = SIZEX / 2;
	toY = 0;
	score = 0;
	
	tm = clock();
	GotoXY(0, 0);
	printGlass();
	
	nextFigure = rnd(countFigures - 1);
	createfigure();
	if (nextFigureExist)
	{
		printNextFigure();
	}

	while (TRUE)
	{
		c = choiceKey();
		//управление фигурами
		switch (c)
		{
		case KEY_UP:
			rotateFigure();
			break;
		case KEY_SPACE:
			for (; moveFigure(toX, toY + 1); toY++);
			for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			if (figure[j][i])
				screen[toX + j][toY + i] = 1;
			deleteLine();
			createfigure();
			break;
		
		case KEY_DOWN:
			/*					//случай, когда KEY_DOWN роняет фигуру
			for (; moveFigure(toX, toY + 1); toY++);
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					if (figure[j][i])
						screen[toX + j][toY + i] = 1;
			deleteLine();
			createfigure();
			*/
			if (moveFigure(toX, toY+1))
				toY++;
			break;
		case KEY_LEFT:
			if (moveFigure(toX - 1, toY))
				toX--;
			break;
		case KEY_RIGHT:
			if (moveFigure(toX + 1, toY))
				toX++;
			break;
		case 'p':
			PauseMenuDrawing();
			PauseMenuNavigation();	
			break;
		case KEY_ESC:
			return;
		}
		
		if ((clock() - tm) > 320)
		{
			tm = clock();

			if (!(moveFigure(toX, toY + 1)))
			{
				for (int i = 0; i < 4; i++)
					for (int j = 0; j < 4; j++)
						if (figure[j][i])
							screen[toX + j][toY + i] = 1;

				createfigure();
				deleteLine();
			}
			else
				toY++;
		}

		print();
		rabbit();
	
		
		SetColor(LightGreen, Black);
		for (int i = 0; i < SIZEX; i++)
		{
			if (screen[i][0])
			{
				system("cls");
				GotoXY(2, 8);
				DrawTheEnd();
				SetColor(LightGreen, Black);
				return;
			}
		}
	}
}

//технические функции
void ChoiceRealization()
{
	SetColor(LightGreen, Black);
	ShowCursor(true);
	system("cls");
}
void ReturnToTheMenu(int key)
{
	cout << "Press any key to return to the menu.\n";
	key = _getch();
	system("cls");
}

void MainMenuDrawing()
{
	SetColor(LightGreen, Black);
	system("cls");
	ShowCursor(false);
	SetColor(LightGreen, Black);

	unsigned char LeftTop = 201;
	unsigned char RightTop = 187;
	unsigned char LeftBottom = 200;
	unsigned char RightBottom = 188;

	WriteChar(25, 12, LeftTop);
	WriteChars(26, 12, Horz, 30);
	WriteChar(56, 12, RightTop);

	WriteChar(25, 13, Vert);
	WriteChars(26, 13, ' ', 30);
	WriteChar(56, 13, Vert);

	WriteChar(25, 14, Vert);
	WriteStr(26, 14, "          Start game      ");
	WriteChar(56, 14, Vert);

	WriteChar(25, 15, Vert);
	WriteStr(26, 15, "          Settings            ");
	WriteChar(56, 15, Vert);

	WriteChar(25, 16, Vert);
	WriteStr(26, 16, "          Exit            ");
	WriteChar(56, 16, Vert);

	WriteChar(25, 17, Vert);
	WriteChars(26, 17, ' ', 30);
	WriteChar(56, 17, Vert);

	WriteChar(25, 18, LeftBottom);
	WriteChars(26, 18, Horz, 30);
	WriteChar(56, 18, RightBottom);
}
// +start
// +start without figure
//+Info
//+exit
void MainMenuNavigation(int curItem)
{
	int key = 0;
	rabbit();
	const int itemCount = 4;
	//int curItem = 1;
	do
	{
		ChangeTextAttr(26, 13 + curItem, White, Black, 30);

		key = _getch();

		if (key == KEY_ENTER)
		{
			switch (curItem)
			{
			case 1:
				StartMenuDrawing();
				rabbit();
				StartMenuNavigation();
				break;
			case 2:
				ChangeTextAttr(26, 13 + curItem, LightGreen, Black, 30);
				Info();
				rabbit();
				InfoNavigation();
				break;
			case 3:
				ChoiceRealization();
				DrawTheLetter();
				return;
				break;
			}
		}

		if (key == 0 || key == 0xE0)
		{
			key = _getch();

			ChangeTextAttr(26, 13 + curItem, LightGreen, Black, 30);

			switch (key)
			{
			case KEY_HOME:		// Home
				curItem = 1;
				break;
			case KEY_UP:		// Up
				if (curItem == 1)
					curItem = itemCount;
				else
					curItem--;
				break;
			case KEY_END:		// End
				curItem = itemCount;
				break;
			case KEY_DOWN:		// Down
				if (curItem == itemCount)
					curItem = 1;
				else
					curItem++;
				break;
			}
		}
		if (key == KEY_ESC)
		{
			DrawTheLetter();
			return;
			break;
		}
	} while (true);
}

void Info()
{
	SetColor(LightGreen, Black);
	const int MaxLineLen = 256;

	char line[MaxLineLen];
	char fileName[_MAX_PATH] = "Info.txt";
	FILE *stream;
	system("cls");
	// Открываем файл и проверяем на ошибки
	if ((stream = fopen(fileName, "r")) == NULL)
	{
		exit(1);
	}
	GotoXY(0, 7);
	// Читаем файл пока не дойдем до его конца
	while (!feof(stream))
	{
		// Если нормально прочли строку, то выводим ее на экран.

		if (fgets(line, MaxLineLen, stream) != NULL)

			cout << line;
	}

	// Закрываем файл
	fclose(stream);

	for (int i = 6; i < 17; i++)
	{
		GotoXY(20, i);
		rep(0, c);
		GotoXY(50, i);
		rep(0, c);
	}

	for (int i = 20; i < 51; i++)
	{
		GotoXY(i, 6);
		rep(0, Horz);
		GotoXY(i, 17);
		rep(0, Horz);
	}

	GotoXY(26, 15);
	SetColor(White, Black);
	cout << "Return to the menu";
	SetColor(LightGreen, Black);
	GotoXY(50, 14);
	rep(0, c);
}
void InfoNavigation()
{
	int key = 0;

	const int itemCount = 2;
	int curItem = 1;
	do
	{
		ChangeTextAttr(26, 13 + curItem, White, Black, 20);
		key = _getch();
		if (key == KEY_ENTER)
		{
				MainMenuDrawing();
				MainMenuNavigation(2);
		}
		
		if (key == KEY_ESC)
		{
			MainMenuDrawing();
			MainMenuNavigation(2);
			break;
		}
	} while (true);
	
}

void StartMenuDrawing()
{
	ShowCursor(false);
	SetColor(LightGreen, Black);
	
	unsigned char LeftTop = 201;
	unsigned char RightTop = 187;
	unsigned char LeftBottom = 200;
	unsigned char RightBottom = 188;

	WriteChar(25, 12, LeftTop);
	WriteChars(26, 12, Horz, 30);
	WriteChar(56, 12, RightTop);

	WriteChar(25, 13, Vert);
	WriteChars(26, 13, ' ', 30);
	WriteChar(56, 13, Vert);

	WriteChar(25, 14, Vert);
	WriteStr(26, 14, "   Start the game             ");
	WriteChar(56, 14, Vert);

	WriteChar(25, 15, Vert);
	WriteStr(26, 15, "   Start without help-figure      ");
	WriteChar(56, 15, Vert);

	WriteChar(25, 16, Vert);
	WriteStr(26, 16, "   Exit                 ");
	WriteChar(56, 16, Vert);

	WriteChar(25, 17, Vert);
	WriteChars(26, 17, ' ', 30);
	WriteChar(56, 17, Vert);

	WriteChar(25, 18, LeftBottom);
	WriteChars(26, 18, Horz, 30);
	WriteChar(56, 18, RightBottom);
}
void StartMenuNavigation()
{
	int key = 0;
	const int itemCount = 3;
	int curItem = 1;
	
	do
	{
		ChangeTextAttr(26, 13 + curItem, White, Black, 30);
		key = _getch();
		if (key == KEY_ENTER)
		{
			switch (curItem)
			{
			//игра с показыванием следующей фигуры
			case 1:
				::nextFigureExist = 1;						//выход на глобальный уровень
				startgame();
				break;
			//игра без фигуры-помощи
			case 2:
				::nextFigureExist = 0;
				startgame();
				break;
			//exit
			case 3:
				MainMenuDrawing();
				MainMenuNavigation(1);				//положение курсора при возвращении в меню
				break;
			}
		}

		if (key == 0 || key == 0xE0)
		{
			key = _getch();
			ChangeTextAttr(26, 13 + curItem, LightGreen, Black, 30);
			switch (key)
			{
			case KEY_HOME:		// Home
				curItem = 1;
				break;
			case KEY_UP:		// Up
				if (curItem == 1)
					curItem = itemCount;
				else
					curItem--;
				break;
			case KEY_END:		// End
				curItem = itemCount;
				break;
			case KEY_DOWN:		// Down
				if (curItem == itemCount)
					curItem = 1;
				else
					curItem++;
				break;
			}
		}
		if (key == KEY_ESC)
		{
			MainMenuDrawing();
			MainMenuNavigation(1);
			break;
		}
	} while (true);
	
}

void PauseMenuDrawing()
{
	ShowCursor(false);
	SetColor(LightGreen, Black);

	unsigned char LeftTop = 201;
	unsigned char RightTop = 187;
	unsigned char LeftBottom = 200;
	unsigned char RightBottom = 188;

	WriteChar(25, 12, LeftTop);
	WriteChars(26, 12, Horz, 30);
	WriteChar(56, 12, RightTop);

	WriteChar(25, 13, Vert);
	WriteChars(26, 13, ' ', 30);
	WriteChar(56, 13, Vert);

	WriteChar(25, 14, Vert);
	WriteStr(26, 14, "       Continue the game      ");
	WriteChar(56, 14, Vert);

	WriteChar(25, 15, Vert);
	WriteStr(26, 15, "        Return to menu         ");
	WriteChar(56, 15, Vert);

	WriteChar(25, 16, Vert);
	WriteChars(26, 16, ' ', 30);
	WriteChar(56, 16, Vert);

	WriteChar(25, 17, LeftBottom);
	WriteChars(26, 17, Horz, 30);
	WriteChar(56, 17, RightBottom);
}
void PauseMenuNavigation()
{
	int key = 0;
	const char c = 32;

	const int itemCount = 2;
	int curItem = 1;
	do
	{
		ChangeTextAttr(26, 13 + curItem, White, Black, 30);
		key = _getch();
		if (key == KEY_ENTER)
		{
			switch (curItem)
			{
			case 1:
				_getch();	
				ChangeTextAttr(26, 13 + curItem, Black, Black, 30);
				for (int i = 25; i < 70; i++)
				{
					for (int j = 12; j < 20; j++)
					{
						GotoXY(i, j);
						rep(10, c);
					}
				}
				return;
				break;
			case 2:
				SetColor(Black, White);
				system("cls");
				MainMenuDrawing();
				MainMenuNavigation(1);
				break;
			}
		}
		if (key == 0 || key == 0xE0)
		{
			key = _getch();
			ChangeTextAttr(26, 13 + curItem, LightGreen, Black, 30);
			switch (key)
			{
			case KEY_HOME:		// Home
				curItem = 1;
				break;
			case KEY_UP:		// Up
				if (curItem == 1)
					curItem = itemCount;
				else
					curItem--;
				break;
			case KEY_END:		// End
				curItem = itemCount;
				break;
			case KEY_DOWN:		// Down
				if (curItem == itemCount)
					curItem = 1;
				else
					curItem++;
				break;
			}
		}
		if (key == KEY_ESC)
		{
			MainMenuDrawing();
			MainMenuNavigation(1);
			break;
		}
	} while (true);
}


void main()
{
	system("mode con cols=80 lines=34");
	ShowCursor(false);							//switch-off the cursor25

	int curItem = 1;							//положение курсора в строке выбора
	SetColor(LightGreen, Black);
	DrawTheZastavka();			
	Sleep(1000);					
	system("cls");
	Neo();						
	system("cls");

	MainMenuDrawing();

	rabbit();
	MainMenuNavigation(curItem);
}