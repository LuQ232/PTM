#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>



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

/*
 *  Gotowe zaimplementowane:
 #define 	bit_is_set(sfr, bit)   (_SFR_BYTE(sfr) & _BV(bit))
 #define 	bit_is_clear(sfr, bit)   (!(_SFR_BYTE(sfr) & _BV(bit)))
 #define 	loop_until_bit_is_set(sfr, bit)   do { } while (bit_is_clear(sfr, bit))
 #define 	loop_until_bit_is_clear(sfr, bit)   do { } while (bit_is_set(sfr, bit))

 */

// MIN/MAX/ABS macros
#define MIN(a,b)			((a<b)?(a):(b))
#define MAX(a,b)			((a>b)?(a):(b))
#define ABS(x)				((x>0)?(x):(-x))


#define START_TIMER0 TCCR1B |= (1 << WGM12) //wlacza timera
#define STOP_TIMER0  TCCR1B &= 0B11111000 //stopuje timer

uint8_t first_press = 0; // Czy guzik zostal wcisniety pierwszy raz
uint8_t counter = 0; // licznik potrzebny do wyswietlania cyfr na 7-seg.
uint8_t timer_working = 0; // zmienna informujaca o pracy licznika

volatile uint8_t minuty, sekundy;
volatile uint16_t liczba7Seg;

volatile char znaki[4];

char cyfra[10] = { 0b1111110, 0b0110000, 0b1101101, 0b1111001,0b0110011,
		0b1011011,0b1011111, 0b1110000, 0b1111111,0b1111011};

//Inicjalizacja Timer1 do wywolywania przerwania z czêstotliwoœci¹ 2Hz
void TimerInit() {

	//Wybranie trybu pracy CTC z TOP OCR1A
	TCCR1B |= (1 << WGM12);
	//Wybranie dzielnika czestotliwosci
	TCCR1B |= (1 << CS12);
	//Zapisanie do OCR1A wartosci odpowiadajacej 1s
	OCR1A = 15625;
	//Uruchomienie przerwania OCIE1A
	TIMSK |= (1 << OCIE1A);
}

//Inicjalizacja Przycisku do wywolywania przerwania
void ButtonInit()
{
	//General Interrupy Controller Register
	// uruchomienie przerwania dla przycisku
	GICR = 0b01000000; // ustawienie bitu 6 (INT0)

	//MCU CONTROL REGISTER
	MCUCR = 0x02; // ustawienie wyzwalacza zbocza opadaj¹cego
	// FALLING EDGE TRIGGER to 0x02
}

//Inicjalizacja portow do obs³ugi wyswietlacza 7 segmentowego
void seg7Init() {
	//Inicjalizacja kolumn
	DDRC = 0b11111111; //Ustawia wyprowadzenia od PC0 do PC7 do pracy jako wyjscie
	//Inicjalizacja segmentu
	PORTC= 0b00000000;
}

//Wyswietla na wyswietlaczu 7 segmentowym cyfre z argumentu
void seg7ShowCyfra(uint8_t cyfraDoWyswietlenia) {
	PORTC = cyfra[cyfraDoWyswietlenia]; //odpowiednie ustawienie calego portu Cs
}


//Funkcja uruchamiana z przerwaniem po przepelnieniu licznika w timer1
ISR(TIMER1_COMPB_vect) {

	if(counter == 10) // jezeli odliczanie dojdzie do zera
	{
		STOP_TIMER0;  // zatrzymanie timer'a
		timer_working = 0; // zmiana statusu o pracy timera
		tbi(PORTD,PD0); // zapalenie diody led
		_delay_ms(5000); // odczekanie 5s
		tbi(PORTD,PD0); // zgaszenie diody led
		counter = 0; // wyzerowanie licznika
		TimerInit(); // Wlaczenie timera
	}

	seg7ShowCyfra(9-counter); // wyswietlenie cyfry na wysw. 7-seg
	counter+=1; // inkrementacja licznika
}

//Funkcja uruchamiana z przerwaniem po nacisnieciu przycisku
ISR(INT0_vect)
{
	if(first_press == 0) // jezeli to pierwsze nacisniecie przycisku
	{
		TimerInit(); // wlaczenie timera
		first_press = 1; // zmiana statusu
		timer_working = 1; // zmiana statusu timera
	}else if(timer_working == 1) // jezeli timer pracuje
	{
		STOP_TIMER0; // zatrzymanie timera
		timer_working = 0; // zmiana statusu timera
	}else if(timer_working == 0) // jezeli timer nie pracuje
	{
		counter = 0; // wyzerowanie licznika
		TimerInit(); // wlaczenie timera
		timer_working = 1; // zmiana statusu timera
	}
}


int main() {
	DDRD = 0b00000001; //Ustawia wyprowadzenie PD0 do pracy jako wyjscie
	PORTD = 0b00000000; // Ustawia stan niski na ca³ym porcie D

	seg7Init(); // inicjalizacja wyswietlacza 7-segm.

	ButtonInit(); // //Inicjalizacja Przycisku do wywolywania przerwania


	sei(); //funkcja uruchamia globalne przerwania

	while (1) {
	}

	return 0;
}


