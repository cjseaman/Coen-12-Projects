/* 	Collin Seaman
	Atkinson COEN 12
	Project 2

	AVERAGE RUN TIMES
						UNIQUE	PARITY
	Bible.txt 			-	5.623s	44.283s
	Genesis.txt			-	0.135s	0.453s
	GreenEggsAndHam.txt		-	0.012s	0.013s
	HoundOfTheBaskervilles.txt	-	0.411s	1.777s	
	Macbeth.txt			-	0.148s	0.380s
	TheCountOfMonteCristo.txt	-	5.463s	39.095s
	TheSecretGarden.txt		-	0.470s	1.935s
	TheWarOfTheWorlds.txt		-	0.559s	2.053s
	TreasureIsland.txt		-	0.494s	2.111s
	TwentyThousandLeagues.txt	-	1.086s	4.706s
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "set.h"
struct set 
{
	int count;
	int length;
	char **data;
};

typedef struct set SET;

SET *createSet(int maxElts)	//	O(1)
{
	SET *sp;	//set pointer
	sp=malloc(sizeof(SET));	//allocate memory for SET (2 int and one pointer)
	assert(sp!=NULL);
	sp->count=0;	//number of elements
	sp->length=maxElts;		
	sp->data=malloc(sizeof(char*)*maxElts);	//allocate memory for char pointers to strings
	assert(sp->data!=NULL);
	return sp;	//return mem location
}

void destroySet(SET *sp)	//	O(n)
{
	int i;
	for(i=0;i<sp->count;i++)
	{
		free(sp->data[i]);	//free each string
	}
	free(sp->data);	//free pointer to string pointers
	free(sp);	//free set pointer
}

int search(SET *sp, char *elt)	//	O(n)
{
	int i;
	for(i=0;i<sp->count;i++)	//loop to traverse data
	{
		if(strcmp(sp->data[i], elt)==0)	//returns index of word if found
		{
			return i;
		}
	}
	return -1;	//else returns -1
}

char *findElement(SET *sp, char *elt)	//	O(n)
{
	int i;
	if((i=search(sp, elt))==-1)	//if not found return null
	{
		return NULL;
	}
	return sp->data[i];		//else return pointer at index i
}

void removeElement(SET *sp, char *elt)	//	O(n)
{
	int locn;
	assert(sp!=NULL&&elt!=NULL);
	locn=search(sp, elt);		//locn is the index of the word
	if (locn!=-1)	//if the search found the word
	{
		free(sp->data[locn]);	//free memory location
		sp->data[locn]=sp->data[sp->count-1];
		sp->count--;				//copy last element to the slot
	}
}

int numElements(SET *sp)	//	O(1)
{
	assert(sp!=NULL);
	return sp->count;		//returns number
}

void addElement(SET *sp, char *elt)	//	O(n)
{
	if(search(sp, elt)==-1)		//if word does not already exist in set
	{	
		sp->data[sp->count]=strdup(elt);	//pointer data[count] now points to string elt
		assert(sp->data[sp->count]!=NULL);	//hopefully
		sp->count++;				//increment count because there are more things
	}
}

char **getElements(SET *sp)	//	O(n)
{
	int i;
	char **strings;	//pointer to char pointers
	strings=malloc(sizeof(char*)*(sp->count));	//allocate memory for string pointers
	assert(strings!=NULL);				
	for(i=0;i<sp->count;i++)	//loop to allocate memory for each string
	{
		strings[i]=sp->data[i];	//copies strings from data to strings
	}
	return strings;	//return pointer to array
}
