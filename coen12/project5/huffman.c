/*
	Collin Seaman
	COEN 12
	Atkinson
	Project 5 Part 2
	Huffman Coding
	
	This is an implementation of a binary tree combined with a priority queue which assigns a unique
	bit value for each character in a file, allowing for lossless compression.
*/
# include "pack.h"
# include "pqueue.h"
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <assert.h>
# include <ctype.h>

static int compare(struct node *x, struct node *y) {
	return (x->count - y->count);
}

int main(int argc, char *argv[]) {
	
	if(argc != 3) {
		printf("Invalid Input! Filename outputname\n");
		printf("%d\n", argc);
		return 0;
	}

	FILE *fp;	
	PQ *pq;		
	int i, bitLength;
	int charCounts[257] = {0};	
	char *finname = argv[1];	
	char *foutname = argv[2];	
	struct node *leaves[257];
	struct node *probe;	
	struct node *left;		
	struct node *right;
/*
	Increment count for characters on each occurance
	Counts are stored in the location of their char
	O(n)
*/
	if((fp = fopen(finname, "r")) == NULL) {
		printf("Error opening file!\n");
		return 0;
	}
	while(feof(fp) == 0) {
		charCounts[fgetc(fp)]++;
	}
/*
	Create leaves for non-zero counts
	Add values from charCounts to leaves and set parent to NULL
	O(1) (Runs 257 times every time)
*/
	pq = createQueue(compare);	
	for(i = 0; i < 257; i++) {
		leaves[i] = NULL;
		if(charCounts[i] > 0) {
			leaves[i] = malloc(sizeof(struct node*)*charCounts[i]);	
			assert(leaves[i] != NULL);
			leaves[i]->count = charCounts[i];	
			leaves[i]->parent = NULL;
			addEntry(pq, leaves[i]);		
		}
	}
/*
	While more than one tree is in the queue, remove the two lowest, 
	create a new tree with those two as the children 
	and the sum of their counts as the data
	the last tree in pqueue is the root of the huffman tree
	Place root pointer in leaves[256]
	O(1) (Runs log(257) times max)
*/
	while(numEntries(pq) > 1) {
		left = removeEntry(pq);		
		right = removeEntry(pq);	
		left->parent = malloc(sizeof(struct node));	
		assert(left->parent != NULL);
		left->parent->count = left->count + right->count;
		right->parent = left->parent;
		addEntry(pq, left->parent);	
	}	
	leaves[256] = left->parent;
	leaves[256]->parent = NULL;
	
/*
	Print the number of occurences of each char
	and the number of bits (height from char to root) per character
	O(1) (Runs log(257) + 257 times max)
*/
	
	for(i = 0; i<256 ; i++) {
		if(leaves[i] != NULL) {
			for(probe = leaves[i], bitLength = 0; probe->parent !=  NULL; probe = probe->parent) {
				bitLength++;
			}
		
			if(isprint(i) != 0) {
				printf("'%c': %d x %d bits = %d bits\n", (char)i, leaves[i]->count, bitLength, leaves[i]->count * bitLength);
			}	else {
				printf("%03o: %d x %d bits = %d bits\n", i, leaves[i]->count, bitLength, leaves[i]->count * bitLength);
			}
		}
	}
/*
	Calls a function to compress the text file into a file with the passed output name
*/
	pack(finname, foutname, leaves);
	
}



	
	
