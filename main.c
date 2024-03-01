#include <stdio.h>

struct Semester {
    float moduleGrades[7][4]; //TD,TP,QUIZ,EXAM
    float evaluationResults[7][2]; // CC,FINAL
    float MG;  
};

void collect_marks(struct Semester *semester);
void compute_cc(struct Semester *semester);
void compute_final_mark(struct Semester *semester);
void displayCCAndFinal(struct Semester *semester,int for_remd);
void compute_display_MG(struct Semester *semester);
void semester_overall(struct Semester *semester);

int main(){
    //define a semester
    struct Semester s1;

    collect_marks(&s1);
    compute_cc(&s1);
    compute_final_mark(&s1);
    displayCCAndFinal(&s1,0);
    compute_display_MG(&s1);
    semester_overall(&s1);
}

void collect_marks(struct Semester *semester){
    //array for classes names.
    char *arr[] = {"TD", "TP", "QUIZ", "Exam"};
    for (int i = 0;i < 7;i++){
        printf("Enter marks for Module %d\n",i+1);
        for (int j = 0;j < 4;j++){
            // some modules does not have all TP,TD,QUIZ so I put -1 and continue.
            if ((j == 0 && (i == 4 || i == 6)) || (j == 1 && (i == 5 || i == 6)) || (j == 2 && i == 6)){
                semester->moduleGrades[i][j] = -1;
                continue;
            }
            //while value is invalid reprompt for new one.
            while (1) {
                printf("  %s: ", arr[j]);
                scanf("%f", &semester->moduleGrades[i][j]);
                if (semester->moduleGrades[i][j] >= 0 && semester->moduleGrades[i][j] <= 20) {
                    break;
                }
                printf("Invalid Value.\n");
            }
        }
    }
}

// Average(TD,TP,QUIZ)
void compute_cc(struct Semester *semester){
    for (int i = 0;i < 7;i++){
        //Intialize 0 to these two vars to refresh them for each module to store sum and number of classes.
        float sum = 0;
        int number = 0;
        for (int j = 0;j < 3;j++){
            //Some Modules dont have TD or TP or QUIZ so dont skip them.
            if (semester->moduleGrades[i][j] != -1){
            //Sum of TD,TP and QUIZ.
            sum += semester->moduleGrades[i][j];
            //Compute number of classes to calculate average.
            number++;
            }
        }
        //some of modules does not have cc because TD,TP,QUIZ does not exist. 
        if (number == 0){
            semester->evaluationResults[i][0] = -1;
            continue;
        }
        //Calculate AVG and Intialize to column of CC.
        semester->evaluationResults[i][0] = sum / number;
    }
    //the last module dont have CC so its -1.
}

//Combination between CC if exists and EXAM.
void compute_final_mark(struct Semester *semester){
    for (int i = 0;i < 7;i++){
        //if CC does not exist so EXAM mark is FINAL mark.
        if (semester->evaluationResults[i][0] == -1){
            semester->evaluationResults[i][1] = semester->moduleGrades[i][3];
        }
        //if one of TD & TP does not exist so FINAL = 50% CC + 50% EXAM
        else if (semester->moduleGrades[i][0] == -1 || semester->moduleGrades[i][1] == -1){
            semester->evaluationResults[i][1] =  0.5 * semester->evaluationResults[i][0] + 0.5 * semester->moduleGrades[i][3];
        }
        //if not so FINAL = 40% CC + 60% EXAM
        else{
            semester->evaluationResults[i][1] =  0.4 * semester->evaluationResults[i][0] + 0.6 * semester->moduleGrades[i][3];
        }
    }
}

void displayCCAndFinal(struct Semester *semester,int for_remd){
    printf("Results: \n");
    printf(" _________________________________\n");
    printf("|  Module   |   CC   | Final Mark |\n");
    printf("|-----------|--------|------------|\n");

    for (int i = 0;i < 7;i++){
        if (for_remd){
            if (semester->evaluationResults[i][1] >= 10){
                continue;
            }
        }
        printf("|     %d     |",i+1);
        //if cc does not exist.
        if (semester->evaluationResults[i][0] == -1){
            printf("  NAN   |");
        }
        else{
            //display cc.
            printf(" %5.2f  |", semester->evaluationResults[i][0]);
        }
        //display final mark anyway.
        printf(" %8.2f   |\n", semester->evaluationResults[i][1]);

    }
    printf("|___________|________|____________|\n\n");
}

//Calculate MG
void compute_display_MG(struct Semester *semester){
    int sum = 0;
    for (int i = 0;i < 7;i++){
        //calculate sum of FINAL marks.
        sum += semester->evaluationResults[i][1];
    }
    //calculate average of them nd print it.
    semester->MG = sum/7;
    printf("MG for this semester is %.2f. \n\n",semester->MG);
}

//rate the semester
void semester_overall(struct Semester *semester){
    //if student failed.
    if (semester->MG < 10){
        printf("You failed the semester.\n");
        printf("Modules you should retake are: \n");
        //call display function with 'for_remd = 1 ' to display only modules with final < 10
        displayCCAndFinal(semester,1);
        printf("Good Luck for next semester.");
    }else{
        printf("Congratulations,You passed the semester.\n");
    }
}
