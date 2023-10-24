# variabile
CC:=gcc # compilator C
CFLAGS:=-Ofast -Ivendor/ # argumente compilator C
LDFLAGS:=-lm # argumente linker

# retete
.PHONY: pngExporter

pngExporter: vendorLibs
	$(CC) $(CFLAGS) pngExporter.c vendor/hgt-lib/hgtlib.o $(LDFLAGS) -o pngExporter.elf

all: pngExporter

vendorLibs:
	$(MAKE) -C vendor