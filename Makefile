# Nome do programa principal
PROG = main

# Defina uma variável DEPS com a lista das dependências .c (sem a extensão)
DEPS = display_lcd temp_sensor_dht11

# Porta de comunicação com o Arduino
PORT = /dev/ttyACM0

# Escolha uma otimização dentre as seguintes:
# -O0 -O1 -O2 -O3 -Os
OPTIMIZE = -Os -flto

# Configuração para o Arduino UNO
# -------------------------------
# Altere apenas se for utilizar outro modelo de Arduino
# Consulte o arquivo /arduino/avr/boards.txt para outras configurações
MCU_TARGET = atmega328p
CPU_FREQ = 16000000
UPLOAD_SPEED = 115200
UPLOAD_PROTOCOL = arduino

# Local do arquivo de configuração do avrdude
AVRDUDE_CONF = /etc/avrdude.conf

# ==========================================
# Não é necessário alterar as regras abaixo.
# ==========================================

OBJS    = $(PROG).o $(DEPS:=.o)
CC      = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS  = $(OPTIMIZE) -I. -g -Wall -mmcu=$(MCU_TARGET) -DF_CPU=$(CPU_FREQ)
ASMBLER = avra

.PHONY: all install clean

all: $(PROG).hex

test:
	@echo $(OBJS)

$(PROG).elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(PROG).hex: $(PROG).elf
	$(OBJCOPY) -O ihex $< $@

$(PROG).eep.hex: $(PROG).elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 --no-change-warnings -O ihex $< $@ \
	|| { echo empty $@ not generated; exit 0; }

install: $(PROG).hex $(PROG).eep.hex
	avrdude -C $(AVRDUDE_CONF) -p $(MCU_TARGET) -c $(UPLOAD_PROTOCOL) -P $(PORT) -b $(UPLOAD_SPEED) -D -U flash:w:$<:i -U eeprom:w:$(word 2,$^):i

clean:
	@rm -f $(PROG).elf $(PROG).eep.hex $(PROG).hex $(OBJS)

clean_all:
	@rm -f *.elf *.hex *.obj *.o

assembler:
	$(ASMBLER) $(PROG).asm

upload:
	avrdude -C $(AVRDUDE_CONF) -p $(MCU_TARGET) -c $(UPLOAD_PROTOCOL) -P $(PORT) -b $(UPLOAD_SPEED) -D -U flash:w:$(PROG).hex:i -U eeprom:w:$(PROG).eep.hex:i
