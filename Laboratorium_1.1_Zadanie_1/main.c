#include "SET.h"

int main() {

	sbi(DDRD, PD0); //Ustawia wyprowadzenie PD0 do pracy jako wyjscie
	sbi(DDRD, PD1); //Ustawia wyprowadzenie PD1 do pracy jako wyjscie
	sbi(DDRD, PD2); //Ustawia wyprowadzenie PD2 do pracy jako wyjscie
	sbi(DDRD, PD3); //Ustawia wyprowadzenie PD3 do pracy jako wyjscie
	sbi(DDRD, PD4); //Ustawia wyprowadzenie PD4 do pracy jako wyjscie
	sbi(DDRD, PD5); //Ustawia wyprowadzenie PD5 do pracy jako wyjscie
	sbi(DDRD, PD6); //Ustawia wyprowadzenie PD6 do pracy jako wyjscie
	sbi(DDRD, PD7); //Ustawia wyprowadzenie PD7 do pracy jako wyjscie


	sbi(PORTD,PD0); //Ustawia stan wysoki na wyprowdzeniu PD0
	sbi(PORTD,PD1); //Ustawia stan wysoki na wyprowdzeniu PD1
	sbi(PORTD,PD2); //Ustawia stan wysoki na wyprowdzeniu PD2
	sbi(PORTD,PD3); //Ustawia stan wysoki na wyprowdzeniu PD3
	sbi(PORTD,PD4); //Ustawia stan wysoki na wyprowdzeniu PD4
	sbi(PORTD,PD5); //Ustawia stan wysoki na wyprowdzeniu PD5
	sbi(PORTD,PD6); //Ustawia stan wysoki na wyprowdzeniu PD6
	sbi(PORTD,PD7); //Ustawia stan wysoki na wyprowdzeniu PD7


	return 0;
}
