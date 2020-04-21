#ifndef SNAKE_H
#define SNAKE_H

#define SNAKE_LIMIT 40
#define SNAKE_INIT_SIZE 5
#define SNAKE_RAD 2//Promien czesci weza

#define SNAKE_START_X_POSITION 50
#define SNAKE_START_Y_POSITION 40
#define SNAKE_STEP 2*SNAKE_RAD//Do rysowania kolejnych cześci weza(2 x promien)

enum
{
    left,
    right,
    up,
    down,
};
typedef struct _Cell{
	uint8_t x,y;
}Cell;

//Struktura odpowiadajaca za weza
typedef struct _Snake{
	Cell snakeParts[SNAKE_LIMIT];
	Cell head;
	uint8_t size;
}Snake;

void initSnake(void);
void MoveSnake(uint8_t direction);

#endif// SNAKE_H
