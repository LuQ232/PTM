#include "SET.h"

int main() {
	DDRD = 0b11111111; //Ustawia wyprowadzenia od PD0 do PD7 do pracy jako wyjscie

	while (1){
				PORTD = 0b11111111; //Ustawia stan wysoki na wyprowdzeniach od PD0 do PD7
				_delay_ms(1000);	// Program czeka 1 sekunde
				PORTD = 0b00000000; //Ustawia stan niski na wyprowdzeniach od PD0 do PD7
				_delay_ms(1000);	// Program czeka 1 sekunde
			 }
}
