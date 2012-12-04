CC = avr-gcc
OBJCOPY = avr-objcopy
DUDE = avrdude

CPU = atmega8
F_CPU=16000000UL

CFLAGS = -Wall -Os -mmcu=$(CPU) -DF_CPU=$(F_CPU)
OBJFLAGS = -j .text -j .data -O ihex
DUDEFLAGS = -p $(CPU) -c stk500v1 -v -b 19200 -F

SOURCE = ArdU1000

# Object files for the firmware 
OBJECTS = $(SOURCE).o

# By default, build the firmware and command-line client, but do not flash
all: $(SOURCE).hex 

# With this, you can flash the firmware by just typing "make flash" on command-line
flash: $(SOURCE).hex
	$(DUDE) $(DUDEFLAGS) -U flash:w:$<

clean:
	$(RM) *.o *.hex *.elf

# From .elf file to .hex
%.hex: %.elf
	$(OBJCOPY) $(OBJFLAGS) $< $@

# $(SOURCE).elf requires additional objects to the firmware, not just $(SOURCE).o
$(SOURCE).elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# From C source to .o object file
%.o: %.c	
	$(CC) $(CFLAGS) -c $< -o $@

