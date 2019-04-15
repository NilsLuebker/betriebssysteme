.PHONY: all clean compile run debug

all: run

clean:
	@echo -e "\e[32m=======================[Cleaning up]=======================\e[0m"
	rm -f ./$(file).o

compile:
	@echo -e "\e[32m========================[Compiling]========================\e[0m"
	gcc $(file).c -o $(file).o

debug:
	@echo -e "\e[32m==========================[Debug]=========================\e[0m"
	gcc -g $(file).c -o $(file).o
	gdb $(file).o

run: compile
	@echo -e "\e[32m=========================[Running]=========================\e[0m"
	./$(file).o
	@echo -e "\e[32m=======================[Cleaning up]=======================\e[0m"
	rm -f ./$(file).o
