deps_dirs := $(sort $(dir $(wildcard ./deps/*/)))
headers := $(foreach dep_dir,$(deps_dirs), -I $(dep_dir)headers)
deps := $(subst /,,$(subst ./deps/,,$(deps_dirs)))
objects := $(foreach dep,$(deps), ./deps/$(dep)/obj/$(dep).o) ./obj/gsip.o

all:
	$(foreach dep,$(deps),\
		mkdir -p ./deps/$(dep)/obj && \
	        $(CC) -Wall -c ./deps/$(dep)/src/$(dep).c -o ./deps/$(dep)/obj/$(dep).o $(headers) && ) true
	mkdir -p ./obj
	$(CC) -Wall -c gsip.c -o ./obj/gsip.o $(headers)
	mkdir -p ./bin
	$(CC) -Wall -o ./bin/gsip $(objects)

clean:
	$(foreach deps_dir,$(deps_dirs), rm -r $(deps_dir)obj && ) true
	rm -r ./obj
	rm -r ./bin
