CUR_PWD := $(shell pwd)

all: maxtweeter


maxtweeter: maxTweeter.c
	gcc -Wall -Werror -o maxtweeter maxTweeter.c

clean:
	@echo "CLEAN	$(CUR_PWD)"
	rm -f maxtweeter

