#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "train_file.h"

void train_classifier(wordLists allWordLists[], headline *arrHeadline, feature allFeature[]){

  int i, totalHeadlines = 0, clickbaitHeadline = 0, nonClickbaitHeadline = 0;

  fill_headline_struct(arrHeadline, &totalHeadlines, &clickbaitHeadline, &nonClickbaitHeadline);
/*
  for(i = 0; i < HEADLINES; i++){
    feature_number(&numberFeature, arrHeadline[i]);
    feature_string_count(&hyperbolicFeature, arrHeadline[i], arrHyperbolic);
    feature_string_count(&slangFeature, arrHeadline[i], arrSlang);
    feature_string_count(&forwardReferenceFeature, arrHeadline[i], arrForwardReference);
    feature_string_count(&contractionFeature, arrHeadline[i], arrContraction);
  }
  */

  for(i = 0; i < HEADLINES; i++){
    feature_number(&allFeature[0], arrHeadline[i]);
    feature_string_count(&allFeature[1], arrHeadline[i], allWordLists[0]);
    feature_string_count(&allFeature[2], arrHeadline[i], allWordLists[1]);
    feature_string_count(&allFeature[3], arrHeadline[i], allWordLists[2]);
    feature_string_count(&allFeature[4], arrHeadline[i], allWordLists[3]);
  }

  printf("%-20s is used int %-5d clickbait and %-5d nonclickbait\n"
         "%-20s is used int %-5d clickbait and %-5d nonclickbait\n"
         "%-20s is used int %-5d clickbait and %-5d nonclickbait\n"
         "%-20s is used int %-5d clickbait and %-5d nonclickbait\n"
         "%-20s is used int %-5d clickbait and %-5d nonclickbait\n", allFeature[0].featureName, allFeature[0].clickbaitNumber, allFeature[0].nonClickbaitNumber,
                                                               allFeature[1].featureName, allFeature[1].clickbaitNumber, allFeature[1].nonClickbaitNumber,
                                                               allFeature[2].featureName, allFeature[2].clickbaitNumber, allFeature[2].nonClickbaitNumber,
                                                               allFeature[3].featureName, allFeature[3].clickbaitNumber, allFeature[3].nonClickbaitNumber,
                                                               allFeature[4].featureName, allFeature[4].clickbaitNumber, allFeature[4].nonClickbaitNumber);

    output_feature_data_document(allFeature[0], allFeature[1], allFeature[2],
                                 allFeature[3], allFeature[4],
                                 clickbaitHeadline, nonClickbaitHeadline);

  free(arrHeadline);
}



void fill_feature_structs(wordLists *arrFeature, char fileName[], char featureName[]){
  FILE *filePointer = fopen(fileName, "r");
  char input[LENGTH];
  int i = 0, len;
  if (filePointer == NULL) {
    printf("Sorry Could Not Open File In fill_feature_structs. Bye.\n");
    exit(EXIT_FAILURE);
  }
  while(fgets(input, LENGTH, filePointer) != NULL){
    len = strlen(input) - 1;
    strcpy(arrFeature->featureName, featureName);
    if(len > 1){
      strcpy(input + len, "\0");
      strcpy(arrFeature->words[i].word, input);
      i++;
    }
  }
  arrFeature->totalWords = i;
  fclose(filePointer);
}



void feature_number(feature *featureData, headline arrHeadline){
  int i = 0, len = strlen(arrHeadline.text), digit = 0;

  do{
    if(digit = isdigit(arrHeadline.text[i])) {
      if(arrHeadline.isClickbait){
        featureData->clickbaitNumber += 1;
      }else{
        featureData->nonClickbaitNumber += 1;
      }
    }
    i++;
  }while((i < len) &&  digit != 1);
}

void feature_string_count(feature *featureData, headline arrHeadline, wordLists words){
  int digit = 0, i = 0, j = 0;
  char tempString[LENGTH];

  strcpy(tempString, arrHeadline.text);
  while(tempString[i]){
     tempString[i] = tolower(tempString[i]);
    i++;
  }
  do{
    if(strstr(tempString, words.words[j].word) != NULL){
      digit = 1;
      if(arrHeadline.isClickbait){
        featureData->clickbaitNumber += 1;
      }else{
        featureData->nonClickbaitNumber += 1;
      }
      // printf("%s - %s\n", words.words[j].word, tempString);
    }
    j++;
  }while(j < words.totalWords && digit != 1);
}

void output_feature_data_document(feature numberFeature, feature hyperbolicFeature, feature slangFeature,
                                  feature forwardReferenceFeature, feature contractionFeature,
                                  int clickbaitHeadline, int nonClickbaitHeadline){
  FILE *writeData = fopen("output_data.txt", "w");
  if(writeData != NULL){
    fprintf(writeData, "%d %d %s\n"
                       "%d %d %s\n"
                       "%d %d %s\n"
                       "%d %d %s\n"
                       "%d %d %s\n"
                       "%d %d overall_headlines\n", numberFeature.clickbaitNumber, numberFeature.nonClickbaitNumber, numberFeature.featureName,
                     hyperbolicFeature.clickbaitNumber, hyperbolicFeature.nonClickbaitNumber, hyperbolicFeature.featureName,
                   slangFeature.clickbaitNumber, slangFeature.nonClickbaitNumber, slangFeature.featureName,
                 forwardReferenceFeature.clickbaitNumber, forwardReferenceFeature.nonClickbaitNumber, forwardReferenceFeature.featureName,
               contractionFeature.clickbaitNumber, contractionFeature.nonClickbaitNumber, contractionFeature.featureName,
             clickbaitHeadline, nonClickbaitHeadline, clickbaitHeadline + nonClickbaitHeadline);
  }else{
    printf("The output fill could not be found...\n");
    exit(EXIT_FAILURE);
  }

  fclose(writeData);
}
