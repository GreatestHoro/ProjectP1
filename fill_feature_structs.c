#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "train_file.h"

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



void set_feature_to_zero(feature *data, char name[]){
  strcpy(data->featureName, name);
  data->clickbaitNumber = 0;
  data->nonClickbaitNumber = 0;
}
