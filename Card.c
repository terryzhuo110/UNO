// The playing card implementation.
//
// This is an implementation of the Card ADT, which represents a single
// playing card in the game of Final Card-Down.
//
// Do not change this file.

#include <assert.h>
#include <stdlib.h>
#include <err.h>

#include "Card.h"

// The following typedef is included in Card.h:
//         typedef struct _card *Card;
// This means that a struct card has to be implemented *somewhere*, but
// not necessarily in the .h file, as we have done with concrete types.
//
// The way we implement this in an ADT is that you design your own card
// struct, with the fields etc that you need to implement your ADT.

typedef struct _card {
    value value;
    color color;
    suit suit;
} card;

// Create a new card.
// These values can only be set at creation time.
// The number should be between 0x0 and 0xF.
Card newCard(value value, color color, suit suit) {
    Card new = calloc(1, sizeof(card));
    if (new == NULL) {
        err(EXIT_FAILURE, "Could not allocate memory for Card");
    }

    new->value = value;
    new->suit = suit;
    new->color = color;

    return new;
}

// Destroy an existing card.
void destroyCard(Card card) {
    free(card);
}

// Get the card suit (HEARTS, DIAMONDS, etc).
suit cardSuit(Card card) {
    assert(card != NULL);
    return card->suit;
}

// Get the card number (0x0 through 0xF).
value cardValue(Card card) {
    assert(card != NULL);
    return card->value;
}

// Get the card's color (RED, BLUE, etc).
color cardColor(Card card) {
    assert(card != NULL);
    return card->color;
}


// Implement your own functions here, don't forget to make them static!

