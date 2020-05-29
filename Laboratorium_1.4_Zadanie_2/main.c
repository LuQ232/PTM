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


void ADC_init()
{
	sbi(ADMUX,REFS0); //konfiguracja napi�cia referencyjnego � wybra� napi�cie AVCC
	sbi(ADCSRA,ADPS0); //konfiguracja podzielnika cz�stotliwo�ci dla uk�adu przetwornika  (16??)
	sbi(ADCSRA,ADEN ); //uruchomienie uk�adu przetwornika

	//TYCH LINII PONIZEJ MOZE NIE BYC BO INITIAL VALUE == 0
	//konfiguracja/wyb�r kana�u/pinu na kt�rym b�dzie dokonywany pomiar ||WYBRANO PA0 czyli ADC0
	cbi(ADMUX,MUX0);
	cbi(ADMUX,MUX1);
	cbi(ADMUX,MUX2);
	cbi(ADMUX,MUX3);
	cbi(ADMUX,MUX4);
}

uint16_t ADC_10bit()
{
	sbi(ADCSRA,ADSC); //uruchomienie pojedynczego pomiaru � ustawienie bitu
	cbi(ADCSRA,ADSC); //oczekiwanie na zako�czenie pomiaru � oczekiwanie na wyzerowanie bitu
	return ADC;

}

uint16_t ADC_to_volt(uint16_t adc_value)
{
	return adc_value*0.4888;   // 500/1023 == 0.4888
}

int main() {
	ADC_init();
	LCD_Initalize();	//Inicjalizacja ekranu LCD
	LCD_Home(); 	//Przywraca poczatkowe wspolrzedne wyswietlacza
	while(1) {
		LCD_GoTo(0, 1);	//Ustawienie kursora na poczatku 1 linii LCD
		LCD_ShowNumber(ADC_to_volt(ADC_10bit()));
		_delay_ms(50);
		LCD_Clear();
	}

}
