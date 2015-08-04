# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

USER_LIB_PATH = ./libraries
ARDUINO_LIBS = SoftwareSerial DMXSerial
ARDUINO_DIR = /home/brian/build/arduino_ide/arduino-1.6.4
AVRDUDE = /usr/bin/avrdude
BOARD_TAG    = uno
include /home/brian/build/Arduino-Makefile/Arduino.mk
