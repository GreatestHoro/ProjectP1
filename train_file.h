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

/*Input: *allHeadliens */
/*What does it do: */
/*Output: allWordLists[] */
void make_calculations(wordLists allWordLists[], headline *allHeadlines);

/*Input: */
/*What does it do: */
/*Output: */
void copy_data_from_file(outputFeatureData featureData[], int *numOfFeat);

/*Input: */
/*What does it do: */
/*Output: */
void evaluate_headline(outputFeatureData featureData[], int numOfFeat, wordLists allWordLists[], char headline[]);

/*Input: */
/*What does it do: */
/*Output: */
void look_through_headline(char headline[], outputFeatureData *featureData, wordLists allWordLists[]);

/*Input: */
/*What does it do: */
/*Output: */
void check_number_feature(char headline[], outputFeatureData *featureData);

/*Input: */
/*What does it do: */
/*Output: */
void calc_naive_bayes(outputFeatureData featureData[], int numOfFeat, double *percentIsCB, double *percentIsNotCB);

/*Input: */
/*What does it do: */
/*Output: */
void calc_prob_is_cb(outputFeatureData featureData[], int numOfFeat, int numOfClickbait, int numOfNonClickbait,
                     int totalHeadlines, double *numerator, double *denominator, double *probIsCB);

/*Input: */
/*What does it do: */
/*Output: */
void calc_prob_is_not_cb(outputFeatureData featureData[], int numOfFeat, int numOfClickbait, int numOfNonClickbait,
                         int totalHeadlines, double *numerator, double denominator, double *probIsNotCB);

/*Input: */
/*What does it do: */
/*Output: */
void run_test_set(headline *arrHeadline, outputFeatureData featureData[], int numOfFeat, wordLists allWordLists[]);

/*Input: */
/*What does it do: */
/*Output: */
void load_test_set(int *headlineAmmount, testSet *testDataSet);

/*Input: */
/*What does it do: */
/*Output: */
void set_prediction(testSet *testDataSet, int headlineAmmount, double percentIsCB);

/*Input: */
/*What does it do: */
/*Output: */
void find_precision_racall(testSet *testDataSet, int headlineAmmount);

/*fill_feature_structs.h*/

/*Input: */
/*What does it do: */
/*Output: */
void initilize_program(wordLists allWordLists[], feature allFeature[]);

/*train_file.h*/

/*Input: */
/*What does it do: */
/*Output: */
void fill_headline_struct(headline *arrHeadline, int *totalHeadlines, int *clickbaitHeadline, int *nonClickbaitHeadline);

/*Input: */
/*What does it do: */
/*Output: */
void train_classifier(wordLists allWordLists[], headline *arrHeadline, feature allFeature[]);

/*Input: */
/*What does it do: */
/*Output: */
void feature_number(feature *numberFeature, headline arrHeadline);

/*Input: */
/*What does it do: */
/*Output: */
void set_feature_to_zero(feature *data, char name[]);

/*Input: */
/*What does it do: */
/*Output: */
void fill_feature_structs(wordLists *arrHyperbolic, char fileName[], char featureName[]);

/*Input: */
/*What does it do: */
/*Output: */
void feature_string_count(feature *featureData, headline arrHeadline, wordLists arrHyperbolic);

/*Input: */
/*What does it do: */
/*Output: */
void output_feature_data_document(feature numberFeature, feature hyperbolicFeature, feature slangFeature,
                                  feature forwardReferenceFeature, feature contractionFeature,
                                  int clickbaitHeadline, int nonClickbaitHeadline);

#endif
