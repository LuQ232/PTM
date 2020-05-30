#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "HD44780.h"


#ifndef _BV
#define _BV(bit)				(1<<(bit))
#endif

#ifndef inb
#define	inb(addr)			(addr)
#endif

#ifndef outb
#define	outb(addr, data)	addr = (data)
#endif

#ifndef sbi
#define sbi(reg,bit)		reg |= (_BV(bit))
#endif

#ifndef cbi
#define cbi(reg,bit)		reg &= ~(_BV(bit))
#endif

#ifndef tbi
#define tbi(reg,bit)		reg ^= (_BV(bit))
#endif

//  Gotowe zaimplementowane:
 #define 	bit_is_set(sfr, bit)   (_SFR_BYTE(sfr) & _BV(bit))
 #define 	bit_is_clear(sfr, bit)   (!(_SFR_BYTE(sfr) & _BV(bit)))
 #define 	loop_until_bit_is_set(sfr, bit)   do { } while (bit_is_clear(sfr, bit))
 #define 	loop_until_bit_is_clear(sfr, bit)   do { } while (bit_is_set(sfr, bit))


// MIN/MAX/ABS macros
#define MIN(a,b)			((a<b)?(a):(b))
#define MAX(a,b)			((a>b)?(a):(b))
#define ABS(x)				((x>0)?(x):(-x))



char cyfra[10] = { 0b1111110, 0b0110000, 0b1101101, 0b1111001,0b0110011,
		0b1011011,0b1011111, 0b1110000, 0b1111111,0b1111011};

volatile int digit1 = 0; // zmienna pomocna do wyswietlania czasu na LCD
volatile int digit2 = 0; // zmienna pomocna do wyswietlania czasu na LCD
volatile int digit3 = 0; // zmienna pomocna do wyswietlania czasu na LCD
volatile int digit4 = 0; // zmienna pomocna do wyswietlania czasu na LCD
volatile int number_counter = 0; // licznik potrzebny do podprogramu 2.Liczby

volatile int stopwatch_counter = 0;
volatile int watch_counter = 0;
volatile int menu_counter = 1; // zmienna przechowujaca aktualny numer wybrany z menu
//////////////////////////////////////////
// aktualny stan programu
// 0- menu , 1 - info
// 2- liczby, 3 - stoper
// 4- zegar, 5- miernik
// 6- koniec programu
volatile int state = 0;
//////////////////////////////////////////

volatile int last_state = 0; // zmienna pomocnicza - potrzebne do odswiezania menu


// Funkcja zwracajaca numer nacisnietego przycisku
int which_button_is_pressed()
{
	if(bit_is_clear(PIND,PD1))
	{
		return 1; // PRZYCISK UP
	}else if(bit_is_clear(PIND,PD2))
	{
		return 2; // PRZYCISK DOWN
	}else if(bit_is_clear(PIND,PD3))
	{
		return 3; // PRZYCISK X
	}else if(bit_is_clear(PIND,PD4))
	{
		return 4; // PRZYCISK OK
	}
	return 0; // BLAD!
}

//Funkcja inkrementujaca // dekrementujaca menu_counter
void update_menu_counter()
{
	switch(which_button_is_pressed()) // jezeli nacisnieto przycisk
				{
				case 1:	// przycisk UP
					if(menu_counter<5) // zabezpieczenie przed przekroczeniem menu
					{
						menu_counter++; // inkrementacja
					}

				break;
				case 2:// przycisk DOWN
					if(menu_counter>1)// zabezpieczenie przed przekroczeniem menu
					{
						menu_counter--; // dekrementacja
					}

				break;
				}
}
// Funkcja wyswietlajaca info
void show_info()
{
	LCD_Clear();
	LCD_Home();
	LCD_WriteText("Program PTM 2020");
	LCD_GoTo(0,1);
	LCD_WriteText("249472");
	_delay_ms(4000);
	state = 0;
	last_state = 0;
	LCD_Clear();
}

// Funkcja  informujaca uzytkownika o koncu programu
void end_of_program()
{
	LCD_Clear();
	LCD_WriteText("KONIEC PROGRAMU");
	_delay_ms(1000);
	LCD_Clear();
}

// Funkcja wyswietlajaca dana opcje menu na  LCD
void show_menu ()
{
	switch(menu_counter)
		{
				case 1:
					LCD_WriteText("1. Info");
				break;
				case 2:
					LCD_WriteText("2. Liczby");
				break;
				case 3:
					LCD_WriteText("3. Stoper");
				break;
				case 4:
					LCD_WriteText("4. Zegar");
				break;
				case 5:
					LCD_WriteText("5. Miernik");
				break;
		}
}

//Wyswietla na wyswietlaczu 7 segmentowym cyfre z argumentu
void seg7ShowCyfra(uint8_t cyfraDoWyswietlenia) {
	PORTC = cyfra[cyfraDoWyswietlenia]; //odpowiednie ustawienie calego portu Cs
}

//Funkcja realizujaca 2 podprogram - liczy
void numbers()
{
	int tmp = 1;
	while(tmp)
	{
		LCD_Clear();
		LCD_ShowNumber(number_counter);
		if(number_counter%2  == 0) //Jezeli parzysta
		{
			cbi(PORTA,PA3);
			sbi(PORTA,PA2);
		}
		if(number_counter%2 == 1) // Jezeli nieparzysta
		{
			cbi(PORTA,PA2);
			sbi(PORTA,PA3);
		}
		if(number_counter == 2 ||number_counter ==3 ||number_counter ==5 || number_counter ==7||
		number_counter == 11 ||number_counter ==13 ||number_counter ==17 || number_counter ==19||
		number_counter == 23 ||number_counter ==29 ||number_counter ==31 || number_counter ==37||
		number_counter == 41 ||number_counter ==43 ||number_counter ==47)
		{
			sbi(PORTA, PA4);
		}else
		{
			cbi(PORTA, PA4);
		}

		if(number_counter<10)
		{
			seg7ShowCyfra(number_counter);
		}else
		{
			PORTC =0b0000001;
		}
		_delay_ms(50);

		switch(which_button_is_pressed()) // jezeli nacisnieto przycisk
						{
						case 1:	// przycisk UP
							if(number_counter<50) // zabezpieczenie przed przekroczeniem menu
							{
								number_counter++; // inkrementacja
							}

						break;
						case 2:// przycisk DOWN
							if(number_counter>0)// zabezpieczenie przed przekroczeniem menu
							{
								number_counter--; // dekrementacja
							}
						break;
						case 3:
							tmp = 0;
							state = 0;
							last_state = -1;
							PORTC =0b0000000; // Zgaszenie wysw. 7 segm
							cbi(PORTA,PA2);// Zgaszenie led1
							cbi(PORTA,PA3);// Zgaszenie led2
							cbi(PORTA,PA4);// Zgaszenie led3
						break;
						}
	}
	LCD_Clear();

}


//Inicjalizacja Timer1 do wywolywania przerwan
void TimerInit() {
	//Wybranie trybu pracy CTC z TOP OCR1A
	sbi(TCCR1B,WGM12);
	//Wybranie dzielnika czestotliwosci
	sbi(TCCR1B,CS12);  // dzielnik 256
	//Zapisanie do OCR1A wartosci odpowiadajacej 0,1s
	OCR1A = 3125;
	//Uruchomienie przerwania OCIE1A
	sbi(TIMSK,OCIE1A);
}





//Funkcja realizujaca 3 podprogram - stoper
void stopwatch()
{
	LCD_Clear();
	LCD_WriteText("00:00 PRESS OK");
	LCD_GoTo(0,1);
	LCD_WriteText("TO START");
	int stopwatch_first_press = 0;
	while(1)
	{
		_delay_ms(50);
		if(which_button_is_pressed() == 4 ) // Jezeli nacisnieto OK
		{

			if(stopwatch_first_press == 0) // jezeli to pierwsze nacisniecie przycisku
			{
				TimerInit(); // wlaczenie timera
				stopwatch_first_press = 1; // zmiana statusu
			}else if(stopwatch_first_press == 1)  // jezeli to nie pierwsze nacisniecie przycisku( timer pracuje)
			{
				cbi(TIMSK,OCIE1A); // zatrzymanie timera
				stopwatch_counter = 0; // wyzerowanie licznika
				stopwatch_first_press = 0; // zmiana statusu
			}
		}else if(which_button_is_pressed() == 3)
		{
			stopwatch_first_press = 0; // zmiana statusu
			stopwatch_counter = 0; // wyzerowanie licznika
			cbi(TIMSK,OCIE1A); // zatrzymanie timera
			state = 0;
			last_state = -1;
			break;
		}

	}

}



//Funkcja realizujaca 4 podprogram - zegar
void watch()
{
	LCD_Clear();
	LCD_WriteText("00:00 PRESS OK");
	LCD_GoTo(0,1);
	LCD_WriteText("TO START");
	while(1)
	{
		_delay_ms(50);
		if(which_button_is_pressed() == 4 ) // Jezeli nacisnieto OK
		{
				TimerInit(); // wlaczenie timera
				stopwatch_counter = 0; // wyzerowanie licznika
		}else if(which_button_is_pressed() == 3)
		{
			stopwatch_counter = 0; // wyzerowanie licznika
			cbi(TIMSK,OCIE1A); // zatrzymanie timera
			PORTC =0b0000000; // Zgaszenie wysw. 7 segm
			cbi(PORTA,PA4);
			state = 0;
			last_state = -1;
			break;
		}

	}

}

//Komparator Analogowy
int compare(uint16_t value)
{
	if(value>=250)	// Jezeli wartosc wieksza niz 2,5 V
	{
		return 1;
	}else
	{
		return 0;
	}
}

//Funkcja zwracajaca wartosc odczytana z przetwornika A/D
uint16_t ADC_10bit()
{
	sbi(ADCSRA,ADSC); //uruchomienie pojedynczego pomiaru – ustawienie bitu
	cbi(ADCSRA,ADSC); //oczekiwanie na zakoñczenie pomiaru – oczekiwanie na wyzerowanie bitu
	return ADC;
}

//Funkcja zamieniajaca wartosc odczytana z przetwornika A/D na liczbe volt( 4,23V -> 423)
uint16_t ADC_to_volt(uint16_t adc_value)
{
	return adc_value*0.4888;   // 500/1023 == 0.4888
}

//Funkcja wyswietlajaca dane miernika na LCD
void display_voltmeter_data()
{
	LCD_GoTo(0, 0);		//Ustawienie kursora na poczatku 1 linii LCD
	LCD_WriteText("ADC = ");
	LCD_ShowNumber(ADC_10bit());	//Wyswietlenie odczytanej liczby voltow
	LCD_GoTo(0, 1);		//Ustawienie kursora na poczatku 2 linii LCD

	digit1 = ADC_to_volt(ADC_10bit())/100;
	digit2 = (ADC_to_volt(ADC_10bit()) - digit1*100)/10;
	digit3 = (ADC_to_volt(ADC_10bit()) - digit1*100)%10;
	LCD_ShowNumber(digit1);
	LCD_WriteText(",");
	LCD_ShowNumber(digit2);
	LCD_ShowNumber(digit3);
	LCD_WriteText("V");
}
//Funkcja realizujaca 5 podprogram - miernik
void voltmeter()
{

		LCD_Clear();
		while(1) {
				display_voltmeter_data();
				if(which_button_is_pressed() == 3)
				{
					LCD_Clear();	// Czyszczenie ekranu
					state = 0;
					last_state = -1;
					break;
				}
				_delay_ms(50);	// delay
				LCD_Clear();	// Czyszczenie ekranu
			}
}
//Inicjalizacja portow do obs³ugi wyswietlacza 7 segmentowego
void seg7Init() {
	//Inicjalizacja kolumn
	DDRC = 0b11111111; //Ustawia wyprowadzenia od PC0 do PC7 do pracy jako wyjscie
	//Inicjalizacja segmentu
	PORTC= 0b0000000;
}



void Led_init()
{
	sbi(DDRA,PA2);
	sbi(DDRA,PA3);
	sbi(DDRA,PA4);
	cbi(PORTA,PA2);
	cbi(PORTA,PA3);
	cbi(PORTA,PA4);
}


//Funkcja Inicjalizujaca przetwornik A/D
void ADC_init()
{
	sbi(ADMUX,REFS0); //konfiguracja napiêcia referencyjnego – wybraæ napiêcie AVCC
	sbi(ADCSRA,ADPS0); //konfiguracja podzielnika czêstotliwoœci dla uk³adu przetwornika  (16??)
	sbi(ADCSRA,ADEN ); //uruchomienie uk³adu przetwornika

	//TYCH LINII PONIZEJ MOZE NIE BYC BO INITIAL VALUE == 0
	//konfiguracja/wybór kana³u/pinu na którym bêdzie dokonywany pomiar ||WYBRANO PA0 czyli ADC0
	cbi(ADMUX,MUX0);
	cbi(ADMUX,MUX1);
	cbi(ADMUX,MUX2);
	cbi(ADMUX,MUX3);
	cbi(ADMUX,MUX4);
}




int main() {


	Led_init(); // Inicjalizacja 3 LEDow
	seg7Init(); // Inicjalizacja wysw. 7 seg.
	ADC_init();			//Inicjalizacja przetwornika A/D
	LCD_Initalize();	//Inicjalizacja ekranu LCD
	LCD_Home(); 	//Przywraca poczatkowe wspolrzedne wyswietlacza
	sei();		//Wlaczenie przerwan
	while(1) {



			if(state == 0)  // MENU
			{

				// TUTAJ MOZE BYC SPRAWDZANIE CZY COS SIE ZMIENILO WTEDY WYSWIETALM
				if(last_state != menu_counter) // JEELI TZREBA OODSIEZYC MENU (JAKAS ZMIANA)
				{
					LCD_Clear();	// Czyszczenie LCD
					show_menu();	// wyswietlenie opcji menu na LCD
					last_state = menu_counter; // ustawienie zmiennej last_state
				}

				if(which_button_is_pressed() == 4) // JEZELI WCISNIETO 'OK'
				{
					state = menu_counter;
				}
			}else if(state == 1) // INFO
			{
				show_info();
			}else if(state == 2)// LICZBY
			{
				numbers();
			}else if (state == 3)// STOPER
			{
				stopwatch();
			}else if (state == 4)// ZEGAR
			{
				watch();
			}else if (state == 5)// MIENRIK
			{
				voltmeter();
			}


			update_menu_counter(); // zaaktualizowanie zmiennej menu_counter
			_delay_ms(50);
		}
}



//Funkcja uruchamiana z przerwaniem po przepelnieniu licznika w timer1
ISR(TIMER1_COMPA_vect) {
	LCD_Clear(); // Czyszczenie

//Odpowiednie przeliczenie cyfr/
	if(state == 3 ) // STOPER
	{
			digit1 = 0;
			digit2 = stopwatch_counter;
			digit3 = digit2/10;
			digit2 -= digit3*10;

			digit4 = digit3/10;
			digit3-= digit4*10;
	}

	if(state == 4) // ZEGAR
	{
		digit1 = stopwatch_counter/10;

		digit2 = digit1 / 10;
		digit1 -= digit2*10;

		digit3 = digit2/6;
		digit2 -= digit3*6;

		digit4 = digit3 / 10;
		digit3 -= digit4*10;


	}

///////////////////////////////

/////////Wyswietlanie czasu ///
	LCD_ShowNumber(digit4);
	LCD_ShowNumber(digit3);
	LCD_WriteText(":");
	LCD_ShowNumber(digit2);
	LCD_ShowNumber(digit1);
///////////////////////////////

	if(state == 4)
	{
		seg7ShowCyfra(digit1);
		if(stopwatch_counter%10 == 0)
			{
				sbi(PORTA,PA4);
			}
		if(stopwatch_counter%10   == 2)
		{
			cbi(PORTA,PA4);
		}

	}





	stopwatch_counter+=1; // inkrementacja licznika timera


}
