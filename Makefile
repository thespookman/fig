CC = g++

COMPILER_FLAGS = -std=c++17 -Wall

INCLUDES = -Iinclude -I.

OUTPUT = libfig.a

.PHONY: all
all: $(OUTPUT)

OBJS = config.o lexer.conf.o value.o parser.tab.o

$(OUTPUT): $(OBJS)
	ar r $@ $^
	ranlib $@

lexer.conf.c: src/lexer.l parser.tab.cc
	flex -Pconf -o $@ $<

parser.tab.cc: src/parser.yy
	bison -d $<

-include $(OBJS:.o=.d)

parser.tab.o: parser.tab.cc
	$(CC) $(COMPILER_FLAGS) -c $<  $(INCLUDES)
	$(CC) -M $(COMPILER_FLAGS) $<  $(INCLUDES) > $*.d

lexer.conf.o: lexer.conf.c
	$(CC) $(COMPILER_FLAGS) -c $<  $(INCLUDES)
	$(CC) -M $(COMPILER_FLAGS) $<  $(INCLUDES) > $*.d

config.o: parser.tab.cc
%.o: src/%.cpp
	$(CC) $(COMPILER_FLAGS) -c $<  $(INCLUDES)
	$(CC) -M $(COMPILER_FLAGS) $<  $(INCLUDES) > $*.d

LINKER_FLAGS = -L. -lfig

TEST_PROG = test/test

TEST_SRC = src/test.cpp

$(TEST_PROG): $(TEST_SRC) $(OUTPUT)
	$(CC) $(COMPILER_FLAGS) $< -o $@ $(INCLUDES) $(LINKER_FLAGS)
	chmod +x $(TEST_PROG)

.PHONY: test
test: $(TEST_PROG)
	cd test && ./test
	

.PHONY: clean
clean:
	rm parser.tab.cc *.hh lexer.conf.c $(OBJS) $(OBJS:.o=.d) $(OUTPUT) $(TEST_PROG)
