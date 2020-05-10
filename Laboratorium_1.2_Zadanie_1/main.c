#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>


#include "HD44780.h"
#include "keyboard.h"

int main() {
	LCD_Initalize();	//Inicjalizacja ekranu LCD
	LCD_Home();	//Przywraca poczatkowe wspolrzedne wyswietlacza
	char first_line[20];	//Zmienna do przetrzymywania tekstu

	while (1) {
		LCD_Clear();	//Usuniecie znakow na ekranie LCD

		LCD_GoTo(0, 0);	//Ustawienie kursora na poczatku 1 linii LCD
		if(check_digit() == -1)	//Jezeli nie wcisnieto zadnego guzika
		{
			LCD_WriteText("Nie wcisnieto");
			LCD_GoTo(0, 1);	//Ustawienie kursora na poczatku 2 linii LCD
			LCD_WriteText("zadnego guzika!");
		}else //Jezeli wcisnieto dowolny guzik
		{
			sprintf(first_line,"Wcisnieto: %d",check_digit());
			LCD_WriteText(first_line);
		}

		_delay_ms(100); //Program czeka 0.1s
	}
}
