# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

USER_LIB_PATH = ./libraries
ARDUINO_LIBS = SPI Ethernet # DMXSerial
ARDUINO_DIR = /Applications/Arduino.app/Contents/Java
AVRDUDE = /usr/local/bin/avrdude
BOARD_TAG = uno
include /Users/brian/build/Arduino-Makefile/Arduino.mk
