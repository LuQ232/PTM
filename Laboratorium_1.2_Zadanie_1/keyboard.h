#include <stdio.h>

#include "HD44780.h"


int check_digit()
{
	//Sprawdzenie 1 kolumny
	PORTD = 0b01000000; //Ustawia pull Up w 1 rzedzie
	if (bit_is_set(PIND, PD0)) //Je¿eli wciœnieto przycisk "1"
		return 1;
	if (bit_is_set(PIND, PD1)) //Je¿eli wciœnieto przycisk "4"
		return 4;
	if (bit_is_set(PIND, PD2)) //Je¿eli wciœnieto przycisk "7"
		return 7;
	if (bit_is_set(PIND, PD3)) //Je¿eli wciœnieto przycisk "*"
		return 10;

	//Sprawdzenie 2 kolumny
	PORTD = 0b00100000; //Ustawia pull Up w 2 rzedzie,
	if (bit_is_set(PIND, PD0)) //Je¿eli wciœnieto przycisk "2"
		return 2;
	if (bit_is_set(PIND, PD1)) //Je¿eli wciœnieto przycisk "5"
		return 5;
	if (bit_is_set(PIND, PD2)) //Je¿eli wciœnieto przycisk "8"
		return 8;
	if (bit_is_set(PIND, PD3)) //Je¿eli wciœnieto przycisk "0"
		return 0;

	//Sprawdzenie 3 kolumny
	PORTD = 0b00010000; //Ustawia pull Up w 3 rzedzie,
	if (bit_is_set(PIND, PD0)) //Je¿eli wciœnieto przycisk "3"
		return 3;
	if (bit_is_set(PIND, PD1)) //Je¿eli wciœnieto przycisk "6"
		return 6;
	if (bit_is_set(PIND, PD2)) //Je¿eli wciœnieto przycisk "9"
		return 9;
	if (bit_is_set(PIND, PD3)) //Je¿eli wciœnieto przycisk "#"
		return 11;

	//Sprawdzenie fizycznego przycisku
	PORTC = 0b00000001; //Ustawia pull Up na przycisku
	if(bit_is_clear(PINC,PC0)) //Je¿eli wciœnieto przycisk
		return 12;

	return -1; // Gdy zaden przycisk nie jest wcisniety
}
