# File:   Makefile
# Author: Randipa Gunathilake, Mike Mika
# Date:   07/10/2019
# Descr:  Makefile for the boxing game

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -I../../utils -I../../fonts -I../../drivers -I../../drivers/avr
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: game.out

# Compile: create object files from C source files.
game.o: game.c game.h players.h ir_com.h ../../drivers/avr/system.h ../../drivers/avr/ir_uart.h ../../drivers/display.h ../../drivers/navswitch.h ../../fonts/font3x5_1.h ../../utils/font.h ../../utils/pacer.h ../../utils/tinygl.h ../../drivers/avr/timer.h ../../drivers/button.h
	$(CC) -c $(CFLAGS) $< -o $@
# Compile: create object file from system source files.
system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@
# Compile: create object file from pacer source files where it wait for the specified length of time.
pacer.o: ../../utils/pacer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@
# Compile: create object file and complies code from navswitch it reads the polls the 5-way navigation switch from north south east west.
navswitch.o: ../../drivers/navswitch.c ../../drivers/avr/delay.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@
# create object file and complies code from ir_uart.c which in the ir_uart reads characters and complies it and generates the excecutable code.
ir_uart.o: ../../drivers/avr/ir_uart.c ../../drivers/avr/ir_uart.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/avr/timer0.h ../../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@
# create object file and complies code from tinygl.c which in the tinygl reads  and complies  it and generates the excecutable code for grahical effects on the matrix platform.
tinygl.o: ../../utils/tinygl.c ../../drivers/avr/system.h ../../drivers/display.h ../../utils/font.h ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@
# create object file and complies code from pio.c which in the pio reads  and complies  it and generates the excecutable code for general purpose I/O ports.
pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@
# create object file and complies code from font.c which in the font reads  and complies  it and generates the excecutable code for general size of fonts.
font.o: ../../utils/font.c ../../drivers/avr/system.h ../../utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@
# create object file and complies code from timer.c which in the timer reads  and complies  it and generates the excecutable code for The rate that the timer is incremented using macro TIMER_RATE.
timer.o: ../../drivers/avr/timer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@
# create object file and complies code from timer0.c which in the timer0 reads  and complies  it and generates the excecutable code for Timer/Counter0 is a general purpose, single channel, 8-bit
# Timer/Counter module.
timer0.o: ../../drivers/avr/timer0.c ../../drivers/avr/bits.h ../../drivers/avr/prescale.h ../../drivers/avr/system.h ../../drivers/avr/timer0.h
	$(CC) -c $(CFLAGS) $< -o $@
# create object file and complies code from usart1.c which in the timer0 reads  and complies  it and generates the excecutable code for usart1 which is a Routines for interfacing with the usart1 on an Atmega32u2
usart1.o: ../../drivers/avr/usart1.c ../../drivers/avr/system.h ../../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@
# create object file and complies code from display.c which in the display reads  and complies  it and generates the excecutable code for Bit-mapped display on the matrix platform.
display.o: ../../drivers/display.c ../../drivers/avr/system.h ../../drivers/display.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@
# create object file complies and excecutes code from ledmat.c which in the ledmat reads  drivers in defining matrix row and columns.
ledmat.o: ../../drivers/ledmat.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@
# create object file and complies code from prescale.c which in the prescale reads a number of the prescaler and complies it and generates the excecutable code for Prescaler selection for 8-bit timer/counters.
prescale.o: ../../drivers/avr/prescale.c ../../drivers/avr/prescale.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@
# create object file and complies code from players.c which draws each players as a figure and its movement moreover it complies it and generates the excecutable code.
players.o: players.c players.h ../../drivers/avr/system.h ../../utils/tinygl.h 
	$(CC) -c $(CFLAGS) $< -o $@
# create object file and complies code from ir_com.c which reads the transmitted either 1 or 0 it complies it and generates the excecutable code.
ir_com.o: ir_com.c ir_com.h game.h ../../drivers/avr/system.h ../../drivers/avr/ir_uart.h
	$(CC) -c $(CFLAGS) $< -o $@
# create object file and complies excecutable code from button.c which reads the state of the button 
button.o: ../../drivers/button.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/button.h
	$(CC) -c $(CFLAGS) $< -o $@
# create object file and complies and excecute code from led.c which in the led reads the matrix leds as High or Low.
led.o: ../../drivers/led.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/led.h
	$(CC) -c $(CFLAGS) $< -o $@
#  create object file and complies and excecute code from led.c which is useful for modulating the luiminance of an LED or beeping a piezo tweeter. 
spwm.o: ../../utils/spwm.c ../../drivers/avr/system.h ../../utils/spwm.h
	$(CC) -c $(CFLAGS) $< -o $@



# Link: create ELF output file from object files.
game.out: game.o system.o pacer.o navswitch.o ir_uart.o tinygl.o pio.o font.o timer.o timer0.o usart1.o display.o ledmat.o prescale.o players.o ir_com.o button.o led.o spwm.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@

# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) -f *.o *.out *.hex


# Target: program project.
.PHONY: program
program: game.out
	$(OBJCOPY) -O ihex game.out game.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash game.hex; dfu-programmer atmega32u2 start
