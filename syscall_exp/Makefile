
INCLUDES = .

all: ts-dispatch

ts-dispatch: ts-dispatch.cpp translate.cpp
	g++ $(INCLUDES:%=-I%) -o $@ $^

psudo: psudo.cpp translate.cpp
	g++ $(INCLUDES:%=-I%) -o $@ $^
