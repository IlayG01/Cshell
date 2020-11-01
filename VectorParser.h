/*****************************************************************************************************\
File Name : VectorParser.h
Author    : Ilay Gilman
Purpose   : header of vectorparser.c
\*****************************************************************************************************/

#ifndef _VECTOR_PARSER_
#define _VECTOR_PARSER_

/* Decleration Section *********************************************************************************/

/*****************************************************************************************************\
* Function Name                           :                 count_appearence
* Function Porpuse                        :                 count appearence of recieved sign in recieved char*[]
* Function Params                         :                 char *argv[], int argv_length, char *sign
* Return Values                           :                 int
* Remarks                                 :                 none
* Authors                                 :                 Ilay Gilman
\*****************************************************************************************************/
int count_appearence(char *argv[], int argv_length, char *sign);

/*****************************************************************************************************\
* Function Name                           :                 get_indexes
* Function Porpuse                        :                 get indexes of recieved char*[] which point to recieved sign
* Function Params                         :                 char *argv[], int argv_length, char *sign[], int sign_length, int dst_array[]
* Return Values                           :                 none
* Remarks                                 :                 dst_array gets modified.
															[0] will contain the first index where any sign was
															[1] will contain the second ... and so...
															values encreased in 1 inorder to sync
* Authors                                 :                 Ilay Gilman
\*****************************************************************************************************/
void get_indexes(char *argv[], int argv_length, char *sign[], int sign_length, int dst_array[]);

/*****************************************************************************************************\
* Function Name                           :                 get_length
* Function Porpuse                        :                 getting length of each cmd
															feed the dst_columns array in the right values
* Function Params                         :                 int src_array[], int dst_array[], int length, int ref
* Return Values                           :                 void
* Remarks                                 :                 dst_array gets modified
															[0] will contain the number of arg first cmd contain
															[1] will contain the number of arg second cmd ... 
															values decreased in 1 inorder to sync
* Authors                                 :                 Ilay Gilman
\*****************************************************************************************************/
void get_length(int src_array[], int dst_array[], int length, int ref);

#endif //_VECTOR_PARSER_