#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<sys/select.h>
#include<unistd.h>
#include<ctype.h>
#include<signal.h>
#include<string.h>

#define WAIT 3

int r1 = 0, r2 = 0, sign = 0, correct_answers = 0, total_score = 0, flag = WAIT;

void bonus_tester(){
    r1 = 0;
    r2 = 0;
    sign = rand() % 4 + 1;

    switch(sign){
        case 1:
            printf("%d + %d = ", r1, r2);
            fflush(stdout);
            break;
        case 2:
            printf("%d - %d = ", r1, r2);
            fflush(stdout);
            break;
        case 3:
            printf("%d * %d = ", r1, r2);
            fflush(stdout);
            break;
        case 4:
            printf("%d / %d = ", r1, r2);
            fflush(stdout);
            break;
    } 
}

void random_numbers(){
    srand(time(NULL));
    r1 = rand() % 10;
    r2 = rand() % 10;
    sign = rand() % 4 + 1;

    switch(sign){
        case 1:
            printf("%d + %d = ", r1, r2);
            fflush(stdout);
            break;
        case 2:
            printf("%d - %d = ", r1, r2);
            fflush(stdout);
            break;
        case 3:
            printf("%d * %d = ", r1, r2);
            fflush(stdout);
            break;
        case 4:
            printf("%d / %d = ", r1, r2);
            fflush(stdout);
            break;
    } 
}

void sig_handler(int signo){
    char repeat, checker = 0;

    printf("\nYou got %d out of %d items correctly\n", correct_answers, total_score);

    do{

        printf("Press y or Y if you want to finish the game. \nPress enter to start a new one.\n");
        system("/bin/stty raw");
        repeat = getchar();
        puts("");
        system("/bin/stty cooked");

        if (repeat == 'y' || repeat == 'Y'){
            exit(1);
        }
        
        else if (repeat == 13){
            puts("pressed enter");
            checker = 1;
            correct_answers = 0;
            total_score = 0;
            break;
        }

        else {
            puts("\n[ERROR] Invalid input\n");
        }  

    }while(repeat != 'y' || repeat != 'Y' || repeat != 13);
}

int number_check(const char *str){
    while(*str){
        if(isdigit(*str++) == 0){
            return 0;
        }
    }
    return 1;
}

void destroyer(char **obj){
    if(*obj != NULL){
        free(*obj);
        *obj = NULL;
    }
}

void show_correct_answers(){
    if (sign == 1){
        int ans = r1 + r2;
        printf("Correct answer: %d\n", ans);
    }

    else if (sign == 2){
        int ans = r1 - r2;
        printf("Correct answer: %d\n", ans);
    }

    else if (sign == 3){
        int ans = r1 * r2;
        printf("Correct answer: %d\n", ans);
    }

    else if (sign == 4){
        int ans = r1 / r2;
        printf("Correct answer: %d\n", ans);
    }

    else {
        puts("[ERROR] sign not detected");
    }
}

void alarm_handler(int signo){
    int now_int_answer = 0;
    char *answer;
    answer = malloc(4);

   if(--flag){

            scanf("%s", answer);

            struct sigaction sig_int_handler;

            sig_int_handler.sa_handler = sig_handler;
            sigemptyset(&sig_int_handler.sa_mask);
            sig_int_handler.sa_flags = 0;

            sigaction(SIGINT, &sig_int_handler, NULL);
                               
            if(number_check(answer)){
                now_int_answer = atoi(answer);

                if (sign == 1 && now_int_answer == (r1+r2)){
                    puts("Correct");
                    correct_answers += 1;
                }

                else if (sign == 2 && now_int_answer == (r1-r2)){
                    puts("Correct");
                    correct_answers += 1;
                }

                else if (sign == 3 && now_int_answer == (r1*r2)){
                    puts("Correct");
                    correct_answers += 1;
                }

                else if (sign == 4 && now_int_answer == (r1/r2)){
                    puts("Correct");
                    correct_answers += 1;
                }

                else {
                    printf("Incorrect. ");
                    show_correct_answers();
                }
            }

            else {
                if(sign == 4 && r2 == 0 && r1 != 0 && (strcmp(answer, "u") == 0)){
                    puts("Correct");
                    correct_answers += 1;
                }

                else if (sign == 4 && r2 == 0 && strcmp(answer, "i") == 0){
                    puts("Correct");
                    correct_answers += 1;
                }

                else{
                    printf("Incorrect. ");
                    show_correct_answers();
                }
            }
        }

        else {
            printf("\nIncorrect. No answer entered within %d seconds.\n", WAIT);
            flag = WAIT;
        }

        alarm(1);

        //destroyer(&answer);
        free(answer);
}

int main(int argc, char*argv[]){
        random_numbers();
        //bonus_tester();

        total_score += 1;

        signal(SIGALRM, alarm_handler);
        alarm(1);
        while(1);

    return 0;
}