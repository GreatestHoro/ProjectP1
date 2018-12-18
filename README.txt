Introduction:

The program has been created in relation to the P1 project created by the team DATA308a at
Aalborg University. Creation period: Autumn 2018 - 1. Semester. Computer Science.
The program is written in C.
The code only runs in a compiler capable of running C code, e.g. http://www.mingw.org/wiki/msys.

The aim of the program:

The aim of the program is to be capable of calculating the likelihood of a headline
beeing clickbait.

How the aim is achieved/About the program:

The program uses a machine learning approach to solve the task. The Naive Bayes classifier
has been implemented in the program. For the corpus of headlines needed for training and
evaluation, the corpus (here: https://github.com/bhargaviparanjape/clickbait/tree/master/dataset)
containing 32,000 labeled headlines has been used. The corpus consists of 16,000 clickbait headlines
and 16,000 non-clickbait headlines. For training 70% of the corpus has been used, and the rest
have been used for the testing phase.

The program has a capability for a user inputting an arbitrary headline to calculate its likelihood
of being clickbait. Further, it can print the metrics and statistics showcased below.

The Naive Bayes classifier achieves following results on the training set.

Confusion Matrix:
TP = 3444 | FP = 848
FN = 1355 | TN = 3953

Program metrics usually used for assessing ML models:
F1 Score: 0.76  | F0.5 Score: 0.78
Precision: 0.80 | Recall: 0.72

How to use/run the program:

The program runs only in the compiler.
It has a makefile, which simplifies the compilation process.
It is compiled by writing the following command in the compiler:

make

Following that, a results.exe file will be created and the program can be executed through the following
command:

results.exe

Following execution, the user will be greeted by a simple and intuitive UI that will
guide the user through the program.
