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
#include <string.h>
#include "dictionary.h"

#define SIZE 380
#define LENGTH 45

//hashtable structure
typedef struct hashtable
{
    char* key;
    struct hashtable* next;
} hashtable;

//root node
hashtable* head[SIZE]; 

//global variable will be used to count words loaded into table
int words;

unsigned int hashf(const char* word);

void freelist(hashtable* list);

/**
 * Returns true if word is in dictionary else false.
 */    
bool check(const char* word)
{
    //calculate the key where the word would be located
    int hashkey = hashf(word);

    //if the first node points to null, the word is not in the dictionary
    if (head[hashkey]->next == NULL)
        return false;
    
    //create a node to go through hashtable and set it to the beginning of the linked list
    hashtable* current = head[hashkey]->next;

    //repeat until we reached the last node in this linked list
    do 
    {
        if (strlen(current->key) != strlen(word))
            current = current->next; 
        
        else 
        {
            //iterate through each word character by character to compare them
            for (int i = 0; current->key[i] != '\0'; i++)
            {
                // strings are not equal, go to next node in linked list
                if (tolower(current->key[i]) != tolower(word[i]))
                {
                    //next node is empty, then the word is not in dicitonary
                    if(current->next == NULL)
                        return false;
                    else
                        current = current->next; 
                }
                else 
                    return true;
            }
        }
    return true;
    
    } while(current->next != NULL);
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
    for (int i = 0; i < SIZE; i++)
        head[i] = calloc(1, sizeof(hashtable));
        
    int index = 0;
    
    char word[LENGTH+1];
    
    //go thru each character in the file until end of file 
    for (int ch = fgetc(inptr); ch != EOF; ch = fgetc(inptr))
    {
        // allow only alphabetical characters and apostrophes
        if (ch != '\n')
        {
            // append character to word
            word[index] = ch;
            index++;
        }

        // we must have found a whole word
        else if (index > 0)
        {
            // terminate current word
            word[index] = '\0';

            // calculate the word's hashkey
            int hashkey = hashf(word);
            
            //create new node for new word and allocate memory for it
            hashtable* newnode = calloc(1, sizeof(hashtable));
            //put data into new node
            newnode->key = word;
            //reset the pointers
            newnode->next = head[hashkey]->next;
            head[hashkey]->next = newnode;
            
            // prepare for next word
            index = 0;
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

unsigned int hashf(const char* word)
{
    //set counter to 0 for each new word
    int counter = 0;
    
    //go thru each string in the file until end of file 
    for (int i = 0; word[i] != '\0'; i++) 
    {
        counter = counter + tolower(word[i]);
    }
    
    //calculates index = hashkey for each word
    int index = counter % 380;
    
    return index;
}

unsigned int size(void)
{
    //set global variable for countring words to 0 before starting to count
    words = 0;
    
    //create a node to go through hashtable and set it to the beginning of the linked list
    hashtable* current;
    
    for (int i = 0; i < SIZE; i++)
    {
        //set a temp cursor to go through the table
        current = head[i];
    
        //repeat until we reached the last node under this hashkey
        while(current->next != NULL)
        {
            //if the current node is the first node in the hash table, move on
            if (current->key == NULL)
                current = current->next;
            
            //if current node has a word, ad it up to the counter for words and move on
            else if (current->key != NULL)
            {
                words++;
                current = current->next;
            }
        }
        
        if (current->next == NULL && current->key != NULL)
            words++;
    }
    
    return words;
}


/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 * Implementation taken from Stackoverflow discussion started by @panzer
 */
bool unload(void)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (head[i]->next != NULL)
            //go in each non null node until the end with the use of recursion
            freelist(head[i]->next); 
    }
    
    for (int i = 0; i < SIZE; i++)
        free(head[i]);
    
    return true;
}

void freelist(hashtable* list)
{
    //if current node not null, go to next untill the end of the list
    if (list->next != NULL)
    {
        freelist(list->next);
    }
    
    free(list);
}
