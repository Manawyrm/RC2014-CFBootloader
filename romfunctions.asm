SECTION ROMFUNCTIONS
org 16320

extern _cf_init
extern _cf_read
extern _putstring_uart

PUBLIC _romfunctions
_romfunctions:
	defw _cf_init 
	defw _cf_read 
	defw _putstring_uart
	defs 58
