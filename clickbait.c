#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "train_file.h"

void initilize_program(wordLists allWordLists[], feature allFeature[]);

int main(void){
  wordLists allWordLists[NUM_OF_WORD_FEATS];
  feature allFeature[5];
  headline *arrHeadline;

  arrHeadline = (headline *) malloc(HEADLINES * sizeof(headline));

  initilize_program(allWordLists, allFeature);
  train_classifier(allWordLists, arrHeadline, allFeature);
  make_calculations(allWordLists, arrHeadline);

  return EXIT_SUCCESS;
}

void initilize_program(wordLists allWordLists[], feature allFeature[]){

  wordLists arrHyperbolic, arrSlang, arrForwardReference, arrContraction;
  feature numberFeature, hyperbolicFeature, slangFeature, contractionFeature, forwardReferenceFeature;

  fill_feature_structs(&arrHyperbolic, "data_Files/hyperbolic.txt", "hyperbolic");
  fill_feature_structs(&arrSlang, "data_Files/slang.txt", "slang");
  fill_feature_structs(&arrForwardReference, "data_Files/forward_reference.txt", "forward_reference");
  fill_feature_structs(&arrContraction, "data_Files/contract.txt", "contraction");

  allWordLists[0] = arrHyperbolic;
  allWordLists[1] = arrSlang;
  allWordLists[2] = arrForwardReference;
  allWordLists[3] = arrContraction;

  set_feature_to_zero(&numberFeature, "number");
  set_feature_to_zero(&hyperbolicFeature, "hyperbolic");
  set_feature_to_zero(&slangFeature, "slang");
  set_feature_to_zero(&forwardReferenceFeature, "forward_reference");
  set_feature_to_zero(&contractionFeature, "contraction");

  allFeature[0] = numberFeature;
  allFeature[1] = hyperbolicFeature;
  allFeature[2] = slangFeature;
  allFeature[3] = contractionFeature;
  allFeature[4] = forwardReferenceFeature;

}

void fill_headline_struct(headline *arrHeadline, int *totalHeadlines, int *clickbaitHeadline, int *nonClickbaitHeadline){
  FILE *clickbaitFile = fopen("headline_Files/clickbait.txt", "r");
  FILE *nonClickbaitFile = fopen("headline_Files/non_clickbait.txt", "r");
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
