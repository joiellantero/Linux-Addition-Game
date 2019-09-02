#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<sys/select.h>
#include<unistd.h>
#include<ctype.h>
#include<signal.h>

#define WAIT 3

int r1 = 0, r2 = 0, sign = 0, correct_answers = 0, total_score = 0;

void random_numbers(){
    srand(time(NULL));
    r1 = rand() % 10;
    r2 = rand() % 10;
    sign = rand() % 4 + 1;

            printf("[SIGN] %d\n", sign);

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

    printf("Press y or Y if you want to finish the game. \nPress enter to start a new one.\n");
    repeat = getchar();

    if (repeat == 'y' || repeat == 'Y'){
        exit(1);
    }
    
    else if (repeat == '\n'){
        puts("pressed enter");
        checker = 1;
        correct_answers = 0;
        total_score = 0;
    }

    else {
        puts("[ERROR] Unknown input");
    }  
}

int main(){
    
    int reading = 0, read_bytes = 0, now_int_answer = 0;
    char *answer;
    answer = malloc(4);
    fd_set input_set;
    struct timeval timeout;

    
    while(1){
        FD_ZERO(&input_set);
        FD_SET(0, &input_set);
        timeout.tv_sec = WAIT;
        timeout.tv_usec = 0;

        random_numbers();

        reading = select(1, &input_set, NULL, NULL, &timeout);

        // if(reading == -1){
        //     perror("[ERROR] ");
        //     return -1;
        // }

        if(reading){
            total_score += 1;

            read_bytes = read(0, answer, sizeof(answer));

            struct sigaction sig_int_handler;

            sig_int_handler.sa_handler = sig_handler;
            sigemptyset(&sig_int_handler.sa_mask);
            sig_int_handler.sa_flags = 0;

            sigaction(SIGINT, &sig_int_handler, NULL);
            
            if(answer[read_bytes-1] == '\n'){
                --read_bytes;
                answer[read_bytes] = '\0';
            }

            if(read_bytes == 0){
                puts("You just hit enter\n");
            }

            else{
                now_int_answer = atoi(answer);
                if (now_int_answer == (r1+r2)){
                    puts("Correct");
                    correct_answers += 1;
                }
                else{
                    if(now_int_answer == 0){
                        printf("[ERROR] Input not a number. Input");
                    }
                    puts("Incorrect");
                }
            }
        }

        else {
            printf("\nNo data within %d seconds.\n", WAIT);
        }

        free(answer);
    }

    return 0;
}