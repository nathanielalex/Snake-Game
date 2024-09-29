#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include<time.h>

#define HEIGHT 15
#define WIDTH 31

char town[HEIGHT][WIDTH] = {
    "##############################",
    "#             ###            #",
    "#    #######  ###  #######   #",
    "#    #                #      #",
    "#    #   #########    #      #",
    "#    #   #       #    #      #",
    "#    #   #       #    #      #",
    "                              ",
    "#    #   #       #    #      #",
    "#    #   #       #    #      #",
    "#    #   #########    #      #",
    "#    #                #      #",
    "#    #######  ###  #######   #",
    "#             ###            #",
    "##############################",
};

char garden[HEIGHT][WIDTH] = {
    "##############################",
    "#            ####            #",
    "#  #     #          #     #  #",
    "#  #    #            #    #  #",
    "#  #    #            #    #  #",
    "#  #    #            #    #  #",
    "#  #    #            #    #  #",
    "                              ",
    "#  #    #            #    #  #",
    "#  #    #            #    #  #",
    "#  #    #            #    #  #",
    "#  #    #            #    #  #",
    "#  #     #          #     #  #",
    "#            ####            #",
    "##############################",
};

char plaza[HEIGHT][WIDTH] = {
    "##############################",
    "#      #     ###     #       #",
    "#      #     ###     #       #",
    "#      #             #       #",
    "#      #     #####   #       #",
    "#      #         #   #       #",
    "#      #         #   #       #",
    "                              ",
    "#      #         #   #       #",
    "#      #         #   #       #",
    "#      #     #####   #       #",
    "#      #             #       #",
    "#      #     ###     #       #",
    "#      #     ###     #       #",
    "##############################",
};

struct mapNode{
	char (*pMap)[WIDTH]; 
	struct mapNode* next;
	struct mapNode* prev;
}*head = NULL, *currMap = NULL;

struct mapNode* createNode(char (*pMap)[WIDTH]) {
    struct mapNode* newNode = (struct mapNode*)malloc(sizeof(struct mapNode));
    newNode->pMap = pMap;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void pushHead(char (*pMap)[WIDTH]) {
	struct mapNode* newNode = createNode(pMap);
    if (head == NULL) {
        head = newNode;
        head->next = head;
        head->prev = head;
    } else {
        newNode->next = head;
        newNode->prev = head->prev;
        head->prev->next = newNode;
        head->prev = newNode;
        head = newNode;
    }
    currMap = head;
}


void printMap(char (*pMap)[WIDTH]) {
	system("cls");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", pMap[i][j]);
        }
        printf("\n");
    }
}

//snake
struct Node{
	int x;
	int y;
	int horizontalSpeed;
	int verticalSpeed;
	char direction;
	struct Node* next;
	struct Node* prev;
}*headSnake = NULL, *tailSnake = NULL;


struct Node* createNode(int x, int y, int horizontalSpeed, int verticalSpeed, char direction) {
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->horizontalSpeed = horizontalSpeed;
	newNode->verticalSpeed = verticalSpeed;
	newNode->x = x;
	newNode->y = y;
	newNode->direction = direction;
	newNode->next = NULL;
	newNode->prev = NULL;
	return newNode;
}

void pushFrontQ(int x, int y, int horizontalSpeed, int verticalSpeed, char direction) {
	struct Node* newNode = createNode(x, y, horizontalSpeed, verticalSpeed, direction);
	if(headSnake == NULL) {
		headSnake = tailSnake = newNode;
	} else {
		newNode->next = headSnake;
		headSnake->prev = newNode;
		headSnake = newNode;
	}
}

void popEndQ() {
	if(headSnake == NULL) {
		puts("empty");
		return;
	} else if(headSnake == tailSnake) {
		headSnake = tailSnake = NULL;
		free(headSnake);
	} else {
		struct Node* temp = tailSnake;
		tailSnake = temp->prev;
		temp->prev = NULL;
		free(temp);
		tailSnake->next = NULL;
	}
}

void gotoxy(int x, int y) {
	COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    HANDLE hOut;
	CONSOLE_CURSOR_INFO ConCurInf;
	
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	
	ConCurInf.dwSize = 10;
	ConCurInf.bVisible = FALSE;
	
	SetConsoleCursorInfo(hOut, &ConCurInf);
}


void addOneFood() {
	int x, y;
	do {
		x = (rand() % 27) + 1;
		y = (rand() % 13) + 1;
		if(currMap->pMap[y][x] == ' ') {
			currMap->pMap[y][x] = '*';
			gotoxy(x, y);
			printf("*");
			
		}
		break;
	} while(1);
	
}


void printSnake() {
	struct Node* curr = headSnake;
	while(curr != NULL) {
		if(curr == headSnake) {
			if(curr->x >= 0 && curr->y >= 0) {
				
				gotoxy(curr->x, curr->y);
				printf("o");
			}
		} else {
			if(curr->x >= 0 && curr->y >= 0) {
				
				gotoxy(curr->x, curr->y);
				printf(".");
			}
		}
		curr = curr->next;
	}
}


void cleanSnake() {
	struct Node* curr = headSnake;
	while(curr != NULL) {
		gotoxy(curr->x, curr->y);
		printf(" ");
		curr->x = -1;
		curr->y = -1;
		curr = curr->next;
	}
}

int len = 0, currLen, changeMap = 0, score = 1, lose = 0, isFood = 0;



void move() {
	int moveInput;
	moveInput = _getch();
	switch(moveInput) {
		case 'w': {

			if(currMap->pMap[headSnake->y-1][headSnake->x] == '*') {
				currMap->pMap[headSnake->y-1][headSnake->x] = ' ';
				pushFrontQ(headSnake->x, headSnake->y-1, 0, 1, 'w');

				addOneFood();
				len++;
				currLen = len;
				score++;
			}
			else if(currMap->pMap[headSnake->y-1][headSnake->x] == '.' || currMap->pMap[headSnake->y-1][headSnake->x] == '#') {
				lose = 1;
				break;
			}
			else if(currMap->pMap[headSnake->y-1][headSnake->x] == ' ') {

				if(headSnake->direction != 's') {
					
					pushFrontQ(headSnake->x, headSnake->y-1, 0, 1, 'w');
					if(currLen != len) {
						currLen++;
					} else {
						gotoxy(tailSnake->x, tailSnake->y);
						printf(" ");
						popEndQ();
					}
				}
			}

			break;
		}
		case 's': {
			if(currMap->pMap[headSnake->y+1][headSnake->x] == '*') {
				currMap->pMap[headSnake->y-1][headSnake->x] = ' ';
				pushFrontQ(headSnake->x, headSnake->y+1, 0, 1, 's');
				addOneFood();
				len++;
				currLen = len;
				score++;
			}
			else if(currMap->pMap[headSnake->y+1][headSnake->x] == '.' || currMap->pMap[headSnake->y+1][headSnake->x] == '#') {
				lose = 1;
				break;
			}
			else if(currMap->pMap[headSnake->y+1][headSnake->x] == ' ') {
				if(headSnake->direction != 'w') {
					
					pushFrontQ(headSnake->x, headSnake->y+1, 0, 1, 's');
					if(currLen != len) {
						currLen++;
					} else {
						gotoxy(tailSnake->x, tailSnake->y);
						printf(" ");
						popEndQ();
					}
					
				}
			}

			break;
		}
		case 'a': {
			if(currMap->pMap[headSnake->y][headSnake->x - 1] == '*') {
				currMap->pMap[headSnake->y-1][headSnake->x] = ' ';
				pushFrontQ(headSnake->x-1, headSnake->y, -1, 0, 'a');
				addOneFood();
				len++;
				currLen = len;
				score++;
			}
			else if(currMap->pMap[headSnake->y][headSnake->x - 1] == '#' || currMap->pMap[headSnake->y][headSnake->x - 1] == '.') {
				lose = 1;
				break;
			}
			else if(headSnake->y == 7 && headSnake->x-1 == 0) {
				
				currMap = currMap->prev;
				currLen = 1;
				cleanSnake();
				printMap(currMap->pMap);
				addOneFood();
				headSnake->y = 7;
				headSnake->x = 29;
				
			} 

			else if(currMap->pMap[headSnake->y][headSnake->x - 1] == ' ') {
				if(headSnake->direction != 'd') {
					
					if(currLen != len) {

						currLen++;
						pushFrontQ(headSnake->x-1, headSnake->y, -1, 0, 'a');
						popEndQ();
		
					} else {
						pushFrontQ(headSnake->x-1, headSnake->y, -1, 0, 'a');

						gotoxy(tailSnake->x, tailSnake->y);
						printf(" ");
						popEndQ();
					}
				}
			}
			
			break;
		}
		case 'd': {
			if(currMap->pMap[headSnake->y][headSnake->x + 1] == '*') {
				currMap->pMap[headSnake->y-1][headSnake->x] = ' ';
				pushFrontQ(headSnake->x+1, headSnake->y, 0, 1, 'd');
				addOneFood();
				len++;
				currLen = len;
				score++;
			}
			else if(currMap->pMap[headSnake->y][headSnake->x + 1] == '#' || currMap->pMap[headSnake->y][headSnake->x + 1] == '.') {
				lose = 1;
				break;
			}

			else if(headSnake->y == 7 && headSnake->x+1 == 29) {

				currMap = currMap->next;
				currLen = 1;
				cleanSnake();
				printMap(currMap->pMap);
				addOneFood();
				headSnake->y = 7;
				headSnake->x = 0;
			}
			else if(currMap->pMap[headSnake->y][headSnake->x + 1] == ' ') {
				if(headSnake->direction != 'a') {

					if(currLen != len) {

						currLen++;
						pushFrontQ(headSnake->x+1, headSnake->y, 0, 1, 'd');
						popEndQ();
					} else {
						pushFrontQ(headSnake->x+1, headSnake->y, 0, 1, 'd');

						gotoxy(tailSnake->x, tailSnake->y);
						printf(" ");
						popEndQ();
					}
				}
			}
			
			break;
		}
	}
	
}

void printQueue() {
	struct Node* curr = headSnake;
	while(curr != NULL) {
		printf("%d %d -> ", curr->x, curr->y);
		curr =curr->next;
	}
}

int main() {
	srand(time(NULL));

	pushHead(garden);
	pushHead(plaza);
	pushHead(town);
	
	printMap(currMap->pMap);
	addOneFood();
	pushFrontQ(13, 7, 1, 0, 'd');
	pushFrontQ(14, 7, 1, 0, 'd');
	len = 2;
	currLen = len;
	do {
		printSnake();
		move();
		gotoxy(50, 0);

		printf("Score: %d", score);
	} while(lose == 0);
	
	system("cls");
	printf("Game Over !\n");
	printf("Score: %d\n", score);
	printf("Press Enter to Continue..."); getchar();

	return 0;
}
