clean:
	@echo -e "\e[32m=======================[Cleaning up]=======================\e[0m"
	rm -f ./main

compile:
	@echo -e "\e[32m========================[Compiling]========================\e[0m"
	gcc -o main main.c

run: compile
	@echo -e "\e[32m=========================[Running]=========================\e[0m"
	./main
	@echo -e "\e[32m=======================[Cleaning up]=======================\e[0m"
	rm -f ./main
