#include <stdio.h>
#include <stdlib.h> // for malloc(), free()
#include <string.h>

#include "word_extractor.h"
#include "word_tally.h"

/**
 * A tool to hold the words we care about in an array, and
 * provide a method for processing the file.
 *
 */

// forward declarations
static int updateWordInTallyList(LLNode **wordLists, int maxLen, char *word);

/*
 * Here we do all the work, processing the
 * file and determining what to do for each word as we
 * read it.  Once we are done reading, we can then
 * examine our wordsToCheck list and see what their
 * state is
 */
int tallyWordsInFile(char *filename, LLNode **wordLists, int maxLen)
{
	struct WordExtractor *wordExtractor = NULL;
	char *aWord = NULL;
	// add any more variables that you need
	int i = 0;

	// create the extractor and open the file
	wordExtractor = weCreateExtractor(filename, maxLen);

	if (wordExtractor == NULL) {
		fprintf(stderr, "Failed creating extractor for '%s'\n", filename);
		return 0;
	}
	
	for(i=0;i<maxLen;i++)
	{
																						
	 while(wordLists[i]!=NULL)    				
	 {
		LLNode *temp= wordLists[i];
		wordLists[i]= wordLists[i]->next;    	
		 free(temp->key);            // Free the memory for the key string
   		 free(temp);   
		 
	 }
	}
	
	// read each word from the file using the WordExtractor,
	// and for each tally how often it has been used in
	// the appropriate list

	// In the loop, figure out whether we have seen this
	// word before, and do the appropriate thing if we have

	int totalWordCount = 0;

	while (weHasMoreWords(wordExtractor)) {
		aWord = weGetNextWord(wordExtractor);
		totalWordCount++;

		// figure out whether we have seen this word before,
		// and do the appropriate thing if we have
		updateWordInTallyList(wordLists, maxLen, aWord);
	}

	// is there anything else you need to do at the end of the loop?
	printf("Total word count %d\n", totalWordCount);

	// Close the file when we are done
	weDeleteExtractor(wordExtractor);

	return 1;
}


static int
updateWordInTallyList(LLNode **wordListHeads, int maxLen, char *word)
{

	
	 int len = strlen(word);

    // Check if the word length is within the specified range
    if (len < 1 || len > maxLen) {
        return 0; // Invalid word length, do nothing
    }

    LLNode *current = wordListHeads[len];

    // Search for the word in the linked list
    while (current != NULL) {
        if (strcmp(current->key, word) == 0) {
            // Word already exists in the list, increment its tally
            current->value = current->value+1;
            return 1; // Word updated successfully
        }
        current = current->next;
    }

    // If the word is not found in the list, create a new node
    LLNode *newNode = llNewNode(strdup(word), 1); // Initialize tally to 1
    if (newNode == NULL) {
        return 0; // Handle memory allocation error
    }

   newNode->next=wordListHeads[len];
   wordListHeads[len]=newNode;

    return 1; // New word added successfully
}


	
	

	


