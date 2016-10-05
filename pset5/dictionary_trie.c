/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "dictionary.h"

// declaring trie structure
typedef struct trie
{
    bool is_word;
    struct trie* letters[27];
} trie;

//root node
trie* head; 

//global variable will be used to count words loaded into trie
int words;

void freef(trie* trie);

unsigned int count(trie* trie);

/**
 * Returns true if word is in dictionary else false.
 */    
bool check(const char* word)
{
    //declare new trie to jump thru characters from the root node
    trie* current = head;
    
    int i = 0;
    
    //go thru the word until it reaches its end and check if all letters are in the trie
    while(word[i] != '\0')
    {
        int j = tolower(word[i]) - 'a';
        //if the character is in the trie, go to the next trie and the next character
        if(word[i] == '\'' && current->letters[26] != NULL)
        {
            current = current->letters[26];
            i++;
        }   
        
        else if(word[i] != '\'' && current->letters[j] != NULL)
        {
            current = current->letters[j];
            i++;
        }
        
        //if the character points to NULL, the word is not in the trie
        else
            return false;
    }
    
    //if we hit the end of the word, check the is_word value is true
    if(word[i] == '\0' && current->is_word == true)
        return true;
        
    //if we hit the end of the word and it's not in dictionary
    else 
        return false;

}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{

    // open dictionary file
    FILE* inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }
    
    //declaring first node of the trie and pointing in to NULL
    head = calloc(1, sizeof(trie));
    
    //declare new trie to jump thru characters
    trie* current = head;

    int ch;
    
    //set global variable for countring words to 0 before starting to count
    words = 0;
    
    //go thru each character in the file until end of file 
    while((ch = fgetc(inptr)) != EOF) 
    {
        // if not end of line and not an apostrophe
        if (ch != '\n' && ch != '\'')
        {
            // calculating the index once
            int i = tolower(ch) - 'a';
            
            //if the pointer for the character doesn't exist (equals NULL) allocate memory
            if (current->letters[i] == NULL)
            {    
                current->letters[i] = calloc(1, sizeof(trie));
            }
            
            //move forward in our trie structure
            current = current->letters[i];
        } 
        
        // if the character is an apostrophe
        else if (ch == '\'')
        {
            //if the pointer for the character doesn't exist (equals NULL) allocate memory
            if (current->letters[26] == NULL)
            {    
                current->letters[26] = calloc(1, sizeof(trie));
            }
            
            //move forward in our trie structure
            current = current->letters[26];
        } 
        
        //if the character read indicates the end of the line
        else 
        {
            //indicate that this is a word in our dictionary in the trie
            current->is_word = true;
            
            //reset our current node to the beginning of the trie
            current = head;
            
            //count words
            words++;
        }
    }

    // close infile
    fclose(inptr);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 *
 */

unsigned int size(void)
{
    return words;
}


/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 * Implementation taken from Stackoverflow discussion started by @panzer
 */
bool unload(void)
{
    //free children nodes
    freef(head);
    
    return true;
}

void freef(trie* trie)
{
    for (int i = 0; i < 27; i++)
    {
        if (trie->letters[i] != NULL)
            //go in each non null node until the end with the use of recursion
            freef(trie->letters[i]);        
    }
    
    free(trie);
}
