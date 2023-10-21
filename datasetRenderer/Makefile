.PHONY: vendor

all: vendor
	gcc -Ivendor/ parser.c vendor/hgt-lib/hgtlib.o -o parser

vendor:
	$(MAKE) -C vendor