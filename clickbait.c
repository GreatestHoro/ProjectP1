#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "train_file.h"

int main(void){
  wordLists allWordLists[NUM_OF_WORD_FEATS];
  feature allFeature[NUM_OF_WORD_FEATS + 1];
  headline *arrHeadline;

  arrHeadline = (headline *) malloc(HEADLINES * sizeof(headline));

  initilize_program(allWordLists, allFeature);
  train_classifier(allWordLists, arrHeadline, allFeature);
  make_calculations(allWordLists, arrHeadline);

  return EXIT_SUCCESS;
}
