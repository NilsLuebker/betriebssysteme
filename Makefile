.PHONY: all clean compile run

all: run

clean:
	@echo -e "\e[32m=======================[Cleaning up]=======================\e[0m"
	rm -f ./$(file)

compile:
	@echo -e "\e[32m========================[Compiling]========================\e[0m"
	gcc -o $(file) $(file).c

debug:
	@echo -e "\e[32m==========================[Debug]=========================\e[0m"
	gcc -g $(file).c -o $(file)
	gdb $(file)

run: compile
	@echo -e "\e[32m=========================[Running]=========================\e[0m"
	./$(file)
	@echo -e "\e[32m=======================[Cleaning up]=======================\e[0m"
	rm -f ./$(file)
