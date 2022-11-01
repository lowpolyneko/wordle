//  Program 3: Best Wordle Words
//
//  Course: CS 211, Fall 2022
//  System: Replit with clang
//  Author: Samuel Effendy, Ethan Wong
//
//  Links to wordle dictionary words at:
//    https://www.reddit.com/r/wordle/comments/s4tcw8/a_note_on_wordles_word_list/
//

#include <stdio.h>
#include <stdlib.h> // for exit( -1)
#include <string.h>


/* structs */
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
int compareFunction(const void *a, const void *b)
{
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
void sortArrays(wordCountStruct *allWords, size_t length)
{
      // in case segfault, set word size to 6 (since wordle has 5 words)
      qsort(allWords, length, sizeof(wordCountStruct), compareFunction);
}

/*
 * Given a file, returns the number of lines in the file.
 */
int textCounter(char *fileName)
{
      // Sample code to read in from a file
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
      while (fscanf(inFilePtr, "%s", inputString) != EOF)
            textCounter++;
  
      return textCounter;
}

/*
 * Given an array of all words (assumed adequate space), read the guesses and answers file
 * and dump the words into allWords. Guesses go first.
 */
void parseText(wordCountStruct *allWords,
      char *guessesFileName,
      char *answersFileName)
{
      // Sample code to read in from a file
      char fileName[81];
      char inputString[81];
      FILE *inFilePtr;
      int index = 0;
  
      strncpy(fileName, guessesFileName, 80);
      read_file:
      inFilePtr = fopen(fileName, "r"); // Connect logical name to filename
  
      // Sanity check: ensure file open worked correctly
      if (inFilePtr == NULL) {
            exit(-1); // must include stdlib.h
      }

      // Read each word from file and print it.  You could do other things along
      // the way, like counting how many words there are.
      while (fscanf(inFilePtr, "%s", inputString) != EOF) {
            strncpy(allWords[index++].word, inputString,
            80); // copies inputstring to allwords[i].word, since allword is a
            // function call of malloc, which is a void pointer, and it
            // can be accessed similarly to an array.
      }

      // Close the file
      fclose(inFilePtr);
  
      if (strcmp(fileName, answersFileName) ==
            0) { // if the files themselves are equal
            return;
      }
  
      strncpy(fileName, answersFileName, 80);
      goto read_file;
}

void removeCharacters(char *firstWord, char *answerWord) {//change type once we know what to return
    int i, j;

    /* first pass, add exact matches and replace characters in buffer */
    for(i = 0; i < 5; i++) {
        if (firstWord[i] == answerWord[i]) {
            answerWord[i] = ' ';
            continue;
        }
          
        for (j = 0; j < 5; j++) {
            //if a letter from first word is the same and is in the exact position of a letter in answer, 
            //set that letter blank in answer.
            if (firstWord[i] == answerWord[j]) {
                answerWord[j] = ' ';
                break;
            }
        }
    }
}

/*
 * given a guess and an answer word, calculate the score of the guess based on the answer
 */
//function to calculate the score for first best words.
int calculateScore(char *guessWord, char *answerWord, wordCountStruct *firstWord)
{
      int i, j;
      int score = 0;
      char guessBuffer[6];
      char answerBuffer[6];
  
      /* do scoring in a buffer */
      strncpy(guessBuffer, guessWord, 5);
      strncpy(answerBuffer, answerWord, 5);
  
      if (firstWord)
          removeCharacters(firstWord->word, answerBuffer);//maybe change this function call, since we are passing in allwords as a struct and using that to loop through
  
      /* first pass, add exact matches and replace characters in buffer */
      for(i = 0; i < 5; i++) {
            if (guessBuffer[i] == answerBuffer[i]) {
                  score += 3;
                  guessBuffer[i] = '-';
                  answerBuffer[i] = '_';
            }
      }

      /* second pass, check each guess character with every answer character and score */
      for(i = 0; i < 5; i++) {
            for(j = 0; j < 5; j++) {
                  if(guessBuffer[i] == answerBuffer[j]) {
                        score+=1;
                        guessBuffer[i] = '-';
                        answerBuffer[j] = '/';
                  }
            }
      }

      return score;
}

/*
 * given the list of all words, the length of the array, and the beginning of answers,
 * calculate the total score for all words in allWords
 */
void scoringFunction(wordCountStruct *allWords, 
                      int allWordsLength,
                      int guessesLength,
                      wordCountStruct *firstWord)
{
      int i, j;
      int totalScore = 0;
      char *guessWord;
      char *answerWord;
  
      /* for all words */
      for (i = 0; i < allWordsLength; ++i) {
            guessWord = allWords[i].word;
          
            /* loop through all answers */
            for (j = guessesLength; j < allWordsLength; ++j) {
                answerWord = allWords[j].word;
                totalScore += calculateScore(guessWord, answerWord, firstWord);
            }
    
            allWords[i].score = totalScore;
            totalScore = 0;
      }
  
}

void scoringFunction2(wordCountStruct *allWords, wordCountStruct *firstWords, int firstWordsLength, int allWordsLength, int guessesLength) {
    //for second best words, after storing words with most points, we loop through each element in firstWords and compare them to answers, removing each instance of every letter in the same position.
    // make a copy of the answer words and go through and blank out letters 
    // that would have already been covered by the first guess word.  
    // After this has been done you can do the same scoring that you did before, this time using this modified copy of all the answer words.  
    int i, j, k;
    int totalScore = 0;
    char answerWordCopy[6];
    wordCountStruct *firstWord;

    wordCountStruct *allWordsBuffer = malloc(allWordsLength * sizeof(wordCountStruct));
  
    for(i = 0; i < firstWordsLength; ++i) {
        firstWord = firstWords + i;
        memcpy(allWordsBuffer, allWords, allWordsLength * sizeof(wordCountStruct));

        scoringFunction(allWordsBuffer, allWordsLength, guessesLength, firstWord);
      
        printf("answerWordsCopy after letters from %s removed:\n", firstWord->word);
        for (int i = guessesLength; i < allWordsLength; i++) {
            strncpy(answerWordCopy, allWordsBuffer[i].word, 6);
            removeCharacters(firstWord->word, answerWordCopy);
            printf("%d. %s\n", i - guessesLength, answerWordCopy);
        }
    
        printf("Words and scores:\n");
        sortArrays(allWordsBuffer, allWordsLength);
        for (int i = 0; i < allWordsLength; i++)
            printf("%d %s\n", allWordsBuffer[i].score, allWordsBuffer[i].word);
    }
}

// -----------------------------------------------------------------------------------------
int main()
{
      char answersFileName[81]; // Stores the answers file name
      char guessesFileName[81]; // Stores the guesses file name
                                // Set default file names
  
                                // variables
      int allWordsLength;
      wordCountStruct *allWords;
      int menuOption = 0;
  
      printf("Default file names are %s and %s\n", answersFileName,
              guessesFileName);
  
      // Display menu, to allow partial credit for different program components
      do {
          printf("\n");
          printf("Menu Options:\n");
          printf("  1. Display best first words only\n");
          printf("  2. Display best first and best second words\n");
          printf("  3. Change answers and guesses filenames\n");
          printf("  4. Exit\n");
          printf("Your choice:\n ");
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
      int answersLength = textCounter(answersFileName);
      int guessesLength = textCounter(guessesFileName);
      allWordsLength = answersLength + guessesLength;
  
      printf("%s has %d words\n", answersFileName, answersLength);
      printf("%s has %d words\n", guessesFileName, guessesLength);
  
      allWords = malloc(allWordsLength * sizeof(wordCountStruct)); // we allocate the right number of space
                                                                   // because we count the number of text in
                                                                   // each file by opening it.
  
      parseText(allWords, guessesFileName, answersFileName);
  
      /* test first word, second word */
      scoringFunction(allWords, allWordsLength, guessesLength, NULL);
      sortArrays(allWords, allWordsLength);
  
      printf("All words in descending order by score:\n");
      for (int i = 0; i < allWordsLength; i++)
            printf("%d %s\n", allWords[i].score, allWords[i].word);
  
      printf("Top scoring words:\n");
      int topScore = allWords->score;
      int firstWordsLength = 0;
      for (int i = 0; i < allWordsLength; i++) {
            if (allWords[i].score != topScore)
                  break;
    
            firstWordsLength++;
            printf("%s %d\n", allWords[i].word, allWords[i].score);
      }
  
      if (menuOption == 1) {
            printf("Done\n");
            return 0;
      }
    
      wordCountStruct *firstWords = malloc(firstWordsLength * sizeof(wordCountStruct));
      for (int i = 0; i < firstWordsLength; i++)
            firstWords[i] = allWords[i];
  
      printf("Words and scores for top first words and second words:\n");
      parseText(allWords, guessesFileName, answersFileName); // Reread allWords
      scoringFunction2(allWords, firstWords, firstWordsLength, allWordsLength, guessesLength);
    
      free(allWords);
      free(firstWords);
      return 0;
} // end main()

// vi: expandtab ts=4 sw=4
