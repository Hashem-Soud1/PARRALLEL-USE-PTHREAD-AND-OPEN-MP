#include <stdio.h>
#include <omp.h>

#define NUM_CARDS 5

char cards[NUM_CARDS] = {'B', 'B', 'R', 'B', 'B'};
int currentIndex = 0;
int red_score = 0;
int blue_score = 0;
int turn = 0; // 0 = red, 1 = blue

int main() {
    omp_set_num_threads(2);  // فريقين فقط

    #pragma omp parallel shared(cards, currentIndex, red_score, blue_score, turn)
    {
        int thread_id = omp_get_thread_num();
        while (1) {
            #pragma omp critical
            {
                if (currentIndex < NUM_CARDS && turn == thread_id) {
                    char card = cards[currentIndex];
                    printf("Thread %d (%s) drew card: %c\n", thread_id,
                           thread_id == 0 ? "Red" : "Blue", card);

                    if (thread_id == 0 && card == 'R') {
                        red_score++;
                    } else if (thread_id == 1 && card == 'B') {
                        blue_score++;
                    }

                    currentIndex++;
                    turn = 1 - turn;
                }
            }

            #pragma omp flush
            if (currentIndex >= NUM_CARDS) break;
        }
    }

    printf("\nFinal Score:\nRed: %d\nBlue: %d\n", red_score, blue_score);
    if (red_score > blue_score)
        printf("Red team wins!\n");
    else if (blue_score > red_score)
        printf("Blue team wins!\n");
    else
        printf("It's a tie!\n");

    return 0;


    //  const int max_threads = 4;
    // const int max_number = 20;

    // omp_set_num_threads(max_threads);

    // #pragma omp parallel
    // {
    //     int tid = omp_get_thread_num();

    //     for (int i = 1; i <= max_number; i++) {
    //         if (i % max_threads == tid) {
    //             #pragma omp critical
    //             {
    //                 printf("Thread %d printed: %d\n", tid, i);
    //             }
    //         }
    //         #pragma omp barrier
    //     }
    // }

    // return 0;
}


