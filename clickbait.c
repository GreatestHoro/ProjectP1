#define NUM_OF_WORD_FEATS 4
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "train_file.h"

typedef struct{
  char featureName[LENGTH];
  int clickbaitNumber;
  int nonClickbaitNumber;
  int isPrevailent;
}outputFeatureData;

void make_calculations(wordLists allWordLists[]);
void copy_data_from_file(outputFeatureData featureData[], int *numOfFeat);
void evaluate_headline(outputFeatureData featureData[], int numOfFeat, wordLists allWordLists[], char headline[]);
void look_through_headline(char headline[], outputFeatureData *featureData, wordLists allWordLists[]);
void check_number_feature(char headline[], outputFeatureData *featureData);
void calc_naive_bayes(outputFeatureData featureData[], int numOfFeat, double *result);


int main(void){
  wordLists allWordLists[NUM_OF_WORD_FEATS];

  train_classifier(allWordLists);
  make_calculations(allWordLists);

  return EXIT_SUCCESS;
}

void make_calculations(wordLists allWordLists[]){
  outputFeatureData featureData[LENGTH];
  char headline[] = "Here They Are, 100 Of The Funniest British Tweets Of 2018";
  int numOfFeat, i;
  double result = 0.0;

  copy_data_from_file(featureData, &numOfFeat);
  evaluate_headline(featureData, numOfFeat, allWordLists, headline);


  calc_naive_bayes(featureData, numOfFeat, &result);

}
//function that takes in featureData array, and calculates the probability for a headline is
//clickbait given these features are isPrevailent in the headline
void calc_naive_bayes(outputFeatureData featureData[], int numOfFeat, double *result){
  double numerator = 1.0, denominator = 1.0;
  int numOfClickbait, numOfNonClickbait, totalHeadlines;

  numOfClickbait = featureData[numOfFeat - 1].clickbaitNumber;
  numOfNonClickbait = featureData[numOfFeat - 1].nonClickbaitNumber;
  totalHeadlines = numOfClickbait + numOfNonClickbait;
  //first objective is to calculate the probability that a HLine is CB given these prevailent features
  // P(CB = Yes | x1, x2, ... xn)
  int i;
  //loop to calculate each numerator of features that are prevailent in the hLine according to NB
  for(i = 0; i < numOfFeat - 1; i++){
     if (featureData[i].isPrevailent == 1){
      numerator *= ((double) featureData[i].clickbaitNumber / (double) numOfClickbait);
      printf("%s - %d %d\n", featureData[i].featureName, featureData[i].clickbaitNumber, featureData[i].nonClickbaitNumber);
      //  printf("numerator %lf\n", numerator);
     }
  }
  numerator *= ((double) numOfClickbait / (double) totalHeadlines);

  for(i = 0; i < numOfFeat - 1; i++){
      denominator *= ((double) featureData[i].clickbaitNumber + featureData[i].nonClickbaitNumber) / (double) totalHeadlines;
  }

  *result = numerator / denominator;

  printf("probability for yes,, -> %.2lf\n", *result);

}

void evaluate_headline(outputFeatureData featureData[], int numOfFeat, wordLists allWordLists[], char headline[]){
  int i, j;
      check_number_feature(headline, &featureData[0]);
      for(i = 1; i < numOfFeat; i++) {
        look_through_headline(headline, &featureData[i], allWordLists);
      }

}

void look_through_headline(char headline[], outputFeatureData *featureData, wordLists allWordLists[]){
  char tempString[LENGTH];
  int i = 0, j = 0;
  featureData->isPrevailent = 0;

  strcpy(tempString, headline);
  while(tempString[i]){
     tempString[i] = tolower(tempString[i]);
    i++;
  }
    for (i = 0; i < NUM_OF_WORD_FEATS; i++) {
      if(strcmp(allWordLists[i].featureName, featureData->featureName) == 0) {
        do{
          if(strstr(tempString, allWordLists[i].words[j].word) != NULL){
            featureData->isPrevailent = 1;
          }
          j++;
        }while(j < allWordLists[i].totalWords && featureData->isPrevailent != 1);
      }
    }
}

void check_number_feature(char headline[], outputFeatureData *featureData){
  int i = 0, len = strlen(headline), digit = 0;

  do{
    if(digit = isdigit(headline[i])) {
      featureData->isPrevailent = 1;
      digit = 1;
    }else{
      featureData->isPrevailent = 0;
    }
    i++;
  }while((i < len) &&  digit != 1);
}

void copy_data_from_file(outputFeatureData featureData[], int *numOfFeat){
  FILE *dataFile = fopen("output_data.txt", "r");
  char input[LENGTH], featName[LENGTH];
  int i = 0, clickNumber, nonClickNumber;
  if(dataFile != NULL){
    while(fgets(input, sizeof(input), dataFile) != NULL){
      sscanf(input, "%d %d %s", &clickNumber, &nonClickNumber, featName);
      strcpy(featureData[i].featureName, featName);
      featureData[i].clickbaitNumber = clickNumber;
      featureData[i].nonClickbaitNumber = nonClickNumber;
      i++;
    }
  }else{
    printf("the output file could not be red into a struct\n");
  }
  *numOfFeat = i;
  fclose(dataFile);
}
