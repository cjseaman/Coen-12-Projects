#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//defines white space characters

# define IS_WHITESPACE(c) c==' '||c=='\n'||c=='\t'||c=='\r'||c=='\v'||c=='\f'

//I do not declare a MAX_WORD_SIZE because I don't read string by string

int main(int argc, char *argv[])
{
	FILE *fp;	//file pointer
	int i=0;	//counter variable
	char c;		//buffer for character
	char *fname=argv[1];	//container for file path

			//open and check file
	if(argc==1)
	{
		printf("No file name!\n");	//checks for file name
		return 0;
	}
	if((fp=fopen(fname, "r"))==NULL)	//checks for file memory
	{
		printf("Error reading file!\n");
		return 0;
	}
	else
	{

								//loop to traverse text file char by char, counting white spaces

		while(feof(fp)==0)
		{
			c=fgetc(fp);
			if(IS_WHITESPACE(c))	//every word is followed by a whitespace char
						//so I can count white spaces instead of words and get the correct number
			{
				i++;
								//loop to skip consecutive whitespace characters
				while(IS_WHITESPACE(c))
				{
					c=getc(fp);
				}
			}
		}
	}
	printf("%d total words\n", i);	//output
	return 1;
}
	
