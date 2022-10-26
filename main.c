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
    }
}

// int calculateScore(char *guessWord, char *answerWord) {
//   int i, j, score = 0;

//   /* first pass, add exact matches */
//   for (i = 0; i < 5; ++i) {
//     if (guessWord[i] == answerWord[i]) {
//       score += 3;
//       guessWord[i] = ' ';
//       answerWord[i] = ' ';
//     }
//   }

//   /* second pass, add match in diff position*/
//   for (i = 0; i < 5; ++i) {
//     for (j = 0; j < 5; ++j) {
//       if (answerWord[i] == '\0') {
//         continue;
//       }
//       if (answerWord[i] == guessWord[j]) {
//         score++;
//       }
//     }
//   }

//   return score;
// }

// void scoringFunction(wordCountStruct *allWords, int allWordsLength,
//                      int answersIndex) {
//   int i, j;
//   int totalScore = 0;

//   for (i = 0; i < allWordsLength; ++i) {
//     for (j = answersIndex + 1; j < allWordsLength; ++j) {
//       totalScore += calculateScore(allWords[i].word, allWords[j].word);
//     }

//     allWords[i].score = totalScore;
//     totalScore = 0;
//   }
// }




int calculateScore2(char *guessWord, char *answerWord) {
    int i, j;
    int score = 0;
    char guessBuffer[6];
    char answerBuffer[6];

    strncpy(guessBuffer, guessWord, 5);
    strncpy(answerBuffer, answerWord, 5);

    for(i = 0; i < 5; i++) {
        if (guessBuffer[i] == answerBuffer[i]) {
            score += 3;
            guessBuffer[i] = '-';
            answerBuffer[i] = '_';
        }
    }

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

void scoringFunction2(wordCountStruct *allWords, int allWordsLength,
        int guessesLength) {
    int i, j;
    int totalScore = 0;
    char *guessWord;
    char *answerWord;

    for (i = 0; i < allWordsLength; ++i) {
        guessWord = allWords[i].word;

        for (j = guessesLength; j < allWordsLength; ++j) {
            answerWord = allWords[j].word;
            totalScore += calculateScore2(guessWord, answerWord);
        }

        allWords[i].score = totalScore;
        totalScore = 0;
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
    scoringFunction2(allWords, allWordsLength, guessesLength);
    sortArrays(allWords, allWordsLength);

    printf("All words in descending order by score:\n");
    for (int i = 0; i < allWordsLength; i++)
        printf("%d %s\n", allWords[i].score, allWords[i].word);

    printf("Top scoring words:\n");
    int topScore = allWords->score;
    for (int i = 0; i < allWordsLength; i++) {
        if (allWords[i].score != topScore)
            break;

        printf("%s %d\n", allWords[i].word, allWords[i].score);
    }

    return 0;
} // end main()

// vi: expandtab ts=4 sw=4
