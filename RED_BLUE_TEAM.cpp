#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_CARDS 5

char cards[NUM_CARDS] = {'A', 'A', 'A', 'A', 'A'};  // البطاقات
int currentIndex = 0;  // المؤشر الحالي للبطاقة
int red_score = 0;
int blue_score = 0;

pthread_mutex_t lock;
pthread_cond_t turn_red;
pthread_cond_t turn_blue;
int is_red_turn = 1;  

void* red_team(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        while (!is_red_turn && currentIndex < NUM_CARDS)
            pthread_cond_wait(&turn_red, &lock);

        if (currentIndex >= NUM_CARDS) {
            pthread_mutex_unlock(&lock);
            pthread_cond_signal(&turn_blue);
            break;
        }

        char card = cards[currentIndex];
        printf("Red team drew card: %c\n", card);
        if (card == 'R') red_score++;

        currentIndex++;
        is_red_turn = 0;
        pthread_cond_signal(&turn_blue);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* blue_team(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        while (is_red_turn && currentIndex < NUM_CARDS)
            pthread_cond_wait(&turn_blue, &lock);

        if (currentIndex >= NUM_CARDS) {
            pthread_mutex_unlock(&lock);
            pthread_cond_signal(&turn_red);
            break;
        }

        char card = cards[currentIndex];
        printf("Blue team drew card: %c\n", card);
        if (card == 'B') blue_score++;

        currentIndex++;
        is_red_turn = 1;
        pthread_cond_signal(&turn_red);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t red, blue;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&turn_red, NULL);
    pthread_cond_init(&turn_blue, NULL);

    pthread_create(&red, NULL, red_team, NULL);
    pthread_create(&blue, NULL, blue_team, NULL);

    pthread_join(red, NULL);
    pthread_join(blue, NULL);

    printf("\nFinal Score:\nRed: %d\nBlue: %d\n", red_score, blue_score);
    if (red_score > blue_score)
        printf("Red team wins!\n");
    else if (blue_score > red_score)
        printf("Blue team wins!\n");
    else
        printf("It's a tie!\n");

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&turn_red);
    pthread_cond_destroy(&turn_blue);

    return 0;
}
