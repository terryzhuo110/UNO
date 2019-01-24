//Terry z5191493
// Written on 2017-10-??
// Tutorial: Helena

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Game.h"
#include "Card.h"
#include "player.h"
#define MAX_COLOR 16



static Card find_Color(Game game);
static Card find_Value(Game game);
static Card find_Suit(Game game);
static Card find_two(Game game);
static Card findD(Game game);
static int find_most_color(Game game);
static Card findValidTwo(Game game);
static Card findValidD(Game game);
static Card findMax(Card c1, Card c2, Card c3);
static Card bigger(Card c1, Card c2);
static bool checkValid(Game game, Card card);
static Card findZERO(Game game);
static playerMove active_two(playerMove move, Game game);
static playerMove compare(Card c1, Card c2, Card c3,
                            playerMove move, Game game);
static Card find_A(Game game);

// find a card which matches the color
static Card find_Color(Game game) {
    int color = getCurrentColor(game);
    int num = handCardCount(game);
    int i = 0;
    while (i < num) {
        Card card = handCard(game, i);
        if (color == cardColor(card)) {
            return card;
        }
        i++;
    }
    return NULL;
}

// find a card which matches the value
static Card find_Value(Game game) {
    int value = cardValue(topDiscard(game));
    int num = handCardCount(game);
    int i = 0;
    while (i < num) {
        Card card = handCard(game, i);
        if (value == cardValue(card)) {
            return card;
        }
        i++;
    }
    return NULL;
}

// find a card which matches the suit
static Card find_Suit(Game game) {
    int suit = cardSuit(topDiscard(game));
    int num = handCardCount(game);
    int i = 0;
    while (i < num) {
        Card card = handCard(game, i);
        if (suit == cardSuit(card)) {
            return card;
        }
        i++;
    }
    return NULL;
}

//find a DRAW_TWO card 
static Card find_two(Game game) {
    int i = 0;
    int num = handCardCount(game);
    Card card = handCard(game, i);
    int val = cardValue(card);
    i++;
    while (i < num) {
        if (cardValue(handCard(game, i)) == DRAW_TWO &&
            checkValid(game, card)) {
            int v1 = cardValue(handCard(game, i));
            if (val < v1) {
                val = cardValue(handCard(game, i));
                card = handCard(game, i);
            }
        }
        i++;
    }
    if (cardValue(card) == DRAW_TWO) {
        return card;
    } else {
        return NULL;
    }
}

//find a D card
static Card findD(Game game) {
    int i = 0;
    int num = handCardCount(game);
    Card card = handCard(game, i);
    int val = cardValue(card);
    i++;
    while (i < num) {
        if (cardValue(handCard(game, i)) == D && checkValid(game, card)) {
            int v1 = cardValue(handCard(game, i));
            if (val < v1) {
                val = cardValue(handCard(game, i));
                card = handCard(game, i);
            }
        }
        i++;
    }
    if (checkValid(game, card)) {
        return card;
    } else {
        return NULL;
    }
}

//find the most of the colors the player has
static int find_most_color(Game game) {
    int arr[MAX_COLOR] = {0};
    int j = 0;
    int num = handCardCount(game);
    Card card = handCard(game, j);
    int color = 0;
    while (handCard(game, j) != NULL) {
        color = cardColor(handCard(game, j));
        arr[color]++;
        j++;
    }
    j = 0;
    int val = 0;
    color = 0;
    while (j < MAX_COLOR) {
        if (val < arr[j]) {
            val = arr[j];
            color = j;
        }
        j++;
    }
    return color;
}

//find a valid D card
static Card findValidD(Game game) {
    int num = handCardCount(game);
    int i = 0;
    while (i < num) {
        Card card = handCard(game, i);
        if (D == cardValue(card) && checkValid(game, card)) {
            return card;
        }
        i++;
    }
    return NULL;
}

//find a valid DRAW_TWO card
static Card findValidTwo(Game game) {
    int i = 0;
    int num = handCardCount(game); 
    while (i < num) {
        Card card = handCard(game, i);
        if (DRAW_TWO == cardValue(card)) {
            return card;
        }
        i++;
    }
    return NULL;
}

//find max value
static Card findMax(Card c1, Card c2, Card c3) {
    int a = cardValue(c1);
    int b = cardValue(c2);
    int c = cardValue(c3);
    if (a < b) {
        if (b < c) {
            return c3;
        } else {
            return c2;
        }
    } else {
        return c1;
    }
}

//find the bigger one
static Card bigger(Card c1, Card c2) {
    int a = cardValue(c1);
    int b = cardValue(c2);
    if (a < b) {
        return c1;
    } else {
        return c2;
    }
}

// check if valid
static bool checkValid(Game game, Card card) {
    if (cardValue(card) == ZERO) {
        return TRUE;
    }
    if (cardColor(card) == getCurrentColor(game) ||
       cardValue(card) == cardValue(topDiscard(game)) ||
       cardSuit(card) == cardSuit(topDiscard(game))) {
        return TRUE;
    } else {
        return FALSE;
    }
}

// find a ZERO card
static Card findZERO(Game game) {
    int num = handCardCount(game);
    int i = 0;
    while (i < num) {
        Card card = handCard(game, i);
        if (ZERO == cardValue(card) && 
            find_most_color(game) == cardColor(card)) {
            return card;
        }
        i++;
    }
    return NULL;
}

//decide if active_two > 0
static playerMove active_two(playerMove move, Game game) {
    move.action = PLAY_CARD;
    if (findValidTwo(game) != NULL && isValidMove(game, move)) {
        move.card = find_two(game);
        move.action = PLAY_CARD;
        return move;
    } else {
        move.action = END_TURN;
        if (!isValidMove(game, move)) {
            move.action = DRAW_CARD;
        }
        return move;
    }
}


//normal strategy
static playerMove compare(Card c1, Card c2, Card c3,
                            playerMove move, Game game) {
    if (c1 == NULL && c2 == NULL && c3 == NULL) {
        move.action = DRAW_CARD;
        return move;
    }
    if (c1 != NULL && c2 != NULL && c3 != NULL) {
        if (findD(game) != NULL) {
            move.card = findD(game);
            move.nextColor = find_most_color(game);
            return move;
        }
        move.card = findMax(c1, c2, c3);
        return move;
    } else if (c1 != NULL && c2 != NULL) {
        move.card = bigger(c1, c2);
        move.action = PLAY_CARD;
        return move;
    } else if (c2 != NULL && c3 != NULL) {
        move.card = bigger(c2, c3);
        return move;
    } else if (c1 != NULL && c3 != NULL) {
        move.card = bigger(c1, c3);
        return move;
    } else {
        if (c1 != NULL) {
            move.card = c1;
            return move;
        } else if (c2 != NULL) {
            move.card = c2;
            return move;
        } else {
            move.card = c3;
            return move;
        }
    }
}

// the main function for decision
playerMove decideMove(Game game) {
    playerMove move;
    move.action = END_TURN;
    if (!isValidMove(game, move)) {
        if (getActiveDrawTwos(game) > 0) {
            return active_two(move, game);
        } else {
            move.action = PLAY_CARD;

            if (findZERO(game) != NULL) {
                move.card = findZERO(game);
                return move;
            }
            Card c1 = find_Value(game);
            Card c2 = find_Suit(game);
            Card c3 = find_Color(game);
            return compare(c1, c2, c3, move, game);
        }
    } else {
        return move;
    }
}
