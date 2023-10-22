.PHONY: vendor

all: vendor
	gcc -Ivendor/ pngExporter.c vendor/hgt-lib/hgtlib.o -o pngExporter

vendor:
	$(MAKE) -C vendor