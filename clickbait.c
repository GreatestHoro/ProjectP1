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
void calc_naive_bayes(outputFeatureData featureData[], int numOfFeat, double *percentIsCB, double *percentIsNotCB);
void calc_prob_is_cb(outputFeatureData featureData[], int numOfFeat, int numOfClickbait,
                     int totalHeadlines, double *numerator, double *denominator, double *probIsCB);
void calc_prob_is_not_cb(outputFeatureData featureData[], int numOfFeat, int numOfNonClickbait, int totalHeadlines,
                         double *numerator, double denominator, double *probIsNotCB);

int main(void){
  wordLists allWordLists[NUM_OF_WORD_FEATS];

  train_classifier(allWordLists);
  make_calculations(allWordLists);

  return EXIT_SUCCESS;
}

void make_calculations(wordLists allWordLists[]){
  outputFeatureData featureData[LENGTH];
  char headline[] = "These Are the, 100 Of Funniest British Tweets Of 2018";
  int numOfFeat, i;
  double percentIsCB, percentIsNotCB;

  //copy the trained classifier over to an array of outputFeatureData structs
  copy_data_from_file(featureData, &numOfFeat);
  //scan a headline for features
  evaluate_headline(featureData, numOfFeat, allWordLists, headline);
  //calculate the chance for the hLine = CB through NB after having scanned its features
  calc_naive_bayes(featureData, numOfFeat, &percentIsCB, &percentIsNotCB);

  printf("\nGiven headline: '%s'\n"
         "There is %.2lf %% chance that the headline is CB and there is\n"
         "%.2lf %% chance it is !CB\n",
         headline, percentIsCB, percentIsNotCB);

}
//function that takes in featureData array, and calculates the probability for a headline is
//clickbait given these features are isPrevailent in the headline
void calc_naive_bayes(outputFeatureData featureData[], int numOfFeat, double *percentIsCB, double *percentIsNotCB){
  double numerator = 1.0, denominator = 1.0, probIsCB, probIsNotCB, totalProbability;
  int numOfClickbait, numOfNonClickbait, totalHeadlines;

  numOfClickbait = featureData[numOfFeat - 1].clickbaitNumber;
  numOfNonClickbait = featureData[numOfFeat - 1].nonClickbaitNumber;
  totalHeadlines = numOfClickbait + numOfNonClickbait;

  //1st objective: is to calculate the probability that a HLine is CB given these prevailent features
  calc_prob_is_cb(featureData, numOfFeat, numOfClickbait, totalHeadlines, &numerator, &denominator, &probIsCB);
  //2nd objective: to calculate the probabilty for that the given hLine is not clickbait according to NB
  //reset the numerator for the new Calculations, denominator is the same
  numerator = 1.0;
  calc_prob_is_not_cb(featureData, numOfFeat, numOfNonClickbait, totalHeadlines, &numerator, denominator, &probIsNotCB);

  //calculate the probabilities to percentages and return those through output parameters
  totalProbability = probIsCB + probIsNotCB;
  *percentIsCB = (probIsCB / totalProbability) * 100;
  *percentIsNotCB = (probIsNotCB / totalProbability) * 100;

}
//function that calculates the probabilty for that af hLine is CB given features
// P(CB = Yes | x1, x2, ... xn)
void calc_prob_is_cb(outputFeatureData featureData[], int numOfFeat, int numOfClickbait, int totalHeadlines,
                      double *numerator, double *denominator, double *probIsCB) {
  int i;
  //loop to calculate each numerator and denominator of features that are prevailent in the hLine according to NB
  //numerator: P(x1 = yes | CB) * P( x2 = yes | CB)) * P(x3 = yes | CB) ... P(xn = yes | CB)
  //denominator:  P(X1 = yes) * P(x2 = yes) * ... P(xn = yes)
  for(i = 0; i < numOfFeat - 1; i++){ //numOfFeat - 1 because the last featureData is not a feauture but holds overall data
     if (featureData[i].isPrevailent == 1){
      *numerator *= ((double) featureData[i].clickbaitNumber / (double) numOfClickbait);
      *denominator *= (((double) featureData[i].clickbaitNumber + featureData[i].nonClickbaitNumber) / (double) totalHeadlines);
      printf("types found: %s\n", featureData[i].featureName);
     }
  }
  //multiply P(CB = Yes) to the numerator
  *numerator *= ((double) numOfClickbait / (double) totalHeadlines);
  //set the probabilty for is CB = P(CB = Yes | x1, x2, ... xn)
  *probIsCB = *numerator / *denominator;
}
//function that calculates the probability for that hLine is Not CB given features
// P(CB = No | x1, x2, ... xn)
void calc_prob_is_not_cb(outputFeatureData featureData[], int numOfFeat, int numOfNonClickbait, int totalHeadlines,
                      double *numerator, double denominator, double *probIsNotCB) {
  int i;
  //loop to calculate the product of each numerator like before, but just now with the new formula
  //numerator: P(x1 = yes | !CB) * P(x2 = yes | !CB) ... P(x3 = yes | !CB)
  for(i = 0; i < numOfFeat - 1; i++) {
    if (featureData[i].isPrevailent == 1){
      *numerator *= ((double) featureData[i].nonClickbaitNumber / (double) numOfNonClickbait);
      //printf("%s - %d %d\n", featureData[i].featureName, featureData[i].clickbaitNumber, featureData[i].nonClickbaitNumber);
      //printf("numerator %lf\n", numerator);
    }
  }
  //multiply P(CB = No) to the numerator
  *numerator *= ((double) numOfNonClickbait / (double) totalHeadlines);
  //set the probability for is !CB = P(CB = No | x1, x2, ... xn) - the denominator is the same
  *probIsNotCB = *numerator / denominator;
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
            printf("specific words/phrases found - '%s' (%s) \n", allWordLists[i].words[j].word, featureData->featureName);
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
