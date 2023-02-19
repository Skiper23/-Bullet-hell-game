#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define LEVEL_WIDTH 1280
#define LEVEL_HEIGHT 960
#define PRZEKATNA 2100
#define POZX_BOSS 320
#define POZY_BOSS 320
#define POZX_BOSS1 960
#define POZY_BOSS1 320
#define POZX_BOSS2 640
#define POZY_BOSS2 600
#define SIZEX_BOSS 46
#define SIZEY_BOSS 80
#define SIZEX_BOSS1 74
#define SIZEY_BOSS1 132
#define SIZEX_BOSS2 20
#define SIZEY_BOSS2 70
#define SIZEX_CHARACTER 14
#define SIZEY_CHARACTER 21
#define SIZEX_POCISK 14
#define SIZEY_POCISK 18
#define MENU 1
#define START 0
#define WYGRANA 2
#define XSTART 40
#define YSTART 60
#define LIFE 3
#define LIFE1 4
#define LIFE2 5
#define DYSTANS_MIEDZY_POCISKAMI 45
enum Semestry
{
	SEMI = 1,
	SEMII,
	SEMIII
};
struct PociskZawodnika
{
	int x, y;
	int rozmiarx, rozmiary;
};
void DrawString(SDL_Surface *screen, int x, int y, const char *text,SDL_Surface *charset) 
{
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text) 
	{
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	}
}
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) 
{
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
}
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) 
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
}
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) 
{
	for(int i = 0; i < l; i++) 
	{
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	}
}
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,Uint32 outlineColor, Uint32 fillColor)
{
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for(i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
}
void Informacje(char *text,SDL_Surface *screen, SDL_Surface *charset, char *komunikat, int y )
{
	sprintf(text, komunikat);
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, y, text, charset);
}
void Strzaly(SDL_Surface *screengame, SDL_Surface *pocisk, int pozx, int pozy, double odl)
{
	int pozxP = pozx + odl;
	int pozxL = pozx - odl;
	int pozxP2 = pozx + odl / 2;
	int pozxL2 = pozx - odl / 2;
	int pozyD = pozy  + odl;
	int pozyG = pozy - odl;
	int pozyD2 = pozy + (odl / 2);
	int pozyG2 = pozy - (odl / 2);

	DrawSurface(screengame, pocisk, pozx, pozyG);
	DrawSurface(screengame, pocisk, pozx, pozyD );
	DrawSurface(screengame, pocisk, pozxP, pozy);
	DrawSurface(screengame, pocisk, pozxL, pozy);
	DrawSurface(screengame, pocisk, pozxP, pozyG);
	DrawSurface(screengame, pocisk, pozxP , pozyD);
	DrawSurface(screengame, pocisk, pozxL , pozyD);
	DrawSurface(screengame, pocisk, pozxL, pozyG );
	DrawSurface(screengame, pocisk, pozxP, pozyD2);
	DrawSurface(screengame, pocisk, pozxP, pozyG2);
	DrawSurface(screengame, pocisk, pozxL, pozyD2);
	DrawSurface(screengame, pocisk, pozxL, pozyG2);
	DrawSurface(screengame, pocisk, pozxP2, pozyD);
	DrawSurface(screengame, pocisk, pozxP2, pozyG);
	DrawSurface(screengame, pocisk, pozxL2, pozyD);
	DrawSurface(screengame, pocisk, pozxL2, pozyG);
}
void Wywolaj_Strzal(SDL_Surface* screengame, SDL_Surface* pocisk, int pozx, int pozy,  double *odl, int pocz, int kon)
{
	for (int i = pocz; i < kon; i++)
	{
		Strzaly(screengame, pocisk, pozx, pozy, odl[i]);
	}
}
void Strzaly_Boss1(SDL_Surface* screengame, SDL_Surface* pocisk, int pozx, int pozy, double *odl, int koniec,double time)
{
	int pox, poy, px, py;
	
	double x = POZX_BOSS1 + time * 120*cos(time*20) ;
	double y = POZY_BOSS1 + time * 120*sin(time*20) ;
	DrawSurface(screengame, pocisk, x, y);
}
void Wywolaj_Strzal2(SDL_Surface* screengame, SDL_Surface* pocisk, int pozx, int pozy, double *odl, int koniec, double time)
{
	for (int i = 100; i > 1; i--)
	{
		Strzaly_Boss1(screengame, pocisk, pozx, pozy, odl, koniec, time/i);
	}
}
void Strzaly_Boss1v2(SDL_Surface* screengame, SDL_Surface* pocisk, int pozx, int pozy, double* odl, int poczatek, int koniec)
{
	int pozxP, pozxL, pozyG, pozyD;
	for (int i = poczatek; i < koniec; i++)
	{
		pozxP = pozx + odl[i];
		pozxL = pozx - odl[i];
		for (int j = 0; j < koniec; j++)
		{
			pozyD = pozy + odl[j];
			pozyG = pozy - odl[j];
			DrawSurface(screengame, pocisk, pozxL, pozyD);
			DrawSurface(screengame, pocisk, pozxL, pozyG);

			DrawSurface(screengame, pocisk, pozxP, pozyD);
			DrawSurface(screengame, pocisk, pozxP, pozyG);

		}
	}
}
void Wywolaj_Strzal2v2(SDL_Surface* screengame, SDL_Surface* pocisk, int pozx, int pozy, double* odl, int koniec)
{
	for (int i = 1; i < koniec; i++)
	{
		Strzaly_Boss1v2(screengame, pocisk, pozx, pozy, odl, i - 1, i);
	}
}
void Strzaly_Boss3v1(SDL_Surface* screengame, SDL_Surface* pocisk, int pozx, int pozy, double* odl, int koniec)
{
	int pozxP, pozxL, pozyG, pozyD;
	for (int i = 0; i < koniec*15; i++)
	{
		pozxP = pozx + odl[i]/10;
		pozxL = pozx - odl[i]/10;
		pozyD = pozy + odl[i]/10;
		pozyG = pozy - odl[i]/10;

		DrawSurface(screengame, pocisk, pozx, pozyD);
		DrawSurface(screengame, pocisk, pozx, pozyG);

		DrawSurface(screengame, pocisk, pozxP, pozy);
		DrawSurface(screengame, pocisk, pozxL, pozy);
	}
}
void Strzaly_Boss3v2(SDL_Surface* screengame, SDL_Surface* pocisk, int pozx, int pozy, double* odl, int koniec)
{
	int pozxP, pozxL, pozyG, pozyD;
	for (int i = 0; i < koniec; i++)
	{
		pozxP = pozx + odl[i];
		pozxL = pozx - odl[i];
		pozyD = pozy + odl[i];
		pozyG = pozy - odl[i];

		DrawSurface(screengame, pocisk, pozx, pozyD);
		DrawSurface(screengame, pocisk, pozx, pozyG);

		DrawSurface(screengame, pocisk, pozxP, pozy);
		DrawSurface(screengame, pocisk, pozxL, pozy);
	}
}
void Menu_Startu(SDL_Surface *screen,SDL_Surface *charset,SDL_Texture *scrtex, SDL_Renderer * renderer, int kolorRamki, int kolorSrodka)
{
	char text[128];
	DrawRectangle(screen, 20, 40, SCREEN_WIDTH - 40, SCREEN_HEIGHT - 80, kolorRamki, kolorSrodka);

	Informacje(text, screen, charset, "Witaj w swiecie studiow przybyszu!", 50);
	Informacje(text, screen, charset, "Przed rozpoczeciem walki wybierz semestr: ", 80);
	Informacje(text, screen, charset, "Semestr I (wcisnij 1): ", 110);
	Informacje(text, screen, charset, "Semestr II (wcisnij 2): ", 140);
	Informacje(text, screen, charset, "Semestr III (wcisnij 3): ", 170);

	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
}
void Menu_Gry(SDL_Surface *screengame, SDL_Rect &kamera,SDL_Surface *screen, SDL_Surface *charset, SDL_Texture *texture, int kolorRamki, int kolorSrodka ,double time, double fps )
{
	char text[128];

	DrawRectangle(screengame, kamera.x + 4, kamera.y + 4, SCREEN_WIDTH - 8, 44, kolorRamki, kolorSrodka);

	sprintf(text, " czas trwania = %.1lf s  %.0lf klatek / s", time, fps);
	DrawString(screengame, kamera.x + screen->w / 2 - strlen(text) * 8 / 2, kamera.y + 10, text, charset);

	sprintf(text, " Esc - wyjscie,n - nowa gra, strzalki - ruch, WSAD - zacznij strzelac");

	DrawString(screengame, kamera.x + screen->w / 2 - strlen(text) * 8 / 2, kamera.y + 22, text, charset);
	sprintf(text, " 1 - Semestr I, 2 - Semestr II, 3- Semestr III");

	DrawString(screengame, kamera.x + screen->w / 2 - strlen(text) * 8 / 2, kamera.y + 34, text, charset);
}
void Nowa_Gra(int& x, int& y, double &worldTime, int &licznik, double &timer, int&widac, int &wystrzalx, int &kierunek, int &zyciaboss, int &zyciaboss1, int &zyciaboss2, int &poziom)
{

	x = XSTART;
	y = YSTART;
	worldTime = 0;
	licznik = 1;
	timer = 0;
	widac = 0;
	wystrzalx = -1;
	kierunek = -1;
	zyciaboss = LIFE;
	zyciaboss1 = LIFE1; 
	zyciaboss2 = LIFE2;
	poziom = SEMI;
}
void Strzal_Gora(SDL_Surface* screengame, SDL_Surface* pocisk, int pozx, int pozy, double odl,SDL_Rect* pociskObszar)
{
	int y = pozy - odl;
	pociskObszar->x = pozx - (SIZEX_POCISK/2);
	pociskObszar->y = y - (SIZEY_POCISK/2);
	DrawSurface(screengame, pocisk, pozx, y);
}
void Strzal_Prawo(SDL_Surface* screengame, SDL_Surface* pocisk, int pozx, int pozy, double odl, SDL_Rect* pociskObszar)
{
	int x = pozx + odl;
	pociskObszar->x = x - (SIZEX_POCISK / 2);
	pociskObszar->y = pozy - (SIZEY_POCISK / 2);
	DrawSurface(screengame, pocisk, x, pozy);
}
void Strzal_Dol(SDL_Surface* screengame, SDL_Surface* pocisk, int pozx, int pozy, double odl, SDL_Rect* pociskObszar)
{
	int y = pozy+ odl;
	pociskObszar->x = pozx - (SIZEX_POCISK / 2);
	pociskObszar->y = y - (SIZEY_POCISK / 2);
	DrawSurface(screengame, pocisk, pozx, y);
}
void Strzal_Lewo(SDL_Surface* screengame, SDL_Surface* pocisk, int pozx, int pozy, double odl, SDL_Rect* pociskObszar)
{
	int x = pozx - odl;
	pociskObszar->x = x - (SIZEX_POCISK / 2);
	pociskObszar->y = pozy - (SIZEY_POCISK / 2);
	DrawSurface(screengame, pocisk, x, pozy);
}
void Wywolaj_Strzal_Zawodnika(SDL_Surface* screengame, SDL_Surface* pocisk, int pozx, int pozy, double* odleglosci,int kierunek,int odl, SDL_Rect *pociskObszar)
{
		if (kierunek == 1)
			Strzal_Gora(screengame, pocisk, pozx, pozy, odleglosci[odl],pociskObszar);
		else if (kierunek == 2)
			Strzal_Prawo(screengame, pocisk, pozx, pozy, odleglosci[odl], pociskObszar);
		else if (kierunek == 3)
			Strzal_Dol(screengame, pocisk, pozx, pozy, odleglosci[odl], pociskObszar);
		else if (kierunek == 4)
			Strzal_Lewo(screengame, pocisk, pozx, pozy, odleglosci[odl], pociskObszar);
}
void Sterowanie(int& x, int& y, double& worldTime, int& licznik, double& timer, int &quit, int &poziom, int &widac,SDL_Surface* screengame, SDL_Surface* strzal, double *odleglosci, int &flaga, int& wystrzalx, int& kierunek, int& zyciaboss, int& zyciaboss1, int& zyciaboss2)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_RIGHT) x += 10;
				else if (event.key.keysym.sym == SDLK_LEFT) x -= 10;
				else if (event.key.keysym.sym == SDLK_UP) y -= 10;
				else if (event.key.keysym.sym == SDLK_DOWN) y += 10;
				else if (event.key.keysym.sym == SDLK_n)
				{
					Nowa_Gra(x, y, worldTime, licznik, timer, widac, wystrzalx, kierunek, zyciaboss, zyciaboss1, zyciaboss2,  poziom);
					poziom = SEMI;
				}
				else if (event.key.keysym.sym == SDLK_1)
				{
					Nowa_Gra(x, y, worldTime, licznik, timer,widac,  wystrzalx, kierunek,  zyciaboss,  zyciaboss1, zyciaboss2,poziom);
					poziom = SEMI;
				}
				else if (event.key.keysym.sym == SDLK_2)
				{
					Nowa_Gra(x, y, worldTime, licznik, timer, widac, wystrzalx,  kierunek, zyciaboss,  zyciaboss1, zyciaboss2, poziom);
					poziom = SEMII;
				}
				else if (event.key.keysym.sym == SDLK_3)
				{
					Nowa_Gra(x, y, worldTime, licznik, timer,  widac, wystrzalx,  kierunek, zyciaboss, zyciaboss1, zyciaboss2, poziom);
					poziom = SEMIII;

				}
				else if (event.key.keysym.sym == SDLK_ESCAPE)
					quit = 1;
				else if (event.key.keysym.sym == SDLK_w)
					flaga = 1;
				else if (event.key.keysym.sym == SDLK_d)
					flaga = 2;
				else if (event.key.keysym.sym == SDLK_s)
					flaga = 3;
				else if (event.key.keysym.sym == SDLK_a)
					flaga = 4;
			}
		}
}
void Ograniczenie_Kamery(int &x, int &y, SDL_Rect &kamera)
{
	if (x > (LEVEL_WIDTH-(SIZEX_CHARACTER/2))) x = LEVEL_WIDTH - SIZEX_CHARACTER;
	if (x < (SIZEX_CHARACTER / 2)) x = (SIZEX_CHARACTER / 2);
	if (y > LEVEL_HEIGHT) y = LEVEL_HEIGHT;
	if (y < (SIZEY_CHARACTER/2)) y = SIZEY_CHARACTER / 2;
	kamera.x = x - SCREEN_WIDTH / 2;
	kamera.y = y - SCREEN_HEIGHT / 2;
	if (kamera.x + kamera.w >= LEVEL_WIDTH) kamera.x = LEVEL_WIDTH - SCREEN_WIDTH;
	if (kamera.x <= 0)kamera.x = 0;
	if (kamera.y + kamera.h >= LEVEL_HEIGHT) kamera.y = LEVEL_HEIGHT - SCREEN_HEIGHT;
	if (kamera.y < 0) kamera.y = 0;
}
void Wybor_Poziomu(int& x, int& y, double& worldTime, int& licznik, double& timer, int& quit, int& poziom)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_1)
			{
				poziom = SEMI;
				quit = START;
			}
			else if (event.key.keysym.sym == SDLK_2)
			{
				poziom = SEMII;
				quit = START;
			}
			else if (event.key.keysym.sym == SDLK_3)
			{
				poziom = SEMIII;
				quit = START;
			}
		}
	}
}
int Czy_W_Kamerze(int x, int y, SDL_Rect& kamera)
{
	if (x<(kamera.x + kamera.w) && x>kamera.x && y< (kamera.y + kamera.h) && y > kamera.y)
		return 1;
	
	return 0;
}
double Zaokraglij(double a)
{
	int b = a;
	
	return round((a * 100)) / 100;
}
void Wygrana(SDL_Surface* screen, SDL_Surface* charset, SDL_Texture* scrtex, SDL_Renderer* renderer, int kolorRamki, int kolorSrodka)
{
	char text[128];
	DrawRectangle(screen, 20, 40, SCREEN_WIDTH - 40, SCREEN_HEIGHT - 80, kolorRamki, kolorSrodka);

	Informacje(text, screen, charset, "PRZEZYLES STUDIA, GRATULACJE! ", 200);

	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
}
void Tworzenie_Odleglosci(double* odleglosci)
{
	for (int i = 1; i < 101; i++)
	{
		odleglosci[i] = odleglosci[i - 1] + DYSTANS_MIEDZY_POCISKAMI;
	}
}
void Wyswietl_Bombe(int czas, double worldTime, int &licznikmini,int x, int y, SDL_Rect kamera, int &widac, double &czaspom, SDL_Surface* screengame, SDL_Surface* bomba, SDL_Surface* pociskmini, double *odleglosci )
{
	static int pozycjax, pozycjay;
	if (czas % 17 == 0 && czas > 0 && (Zaokraglij(worldTime) - czas) == 0)
	{
		licznikmini = 0;
		pozycjax = x + 100;
		pozycjay = y + 100;
		while (!Czy_W_Kamerze(pozycjax, pozycjay, kamera))
		{
			pozycjax -= 200;
			pozycjay -= 200;
		}
		widac = 1;
		czaspom = czas;
	}
	if (widac == 1 && czas - czaspom < 3)
	{
		DrawSurface(screengame, bomba, pozycjax, pozycjay);
	}
	if (widac == 1 && (czas - czaspom) == 3)
	{
		Wywolaj_Strzal(screengame, pociskmini, pozycjax, pozycjay, odleglosci, licznikmini - 6, licznikmini);
	}
}
void Mocne_Strzaly(double &timermocny, int& licznikmocny)
{
	if (timermocny > 0.1)
	{
		licznikmocny++;
		timermocny = 0;
		if (licznikmocny >= 85) licznikmocny = 0;
	}
}
void Mini_strzaly(double &timermini, int& licznikmini)
{
	if (timermini > 0.45)
	{
		licznikmini++;
		timermini = 0;
	}
}
void Normalne_Strzaly(double &timer, int &licznik)
{
	if (timer > 0.25)
	{
		if (licznik < 25) licznik++;
		timer = 0;
		if (licznik >= 25) licznik = 0;
	}
}
void Postac_Strzaly(double &timerstrzal, int poprzednikierunek, int kierunek, int& dalekiStrzal,int &wystrzalx, int& wystrzaly, int x, int y, int &koniec )
{
	if (timerstrzal > 0.25)
	{
		if (poprzednikierunek == kierunek && dalekiStrzal < 7)
			dalekiStrzal++;
		else if (dalekiStrzal == 7)
		{
			dalekiStrzal = 0;
			wystrzalx = x;
			wystrzaly = y;
		}
		timerstrzal = 0;
	}
	if (poprzednikierunek != kierunek || koniec == 1)
	{
		dalekiStrzal = 0;
		wystrzalx = x;
		wystrzaly = y;
		timerstrzal = 0;
		koniec = 0;
	}
}
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char** argv) 
{
	int t1, t2, quit, frames, rc, x, y,licznik, poziom, widac = 0,licznikmini=0,licznikmocny=0,kierunek=-1,dalekiStrzal=0,poprzednikierunek=-2,zyciaboss=LIFE,zyciaboss1=LIFE1,zyciaboss2=LIFE2,koniec=0;
	int wystrzalx=-1, wystrzaly=-1,czas;
	double delta, fpsTimer, fps, odleglosci[101] = {},worldTime, timer = 0,timermini =0,timermocny=0,aktualxBoss3,aktualyBoss3,czaspom=-1, pozycjaxBomby=-1, pozycjayBomby=-1,timerstrzal=0;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderTarget = nullptr, * renderer;
	SDL_Surface* screen, * charset,* screengame,* postac, * tlo, *boss, *pocisk, *boss1, *boss2,* bomba, *pociskmini, *mocnyPocisk, *strzal;
	SDL_Texture* scrtex;
	SDL_Rect kamera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, bossObszar = { POZX_BOSS - (SIZEX_BOSS / 2),POZY_BOSS - (SIZEY_BOSS/2), SIZEX_BOSS,SIZEY_BOSS };
	SDL_Rect boss1Obszar = { POZX_BOSS1 - (SIZEX_BOSS1 / 2),POZY_BOSS1 - (SIZEY_BOSS1/2), SIZEX_BOSS1,SIZEY_BOSS1 }, boss2Obszar = { POZX_BOSS2 - (SIZEX_BOSS2 / 2),POZY_BOSS2 - (SIZEY_BOSS2/2), SIZEX_BOSS2,SIZEY_BOSS2 };
	SDL_Texture* texture;
	PociskZawodnika PociskZawodnika;
	PociskZawodnika.rozmiarx = SIZEX_POCISK;
	PociskZawodnika.rozmiary = SIZEY_POCISK;
	SDL_Rect pociskObszar = { 0,0,PociskZawodnika.rozmiarx,PociskZawodnika.rozmiary };

	kamera.x = 0;
	kamera.y = 0;

	Nowa_Gra(x, y, worldTime, licznik, timer,widac,wystrzalx,  kierunek,  zyciaboss,  zyciaboss1, zyciaboss2, poziom);

	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,&window, &renderer);
	renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );

	if(rc != 0) 
	{
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	}
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "study challenge");

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	screengame = SDL_CreateRGBSurface(0, LEVEL_WIDTH, LEVEL_HEIGHT, 32,0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,SCREEN_WIDTH, SCREEN_HEIGHT);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, LEVEL_WIDTH, LEVEL_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);
	
	charset = SDL_LoadBMP("./cs8x8.bmp");
	
	SDL_SetColorKey(charset, true, 0x000000);

	postac = SDL_LoadBMP("./po.bmp");
	tlo = SDL_LoadBMP("./t.bmp");
	boss = SDL_LoadBMP("./boss.bmp");
	boss1 = SDL_LoadBMP("./boss2.bmp");
	boss2 = SDL_LoadBMP("./boss3.bmp");
	strzal = SDL_LoadBMP("./strzal.bmp");
	pocisk = SDL_LoadBMP("./pocisk.bmp");
	bomba = SDL_LoadBMP("./bomba.bmp");
	pociskmini = SDL_LoadBMP("./pociskmini.bmp");
	mocnyPocisk = SDL_LoadBMP("./mocny.bmp");

	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 1;

	Menu_Startu(screen, charset, scrtex, renderer, czerwony, niebieski);

	while(quit==MENU)
	{
		Wybor_Poziomu(x, y, worldTime, licznik, timer, quit, poziom);
	}
	
	Tworzenie_Odleglosci(odleglosci);

	licznik = 1;
	t1 = SDL_GetTicks();
	while(quit==START) 
	{
		DrawSurface(screengame, tlo, LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2);
		
		t2 = SDL_GetTicks();

		delta = (t2 - t1) * 0.001;
	
		t1 = t2;

		worldTime += delta;
		timer += delta;
		timermini += delta;
		timermocny += delta;
		timerstrzal += delta;
		czas = worldTime;
		if(poziom==SEMI)
		DrawSurface(screengame, boss, POZX_BOSS , POZY_BOSS );
		if (poziom == SEMII)
		DrawSurface(screengame, boss1, POZX_BOSS1 , POZY_BOSS1 );
		
		DrawSurface(screengame, postac, x, y);
		Wyswietl_Bombe(czas, worldTime, licznikmini, x, y, kamera, widac, czaspom, screengame, bomba, pociskmini, odleglosci);

		if (poziom == SEMI)
		{
			if (SDL_HasIntersection(&pociskObszar, &bossObszar))
			{
				koniec = 1;
				zyciaboss--;
				if (zyciaboss == 0)
				{
					Nowa_Gra(x, y, worldTime, licznik, timer,  widac, wystrzalx,  kierunek,zyciaboss, zyciaboss1,zyciaboss2,poziom);
					poziom = SEMII;
				}
			}
			if (czas  % 23 == 0&&czas>0)
				Wywolaj_Strzal(screengame, mocnyPocisk, POZX_BOSS, POZY_BOSS, odleglosci,licznikmocny%6 , licznikmocny%6+3);
			else if (licznik > 5) 
				Wywolaj_Strzal(screengame, pocisk, POZX_BOSS, POZY_BOSS, odleglosci, licznik - 5, licznik);
			if (czas % 11 == 0)
				Wywolaj_Strzal(screengame, pocisk, POZX_BOSS, POZY_BOSS, odleglosci, 0, licznik);
		}
		if (poziom == SEMII)
		{
			if (SDL_HasIntersection(&pociskObszar, &boss1Obszar))
			{
				koniec = 1;
				zyciaboss1--;
				if (zyciaboss1 == 0)
				{
					Nowa_Gra(x, y, worldTime, licznik, timer, widac, wystrzalx,  kierunek,zyciaboss, zyciaboss1,zyciaboss2,poziom);
					poziom = SEMIII;
				}
			}
			if (czas % 23 == 0 && czas > 0)
				Wywolaj_Strzal(screengame, mocnyPocisk, POZX_BOSS1, POZY_BOSS1, odleglosci, licznikmocny % 6, licznikmocny % 6 + 3);
			else if ((czas / 10) % 3 == 0)
				Wywolaj_Strzal2v2(screengame, pocisk, POZX_BOSS1, POZY_BOSS1, odleglosci, licznik);
			else
				Wywolaj_Strzal2(screengame, pocisk, POZX_BOSS1, POZY_BOSS1, odleglosci, licznik, worldTime);
		}

	aktualxBoss3 = POZX_BOSS2 + cos(worldTime) * POZX_BOSS2 / 4;
	aktualyBoss3 = POZX_BOSS2 + sin(worldTime) * POZY_BOSS2 / 4;

		if (poziom == SEMIII)
		{
			DrawSurface(screengame, boss2, aktualxBoss3, aktualyBoss3);
				
			boss2Obszar.x = aktualxBoss3-(SIZEX_BOSS2/2);
			boss2Obszar.y = aktualyBoss3- (SIZEY_BOSS2 / 2);

			if (SDL_HasIntersection(&pociskObszar, &boss2Obszar))
			{
				koniec = 1;
				zyciaboss2--;
				if (zyciaboss2 == 0)
					quit = WYGRANA;
			}
			if (czas % 23 == 0 && czas > 0)
				Wywolaj_Strzal(screengame, mocnyPocisk, aktualxBoss3, aktualyBoss3, odleglosci, licznikmocny % 6, licznikmocny % 6 + 3);
			else if ((czas / 10) % 3 == 0)
				Strzaly_Boss3v1(screengame, pocisk, aktualxBoss3, aktualyBoss3, odleglosci, licznik);
			else
				Strzaly_Boss3v2(screengame, pocisk, aktualxBoss3, aktualyBoss3, odleglosci, licznik);
		}
			
		Mocne_Strzaly(timermocny, licznikmocny);

		Mini_strzaly(timermini, licznikmini);

		Normalne_Strzaly(timer, licznik);

		
		fpsTimer += delta;
		
		if(fpsTimer > 0.5) 
		{
			fps = frames * 2.0;
			frames = 0;
			fpsTimer -= 0.5;
		}

		Postac_Strzaly(timerstrzal, poprzednikierunek, kierunek, dalekiStrzal, wystrzalx, wystrzaly, x, y, koniec);

		Menu_Gry(screengame, kamera, screen, charset, texture, czerwony, niebieski, worldTime, fps);

		if (wystrzalx != -1 && poprzednikierunek == kierunek)
		{
			Wywolaj_Strzal_Zawodnika(screengame, strzal, wystrzalx, wystrzaly, odleglosci, kierunek, dalekiStrzal,&pociskObszar);
		}

		if(kierunek!=-1)
		poprzednikierunek = kierunek;
		Sterowanie(x, y, worldTime, licznik, timer,  quit,poziom,widac,screengame,  mocnyPocisk,  odleglosci,kierunek, wystrzalx,  kierunek, zyciaboss, zyciaboss1, zyciaboss2);

		SDL_UpdateTexture(texture, NULL, screengame->pixels, screengame->pitch);

		SDL_RenderCopy(renderer, texture, &kamera, NULL);

		SDL_RenderPresent(renderer);

		frames++;
		Ograniczenie_Kamery(x, y, kamera);
	}
	while (quit == WYGRANA)
		Wygrana(screen, charset, scrtex, renderer, czerwony, niebieski);

	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);

	SDL_Quit();
	return 0;
}
