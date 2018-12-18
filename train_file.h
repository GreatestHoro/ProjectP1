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
/* Used when looking at the headline written by the user */
typedef struct{
  char featureName[LENGTH];
  int clickbaitNumber;
  int nonClickbaitNumber;
  int isPrevailent;
}outputFeatureData;

typedef struct{
  int truePositives;
  int falsePositives;
  int trueNegatives;
  int falseNegatives;
  int result;
  int headlineAmmount;
  double f_one;
  double f_half;
  double precision;
  double recall;
}precisionOfProgram;

typedef struct{
  char text[LENGTH];
  int isCB;
  int ourPrediction;
}testSet;

/*train_file.h*/
/* Where all the headlines are stored, isClickbait is 1 if clickbait and 0 if not */
typedef struct{
  char text[LENGTH];
  int isClickbait;
}headline;

/* Struct in a struct to store the words we are looking for */
typedef struct {
  char word[LENGTH];
} wordStrut;

/*The wordlists of the words we a looking for and how many words there are*/
typedef struct{
  char featureName[LENGTH];
  wordStrut words[LENGTH];
  int totalWords;
}wordLists;

/*each featurename and how many times they are sued in clicbait and non clickbait headlines*/
typedef struct{
  char featureName[LENGTH];
  int clickbaitNumber;
  int nonClickbaitNumber;
}feature;

/*make_calc.h*/

/*Input: *allHeadliens, allWordLists[]*/
/*What does it do: Calculates ther percent of a headline beeing clickbait */
/*Output: */
void make_calculations(wordLists allWordLists[], headline *arrHeadline, precisionOfProgram *allPrecisionData, char headlineToCheck[]);

/*Input: */
/*What does it do: Reads output_data.txt with probability of each feature into that feature */
/*Output: featureData[], numOfFeat */
void copy_data_from_file(outputFeatureData featureData[], int *numOfFeat);

/*Input: featureData[], numOfFeat, allWordLists, headline*/
/*What does it do: looks for all our features in a headline with two different functions, one for words and one for numbers*/
/*Output: featureData[] */
void evaluate_headline(outputFeatureData featureData[], int numOfFeat, wordLists allWordLists[], char headline[]);

/*Input: headline, featuredata, allWordLists */
/*What does it do: looks for each feature consisting of words and sets isPrevailent to 1 if found */
/*Output: featureData[] */
void look_through_headline(char headline[], outputFeatureData *featureData, wordLists allWordLists[]);

/*Input: headline, featureData[] */
/*What does it do: looks for numbers in the headline and sets isPrevailent to 1 */
/*Output: featureData[] */
void check_number_feature(char headline[], outputFeatureData *featureData);

/*Input: featureData, numOfFeat */
/*What does it do: looks at headline and returns a percent of it beeing clickbait based on 2 help functions */
/*Output: percentIsCB, percentIsNotCB */
void calc_naive_bayes(outputFeatureData featureData[], int numOfFeat, double *percentIsCB, double *percentIsNotCB);

/*Input: featureData, numOfFeat, numOfClickbait, numOfNonClickbait, totalHeadlines */
/*What does it do: Calculates the probabilty a headline is clickbait baised on the features apearing in it */
/*Output: numerator, denominator, probIsCB */
void calc_prob_is_cb(outputFeatureData featureData[], int numOfFeat, int numOfClickbait, int numOfNonClickbait,
                     int totalHeadlines, double *numerator, double *denominator, double *probIsCB);

/*Input: featureData, numOfFeat, numOfClickbait, numOfNonClickbait, totalHeadlines */
/*What does it do: Calculates the probabilty a headline is not clickbait baised on the features apearing in it */
/*Output: numerator, denominator, probIsNotCB */
void calc_prob_is_not_cb(outputFeatureData featureData[], int numOfFeat, int numOfClickbait, int numOfNonClickbait,
                         int totalHeadlines, double *numerator, double denominator, double *probIsNotCB);

/*Input: arrHeadline, featureData, numOfFeat. allWordLists */
/*What does it do: calculates the precesion, recall and F1 score for our program, based on 30% of our headlines with help functions */
/*Output: precision, recall, f_one */
void run_test_set(headline *arrHeadline, outputFeatureData featureData[], int numOfFeat, wordLists allWordLists[], precisionOfProgram *allPrecisionData);

/*Input: testDataSet */
/*What does it do: Loads the test headlines and wether it is clickbait or not into a struct */
/*Output: headlineAmmount */
void load_test_set(int *headlineAmmount, testSet *testDataSet);

/*Input: headlineAmmount, percentIsCB */
/*What does it do: According to the rating we gave it, we set our predicont to either 1 if we think it is clickbait or 0 if not */
/*Output: testDataSet */
void set_prediction(testSet *testDataSet, int headlineAmmount, double percentIsCB);

/*Input: testDataSet, headlineAmmount */
/*What does it do: compares if our prediction if a headline is clicbait or not to calulate precision and recall */
/*Output: f_one, precision, recall */
void find_precision_racall(testSet *testDataSet, int headlineAmmount, precisionOfProgram *allPrecisionData);

/*fill_feature_structs.h*/

/*Input: */
/*What does it do: uses 2 help functions to put the clicbait words into a struct */
/*Output: allWordLists, allFeature */
void initilize_program(wordLists allWordLists[], feature allFeature[]);

/*Input: arrFeature, file, featureName */
/*What does it do: fills the words to look for in headlines into a struct */
/*Output: */
void fill_feature_structs(wordLists *arrHyperbolic, char fileName[], char featureName[]);

/*Input: feature */
/*What does it do: sets the ammount each feature have been seen to 0 to make sure we start counting from 0 */
/*Output: feature name */
void set_feature_to_zero(feature *data, char name[]);

/*train_file.h*/

/*Input: clickbait.txt, non_clickbait.txt */
/*What does it do: opens our text files with headlines, loads them into a struct and counts them */
/*Output: arrHeadline, totalHeadlines, clickbaitHeadline, nonClickbaitHeadline */
void fill_headline_struct(headline *arrHeadline, int *totalHeadlines, int *clickbaitHeadline, int *nonClickbaitHeadline);

/*Input: allWordLists, allFeature */
/*What does it do: loads our headlines in and looks for features and saves in a text file */
/*Output: output_data.txt arrHeadline */
void train_classifier(wordLists allWordLists[], headline *arrHeadline, feature allFeature[]);

/*Input: arrHeadline */
/*What does it do: looks for numbers in a headline, if found a number is counted up and loop stops */
/*Output: numberFeature */
void feature_number(feature *numberFeature, headline arrHeadline);

/*Input: allFeature, arrHeadline, allWordLists */
/*What does it do: Looks for features in a headlines, if found a featureData is counted up */
/*Output: featureData */
void feature_string_count(feature *featureData, headline arrHeadline, wordLists arrHyperbolic);

/*Input: numberFeature, hyperbolicFeature, slangFeature, forwardReferenceFeature, contractionFeature, clickbaitHeadline, nonClickbaitHeadline */
/*What does it do: Loads all the data found into a document */
/*Output: output_data.txt */
void output_feature_data_document(feature numberFeature, feature hyperbolicFeature, feature slangFeature,
                                  feature forwardReferenceFeature, feature contractionFeature,
                                  int clickbaitHeadline, int nonClickbaitHeadline);

#endif
