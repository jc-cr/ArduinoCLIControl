# Configuration
TARGET = main
MCU = atmega328p
F_CPU = 16000000
UPLOAD_SPEED = 57600
AVRDUDE_PROGRAMMER = arduino
PORT = /dev/arduinonano

# Directories
OBJDIR = obj
BINDIR = bin
SRCDIR = src
LIBDIR = ledController

# Tools
CC = avr-gcc
CXX = avr-g++
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# Flags
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -w -Wl,--gc-sections -std=gnu11 -I$(LIBDIR)
CXXFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -w -Wl,--gc-sections -std=gnu++11 -I$(LIBDIR)
AVRDUDE_FLAGS = -V -p $(MCU) -c $(AVRDUDE_PROGRAMMER) -P $(PORT) -b $(UPLOAD_SPEED)
CFLAGS += -DF_CPU=16000000UL

# Files
SRC = $(wildcard *.cc) $(wildcard *.cpp) $(wildcard $(LIBDIR)/*.cc) $(wildcard $(LIBDIR)/*.cpp)
OBJ = $(addprefix $(OBJDIR)/,$(notdir $(SRC:.cc=.o)))

all: $(BINDIR)/$(TARGET).hex

$(OBJDIR)/%.o: %.cc
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(LIBDIR)/%.cc
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(LIBDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BINDIR)/$(TARGET).elf: $(OBJ)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BINDIR)/$(TARGET).hex: $(BINDIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

upload: $(BINDIR)/$(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$<:i

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all upload clean
