/*****************************************************************************************************\
File Name : CmdHandler.c
Author    : Ilay Gilman
Purpose   : Holds the implementation for cmd_handler function
\*****************************************************************************************************/

/* Include Section ***********************************************************************************/
/* Standard I/O library - Includes, among else the declaration for printf */
#include <stdio.h>
/* Standard library - various functions for manipulating arrays of characters */
#include <string.h>
/* defines miscellaneous symbolic constants and types, and declares miscellaneous functions */
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "VectorParser.h"
#include "RedirectionsHandler.h"

/* Definition Section ********************************************************************************/
/* logical and */
#define LOGICAL_AND ("&&")
/* logical or */
#define LOGICAL_OR ("||")
/* redirect stdout */
#define STDOUT_RED (">")
/* redirect stdout */
#define DSTDOUT_RED (">>")
/* redirect stdin */
#define STDIN_RED ("<")

/* Decleration Section *******************************************************************************/

/*****************************************************************************************************\
* Function Name                           :                 execute
* Function Porpuse                        :                 execute each execv we need
* Function Params                         :                 char *cmd_argv[], int cmd_argv_len,
															int *ret_val, int input_fd, int output_fd
* Return Values                           :                 int
* Remarks                                 :                 ret val might change in the function
															 - depends on return val of execv
* Authors                                 :                 Ilay Gilman
\*****************************************************************************************************/
int execute(char *cmd_argv[], int cmd_argv_len, int *ret_val, int input_fd, int output_fd);

/*****************************************************************************************************\
* Function Name                           :                 restore_io
* Function Porpuse                        :                 restore input & output
* Function Params                         :                 int input_fd, int output_fd
* Return Values                           :                 int
* Remarks                                 :                 stdin & stdout of cmd restored
* Authors                                 :                 Ilay Gilman
\*****************************************************************************************************/
int restore_io(int input_fd, int output_fd);

/* Function Section **********************************************************************************/

/****************************************************************************************************\
* Documentation is in the declertion section
\****************************************************************************************************/
int restore_io(int input_fd, int output_fd)
{
	//WARNING - might close essential fd
	dup2(input_fd, 0);
	//close(input_fd);
	dup2(output_fd, 1);
	//close(output_fd);
	return 0;
}

/****************************************************************************************************\
* Documentation is in the declertion section
\****************************************************************************************************/
int execute(char *cmd_argv[], int cmd_argv_len, int *ret_val, int input_fd, int output_fd)
{
	/* creating the env for the execv, a bit bigger to store null at the end */
	char *new_argv[cmd_argv_len + 1];
	for (int index = 0; index < cmd_argv_len; index++)
	{
		// DBG - printf("execute -- trying to mv this param --> %s\n",cmd_argv[index]);
		if (0 == strcmp(cmd_argv[index], STDOUT_RED) || 0 == strcmp(cmd_argv[index], DSTDOUT_RED) || 0 == strcmp(cmd_argv[index], STDIN_RED))
		{
			// TODO - recover from this gap in argv
			/* this params are not for execv */
			index += 2;
		}
		else
		{
			new_argv[index] = cmd_argv[index];
			new_argv[index + 1] = NULL;
		}
	}

	/* handling resdirection if there are */
	int io_status = handle_redirections(cmd_argv, cmd_argv_len);
	if (io_status != 0)
	{
		return io_status;
	}

	/* true/false need special treatment */
	if ((0 == strcmp(new_argv[0], "false")) && cmd_argv_len == 1 )
	{
		*ret_val = 1;
		return 0;
	}
	if ((0 == strcmp(new_argv[0], "true")) && cmd_argv_len == 1 )
	{
		*ret_val = 0;
		return 0;
	}

	/* the binary we are going to exec */
	char full_bin_loc[20] = "/bin/";
	strcat(full_bin_loc, new_argv[0]);
    // DBG - printf("execute -- full path %s\n", full_bin_loc);

    /* calling the binary */
	/* first we are going to fork the process, than execute the binary */
	pid_t bin_proc;
	bin_proc = fork();

	if (bin_proc < 0)
    {
        // fork() failed.
        perror("fork");
        return 4;
    }

    if (bin_proc == 0)
    {
    	/* child proc */
    	//DBG -printf("Child's PID is %d. Parent's PID is %d\n", (int)getpid(), (int)getppid());
    	*ret_val = 0;
        execv(full_bin_loc, new_argv);
        *ret_val = 1;
        perror("execv"); // Ne need to check execv() return value. If it returns, you know it failed.
        return 5;
    }
    else
    {
    	int status;
	    pid_t wait_result;

	    while ((wait_result = wait(&status)) != -1)
	    {
	        //DBG - printf("Process %lu returned result: %d\n", (unsigned long) wait_result, status);
	        int garbage = 0;
	    }

	    //DBG - printf("All children have finished.\n");
	    *ret_val = 0;
	    return 0;
    }
}

/****************************************************************************************************\
* Documentation is in the header file
\****************************************************************************************************/
int cmd_handler(char* cmd_argv[], int length, int input_fd, int output_fd)
{
	//DBG -printf("[#] cmd handler - let's see what we got in this command [#]\n");
	// for (int i = 0; i < length; i ++)
	// {
	// 	printf("arg num %d --- %s\n", i, cmd_argv[i]);
	// }

	char *signs[] = {LOGICAL_AND, LOGICAL_OR};
	int or_sign_count = count_appearence(cmd_argv, length, LOGICAL_OR);   
	int and_sign_count = count_appearence(cmd_argv, length, LOGICAL_AND); 
	int logical_signs_boundaries = or_sign_count + and_sign_count;

	if (logical_signs_boundaries == 0)
	{
		//DBG - printf("cmd handler -- no logical signs\n");
		restore_io(input_fd, output_fd);
		execute(cmd_argv, length, &logical_signs_boundaries, input_fd, output_fd);
		return 0;
	}

	int total_sections = logical_signs_boundaries + 1;  /* total bins to run */
	
	int parsed_argv_index[total_sections]; /* holds start section indexes */
	int parsed_argv_len[total_sections];   /* holds argv_len for each pipe */

	get_indexes(cmd_argv, length, signs, 2, parsed_argv_index);
	parsed_argv_index[0] = 0; /* initialize the first section to 0 after get_indexes() set to 1 */
	// DBG
	// for(int i = 0; i < total_sections; i++)
	// {
	// 	printf("%d\n", parsed_argv_index[i]);
	// }
	
	get_length(parsed_argv_index, parsed_argv_len, total_sections, length);

	// DBG
	// for (int j = 0; j < total_sections; j++)
	// {
	// 	printf("cmd handler - section entery - %d\n", parsed_argv_index[j]);
	// 	printf("cmd handler - section length - %d\n", parsed_argv_len[j]);
	// }

	/* execution start */
	/* this varaible hold ret from exec (0 if true, 1 if false)*/
	int last_section_ret = -1; //initial value is -1 to allow exec both for && \ || cases
	for (int current_section = 0; current_section < total_sections; current_section++)
	{	
		restore_io(input_fd, output_fd); //if last execv had redirection, we need to initialize the io.
		if (last_section_ret == -1)
		{
			int entery = parsed_argv_index[current_section];
			execute(cmd_argv+entery, parsed_argv_len[current_section], &last_section_ret, input_fd, output_fd);
		}
		else
		{
			int entery = parsed_argv_index[current_section];
			/* if delimeter_sign == 0 than its an or section, else its an and section */
			int delimeter_sign = strcmp(*(cmd_argv+entery-1), LOGICAL_OR);
			// DBG - printf("cmd handle -- delim sign is %d -- last section ret is %d\n", delimeter_sign, last_section_ret);
			if ((last_section_ret == 0 && delimeter_sign == 0) || (last_section_ret == 1 && delimeter_sign != 0))
			{
				continue;
			}
			else
			{
				int entery = parsed_argv_index[current_section];
				execute(cmd_argv+entery, parsed_argv_len[current_section], &last_section_ret, input_fd, output_fd);
			}
		}
	}
	return 0;
}

