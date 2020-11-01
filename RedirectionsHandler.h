/*****************************************************************************************************\
File Name : RedirectionsHandler.h
Author    : Ilay Gilman
Purpose   : header of RedirectionHandler.c
\*****************************************************************************************************/

#ifndef _RED_HANDLER_
#define _RED_HANDLER_

/* Decleration Section *******************************************************************************/

/*****************************************************************************************************\
* Function Name                           :                 handle_redirections
* Function Porpuse                        :                 setting new input & output to cmd if needed
* Function Params                         :                 char *cmd_argv[], int cmd_argv_len
* Return Values                           :                 int
* Remarks                                 :                 stdin & stdout of current exec might change
* Authors                                 :                 Ilay Gilman
\*****************************************************************************************************/
int handle_redirections(char *cmd_argv[], int cmd_argv_len);

#endif //_RED_HANDLER_