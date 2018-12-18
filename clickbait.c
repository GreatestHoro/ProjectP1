#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "train_file.h"

#define STR_SIZE 200

void user_interface_two(int *answer);
void user_interface_one(int *answer);
void print_precisiont_and_recall(precisionOfProgram allPrecisionData[]);

int main(void){
  wordLists allWordLists[NUM_OF_WORD_FEATS];
  feature allFeature[NUM_OF_WORD_FEATS + 1];
  precisionOfProgram allPrecisionData[1];
  char headlineToCheck[STR_SIZE], temp;
  headline *arrHeadline;
  int answer = 0;

  arrHeadline = (headline *) malloc(HEADLINES * sizeof(headline));

  initilize_program(allWordLists, allFeature);
  user_interface_one(&answer);
  if(answer == 1){
    train_classifier(allWordLists, arrHeadline, allFeature);
  }

  do{
    user_interface_two(&answer);
    if(answer == 1){
        printf("Insert a headline you would like to scan> \n");
        scanf("%c", &temp); // to read a character from input buffer
        scanf("%[^\n]", headlineToCheck);
        make_calculations(allWordLists, arrHeadline, allPrecisionData, headlineToCheck);
    }else if(answer == 2){
      print_precisiont_and_recall(allPrecisionData);
    }else{
      printf("Bye!\n");
    }
  }while(answer < 3 && answer > 0);

  return EXIT_SUCCESS;
}




void print_precisiont_and_recall(precisionOfProgram allPrecisionData[]){
  printf("On The Test Of %d Headlines - "
         "The Programs Metrics Are\n"
         "F1 Score: %.2lf\n"
         "F0.5 Score: %.2lf\n"
         "Precision: %.2lf\nRecall: %.2lf", allPrecisionData[0].headlineAmmount,
                                                             allPrecisionData[0].f_one,
                                                             allPrecisionData[0].f_half,
                                                             allPrecisionData[0].precision,
                                                             allPrecisionData[0].recall);
}

void user_interface_one(int *answer){
  int i;
  printf("\n---------------------------------------------------------\n"
         "Welcome to the clicbait detector!\n"
         "Do you wish to update the database before starting?\n"
         "Yes[1] or No[2]\n"
         "---------------------------------------------------------\n"
         "Answer: ");
  scanf("%d", &i);
  *answer = i;
}

void user_interface_two(int *answer){
  int i;
  printf("\n---------------------------------------------------------\n"
         "Welcome to the clicbait detector!\n"
         "What do you wish to do?\n\n"
         "Press [1] to check if a headline is clickbait\n"
         "Press [2] to see the precision and recall for the program\n"
         "Press [3] to exit the program\n"
         "---------------------------------------------------------\n"
         "Answer: ");
  scanf("%d", &i);
  *answer = i;
}
