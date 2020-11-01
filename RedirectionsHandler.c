/*****************************************************************************************************\
File Name : RedirectionsHandler.c
Author    : Ilay Gilman
Purpose   : Holds the implementation for handle_redirections function
\*****************************************************************************************************/

/* Include Section ***********************************************************************************/
/* Standard I/O library - Includes, among else the declaration for printf */
#include <stdio.h>
/* Standard library - various functions for manipulating arrays of characters */
#include <string.h>
/* defines miscellaneous symbolic constants and types, and declares miscellaneous functions */
#include <unistd.h>
/* files modes */
#include <fcntl.h>

#include "VectorParser.h"

/* Definition Section ********************************************************************************/
/* redirect stdout */
#define STDOUT_RED (">")
/* redirect stdout */
#define DSTDOUT_RED (">>")
/* redirect stdin */
#define STDIN_RED ("<")

/* Decleration Section *******************************************************************************/

/*****************************************************************************************************\
* Function Name                           :                 open_file
* Function Porpuse                        :                 open file & return its fd
* Function Params                         :                 char *path, int mode, int *ret_fd
* Return Values                           :                 int
* Remarks                                 :                 available modes -
															0 - create & trunc file
															1 - append
															2 - read
* Authors                                 :                 Ilay Gilman
\*****************************************************************************************************/
int open_file(char *path, int mode, int *ret_fd);

/* Function Section **********************************************************************************/
/****************************************************************************************************\
* Documentation is in the declertion section
\****************************************************************************************************/
int open_file(char *path, int mode, int *ret_fd)
{
	int fd;
	/* trunc */
	if (mode == 0)
	{
		fd = open(path, O_WRONLY | O_CREAT | O_TRUNC ,0644);
		if (fd == -1)
	    {
	        return 7;
	    }
	    *ret_fd = fd;
	}
	/* append */
	else if (mode == 1)
	{
		fd = open(path, O_WRONLY | O_CREAT | O_APPEND ,0644);
		if (fd == -1)
	    {
	        return 7;
	    }
	    *ret_fd = fd;
	}
	/* check if existed */
	else if (mode == 2)
	{
		fd = open (path, O_RDONLY);
		if (fd == -1)
	    {
	        return 9;
	    }
	    *ret_fd = fd;
	}
	else
	{
		return 8;
	}
	return 0;
}

/****************************************************************************************************\
* Documentation is in the header file
\****************************************************************************************************/
int handle_redirections(char *cmd_argv[], int cmd_argv_len)
{
	//DBG - printf("[-]handle_redrections[-]\n");
	char *signs[] = {STDOUT_RED, DSTDOUT_RED, STDIN_RED};
	int stdout_red_sign_count = count_appearence(cmd_argv, cmd_argv_len, STDOUT_RED);   
	int dstdout_red_sign_count = count_appearence(cmd_argv, cmd_argv_len, DSTDOUT_RED); 
	int stdint_red_sign_count = count_appearence(cmd_argv, cmd_argv_len, STDIN_RED); 
	int redirections_boundaries = stdout_red_sign_count + dstdout_red_sign_count + stdint_red_sign_count;

	if (redirections_boundaries == 0)
	{
		//DBG - printf("handle redirecitons got nthn\n");
		return 0;
	}

	int red_indexes[redirections_boundaries + 1];
	get_indexes(cmd_argv, cmd_argv_len, signs, 3, red_indexes);

	/* no argument next to redirection */
	if (red_indexes[redirections_boundaries] == (cmd_argv_len))
	{
		return 6;
	}

	for (int i = 1; i < redirections_boundaries + 1; i++)
	{
		int file_status = 0;
		int file_name_index = red_indexes[i];
		char *file_name = cmd_argv[file_name_index];
		if (0 == strcmp(*(cmd_argv+file_name_index-1), STDOUT_RED))
		{
			int file_fd;
			file_status = open_file(file_name, 0, &file_fd);
			if (file_status != 0)
			{
				return file_status;
			}
			dup2(file_fd, 1);
			close(file_fd);

		}
		if (0 == strcmp(*(cmd_argv+file_name_index-1), DSTDOUT_RED))
		{
			int file_fd;
			file_status = open_file(file_name, 1, &file_fd);
			if (file_status != 0)
			{
				return file_status;
			}
			dup2(file_fd, 1);
			close(file_fd);

		}
		if (0 == strcmp(*(cmd_argv+file_name_index-1), STDIN_RED))
		{
			int file_fd;
			file_status = open_file(file_name, 2, &file_fd);
			if (file_status != 0)
			{
				return file_status;
			}
			dup2(file_fd, 0);
			close(file_fd);
		}
	}
	return 0;
}