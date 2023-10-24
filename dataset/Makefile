# variabile
CC:=gcc # compilator C
CFLAGS:=-std=gnu11 -Ofast -Ivendor/ # argumente compilator C
LDFLAGS:=-lm # argumente linker

# retete
.PHONY: pngExporter stlExporter

all: pngExporter stlExporter

pngExporter: vendorLibs
	$(CC) $(CFLAGS) pngExporter.c vendor/hgt-lib/hgtlib.o $(LDFLAGS) -o pngExporter.elf

stlExporter: vendorLibs
	$(CC) $(CFLAGS) stlExporter.c vendor/hgt-lib/hgtlib.o $(LDFLAGS) -o stlExporter.elf

vendorLibs:
	$(MAKE) -C vendor