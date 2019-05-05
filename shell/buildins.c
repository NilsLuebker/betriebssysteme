#include "buildins.h"
#include "jobctl.h"

const struct Buildin BUILDINS[BUILDINS_SIZE] = {
	{ .keyword="logout", .func=&logout },
	{ .keyword="exit",   .func=&logout },
	{ .keyword="test",   .func=&test },
	{ .keyword="jobs",   .func=&jobs }
};

bool execute_buildins(char** argv)
{
	for(int i = 0; i < BUILDINS_SIZE; i++)
	{
		if(!strcmp(argv[0], BUILDINS[i].keyword))
		{
			BUILDINS[i].func(++argv);
			return true;
		}
	}
	return false;
}

void jobs(char** argv) {
	print_background_processes();
}

void logout(char** argv)
{
	char* line = NULL;
	size_t line_size = 0;
	while(true)
	{
		printf("Are you sure you want to logout? (y/n) ");
		getline(&line, &line_size, stdin);
		switch(line[0])
		{
			case 'y':
				exit(0);
			case 'n':
				return;
			default:
				continue;
		}
	}
}

void test(char** argv)
{
	if(*argv)
	{
		printf("Arg: %s\n", *argv);
	}
	else
	{
		printf("No Args\n");
	}
}
