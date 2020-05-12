#include <stdio.h>
#include <string.h>

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


void run_calc()
{
	// Zmienna informujaca czy wprowadzano jakakolwiek cyfre
	int was_any_digit =0;
	// Zmienna dla pierwszej liczby dzialania
	int number_1=0;
	// Zmienna dla drugiej liczby dzialania
	int number_2=0;
	// Zmienna informujaca czy wprowadzna cyfra to pierwsza cyfra
	int counter = 0;
	 // jaka operacja : 1 dla dodawania ; 2 dla odejmowania
	int operation = 0;
	LCD_Initalize();	//Inicjalizacja ekranu LCD
	LCD_Home();	//Przywraca poczatkowe wspolrzedne wyswietlacza
	LCD_Clear();

while(1)
{
	while (1)
	{
			switch(check_digit())
			{
			case 0: //Gdy wcisnieto "0"
					if(counter == 0)//Gdy jest to pierwsza cyfra
					{
						number_1=0;
						was_any_digit =1;
						counter = 1;
					}else //Gdy nie jest to pierwsza cyfra liczby
					{
						number_1 = 10*number_1+0;
						was_any_digit = 1;
					}
			break;

			case 1://Gdy wcisnieto "1"
				if(counter == 0)//Gdy jest to pierwsza cyfra
				{
					number_1=1;
					was_any_digit =1;
					counter = 1;
				}else
				{
					number_1 = 10*number_1+1;
					was_any_digit = 1;
				}
			break;

			case 2://Gdy wcisnieto "2"
				if(counter == 0)//Gdy jest to pierwsza cyfra
				{
					number_1=2;
					was_any_digit =1;
					counter = 1;
				}else
				{
					number_1 = 10*number_1+2;
					was_any_digit = 1;
				}
			break;
			case 3://Gdy wcisnieto "3"
				if(counter == 0)//Gdy jest to pierwsza cyfra
				{
					number_1=3;
					was_any_digit =1;
					counter = 1;
				}else
				{
					number_1 = 10*number_1+3;
					was_any_digit = 1;
				}
			break;
			case 4://Gdy wcisnieto "4"
				if(counter == 0)//Gdy jest to pierwsza cyfra
				{
					number_1=4;
					was_any_digit =1;
					counter = 1;
				}else
				{
					number_1 = 10*number_1+4;
					was_any_digit = 1;
				}
			break;
			case 5://Gdy wcisnieto "5"
				if(counter == 0)//Gdy jest to pierwsza cyfra
				{
					number_1=5;
					was_any_digit =1;
					counter = 1;
				}else
				{
					number_1 = 10*number_1+5;
					was_any_digit = 1;
				}
			break;
			case 6://Gdy wcisnieto "6"
				if(counter == 0)//Gdy jest to pierwsza cyfra
				{
					number_1=6;
					was_any_digit =1;
					counter = 1;
				}else
				{
					number_1 = 10*number_1+6;
					was_any_digit = 1;
				}
			break;
			case 7://Gdy wcisnieto "7"
				if(counter == 0)//Gdy jest to pierwsza cyfra
				{
					number_1=7;
					was_any_digit =1;
					counter = 1;
				}else
				{
					number_2 = 10*number_1+7;
					was_any_digit = 1;
				}
			break;
			case 8://Gdy wcisnieto "8"
				if(counter == 0)//Gdy jest to pierwsza cyfra
				{
					number_1=8;
					was_any_digit =1;
					counter = 1;
				}else
				{
					number_1 = 10*number_1+8;
					was_any_digit = 1;
				}
			break;
			case 9://Gdy wcisnieto "9"
				if(counter == 0)//Gdy jest to pierwsza cyfra
				{
					number_1=9;
					was_any_digit =1;
					counter = 1;
				}else
				{
					number_1 = 10*number_1+9;
					was_any_digit = 1;
				}
			break;
			case 10://Gdy wcisnieto "+"
				number_2 = number_1;
				number_1=0;
				operation = 1;
				counter = 0;
				LCD_GoTo(0,1);
				LCD_WriteText("+");

			break;
			case 11://Gdy wcisnieto "-"
				number_2 = number_1;
				number_1=0;
				operation = 2;
				counter = 0;
				LCD_GoTo(0,1);
				LCD_WriteText("-");

			break;
			case 12://Gdy wcisnieto "="
				if( operation == 1) // dodawanie
				{
					number_1 = number_1 +number_2; //Wynik
				}else if(operation == 2) // odejmowanie
				{
					number_1 = number_2 - number_1; //Wynik
				}
				operation = 0;
				counter = 0;
				was_any_digit = 1;

				LCD_GoTo(0,1);
				LCD_WriteText("=");
			break;
			}
			_delay_ms(100); //Program czeka 0.1s
			if(was_any_digit == 1) //Jezeli wpisano minimum 1 cyfre
			{
				LCD_Clear(); //Czyszczenie ekranu
				LCD_ShowNumber(number_1); //Wyswietla wpisana liczbe
				was_any_digit = 0;
				break;
			}

		}

	}
}
