#include "SSD1331.h"//biblioteka wyswietlacza OLED
#include "snake.h"

static Snake snake;//Stworzenie obiektu typu "Snake"

//Stworzenie jabłuszka
static Cell Apple =
		{
				.x = 5,
				.y = 10,
		};

//Funkcja inicjalizujaca oraz wyswietlajaca weza na ekranie
void initSnake(void)
{
	uint8_t i;
	snake.size = SNAKE_INIT_SIZE;
	snake.head.x = SNAKE_START_X_POSITION;
	snake.head.y = SNAKE_START_Y_POSITION;
	//Rysuj glowe
	ssd1331_draw_circle(snake.head.x,snake.head.y,SNAKE_RAD,YELLOW);
	for(i=1;i<snake.size;i++)
	{
		snake.snakeParts[i].x = SNAKE_START_X_POSITION + (i*SNAKE_STEP);
		snake.snakeParts[i].y = SNAKE_START_Y_POSITION;
		ssd1331_draw_circle(snake.snakeParts[i].x,snake.snakeParts[i].y,SNAKE_RAD, GREEN_SNAKE);
	}
	ssd1331_draw_circle(Apple.x,Apple.y,SNAKE_RAD,RED);
}
//Fukcja odpowiada za poruszanie wezem
void MoveSnake(uint8_t direction)
{
	uint8_t i, partsCount = snake.size,pom_x,pom_y;
	pom_x=snake.head.x;
	pom_y=snake.head.y;
	//ssd1331_clear_screen(BLACK);
	snake.snakeParts[0].x=pom_x;
	snake.snakeParts[0].y=pom_y;
	ssd1331_draw_circle(snake.snakeParts[partsCount-1].x,snake.snakeParts[partsCount-1].y,SNAKE_RAD, GREEN_BACKGROUND);
	//ssd1331_fill_rect(snake.snakeParts[partsCount-1].x,snake.snakeParts[partsCount-1].y,SNAKE_STEP+1,SNAKE_STEP+1,BLACK);
	for(i=partsCount-1;i>0;i--)
	{
		snake.snakeParts[i].x=snake.snakeParts[i-1].x;
		snake.snakeParts[i].y=snake.snakeParts[i-1].y;
	}
	switch (direction)
	{
		case left:
		    snake.head.x -= SNAKE_STEP;
		    ssd1331_draw_circle(snake.head.x,snake.head.y,SNAKE_RAD,YELLOW);
		    for(i=1;i<partsCount;i++)
		    {
		    	//snake.snakeParts[i].x -= SNAKE_STEP;
		    	ssd1331_draw_circle(snake.snakeParts[i].x,snake.snakeParts[i].y,SNAKE_RAD,GREEN_SNAKE);
		    }
		break;
		case right:
		   	 snake.head.x += SNAKE_STEP;
		   	 ssd1331_draw_circle(snake.head.x,snake.head.y,SNAKE_RAD,YELLOW);
		   	 for(i=1;i<partsCount;i++)
		   	 {
		   	 	//snake.snakeParts[i].x += SNAKE_STEP;
		   	 	ssd1331_draw_circle(snake.snakeParts[i].x,snake.snakeParts[i].y,SNAKE_RAD,GREEN_SNAKE);
		   	 }
		break;
		case up:
		   	 snake.head.y -= SNAKE_STEP;
		   	 ssd1331_draw_circle(snake.head.x,snake.head.y,SNAKE_RAD,YELLOW);
		   	 for(i=1;i<partsCount;i++)
		   	 {
		   	 	//snake.snakeParts[i].y -= SNAKE_STEP;
		   	 	ssd1331_draw_circle(snake.snakeParts[i].x,snake.snakeParts[i].y,SNAKE_RAD,GREEN_SNAKE);
		   	 }
		break;
		case down:
			 snake.head.y += SNAKE_STEP;
		   	 ssd1331_draw_circle(snake.head.x,snake.head.y,SNAKE_RAD,YELLOW);
		   	 for(i=1;i<partsCount;i++)
		   	 {
		   	 	//snake.snakeParts[i].y += SNAKE_STEP;
		   	 	ssd1331_draw_circle(snake.snakeParts[i].x,snake.snakeParts[i].y,SNAKE_RAD,GREEN_SNAKE);
		   	 }
		break;
	}

	omnomnom();

}
//Funkcja sprawdza czy nastpila kolizja glowy weza z reszta jego ciala-> zwraca "true" jesli nastapila kolizja
bool checkCollision(){
	for(int i=1; i<snake.size; i++)
	{
		if((snake.snakeParts[i].x-SNAKE_STEP<snake.head.x)&&(snake.head.x<snake.snakeParts[i].x+SNAKE_STEP)
			&&(snake.snakeParts[i].y-SNAKE_STEP<snake.head.y)&&(snake.head.y<snake.snakeParts[i].y+SNAKE_STEP)){
			return true;
		}
	}
return false;
}

//Funkcja sprawcza czy głowa weza najechala na jabluszko, jeśli tak powieksza weza i rysuje nowe jabluszko
void omnomnom()
{
	if((Apple.x-SNAKE_STEP<snake.head.x)&&(snake.head.x<Apple.x+SNAKE_STEP)
			&&(Apple.y-SNAKE_STEP<snake.head.y)&&(snake.head.y<Apple.y+SNAKE_STEP))
	{
		ssd1331_draw_circle(Apple.x,Apple.y,SNAKE_RAD,GREEN_BACKGROUND);
			Apple.x = rand()%80;
			Apple.y = rand()%60;
			ssd1331_draw_circle(Apple.x,Apple.y,SNAKE_RAD,RED);
	}
}
