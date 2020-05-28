#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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

volatile uint8_t minuty, sekundy;
volatile uint16_t liczba7Seg;
volatile uint8_t counter = 0; // licznik potrzebny do wyswietlania cyfr na 7-seg.
volatile uint8_t first_press = 0;
volatile char znaki[4];

char cyfra[10] = { 0b1111110, 0b0110000, 0b1101101, 0b1111001,0b0110011,
		0b1011011,0b1011111, 0b1110000, 0b1111111,0b1111011};


//Inicjalizacja Timer1 do wywolywania przerwania z czêstotliwoœci¹ 2Hz
void TimerInit() {
	//Wybranie trybu pracy CTC z TOP OCR1A
	sbi(TCCR1B,WGM12);
	//Wybranie dzielnika czestotliwosci
	sbi(TCCR1B,CS12);  // dzielnik 256
	//sbi(TCCR1B,);
	//Zapisanie do OCR1A wartosci odpowiadajacej 0,5s
	OCR1A = 15625;
	//Uruchomienie przerwania OCIE1A
	sbi(TIMSK,OCIE1A);
}

//Inicjalizacja portow do obs³ugi wyswietlacza 7 segmentowego
void seg7Init() {
	//Inicjalizacja kolumn
	DDRC = 0b11111111; //Ustawia wyprowadzenia od PC0 do PC7 do pracy jako wyjscie
	//Inicjalizacja segmentu
	PORTC= 0b00000000;
}

void button_init()
{
	//General Interrupy Controller Register
	// uruchomienie przerwania dla przycisku
	sbi(GICR,INT0);	// ustawienie pinu PD2 (INT0)

	//MCU CONTROL REGISTER
	sbi(MCUCR,ISC11); // ustawienie wyzwalacza zbocza opadaj¹cego
}

//Wyswietla na wyswietlaczu 7 segmentowym cyfre z argumentu
void seg7ShowCyfra(uint8_t cyfraDoWyswietlenia) {
	PORTC = cyfra[cyfraDoWyswietlenia]; //odpowiednie ustawienie calego portu Cs
}


int main() {
	sbi(DDRB,PB0);
	seg7Init();
	button_init();

	sei();
	while(1)
	{
	}
	return 0;
}
//Funkcja uruchamiana z przerwaniem po nacisnieciu przycisku
ISR(INT0_vect){
	if(first_press == 0) // jezeli to pierwsze nacisniecie przycisku
	{
		TimerInit(); // wlaczenie timera
		first_press = 1; // zmiana statusu
	}else if(first_press == 1)  // jezeli to nie pierwsze nacisniecie przycisku( timer pracuje)
	{
		cbi(TIMSK,OCIE1A); // zatrzymanie timera
		counter = 0; // wyzerowanie licznika
		first_press = 0; // zmiana statusu
	}

}

//Funkcja uruchamiana z przerwaniem po przepelnieniu licznika w timer1
ISR(TIMER1_COMPA_vect) {

	if(counter >= 10) // jezeli odliczanie dojdzie do zera
		{
			sbi(PORTB,PB0); // zapalenie diody led
			if(counter == 15)
			{
				cbi(PORTB,PB0); // zgaszenie diody led
				counter = -1; // wyzerowanie licznika
			}
		}else
		{
			seg7ShowCyfra(9-counter); // wyswietlenie cyfry na wysw. 7-seg
		}
	counter+=1; // inkrementacja licznika

}
