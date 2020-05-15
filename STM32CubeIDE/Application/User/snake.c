#include "SSD1331.h"//biblioteka wyswietlacza OLED
#include "snake.h"

static Snake snake;//Stworzenie obiektu typu "Snake"

//Stworzenie jabłuszka
static Cell Apple ={
	.x = 5,
	.y = 10,
};

static Cell Berry ={
	.x = 30,
	.y = 40,
};

int points=0;
static int only3=0;
static int pomocnicza=0;

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
	ssd1331_draw_circle(Berry.x,Berry.y,SNAKE_RAD,BLUE);
	ssd1331_draw_line(0, 52, 95, 52, BLACK);
	ssd1331_draw_line(0, 53, 95, 53, BLACK);
	ssd1331_display_string(3, 54, "Points:", FONT_1206, GREEN);
	ssd1331_display_num(31, 54, 0, sizeof(int), FONT_1206, WHITE);
}
//Fukcja odpowiada za poruszanie wezem
void MoveSnake(uint8_t direction)
{
	uint8_t i, partsCount = snake.size,pom_x,pom_y;
	pom_x=snake.head.x;
	pom_y=snake.head.y;
	snake.snakeParts[0].x=pom_x;
	snake.snakeParts[0].y=pom_y;
	ssd1331_draw_circle(snake.snakeParts[partsCount-1].x,snake.snakeParts[partsCount-1].y,SNAKE_RAD, GREEN_BACKGROUND);
	for(i=partsCount-1;i>0;i--)
	{
		snake.snakeParts[i].x=snake.snakeParts[i-1].x;
		snake.snakeParts[i].y=snake.snakeParts[i-1].y;
	}
	switch (direction)
	{
		case left:
		    snake.head.x -= SNAKE_STEP;
		    snake.head.x %= 96 + 65;//Jesli wyjedzie za lewa krawedz ekranu wyjedz prawa krawedzia ekranu [snake.head.x=127%96+65 = 96->prawa krawedz ekranu]
		    ssd1331_draw_circle(snake.head.x,snake.head.y,SNAKE_RAD,YELLOW);
		    for(i=1;i<partsCount;i++)
		    {
		    	//snake.snakeParts[i].x -= SNAKE_STEP;
		    	ssd1331_draw_circle(snake.snakeParts[i].x,snake.snakeParts[i].y,SNAKE_RAD,GREEN_SNAKE);
		    }
		break;
		case right:
		   	 snake.head.x += SNAKE_STEP;
		   	 snake.head.x %= 96;//Jesli wyjedzie za prawa krawedz ekranu wyjedz lewa krawedzia ekranu[snake.head.x=96%96 = 0->lewa krawedz ekranu]
		   	 ssd1331_draw_circle(snake.head.x,snake.head.y,SNAKE_RAD,YELLOW);
		   	 for(i=1;i<partsCount;i++)
		   	 {
		   	 	//snake.snakeParts[i].x += SNAKE_STEP;
		   	 	ssd1331_draw_circle(snake.snakeParts[i].x,snake.snakeParts[i].y,SNAKE_RAD,GREEN_SNAKE);
		   	 }
		break;
		case up:
		   	 snake.head.y -= SNAKE_STEP;
		   	 snake.head.y %= 52;//Jesli wyjedzie za gorna krawedz ekranu wyjedz dolna krawedzia ekranu[snake.head.y=127%64+1 = 64->dolna krawedz ekranu]
		   	 ssd1331_draw_circle(snake.head.x,snake.head.y,SNAKE_RAD,YELLOW);
		   	 for(i=1;i<partsCount;i++)
		   	 {
		   	 	//snake.snakeParts[i].y -= SNAKE_STEP;
		   	 	ssd1331_draw_circle(snake.snakeParts[i].x,snake.snakeParts[i].y,SNAKE_RAD,GREEN_SNAKE);
		   	 }
		break;
		case down:
			 snake.head.y += SNAKE_STEP;
			 snake.head.y %= 52;//Jesli wyjedzie za dolna krawedz ekranu wyjedz gorna krawedzia ekranu[snake.head.y=64%64 = 0->gorna krawedz ekranu]
		   	 ssd1331_draw_circle(snake.head.x,snake.head.y,SNAKE_RAD,YELLOW);
		   	 for(i=1;i<partsCount;i++)
		   	 {
		   	 	//snake.snakeParts[i].y += SNAKE_STEP;
		   	 	ssd1331_draw_circle(snake.snakeParts[i].x,snake.snakeParts[i].y,SNAKE_RAD,GREEN_SNAKE);
		   	 }
		break;
	}

	omnomnom();
	if(pomocnicza==0) zjedzBerry();
}
//Funkcja sprawdza czy nastpila kolizja glowy weza z reszta jego ciala-> zwraca "true" jesli nastapila kolizja
bool checkCollision(){
	for(int i=1; i<snake.size; i++)
	{
		if((snake.snakeParts[i].x-SNAKE_STEP<snake.head.x)&&(snake.head.x<snake.snakeParts[i].x+SNAKE_STEP)
			&&(snake.snakeParts[i].y-SNAKE_STEP<snake.head.y)&&(snake.head.y<snake.snakeParts[i].y+SNAKE_STEP)){
			//points= snake.size-SNAKE_INIT_SIZE;
			printf("You received %d points! \r\n", points);
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
		ssd1331_display_num(31, 54, points, sizeof(int), FONT_1206, GREEN_BACKGROUND);
		snake.size++;
		points+=1;
		Apple.x = rand()%80;
		Apple.y = rand()%54;
		for(int i=1; i<snake.size; i++)
		{
			if((Apple.x-SNAKE_STEP<snake.snakeParts[i].x)&&(snake.snakeParts[i].x<Apple.x+SNAKE_STEP) //gdyby jabluszko mialo byc na ogonie weza nowe wartosci wspolrzednych
				&&(Apple.y-SNAKE_STEP<snake.snakeParts[i].y)&&(snake.snakeParts[i].y<Apple.y+SNAKE_STEP))
			{Apple.x = rand()%80;
			Apple.y = rand()%54;}
		}
		ssd1331_draw_circle(Apple.x,Apple.y,SNAKE_RAD,RED);
		//ssd1331_display_string(3, 54, "Points:", FONT_1206, GREEN);
		ssd1331_display_num(31, 54, points, sizeof(int), FONT_1206, WHITE);
		printf("You ate Apple! \r\n");

		only3++;
		if(only3==3) {
			only3=0;
			wyswietlBerry();
		}
	}
}

void wyswietlBerry(){
				ssd1331_draw_circle(Berry.x,Berry.y,SNAKE_RAD,BLUE);
				pomocnicza=0;
}

void zjedzBerry(){
	if((Berry.x-SNAKE_STEP<snake.head.x)&&(snake.head.x<Berry.x+SNAKE_STEP)
					&&(Berry.y-SNAKE_STEP<snake.head.y)&&(snake.head.y<Berry.y+SNAKE_STEP))
			{
				ssd1331_draw_circle(Berry.x,Berry.y,SNAKE_RAD,GREEN_BACKGROUND);
				ssd1331_display_num(31, 54, points, sizeof(int), FONT_1206, GREEN_BACKGROUND);
				ssd1331_draw_circle(snake.snakeParts[snake.size-1].x,snake.snakeParts[snake.size-1].y,SNAKE_RAD,GREEN_BACKGROUND);
				snake.size--;
				points+=2;
				Berry.x = rand()%80;
				Berry.y = rand()%54;
				for(int i=1; i<snake.size; i++)
				{
					if((Berry.x-SNAKE_STEP<snake.snakeParts[i].x)&&(snake.snakeParts[i].x<Berry.x+SNAKE_STEP) //gdyby jabluszko mialo byc na ogonie weza nowe wartosci wspolrzednych
						&&(Berry.y-SNAKE_STEP<snake.snakeParts[i].y)&&(snake.snakeParts[i].y<Berry.y+SNAKE_STEP)
						|| Apple.x==Berry.x && Apple.y==Berry.y)
					{Berry.x = rand()%80;
					Berry.y = rand()%54;}
				}
				ssd1331_display_num(31, 54, points, sizeof(int), FONT_1206, WHITE);
				printf("You ate Berry! \r\n");
			}
	pomocnicza==1;
}

int Points()
{
	return points;
}
