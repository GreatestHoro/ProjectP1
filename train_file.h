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
void output_feature_data_document(feature numberFeature, feature hyperbolicFeature, feature slangFeature,
                                  feature forwardReferenceFeature, feature contractionFeature,
                                  int clickbaitHeadline, int nonClickbaitHeadline);
