

build:
	$(MAKE) -C lib_xs3_math build
	$(MAKE) -C test all

clean:
	$(MAKE) -C lib_xs3_math clean
	$(MAKE) -C test clean

docs:
	$(MAKE) -C lib_xs3_math docs

all: build docs