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

volatile char znaki[4];

char cyfra[10] = { 0b1111110, 0b0110000, 0b1101101, 0b1111001,0b0110011,
		0b1011011,0b0011111, 0b1110000, 0b1111111,0b1110011};

//Inicjalizacja Timer1 do wywolywania przerwania z czêstotliwoœci¹ 2Hz
void TimerInit() {

	//Wybranie trybu pracy CTC z TOP OCR1A
	TCCR1B |= (1 << WGM12);
	//Wybranie dzielnika czestotliwosci
	TCCR1B |= (1 << CS12);
	//Zapisanie do OCR1A wartosci odpowiadajacej 0,5s
	OCR1A = 15625;
	//Uruchomienie przerwania OCIE1A
	TIMSK |= (1 << OCIE1A);

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
	PORTC = cyfra[cyfraDoWyswietlenia];
}


//Funkcja uruchamiana z przerwaniem po przepelnieniu licznika w timer1
ISR(TIMER1_COMPB_vect) {
	tbi(PORTB,PB0);
}


int main() {
	DDRB = 0b00000001; //Ustawia wyprowadzenia od PB0 do PB7 do pracy jako wyjscie
	TimerInit();
	seg7Init();

	sei(); //funkcja uruchamia globalne przerwania

	while (1) {
		for (uint8_t i = 0; i < 10; i++) {
			seg7ShowCyfra(i);
			_delay_ms(2000);
		}
	}

	return 0;
}


