#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

/*
*   Programmed by Tristan Chilvers
*   ID: 2288893
*   Email: chilvers@chapman.edu
*
*   This program will take two files as arguments and concatenate the data from
*   the first file (input) into the second file (output).
*
*   References: geeksforgeeks.com regarding general standard functions
*								Built off of the example unix_pipe.c file
*
*/

#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1

int main(void)
{
	//	-------------------------------------------------------------------
	//	Instance Variables
	//

	char write_msg[BUFFER_SIZE];
	char read_msg[BUFFER_SIZE];
	char exit1[3] = "q\n";	//	used later to check if user input is "q"
	char exit2[6] = "quit\n";	//	used later to check if user input is "quit"
	pid_t pid;	//	process identification
	int fd[2];	//	file descriptor

	//	-------------------------------------------------------------------
	//	Create pipe and fork
	//

	//	Create the Pipe
	if (pipe(fd) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}

	//	Fork and create a child process
	pid = fork();

	//	Catch Fork Failure
	if (pid < 0) {
		fprintf(stderr, "Fork failed");
		return 1;
	}

	//	-------------------------------------------------------------------
	//	PARENT / CHILD PROCESSES
	//

	//	PARENT PROCESS:
	if (pid > 0) {
		//	close the unused (read) end of the pipe
		close(fd[READ_END]);
		printf("osh> ");

		//	loop program until user writes "q" or "quit"
		while((strcmp(exit1, write_msg) != 0) && (strcmp(exit2, write_msg) != 0))
		{
			//	grab input from console
			fgets(write_msg, BUFFER_SIZE, stdin);

			//	write input to pipe
			write(fd[WRITE_END], write_msg, strlen(write_msg)+1);
		}

		//	close the write end of the pipe when exiting
		close(fd[WRITE_END]);
	}

	//	CHILD PROCESS:
	else {
		//	close the unused (write) end of the pipe
		close(fd[WRITE_END]);

		//	read input from the parent
		read(fd[READ_END], read_msg, BUFFER_SIZE);

		//	loop program until the input is "q" or "quit"s
		while((strcmp(exit1, read_msg) != 0) && (strcmp(exit2, read_msg) != 0))
		{
			//	run the input from user
			system(read_msg);
			printf("osh> ");
			//	clear the output buffer to resolve newline issue
			fflush(stdout);

			//	read input from the parent
			read(fd[READ_END], read_msg, BUFFER_SIZE);
		}

		//	close the read end of the pipe when exiting
		close(fd[READ_END]);
	}

	return 0;
}
