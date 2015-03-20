.PRECIOUS : got/%.s got/%.i %.assemble.log %.link.log %.compile.log

CPP		= gcc-4.6 -E

# You are providing the ARM compiler
ARM_CC	?= ../bin/compiler

# Do you have ARM binutils as well?
ARM_AS	= arm-none-eabi-as
ARM_GCC	= arm-none-eabi-gcc

###############################
## General rules for building a test-case

got/%.i						: input/%.c
	# We use a special include dir to get the simplified stdio.h
	$(CPP) -nostdinc -I./include -o got/$*.i input/$*.c

got/%.s got/%.compile.log	: got/%.i
	echo "Compile"
	$(ARM_CC) -S -o got/$*.s got/$*.i 2>&1 | tee got/$*.compile.log

# These are all things you can do if you set up ARM cross-compilers

got/%.o got/%.assemble.log	: got/%.s
	$(ARM_AS) -o $@ $< 2>&1 | tee got/$*.assemble.log

got/%.exe got/%.link.log	: got/%.o
	$(ARM_GCC) -o $@ $< 2>&1 | tee got/$*.link.log

got/%.input					:
	if [ -e input/$*.input ] ; then cp input/$*.input $@; else echo "" > $@; fi

got/%.output				: got/%.exe got/%.input
	scp -i -P 55555 got/%.{exe,input} root@localhost:~/test/got/
	ssh -i -P 55555 root@localhost << ENDSSH
	cd test
	cat got/$*.input | got/$*.exe > got/$*.output
	ENDSSH
	scp -i -P 55555 root@localhost:~/test/got/ got/%.output


###############################
## Reference rules

ref/%.exe					: input/%.c
	$(CC) -o $@ $<

ref/%.input					:
	if [ -e input/$*.input ] ; then cp input/$*.input $@; else echo "" > $@; fi
	
ref/%.output				: ref/%.exe ref/%.input
	cat ref/$*.input | ref/$*.exe > ref/$*.output

###############################
TEST_INPUTS	:=$(wildcard input/*.c)
TEST_NAMES	:=$(patsubst input/%.c,%,$(TEST_INPUTS))

TEST_ASMS	:=$(foreach i,$(TEST_NAMES), got/$(i).s)
TEST_EXES	:=$(foreach i,$(TEST_NAMES), got/$(i).exe)

build						: $(TEST_EXES)

got/%.diff.log				: got/%.output ref/%.output
	diff $^ > $@

test						: got/$(TEST_NAMES).diff.log
