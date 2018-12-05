#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define HEADLINES 32000
#define LENGTH 250

typedef struct{
  char text[LENGTH];
  int isClickbait;
}headline;

typedef struct{
  int clickbaitNumber;
  int nonClickbaitNumber;
}features;

void fill_headline_struct(headline *arrHeadline, int *totalHeadlines, int *clickbaitHeadline, int *nonClickbaitHeadline);
void train_classifier(features *featureData);
void feature_number(features *featureData, headline arrHeadline);
void set_feature_to_zero(features *featureData);

int main(void){
  features featureData;

  train_classifier(&featureData);

  return EXIT_SUCCESS;
}

void train_classifier(features *featureData){
  headline *arrHeadline;
  int i, totalHeadlines = 0, clickbaitHeadline = 0, nonClickbaitHeadline = 0;

  arrHeadline = (headline *) malloc(HEADLINES * sizeof(headline));

  fill_headline_struct(arrHeadline, &totalHeadlines, &clickbaitHeadline, &nonClickbaitHeadline);
  printf("%s\n", arrHeadline[20].text);
  set_feature_to_zero(featureData);
  for(i = 0; i < HEADLINES; i++){
    feature_number(featureData, arrHeadline[i]);
    // printf("%s\n", arrHeadline[i].text);

  }
  printf("Clickbait artikler : %d || Non clickbait artikler %d\n", featureData->clickbaitNumber, featureData->nonClickbaitNumber);
  free(arrHeadline);
}

void feature_number(features *featureData, headline arrHeadline){
  int i = 0, len = strlen(arrHeadline.text), digit = 0;
  //printf("%s\n", arrHeadline.text);

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

void set_feature_to_zero(features *featureData){
  featureData->clickbaitNumber = 0;
  featureData->nonClickbaitNumber = 0;
}
