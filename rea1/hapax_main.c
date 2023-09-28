#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "word_tally.h"


/** function to be passed in to llFree to delete allocated keys */
void deleteKey(LLNode *node, void *userdata)
{
	if (node->key != NULL) {
		free(node->key);
	}
}


/** Delete the contents of all of the word lists
 *
 * This function assumes that both the nodes and
 * the keys within the nodes have been allocated
 * using malloc() and need to be freed.
 */
void
deleteWordLists(LLNode **wordListHeads, int maxLen)
{
	int i;

	for (i = 0; i <= maxLen; i++) {
		if (wordListHeads[i] != NULL) {
			llFree(wordListHeads[i], deleteKey, NULL);
			wordListHeads[i] = NULL;
		}
	}
}


/** print out all of the data in a word list */
int
printData(char *filename, LLNode *wordListHeads[], int maxLen)
{
	LLNode *node;
	int i;

	printf("All word count data from file '%s':\n", filename);

	/**
	 * For each length, if the list is not null, print out
	 * the values in the list
	 */
	for (i = 0; i <= maxLen; i++) {
		node = wordListHeads[i];
		if (node != NULL) {
			printf("Length %d:\n", i);
			while (node != NULL) {
				printf("    '%s' %d\n", node->key, node->value);
                node = node->next;
			}
		}
	}
	return 1;
}


int printHapax(char *filename, LLNode *wordListHeads[],
		int maxLen, int hapaxLength){

	LLNode *node;
    

   
    // Iterate through the specified word list (based on hapaxLength)
    node = wordListHeads[hapaxLength];
    if (node != NULL) {
        while (node != NULL){
            // Print words that have a count of 1 (hapax legomena)
            if (node->value == 1) {
                printf("	%s \n", node->key);
            }
            node = node->next;
        }
    }
	

	return 1;
}


/* print out the command line help */
void
usage()
{
	fprintf(stderr, "\n");
	fprintf(stderr, "Find and print the hapax legomena in one or more files.\n");
	fprintf(stderr, "A \"hapax legomenon\" is a word that occurs only once in the file\n");

	fprintf(stderr, "\n");
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "    hapax [<options>] <datafile> [ <datafile> ...]\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "-d     : print out all data loaded before printing hapax legomena.\n");
	fprintf(stderr, "-h     : this help.  You are looking at it.\n");
	fprintf(stderr, "-l <N> : only print hapax legomena of length <N>.\n");
	fprintf(stderr, "       : If no -l option is given, all hapax legomena are printed.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Sample command line:\n");
	fprintf(stderr, "    hapax -l5 smalldata.txt");
	fprintf(stderr, "\n");
	fprintf(stderr, "This example would print all words of length 5 that exist in the\n");
	fprintf(stderr, "file \"smalldata.txt\".\n");
	fprintf(stderr, "\n");

	// exit the program
	exit(1);
}


// define the maximum length of word we will look for, and by extension,
// the number of entries that must be in the array of word lists
#define	MAX_WORD_LEN	24

int main(int argc, char **argv) {
    int i, shouldPrintData = 0, didProcessing = 0, printHapaxLength = -1;
	
    LLNode *wordListHeads[MAX_WORD_LEN + 1];
    memset(wordListHeads, 0, sizeof(wordListHeads));  // Initialize to NULL

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "-d") == 0) {
                shouldPrintData = 1;
            } else if (strcmp(argv[i], "-h") == 0) {
                usage();
            } else if (strcmp(argv[i], "-l") == 0) {
                if (i + 1 < argc) {
                    printHapaxLength = atoi(argv[i + 1]);
                    i++;  // Skip the next argument (length)
                } else {
                    fprintf(stderr, "Error: Missing length argument for -l option.\n");
                    usage();
                }
            }
			
			else {
                fprintf(stderr, "Error: Unknown option '%s'\n", argv[i]);
                usage();
            }
        } else {
            if (tallyWordsInFile(argv[i], wordListHeads, MAX_WORD_LEN) == 0) {
                fprintf(stderr, "Error: Processing '%s' failed -- exiting\n", argv[i]);
                return 1;
            }

            didProcessing = 1;
		
            printf("Tally loaded\n");

            if (shouldPrintData) {
                printData(argv[i], wordListHeads, MAX_WORD_LEN);
            }

            if (printHapaxLength >= 1) {
				 printf("Hapax legomena from file '%s'\n", argv[1]);

                printHapax(argv[i], wordListHeads, MAX_WORD_LEN, printHapaxLength);
            }
        }
    }
	if(argc==2){
		 printf("Hapax legomena from file '%s'\n", argv[1]);
	 for (int len = 0; len < MAX_WORD_LEN; len++) {
        if (wordListHeads[len] != NULL) {
            printHapax(argv[1], wordListHeads, MAX_WORD_LEN, len);
        }
    }}
	
		

    if (!didProcessing) {
        fprintf(stderr, "No data processed -- provide the name of a file on the command line\n");
        usage();
        return 1;
    }
    deleteWordLists(wordListHeads, MAX_WORD_LEN);
    return 0;
}
