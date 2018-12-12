#ifndef TRAIN_FILE_H
#define TRAIN_FILE_H
#include "train_file.h"

// #include "make_calc.h"
#define HEADLINES 32000
#define LENGTH 250

/*make_calc.h*/
#define TESTSIZE 10000
#define HEADLINES 32000
#define LENGTH 250
#define NUM_OF_WORD_FEATS 4

/*make_calc.h*/
typedef struct{
  char featureName[LENGTH];
  int clickbaitNumber;
  int nonClickbaitNumber;
  int isPrevailent;
}outputFeatureData;

typedef struct{
  char text[LENGTH];
  int isCB;
  int ourPrediction;
}testSet;

/*train_file.h*/
typedef struct{
  char text[LENGTH];
  int isClickbait;
}headline;

typedef struct {
  char word[LENGTH];
} wordStrut;

typedef struct{
  char featureName[LENGTH];
  wordStrut words[LENGTH];
  int totalWords;
}wordLists;

typedef struct{
  char featureName[LENGTH];
  int clickbaitNumber;
  int nonClickbaitNumber;
}feature;

/*make_calc.h*/
void make_calculations(wordLists allWordLists[], headline *allHeadlines);
void copy_data_from_file(outputFeatureData featureData[], int *numOfFeat);
void evaluate_headline(outputFeatureData featureData[], int numOfFeat, wordLists allWordLists[], char headline[]);
void look_through_headline(char headline[], outputFeatureData *featureData, wordLists allWordLists[]);
void check_number_feature(char headline[], outputFeatureData *featureData);
void calc_naive_bayes(outputFeatureData featureData[], int numOfFeat, double *percentIsCB, double *percentIsNotCB);
void calc_prob_is_cb(outputFeatureData featureData[], int numOfFeat, int numOfClickbait, int numOfNonClickbait,
                     int totalHeadlines, double *numerator, double *denominator, double *probIsCB);
void calc_prob_is_not_cb(outputFeatureData featureData[], int numOfFeat, int numOfClickbait, int numOfNonClickbait,
                         int totalHeadlines, double *numerator, double denominator, double *probIsNotCB);
void run_test_set(headline *arrHeadline, outputFeatureData featureData[], int numOfFeat, wordLists allWordLists[]);
void load_test_set(int *headlineAmmount, testSet *testDataSet);
void set_prediction(testSet *testDataSet, int headlineAmmount, double percentIsCB);
void find_precision_racall(testSet *testDataSet, int headlineAmmount);

/*train_file.h*/
void fill_headline_struct(headline *arrHeadline, int *totalHeadlines, int *clickbaitHeadline, int *nonClickbaitHeadline);
void train_classifier(wordLists allWordLists[], headline *arrHeadline);
void feature_number(feature *numberFeature, headline arrHeadline);
void set_feature_to_zero(feature *data, char name[]);
void fill_feature_structs(wordLists *arrHyperbolic, char fileName[], char featureName[]);
void feature_string_count(feature *featureData, headline arrHeadline, wordLists arrHyperbolic);
void output_feature_data_document(feature numberFeature, feature hyperbolicFeature, feature slangFeature,
                                  feature forwardReferenceFeature, feature contractionFeature,
                                  int clickbaitHeadline, int nonClickbaitHeadline);

#endif
