#include "Game.h";
using namespace std;

int **board;
HANDLE outHandle;
int points;
int BOARD_SIZE;

bool checkGameEnd() {
	int usedCells = 0;

	for (int j = 0; j < BOARD_SIZE; j++) {
		for (int i = 0; i < BOARD_SIZE; i++) {
			if (board[j][i] > 0) {
				usedCells++;
			}
		}
	}

	return usedCells == (BOARD_SIZE * BOARD_SIZE);
}

void initGame(int boardSize) {
	BOARD_SIZE = boardSize;
	points = 0;
	outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	board = new int*[BOARD_SIZE];


	for (int i = 0; i < BOARD_SIZE; i++) {
		board[i] = new int[BOARD_SIZE];

		for (int j = 0; j < BOARD_SIZE; j++) {
			board[i][j] = 0;
		}
	}

	board[1][1] = 2;
	board[1][2] = 2;
	drawBoard();

	int dir;
	bool hasMoved;
	while (1) {
		dir = _getch();

		if (dir == KEY_UP || dir == KEY_DOWN || dir == KEY_RIGHT || dir == KEY_LEFT) {
			hasMoved = swipe(dir);

			if (hasMoved) {
				appendElement();
				drawBoard();

				if (checkGameEnd()) {
					goToXY(BOARD_SIZE * CELL_W + 3, 11);
					system("pause");
					break;
				}
			}
		}
	}

	showMenu();
}

void goToXY(int x, int y) {
	COORD coords;
	coords.X = x;
	coords.Y = y;
	SetConsoleCursorPosition(outHandle, coords);
}

void drawCell(int x, int y) {

	switch (board[y][x]) {
		case 2: SetConsoleTextAttribute(outHandle, FOREGROUND_BLUE); break;
		case 4: SetConsoleTextAttribute(outHandle, FOREGROUND_RED); break;
		case 8: SetConsoleTextAttribute(outHandle, FOREGROUND_GREEN); break;
		case 16: SetConsoleTextAttribute(outHandle, FOREGROUND_GREEN | FOREGROUND_BLUE); break;
		case 32: SetConsoleTextAttribute(outHandle, FOREGROUND_RED | FOREGROUND_BLUE); break;
		case 64: SetConsoleTextAttribute(outHandle, FOREGROUND_GREEN | FOREGROUND_RED); break;
		case 128: SetConsoleTextAttribute(outHandle, FOREGROUND_BLUE | FOREGROUND_INTENSITY); break;
		case 256: SetConsoleTextAttribute(outHandle, FOREGROUND_RED) | FOREGROUND_INTENSITY; break;
		case 512: SetConsoleTextAttribute(outHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY); break;
		case 1024: SetConsoleTextAttribute(outHandle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); break;
		case 2048: SetConsoleTextAttribute(outHandle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); break;
		case 4096: SetConsoleTextAttribute(outHandle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY); break;
		default: SetConsoleTextAttribute(outHandle, FOREGROUND_INTENSITY);
	}

	for (int yi = 0; yi < CELL_H; yi++) {
		for (int xi = 0; xi < CELL_W; xi++) {
			goToXY(x * CELL_W + xi, y * CELL_H + yi);
			
			if (yi == 0 && xi == 0) { // top left
				cout << (char)201;
			}
			else if (yi == 0 && xi == CELL_W - 1) { // top right
				cout << (char)187;
			}
			else if (yi == CELL_H - 1 && xi == 0) { // bottom left
				cout << (char)200;
			}
			else if (yi == CELL_H - 1 && xi == CELL_W - 1) { // bottom right
				cout << (char)188;
			}
			else if (yi == 0 || yi == CELL_H - 1) { // horizontal line
				cout << (char)205;
			}
			else if (xi == 0 || xi == CELL_W - 1) { // vertical line
				cout << (char)186;
			}
			else { // inside
				if (yi == CELL_H / 2 && xi == 1) {
					if (board[y][x] >= 1000) {
						cout.width((CELL_W - 2) / 2 + 2);
					}
					else if (board[y][x] >= 100) {
						cout.width((CELL_W - 2) / 2 + 2);
					}
					else if (board[y][x] >= 10) {
						cout.width((CELL_W - 2) / 2 + 1);
					}
					else {
						cout.width((CELL_W - 2) / 2 + 1);
					}
					
					cout << board[y][x];
				}
			}

		}
	}
}

void drawBoard() {
	system("cls");
	for (int y = 0; y < BOARD_SIZE; y++) {
		for (int x = 0; x < BOARD_SIZE; x++) {
			drawCell(x, y);
		}
	}

	goToXY(BOARD_SIZE * CELL_W + 3, 10);
	cout << "Points: " << points;
}

bool order(int *items) {
	int v = 0; // liczba pol roznych od zera
	for (int i = 0; i < BOARD_SIZE; i++) {
		if (items[i] > 0) {
			v++;
		}
	}

	int *ordered = new int[v];
	int oi = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		if (items[i] > 0) {
			ordered[oi++] = items[i];
		}
	}
	
	for (int i = 0; i < v - 1; i++) {
		if (ordered[i] == ordered[i + 1]) {
			ordered[i] *= 2;
			points += ordered[i];
			ordered[i + 1] = 0;
		}
	}

	int w = 0; // liczba pustych pol w drugiej iteracji
	for (int i = 0; i < v; i++) {
		if (ordered[i] > 0) {
			w++;
		}
	}

	int *unique = new int[w];
	int oj = 0;
	for (int i = 0; i < v; i++) {
		if (ordered[i] > 0) {
			unique[oj++] = ordered[i];
		}
	}

	delete ordered;
	bool hasMoved = false;

	for (int i = 0; i < BOARD_SIZE; i++) {
		if (i < w) {
			if (items[i] != unique[i]) {
				hasMoved = true;
			}

			items[i] = unique[i];
		}
		else {
			items[i] = 0;
		}
	}

	return hasMoved;
}

bool swipe(int dir){
	bool hasMoved = false;

	if (dir == KEY_LEFT) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			if (order(board[row])) {
				hasMoved = true;
			}
		}
	}
	if (dir == KEY_RIGHT) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			int *tmp = new int[BOARD_SIZE];
			int i = 0;
			for (int xi = BOARD_SIZE - 1; xi >= 0; xi--) {
				tmp[i++] = board[row][xi];
			}

			if (order(tmp)) {
				hasMoved = true;
			}

			i = 0;
			for (int xi = BOARD_SIZE - 1; xi >= 0; xi--) {
				board[row][xi] = tmp[i++];
			}

			delete tmp;
		}
	}
	if (dir == KEY_DOWN) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			int *tmp = new int[BOARD_SIZE];
			int i = 0;
			for (int yi = BOARD_SIZE - 1; yi >= 0; yi--) {
				tmp[i++] = board[yi][col];
			}

			if (order(tmp)) {
				hasMoved = true;
			}

			i = 0;
			for (int yi = BOARD_SIZE - 1; yi >= 0; yi--) {
				board[yi][col] = tmp[i++];
			}

			delete tmp;
		}
	}
	if (dir == KEY_UP) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			int *tmp = new int[BOARD_SIZE];

			for (int yi = 0; yi < BOARD_SIZE; yi++) {
				tmp[yi] = board[yi][col];
			}

			if (order(tmp)) {
				hasMoved = true;
			}

			for (int yi = 0; yi < BOARD_SIZE; yi++) {
				board[yi][col] = tmp[yi
];
			}

			delete tmp;
		}
	}

	return hasMoved;
}

void appendElement() {
	int x, y;

	srand(time(NULL));

	x = rand() % BOARD_SIZE;
	y = rand() % BOARD_SIZE;

	while (board[y][x] > 0) {
		x = rand() % BOARD_SIZE;
		y = rand() % BOARD_SIZE;
	}

	board[y][x] = 2;
}

void showMenu() {
	system("cls");
	system("color 0f");
	cout << "######  ######  #    #  ######" << endl;
	cout << "     #  #    #  #    #  #    #" << endl;
	cout << "######  #    #  ######  ######" << endl;
	cout << "#       #    #       #  #    #" << endl;
	cout << "######  ######       #  ######" << endl << endl;
	cout << "Podaj rozmiar planszy [3, 4, 5]: ";

	char ch = _getch();

	while (ch != '3' && ch != '4' && ch != '5') {
		ch = _getch();
	}

	if (ch == '4') {
		initGame(4);
	}

	if (ch == '3') {
		initGame(3);
	}

	if (ch == '5') {
		initGame(5);
	}
}

int main() {
	showMenu();
}