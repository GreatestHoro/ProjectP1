#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>

#define HEADLINES 32000
#define LENGTH 250


typedef struct{
  char text[LENGTH];
  int isClickbait;
}headline;

typedef struct {
  char word[LENGTH];
} wordStrut;

typedef struct{
  wordStrut words[LENGTH];
  int totalWords;
}wordLists;

typedef struct{
  char featureName[LENGTH];
  int clickbaitNumber;
  int nonClickbaitNumber;
}feature;

void fill_headline_struct(headline *arrHeadline, int *totalHeadlines, int *clickbaitHeadline, int *nonClickbaitHeadline);
void train_classifier(void);
void feature_number(feature *numberFeature, headline arrHeadline);
void set_feature_to_zero(feature *data, char name[]);
void fill_feature_structs(wordLists *arrHyperbolic, char fileName[]);
void feature_string_count(feature *featureData, headline arrHeadline, wordLists arrHyperbolic);

int main(void){

  train_classifier();

  return EXIT_SUCCESS;
}

void train_classifier(void){
  headline *arrHeadline;
  wordLists arrHyperbolic, arrSlang, arrForwardReference, arrContraction;
  feature numberFeature, hyperbolicFeature, slangFeature, contractionFeature, forwardReferenceFeature;
  int i, totalHeadlines = 0, clickbaitHeadline = 0, nonClickbaitHeadline = 0;

  arrHeadline = (headline *) malloc(HEADLINES * sizeof(headline));

  fill_headline_struct(arrHeadline, &totalHeadlines, &clickbaitHeadline, &nonClickbaitHeadline);
  fill_feature_structs(&arrHyperbolic, "hyperbolic.txt");
  fill_feature_structs(&arrSlang, "slang.txt");
  fill_feature_structs(&arrForwardReference, "forward_reference.txt");
  fill_feature_structs(&arrContraction, "contract.txt");


  set_feature_to_zero(&numberFeature, "number");
  set_feature_to_zero(&hyperbolicFeature, "hyperbolic");
  set_feature_to_zero(&slangFeature, "slang");
  set_feature_to_zero(&forwardReferenceFeature, "forward_reference");
  set_feature_to_zero(&contractionFeature, "contraction");

  for(i = 0; i < HEADLINES; i++){
    feature_number(&numberFeature, arrHeadline[i]);
    feature_string_count(&hyperbolicFeature, arrHeadline[i], arrHyperbolic);
    feature_string_count(&slangFeature, arrHeadline[i], arrSlang);
    feature_string_count(&forwardReferenceFeature, arrHeadline[i], arrForwardReference);
    feature_string_count(&contractionFeature, arrHeadline[i], arrContraction);



  }
  printf("%-20s is used int %-5d clickbait and %-5d nonclickbait\n"
         "%-20s is used int %-5d clickbait and %-5d nonclickbait\n"
         "%-20s is used int %-5d clickbait and %-5d nonclickbait\n"
         "%-20s is used int %-5d clickbait and %-5d nonclickbait\n"
         "%-20s is used int %-5d clickbait and %-5d nonclickbait\n", numberFeature.featureName, numberFeature.clickbaitNumber, numberFeature.nonClickbaitNumber,
                                                               hyperbolicFeature.featureName, hyperbolicFeature.clickbaitNumber, hyperbolicFeature.nonClickbaitNumber,
                                                               slangFeature.featureName, slangFeature.clickbaitNumber, slangFeature.nonClickbaitNumber,
                                                               forwardReferenceFeature.featureName, forwardReferenceFeature.clickbaitNumber, forwardReferenceFeature.nonClickbaitNumber,
                                                               contractionFeature.featureName, contractionFeature.clickbaitNumber, contractionFeature.nonClickbaitNumber);

  free(arrHeadline);
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

void fill_feature_structs(wordLists *arrFeature, char fileName[]){
  FILE *filePointer = fopen(fileName, "r");
  char input[LENGTH];
  int i = 0, len;
  if (filePointer == NULL) {
    printf("Sorry Could Not Open File In fill_feature_structs. Bye.\n");
    exit(EXIT_FAILURE);
  }
  while(fgets(input, LENGTH, filePointer) != NULL){
    len = strlen(input) - 1;
    if(len > 1){
      strcpy(input + len, "\0");
      strcpy(arrFeature->words[i].word, input);
      i++;
    }
  }

  arrFeature->totalWords = i;
  fclose(filePointer);
}

void fill_headline_struct(headline *arrHeadline, int *totalHeadlines, int *clickbaitHeadline, int *nonClickbaitHeadline){
  FILE *clickbaitFile = fopen("clickbait.txt", "r");
  FILE *nonClickbaitFile = fopen("non_clickbait.txt", "r");
  char input[LENGTH];


  if(clickbaitFile != NULL && nonClickbaitFile != NULL){
    while(fgets(input, sizeof(input), clickbaitFile) != NULL){
      if(strlen(input) > 1){
        strcpy(arrHeadline[*totalHeadlines].text, input);
        arrHeadline[*totalHeadlines].isClickbait = 1;
        //printf("%d - %s", *totalHeadlines ,arrHeadline[*totalHeadlines].text);
        *clickbaitHeadline += 1;
        *totalHeadlines += 1;
      }
    }
    while(fgets(input, sizeof(input), nonClickbaitFile) != NULL){
      if(strlen(input) > 1){
        strcpy(arrHeadline[*totalHeadlines].text, input);
        arrHeadline[*totalHeadlines].isClickbait = 0;
        //printf("%d - %s", *totalHeadlines ,arrHeadline[*totalHeadlines].text);
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

void set_feature_to_zero(feature *data, char name[]){
  strcpy(data->featureName, name);
  data->clickbaitNumber = 0;
  data->nonClickbaitNumber = 0;
}
