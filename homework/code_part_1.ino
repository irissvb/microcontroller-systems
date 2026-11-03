#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// ===== TASK 1 (sequence) =====
uint8_t seq[] = {0b001, 0b100, 0b001, 0b100, 0b011, 0b110, 0b011, 0b110};
uint8_t seq_idx = 0;

// ===== TASK 2 (running line) =====
uint8_t run_line[] = {(1 << PB5), (1 << PB4), (1 << PB3)};
uint8_t run_index = 0;
uint8_t is_running = 1;

// ===== BUTTON STATES (for edge detection) =====
uint8_t prev_M5 = 1;
uint8_t prev_M4 = 1;

// ===== TIMER =====
uint16_t tick = 0;   // increments every 10ms

void setup_registers() {  
  // LED8-LED10 as outputs (PC2, PC1, PC0)
  DDRC |= 0b00000111;
  // M5 button as input (PD3)
  DDRD &= ~(1 << PD3);
  
  // LED1-LED3 as outputs (PB5, PB4, PB3)
  DDRB |= 0b00111000;
  // M4 button as input(PD4)
  DDRD &= ~(1 << PD4);

  // Enable pull-ups (Buttons are LOW when pressed)
  PORTD |= (1 << PD3) | (1 << PD4);
}

int main(void) {
  setup_registers();

  while (1) {
    // small fixed loop delay
    _delay_ms(10);
    tick++;

    // read the buttons
    uint8_t curr_M5 = (PIND & (1 << PD3));
    uint8_t curr_M4 = (PIND & (1 << PD4));

    // TASK 1: check if M5 is pressed (pin is low)
    if (prev_M5 && !curr_M5) {  // falling edge (pressed)
      PORTC = (PORTC & ~0x07) | seq[seq_idx];
      seq_idx = (seq_idx + 1) % 8;
    }

    // TASK 2: check if M4 is pressed (pin is low)
    if (prev_M4 && !curr_M4) {
      is_running = !is_running;
    }

    // update previous states
    prev_M5 = curr_M5;
    prev_M4 = curr_M4;

    // TASK 2: running line (every 500 ms)
    if (tick >= 50) {   // 50 * 10ms = 500ms
      tick = 0;
      if (is_running) {
        // Clear LEDs
        PORTB &= ~((1 << PB5) | (1 << PB4) | (1 << PB3));

        // Set current LED
        PORTB |= run_line[run_index];

        // Move to next
        run_index = (run_index + 1) % 3;
      }
    }
  }

    return 0;
}