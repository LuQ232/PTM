#include "SET.h"

int main() {
	sbi(DDRD, PD7);

	while (1) {
		sbi(PORTD, PD7); // 1
		_delay_ms(500);
		cbi(PORTD, PD7); // 0
		_delay_ms(500);

	}
}
