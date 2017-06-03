CMD=arduino
BOARD=arduino:avr:nano:cpu=atmega328
#BOARD=arduino:avr:mega
PORT=/dev/ttyUSB0
MAIN=DeviceControl.ino
BAUD=115200

upload:
	$(CMD) --board $(BOARD) --upload $(MAIN)

verify:
	$(CMD) --verify $(MAIN)

view:
	cu -l $(PORT) -s $(BAUD)
