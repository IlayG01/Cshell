/*****************************************************************************************************\
File Name : cshell.c
Author    : Ilay Gilman
Purpose   : implementing linux shell
\*****************************************************************************************************/

/* Include Section ***********************************************************************************/
#include <unistd.h>
#include <stdio.h>

/* out header files */
#include "VectorParser.h"
#include "CmdHandler.h"


/* Definition Section ********************************************************************************/
/* pipe char */
#define PIPLINE ("|")


/* Enum Section *************************************************************************************/
/* program error codes & meaning */
typedef enum program_status_e
{
	STATUS_INVALID = -1,
	STATUS_OK = 0,
	STATUS_BAD_USAGE,
	STATUS_BAD_PIPE,
	STATUS_BAD_FORK,
	STATUS_BAD_CMD,
	STATUS_BAD_ARGUMENT_NEXT_TO_REDIRECTION,
	STATUS_UNABLE_CREAT_FILE,
	STATUS_NO_FILE_FOUND,
} status_t;


int main(int argc, char* argv[])
{
	// TODO - Interactive shell, with exit option.
	int status = STATUS_INVALID;

	/* checks valid usage */
	if (argc < 2)
	{
		status = 1;
		return status;
	}
	
	int pipes_count = count_appearence(argv, argc, PIPLINE); /* how many cmd to exec */
	if (pipes_count == 0)
	{
		status = cmd_handler(argv+1, argc-1, 0, 1);
		return status;
	}

	int total_cmd = pipes_count + 1;  /* total cmd to run = numofpipes + 1 */
	int parsed_argv_index[total_cmd]; /* holds pipline indexes */
	int parsed_argv_len[total_cmd];   /* holds argv_len for each pipe */

	char *pipeline[] = {PIPLINE}; /* stupid but required for get_indexes */
	get_indexes(argv, argc, pipeline, 1, parsed_argv_index);
	get_length(parsed_argv_index, parsed_argv_len, total_cmd, argc);
	
	/* this pipe holds read & write fd's for cmd communication */
	// TODO - working with multiple pipes
	int pipe_fd[2];
	if (pipe(pipe_fd) == -1)
	{
		status = 2;
		return status;
	}

	/* this int[] holds original stdout stdin fds */
	int original_fd[2];
	original_fd[0] = dup(0);
	original_fd[1] = dup(1);

	/* handle each cmd */
	/* first */
	status = cmd_handler(argv + 1, parsed_argv_len[0], original_fd[0], pipe_fd[1]);
	if (status != 0)
	{
		return status;
	}
	close(pipe_fd[1]); //after writing to pipe, we need to close thr end to able the other cmd reading from it
	for(int current_proc = 1; current_proc < pipes_count; current_proc++)
	{
		status = cmd_handler(argv + parsed_argv_index[current_proc], parsed_argv_len[current_proc], pipe_fd[0], pipe_fd[1]);
		if (status != 0)
		{
			return status;
		}
	}
	status = cmd_handler(argv + parsed_argv_index[total_cmd-1], parsed_argv_len[total_cmd-1], pipe_fd[0], original_fd[1]); /* last cmd */
	if (status != 0)
	{
		return status;
	}
	close(pipe_fd[0]); //after reading from pipe
}

	// future used error
	// switch (status)
	// {
	// case STATUS_INVALID:
	// 	printf("Shell invalid\n");
	// 	break;
	// case STATUS_OK:
	// 	printf("Command ended successfully\n");
	// 	break;
	// case STATUS_BAD_USAGE:
	// 	printf("Usage: > <command>\n");
	// 	break;
	// case STATUS_BAD_PIPE:
	// 	printf("Unable to creat a pipe\n");
	// 	break;
	// case STATUS_BAD_FORK:
	// 	printf("Unable to fork the current proc\n");
	// 	break;
	// case STATUS_BAD_CMD:
	// 	printf("Invalid command passed to the program\n");
	// 	break;
	// case STATUS_BAD_ARGUMENT_NEXT_TO_REDIRECTION:
	// 	printf("When using > source/dest file should be specified\n");
	// 	break;
	// case STATUS_UNABLE_CREAT_FILE:
	// 	printf("Unabled to creat the specified file\n");
	// 	break;
	// case STATUS_NO_FILE_FOUND:
	// 	printf("Unabled to find the specified file\n");
	// 	break;
	// default:
	// 	return status;
	// }
