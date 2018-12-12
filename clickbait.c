#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "train_file.h"

int main(void){
  wordLists allWordLists[NUM_OF_WORD_FEATS];
  headline *arrHeadline;

  arrHeadline = (headline *) malloc(HEADLINES * sizeof(headline));

  train_classifier(allWordLists, arrHeadline);
  make_calculations(allWordLists, arrHeadline);

  return EXIT_SUCCESS;
}
