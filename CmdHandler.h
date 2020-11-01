/*****************************************************************************************************\
File Name : CmdHandler.h
Author    : Ilay Gilman
Purpose   : header of CmdHandler.c
\*****************************************************************************************************/

#ifndef _CMD_HANDLER_
#define _CMD_HANDLER_

/* Decleration Section *******************************************************************************/

/*****************************************************************************************************\
* Function Name                           :                 cmd_handler
* Function Porpuse                        :                 handle proc scope command, same read & write for each
* Function Params                         :                 char* cmd_argv[], int length, int input_fd, int output_fd
* Return Values                           :                 int
* Remarks                                 :                 none
* Authors                                 :                 Ilay Gilman
\*****************************************************************************************************/
int cmd_handler(char* cmd_argv[], int length, int input_fd, int output_fd);

#endif //_CMD_HANDLER_