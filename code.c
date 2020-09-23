#include<stdio.h>
#include <stdlib.h>
#include <time.h>

/*  Week 2 Assignment: Using struct
 *  Created at: 18-09-2020
 *  Last modification: 18-09-2020
 *  Made by: Isaac Da Pena
 */

#define HAND_LENGTH 5
#define MAX_RANDOM_HANDS 1000000

// Card struct
typedef struct {
    enum types {hearts, tiles, clovers, pikes} type;
    enum values {joker, one, two, three, four, five, six, seven, eigth, nine, ten, J, Q, K} value;
} card;

enum matches {full_house, no_pair, one_pair, two_pair, three_of_kind, four_of_kind};

// This functions swap two cards and it is for sort function
void swap(card *a, card *b) {
    card c = *a;
    *a = *b;
    *b = c;
}

// Sort function
void sort(card *hand) {
    card *c;
    for (int i = 0; i < HAND_LENGTH; i++) {
        c = hand + i;
        for (int j = i; j < HAND_LENGTH; j++)
            if(c->value < hand[j].value)
                c = hand + j;
        swap(hand + i, c);
    }
}

/*  This function calcs th number of pairs, triplets... that are present in the hand, also it counts the jokers and
 *  no pair cards. After that it returns an dynamic array with that information.
 *  Index:
 *  res[0]: Number of jokers in the hand
 *  res[1]: Number of no pair cards in the hand
 *  res[2]: Number of pair cards in the hand
 *  res[3]: Number of triplets in the hand (it can be 0 or 1)
 *  res[4]: Number of quartet in the hand (it can be 0 or 1)
 */
int *getNumOfSame(card *hand) {
    int marked[HAND_LENGTH] = {0};

    int *res = (int *) malloc(sizeof(int)*HAND_LENGTH);
    for (int i = 0; i < HAND_LENGTH; i++)
        res[i] = 0;

    for (int i = 0, count = 1; i < HAND_LENGTH; i++, count = 1) {
        if(marked[i] != 0) continue;
        marked[i] = 1;
        for (int j = i + 1; j < HAND_LENGTH; j++) {
            if (marked[j] == 0) {
                if (hand[i].value == hand[j].value) {
                    count++;
                    marked[j] = 1;
                } else if (hand[j].value == joker) {
                    marked[j] = 1;
                    res[0]++;
                }
            }
        }
        res[count]++;
    }
    return res;
}

/*  This function generates random hands of five cards in the hand array
 */
void getRandomHand(card *hand) {
    int jokers = 2;
    for (int counter = 0; counter < HAND_LENGTH; counter++) {
        hand[counter] = (card) {rand() % 4, rand() % 14};
        if(hand[counter].value == joker) jokers--;
        for (int j = 0; j < counter; j++)
            if ((hand[counter].value == hand[j].value && hand[counter].type == hand[j].type && hand[counter].value != joker) ||
            (hand[counter].value == joker && jokers < 1)) {
                counter--;
                break;
            }
    }
}

/* This function makes the histogram
 */
void calcProbs(card *hand, unsigned int *counter) {
    int *maxOfKind = getNumOfSame(hand);
    if ((maxOfKind[0] == 2 && maxOfKind[2] == 1) || maxOfKind[4] == 1)
        counter[four_of_kind] += 1;
    else if ((maxOfKind[2] == 1 && maxOfKind[3] == 1) || (maxOfKind[2] == 1 && maxOfKind[0] == 2 ) || maxOfKind[2] == 2 && maxOfKind[0] == 1)
        counter[full_house] += 1;
    else if (maxOfKind[0] == 2 || (maxOfKind[0] == 1 && maxOfKind[2] == 1) || maxOfKind[3] == 1)
        counter[three_of_kind] += 1;
    else if (maxOfKind[2] > 1 || (maxOfKind[2] == 1 && maxOfKind[0] > 0))
        counter[two_pair] += 1;
    else if (maxOfKind[2] == 1 || maxOfKind[0] > 0)
        counter[one_pair] += 1;
    else if (maxOfKind[1] == 5 || maxOfKind[1] + maxOfKind[0] == 5)
        counter[no_pair] += 1;
    free(maxOfKind);
}

/*  This function prints the probs
 */
void printProbs(const unsigned int *counter) {
    printf("\tProbabilities of cards pairs\n\n");
    printf("No pair equals cards found %d, probability: %lf\n", counter[no_pair], (double) counter[no_pair] / MAX_RANDOM_HANDS);
    printf("One pair equals cards found %d, probability: %lf\n", counter[one_pair], (double) counter[one_pair] / MAX_RANDOM_HANDS);
    printf("Two pair equals cards found %d, probability: %lf\n", counter[two_pair], (double) counter[two_pair] / MAX_RANDOM_HANDS);
    printf("Three equals cards found %d, probability: %lf\n",  counter[three_of_kind], (double) counter[three_of_kind] / MAX_RANDOM_HANDS);
    printf("Four equals cards found %d, probability: %lf\n", counter[four_of_kind], (double) counter[four_of_kind] / MAX_RANDOM_HANDS);
    printf("Full house cards found %d, probability: %lf\n", counter[full_house], (double) counter[full_house] / MAX_RANDOM_HANDS);
}


int main(void) {
    unsigned int counter[HAND_LENGTH+1] = {0};
    card hand[HAND_LENGTH];
    srand((unsigned) time(NULL));
    for (int i = 0; i < MAX_RANDOM_HANDS; i++) {
        getRandomHand(hand);
        sort(hand);
        calcProbs(hand, counter);
    }
    printProbs(counter);
    return 0;
}
