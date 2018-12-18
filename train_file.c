#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "train_file.h"

void train_classifier(wordLists allWordLists[], headline *arrHeadline, feature allFeature[]){

  int i, totalHeadlines = 0, clickbaitHeadline = 0, nonClickbaitHeadline = 0;

  /*Loads all the headlines from 2 text files into a struct and marks them either clickbait or not clickbait*/
  fill_headline_struct(arrHeadline, &totalHeadlines, &clickbaitHeadline, &nonClickbaitHeadline);

  /*Looks for our 5 files in each headlines*/
  for(i = 0; i < HEADLINES; i++){
    feature_number(&allFeature[0], arrHeadline[i]);                        /*Number*/
    feature_string_count(&allFeature[1], arrHeadline[i], allWordLists[0]); /*Hyperbolic*/
    feature_string_count(&allFeature[2], arrHeadline[i], allWordLists[1]); /*Slang*/
    feature_string_count(&allFeature[3], arrHeadline[i], allWordLists[2]); /*contraction*/
    feature_string_count(&allFeature[4], arrHeadline[i], allWordLists[3]); /*Forward reference*/
  }

  /*Prints how many times each feature have been seen*/
  printf("%-20s is used in %-5d clickbait and %-5d nonclickbait\n"
         "%-20s is used in %-5d clickbait and %-5d nonclickbait\n"
         "%-20s is used in %-5d clickbait and %-5d nonclickbait\n"
         "%-20s is used in %-5d clickbait and %-5d nonclickbait\n"
         "%-20s is used in %-5d clickbait and %-5d nonclickbait\n", allFeature[0].featureName, allFeature[0].clickbaitNumber, allFeature[0].nonClickbaitNumber,
                                                               allFeature[1].featureName, allFeature[1].clickbaitNumber, allFeature[1].nonClickbaitNumber,
                                                               allFeature[2].featureName, allFeature[2].clickbaitNumber, allFeature[2].nonClickbaitNumber,
                                                               allFeature[3].featureName, allFeature[3].clickbaitNumber, allFeature[3].nonClickbaitNumber,
                                                               allFeature[4].featureName, allFeature[4].clickbaitNumber, allFeature[4].nonClickbaitNumber);

    /*Writes the data into a text file*/
    output_feature_data_document(allFeature[0], allFeature[1], allFeature[2],
                                 allFeature[3], allFeature[4],
                                 clickbaitHeadline, nonClickbaitHeadline);

  free(arrHeadline);
}

void fill_headline_struct(headline *arrHeadline, int *totalHeadlines, int *clickbaitHeadline, int *nonClickbaitHeadline){
  FILE *clickbaitFile = fopen("headline_Files/clickbait.txt", "r");
  FILE *nonClickbaitFile = fopen("headline_Files/non_clickbait.txt", "r");
  char input[LENGTH];

  /*Loop starts if both files can be opended*/
  if(clickbaitFile != NULL && nonClickbaitFile != NULL){
    while(fgets(input, sizeof(input), clickbaitFile) != NULL){
      /*If the line is not emty the headline is not empty it is loaded in and marked either clicbait or clickbait and total ammount counted up*/
      if(strlen(input) > 1){
        strcpy(arrHeadline[*totalHeadlines].text, input);
        arrHeadline[*totalHeadlines].isClickbait = 1;
        *clickbaitHeadline += 1;
        *totalHeadlines += 1;
      }
    }
    while(fgets(input, sizeof(input), nonClickbaitFile) != NULL){
      if(strlen(input) > 1){
        strcpy(arrHeadline[*totalHeadlines].text, input);
        arrHeadline[*totalHeadlines].isClickbait = 0;
        *nonClickbaitHeadline += 1;
        *totalHeadlines += 1;
      }
    }
  }else{
    printf("Error!\n");
    exit(EXIT_FAILURE);
  }
  printf("%d headlines, %d clickbait and %d not clickbait\n", *totalHeadlines, *clickbaitHeadline, *nonClickbaitHeadline);

  fclose(clickbaitFile);
  fclose(nonClickbaitFile);
}

void feature_number(feature *featureData, headline arrHeadline){
  int i = 0, len = strlen(arrHeadline.text), digit = 0;

  /*Looks headline through one character at a times if it is a number. If it is the ammounnt seen is counted up-*/
  /*based on if it is clickbait or not and the loop stops*/
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

  /*All of our features are in lower case, so first we make the headline into lowe in a-*/
  /*temporary string so we can use strstr*/
  strcpy(tempString, arrHeadline.text);
  while(tempString[i]){
     tempString[i] = tolower(tempString[i]);
    i++;
  }
  /*After we use strstr to look for whole words in our headline and counts up if found*/
  do{
    if(strstr(tempString, words.words[j].word) != NULL){
      digit = 1;
      if(arrHeadline.isClickbait){
        featureData->clickbaitNumber += 1;
      }else{
        featureData->nonClickbaitNumber += 1;
      }
    }
    j++;
  }while(j < words.totalWords && digit != 1);
}

void output_feature_data_document(feature numberFeature, feature hyperbolicFeature, feature slangFeature,
                                  feature forwardReferenceFeature, feature contractionFeature,
                                  int clickbaitHeadline, int nonClickbaitHeadline){
  FILE *writeData = fopen("output_data.txt", "w");
  /*Opens a file and overwrites the data already in there, with new data based on what our software found using fprintf*/
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
