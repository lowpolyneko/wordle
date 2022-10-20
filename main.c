//  bestWordle.c
//  Author:
//
//  Links to wordle dictionary words at:
//    https://www.reddit.com/r/wordle/comments/s4tcw8/a_note_on_wordles_word_list/
//

#include <stdio.h>
#include <stdlib.h> // for exit( -1)
#include <string.h>

struct word_struct {
  char word[81];
  int score;
};

typedef struct word_struct wordCountStruct;

//----------------------------------------------------------------------------------------
// Comparator for use in built-in qsort(..) function.  Parameters are declared
// to be a generic type, so they will match with anything. This is a two-part
// comparison.  First the scores are compared.  If they are the same, then the
// words themselves are also compared, so that the results are in descending
// order by score, and within score they are in alphabetic order.
int compareFunction(const void *a, const void *b) {
  // Before using parameters we have cast them into the actual type they are in
  // our program and then extract the numerical value used in comparison
  int firstScore = ((wordCountStruct *)a)->score;
  int secondScore = ((wordCountStruct *)b)->score;

  // If scores are different, then that's all we need for our comparison.
  if (firstScore != secondScore) {
    // We reverse the values, so the result is in descending vs. the otherwise
    // ascending order return firstScore - secondScore;   // ascending order
    return secondScore - firstScore; // descending order
  } else {
    // Scores are equal, so check words themselves, to put them in alphabetical
    // order Hint: return the value of strcmp( parameter a word,  parameter b
    // word)
    return strcmp(((wordCountStruct *)a)->word, ((wordCountStruct *)b)->word);
  }
} // end compareFunction(..)

// ...
// Sort the allWords array in descending order by score, and within score they
// should also be sorted into ascending order alphabetically.  Use the built-in
// C quick sort qsort(...). The last parameter is the name of the comparison
// function we use (a function pointer).
//
// ... uncomment out the line below once you have it in a meaningful context in
// your code. qsort( theArray, numElementsInArray, sizeof( your struct),
// compareFunction);

void sortArrays(wordCountStruct *allWords, size_t length) {
  // in case segfault, set word size to 6 (since wordle has 5 words)
  qsort(allWords, length, sizeof(wordCountStruct), compareFunction);
  for (int i = length - 1; i > 0; i++) {
    // printf("%d", )
  }
  // Sort the allWords array in descending order by score, and within score they
  // should also be sorted into ascending order alphabetically.
}

int textCounter(char *fileName) {
  FILE *inFilePtr = fopen(fileName, "r"); // Connect logical name to filename
  char inputString[81];
  int textCounter = 0;

  // Sanity check: ensure file open worked correctly
  if (inFilePtr == NULL) {
    printf("Error: could not open %s for reading\n", fileName);
    exit(-1); // must include stdlib.h
  }

  // Read each word from file and print it.  You could do other things along the
  // way, like counting how many words there are.
  while (fscanf(inFilePtr, "%s", inputString) != EOF) {
    textCounter++;
  }

  return textCounter;
}

void parseText(wordCountStruct *allWords, char *guessesFileName,
               char *answersFileName) {
  // Sample code to read in from a file
  char fileName[81];
  char inputString[81];
  FILE *inFilePtr;
  int index = 0;

  strncpy(fileName, guessesFileName, 80);
  for (;;) {
    inFilePtr = fopen(fileName, "r"); // Connect logical name to filename

    // Sanity check: ensure file open worked correctly
    if (inFilePtr == NULL) {
      exit(-1); // must include stdlib.h
    }

    // Read each word from file and print it.  You could do other things along
    // the way, like counting how many words there are.
    while (fscanf(inFilePtr, "%s", inputString) != EOF) {
      strncpy(allWords[index++].word, inputString, 80);
    }

    // Close the file
    fclose(inFilePtr);

    if (strcmp(fileName, answersFileName) == 0) {
      return;
    }

    strncpy(fileName, answersFileName, 80);
  }
}

// -----------------------------------------------------------------------------------------
int main() {
  char answersFileName[81]; // Stores the answers file name
  char guessesFileName[81]; // Stores the guesses file name
  // Set default file names

  // variables
  int allWordsLength;
  wordCountStruct *allWords;

  printf("Default file names are %s and %s\n", answersFileName,
         guessesFileName);

  // Display menu, to allow partial credit for different program components
  int menuOption = 0;
  do {
    printf("\n");
    printf("Menu Options:\n");
    printf("  1. Display best first words only\n");
    printf("  2. Display best first and best second words\n");
    printf("  3. Change answers and guesses filenames\n");
    printf("  4. Exit\n");
    printf("Your choice: ");
    scanf("%d", &menuOption);

    // Handle menu option 3 to exit the program
    if (menuOption == 4) {
      exit(1); // Exit the program
    } else if (menuOption == 3) {
      // Change file names.  Menu will then be redisplayed.
      printf("Enter new answers and guesses filenames: ");
      scanf("%s %s", answersFileName, guessesFileName);
    }
  } while (menuOption == 3);

  // runs from option 1, 2
  allWordsLength = textCounter(guessesFileName) + textCounter(answersFileName);
  allWords = malloc(allWordsLength * sizeof(wordCountStruct));

  parseText(allWords, guessesFileName, answersFileName);

  for (int i = 0; i < allWordsLength; i++)
    printf("%s\n", allWords[i].word);

  printf("Done\n");
  return 0;
} // end main()
