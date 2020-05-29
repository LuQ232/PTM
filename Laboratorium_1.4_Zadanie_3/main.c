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


int main() {
	sbi(DDRC,PC3);	// Inicjalizacja portu PC3 (LED)

	ADC_init();			//Inicjalizacja przetwornika A/D
	LCD_Initalize();	//Inicjalizacja ekranu LCD
	LCD_Home(); 	//Przywraca poczatkowe wspolrzedne wyswietlacza

	while(1) {
		LCD_GoTo(0, 0);		//Ustawienie kursora na poczatku 1 linii LCD
		LCD_ShowNumber(ADC_to_volt(ADC_10bit()));	//Wyswietlenie odczytanej liczby voltow
		LCD_GoTo(0, 1);		//Ustawienie kursora na poczatku 2 linii LCD

		if(compare(ADC_to_volt(ADC_10bit())) == 1) //Jezeli stan komparatora jest wysoki
		{
			LCD_WriteText("on");	// wyswietl napis on
			sbi(PORTC,PC3);			//Zapalenie diody Led
		}else{
			LCD_WriteText("off");	// wyswietl napis off
			cbi(PORTC,PC3);			//Zgaszenie diody Led
		}
		_delay_ms(50);	// delay
		LCD_Clear();	// Czyszczenie ekranu
	}

}
