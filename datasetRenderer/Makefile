.PHONY: vendor

all: vendor
	gcc -Ofast -Ivendor/ pngExporter.c -lm vendor/hgt-lib/hgtlib.o -o pngExporter

vendor:
	$(MAKE) -C vendor