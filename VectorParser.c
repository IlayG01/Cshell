/*****************************************************************************************************\
File Name : vectorparser.c
Author    : Ilay Gilman
Purpose   : Holding the implementation of functions that parsing char *[] into several sections.
\*****************************************************************************************************/

/* Include Section ***********************************************************************************/

/* standard input output */
#include <stdio.h>
/* several string function - strcmp */
#include <string.h>

/* Function Section ***********************************************************************************/

/****************************************************************************************************\
* Documentation is in the header file
\****************************************************************************************************/
int count_appearence(char *argv[], int argv_length, char *sign)
{
	int sign_counter = 0;
	for (int i = 0; i < argv_length; i++)
	{
		if (0 == strcmp(argv[i], sign))
		{
			sign_counter++;

		}
	}
	return sign_counter;
}

/****************************************************************************************************\
* Documentation is in the header file
\****************************************************************************************************/
void get_indexes(char *argv[], int argv_length, char *sign[], int sign_length, int dst_array[])
{
	/* THE STORED VAL IS ENCREASED IN ORDER TO SYNC ARGV AFTER THE DELIMETER */
	/* WARNING - FIRST VAL IS SET TO 1 NOT 0 */
	int sign_counter = 1;
	dst_array[0] = 1;
	for (int i = 0; i < argv_length; i++)
	{
		for (int j = 0; j < sign_length; j++)
		{
			if (0 == strcmp(argv[i], sign[j]))
			{
				*(dst_array + sign_counter) = i + 1;
				sign_counter++;

			}
		}
	}
	return;
}

/****************************************************************************************************\
* Documentation is in the header file
\****************************************************************************************************/
void get_length(int src_array[], int dst_array[], int length, int ref)
{
	/* THE STORED VAL IS DECREASED IN ORDER TO SYNC LENGTH NOT TO REACH NEXT DELIMETER */
	for (int i = 0; i < length - 1; i++)
	{
		dst_array[i] = (src_array[i + 1] - src_array[i] - 1);
	}
	dst_array[length - 1] = (ref - src_array[length - 1]);
	for (int i = 0; i < length; i++)
	{
		//DBG - printf("row -- %d from -- %d storedVal -- %d\n", i+1, length, dst_array[i]);
	}
	return;
}