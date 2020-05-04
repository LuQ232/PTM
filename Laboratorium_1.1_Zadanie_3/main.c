#include "SET.h"

int main() {

	int array [] = {0b00000001,0b00000010,0b00000100,0b00001000,0b00010000,0b00100000,0b01000000,0b10000000};// tablica z wartoœciami 1 dla kolejnych diod
	DDRD = 0b11111111; //Ustawia wyprowadzenia od PD0 do PD7 do pracy jako wyjscie
	int i = 0, direction = -1; // ustawienie indeksu dla tablicy oraz poczatkowego kierunku
	for (i = 0;; i += direction)
	{

		if ( i == 0 || i == 7 )
		{
			direction *= -1;	//Zmiana kierunku
		}

		PORTD = array[i];   // Zapalenie odpowiedniej diody
		_delay_ms(500);   	// Program czeka 0.5 sekundy
	}

}
