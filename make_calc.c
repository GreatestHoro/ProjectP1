
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "train_file.h"

void make_calculations(wordLists allWordLists[], headline *arrHeadline){
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

  run_test_set(arrHeadline, featureData, numOfFeat, allWordLists);

  printf("\nGiven headline: '%s'\n"
         "There is %.2lf %% chance that the headline is CB and there is\n"
         "%.2lf %% chance it is !CB\n",
         headline, percentIsCB, percentIsNotCB);

}

void run_test_set(headline *arrHeadline, outputFeatureData featureData[], int numOfFeat, wordLists allWordLists[]){
  testSet *testDataSet;
  testDataSet = (testSet *) malloc(TESTSIZE * sizeof(testSet));
  int i, headlineAmmount;
  double percentIsCB = 0.0, percentIsNotCB = 0.0;

  load_test_set(&headlineAmmount, testDataSet);

  for(i = 0; i < headlineAmmount; i++){
    evaluate_headline(featureData, numOfFeat, allWordLists, testDataSet[i].text);
    calc_naive_bayes(featureData, numOfFeat, &percentIsCB, &percentIsNotCB);
    set_prediction(&testDataSet[i], headlineAmmount, percentIsCB);
  }
  find_precision_racall(testDataSet, headlineAmmount);
}

void find_precision_racall(testSet *testDataSet, int headlineAmmount){
  int truePositives = 0, falsePositives = 0, trueNegatives = 0, falseNegatives = 0, i, result;
  double precision = 0.0, recall = 0.0, f_one = 0.0;

  for(i = 0; i < headlineAmmount; i++){
    // printf("ourPrediction = %d isCB = %d\n", testDataSet[i].ourPrediction, testDataSet[i].isCB);
    if(testDataSet[i].ourPrediction == 1 && testDataSet[i].isCB == 1){
      truePositives++;
    }else if(testDataSet[i].ourPrediction == 1 && testDataSet[i].isCB == 0){
      falsePositives++;
    }else if(testDataSet[i].ourPrediction == 0 && testDataSet[i].isCB == 0){
      trueNegatives++;
    }else{  /*our prediction == 0 and isCB == 1*/
      falseNegatives++;
    }
  }
  result = truePositives + falsePositives + trueNegatives + falseNegatives;
  precision = (double) truePositives / (double) (truePositives + falsePositives);
  recall = (double)  truePositives / (double) (truePositives + falseNegatives);
  f_one = 2 * ((precision * recall) / (precision + recall));


  printf("TP = %d, FP = %d, TN = %d, FN = %d, result = %d, allHeadliens = %d\n", truePositives, falsePositives, trueNegatives, falseNegatives, result, headlineAmmount);
  printf("The F1 score is %.4lf\n", f_one);
  printf("Our precision is %.4lf\n"
         "and recall is %.4lf\n", precision, recall);
}

void set_prediction(testSet *testDataSet, int headlineAmmount, double percentIsCB){
  if(percentIsCB > 50.0){
    // printf("%lf\n", percentIsCB);
    testDataSet->ourPrediction = 1;
  }else{
    testDataSet->ourPrediction = 0;
  }
}

void load_test_set(int *headlineAmmount, testSet *testDataSet){
  FILE *fileNonCB = fopen("headline_Files/non_cb_test_set.txt", "r");
  FILE *fileCB = fopen("headline_Files/cb_test_set.txt", "r");
  char input[LENGTH];
  int i = 0, len;
  if (fileNonCB == NULL || fileCB == NULL) {
    printf("Sorry Could Not Open The Test Files. Bye.\n");
    exit(EXIT_FAILURE);
  }
  while(fgets(input, LENGTH, fileNonCB) != NULL){
    len = strlen(input) - 1;
    if(len > 1){
      strcpy(testDataSet[i].text, input);
      strcpy(input + len, "\0");
      testDataSet[i].isCB = 0;
      // printf("%s\n", testDataSet[i].text);
      i++;
    }
  }
    while(fgets(input, LENGTH, fileCB) != NULL){
      len = strlen(input) - 1;
      if(len > 1){
        strcpy(testDataSet[i].text, input);
        strcpy(input + len, "\0");
        testDataSet[i].isCB = 1;
        // printf("%s\n", testDataSet[i].text);
        i++;
    }
  }
  *headlineAmmount = i;
  // printf("%d headlines\n", *headlineAmmount);
  fclose(fileNonCB);
  fclose(fileCB);
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
  calc_prob_is_cb(featureData, numOfFeat, numOfClickbait, numOfNonClickbait, totalHeadlines, &numerator, &denominator, &probIsCB);
  //2nd objective: to calculate the probabilty for that the given hLine is not clickbait according to NB
  //reset the numerator for the new Calculations, denominator is the same
  numerator = 1.0;
  calc_prob_is_not_cb(featureData, numOfFeat, numOfClickbait, numOfNonClickbait, totalHeadlines, &numerator, denominator, &probIsNotCB);

  //calculate the probabilities to percentages and return those through output parameters
  totalProbability = probIsCB + probIsNotCB;
  *percentIsCB = (probIsCB / totalProbability) * 100;
  *percentIsNotCB = (probIsNotCB / totalProbability) * 100;

}
//function that calculates the probabilty for that af hLine is CB given features
// P(CB = Yes | x1, x2, ... xn)
void calc_prob_is_cb(outputFeatureData featureData[], int numOfFeat, int numOfClickbait, int numOfNonClickbait, int totalHeadlines,
                      double *numerator, double *denominator, double *probIsCB) {
  int i;
  //loop to calculate each numerator and denominator of features that are prevailent in the hLine according to NB
  //numerator: P(x1 = yes | CB) * P( x2 = yes | CB)) * P(x3 = yes | CB) ... P(xn = yes | CB)
  //denominator:  P(X1 = yes) * P(x2 = yes) * ... P(xn = yes)
  for(i = 0; i < numOfFeat - 1; i++){ //numOfFeat - 1 because the last featureData is not a feauture but holds overall data
     if (featureData[i].isPrevailent == 1){
      *numerator *= ((double) featureData[i].clickbaitNumber / (double) numOfClickbait);
      *denominator *= (((double) featureData[i].clickbaitNumber + featureData[i].nonClickbaitNumber) / (double) totalHeadlines);
    }else{
      *numerator *= ((double) (numOfClickbait - featureData[i].clickbaitNumber) / (double) numOfNonClickbait);
      *denominator *= (((double) (totalHeadlines - featureData[i].clickbaitNumber - featureData[i].nonClickbaitNumber)) / (double) totalHeadlines);
    }

  }
  //multiply P(CB = Yes) to the numerator
  *numerator *= ((double) numOfClickbait / (double) totalHeadlines);
  //set the probabilty for is CB = P(CB = Yes | x1, x2, ... xn)
  *probIsCB = *numerator / *denominator;
}
//function that calculates the probability for that hLine is Not CB given features
// P(CB = No | x1, x2, ... xn)
void calc_prob_is_not_cb(outputFeatureData featureData[], int numOfFeat, int numOfClickbait, int numOfNonClickbait, int totalHeadlines,
                      double *numerator, double denominator, double *probIsNotCB) {
  int i;
  //loop to calculate the product of each numerator like before, but just now with the new formula
  //numerator: P(x1 = yes | !CB) * P(x2 = yes | !CB) ... P(x3 = yes | !CB)
  for(i = 0; i < numOfFeat - 1; i++) {
    if (featureData[i].isPrevailent == 1){
      *numerator *= ((double) featureData[i].nonClickbaitNumber / (double) numOfNonClickbait);
    }else{
      *numerator *= ((double)(numOfNonClickbait - featureData[i].nonClickbaitNumber) / (double) numOfNonClickbait);
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
            // printf("specific words/phrases found - '%s' (%s) \n", allWordLists[i].words[j].word, featureData->featureName);
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
