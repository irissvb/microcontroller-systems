# Microcontroller Systems - 2026

Coursework and lab exercises developed for the Microcontroller Systems course.

**Boards:** STM32F429I-DISC1 and Arduino Uno
**Tools:** STM32CubeIDE, STM32CubeMX, and Arduino IDE

## Repository structure

### STM32 labs

Each lab is a self-contained STM32CubeIDE project with its source code,
configuration files, drivers, and related course documents.

- [`lab-01`](labs/lab-01/) - GPIO configuration and LED blinking.
- [`lab-02`](labs/lab-02/) - General-purpose I/O at register level. The objective
  is to understand GPIO architecture and configuration on a 32-bit
  microcontroller and develop register-level programming skills.
- [`lab-03`](labs/lab-03/) - Internal and external interrupts. The objective
  is to understand interrupt controllers and configure interrupt-driven
  programs on a 32-bit microcontroller.
- [`lab-04`](labs/lab-04/) - Timers, counters, and ADC converters. The
  objective is to learn their architecture and configuration and use them in
  microcontroller applications.
- [`lab-05`](labs/lab-05/) - SPI serial communication. The objective is to
  configure SPI on the STM32F429I-DISC1 and implement loopback and
  master-slave communication examples.
- [`lab-06`](labs/lab-06/) - I2C serial communication. The objective is to
  configure one STM32F429I-DISC1 as master and another as slave, exchange
  one-byte and three-byte frames, and display the results on the onboard TFT
  screen.
- [`lab-06-master`](labs/lab-06-master/) - Master version of the lab 6 project.

### Arduino homework

- [`homework`](homework/) - Arduino code, schematics, instructions, and report.

## Notes

Build output and generated compiler files are excluded through `.gitignore`.
Open each STM32 project from its own folder in STM32CubeIDE.
