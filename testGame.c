//terry z5191493
// Written on 2018-10-26
// Tutorial: Helena

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "Game.h"
#include "Card.h"

#define STARTING_HAND_SIZE 7
#define NUM_PLAYERS 4

#define NUM_VALUES 16
#define NUM_COLORS 5
#define NUM_SUITS 5

//test 1 inspired by stage1 (The style of Jacob's code is quite 'unique')
static void check_hand(Game game, int player, value values[], 
                        color colors[], suit suits[]);
static void check_deck(Game game, int deck_size, value values[], 
                        color colors[], suit suits[]);
static void check_discard(Game game, int deck_size, value values[], 
                            color colors[], suit suits[]);
static int cardIsInHand(Game game, int player, int alreadyMatched[], 
                        value v, color c, suit s);
static int cardMatchesComponents(Card card, value v, color c, suit s);
static void printCardByComponents(value v, color c, suit s);
static void test1_basic(void);

//test 2 mainly tests the cases of A
// topdiscard's value will be A, so Player0 is skipped
static void test2_A(void);
static void checkPlayer1Turn1(Game game);
static void end_2(Game game);


//test3 mainly test the cases of DRAW_TWO
//Also the case of flipping over the deck will be tested
static void test3_TWOS(void);
static void check3_player0Turn0(Game game);
static void check3_player1Turn1(Game game);
static void check3_player2Turn2(Game game);
static void check3_player3Turn3(Game game);
static void check3_player0Turn4(Game game);
static void check3_player1Turn5(Game game);
static void check_flip(Game game);
static void end_3(Game game);

//test4 mainly test the cases of D 
static void test4_D(void);
static void player0Turn0(Game game);
static void player1Turn1(Game game);
static void player2Turn2(Game game);
static void player3Turn3(Game game);
static void player0Turn4(Game game);
static void player1Turn5(Game game);
static void end_4(Game game);


// the main function of four tests
int main (void) {
    test1_basic();
    test2_A();
    test3_TWOS();
    test4_D();

}

static void test1_basic(void) {
    int deck_size = 50;
    value values[] = {
        ZERO, ONE, DRAW_TWO, THREE, FOUR, 
        FIVE, SIX, SEVEN, EIGHT, NINE,
        A, B, C, D, E, F,
        ZERO, ONE, DRAW_TWO, THREE,
        FOUR, FIVE, SIX, SEVEN, EIGHT, 
        NINE, A, B, C, D,
        E, F, ZERO, ONE, DRAW_TWO, 
        THREE, FOUR, FIVE, SIX, SEVEN,
        EIGHT, NINE, A, B, C, 
        D, E, F, ZERO, ONE
    };

    color colors[] = {
        RED, RED, RED, RED, RED, 
        RED, RED, RED, RED, RED,
        BLUE, BLUE, BLUE, BLUE, BLUE, 
        BLUE, BLUE, BLUE, BLUE, BLUE,
        GREEN, GREEN, GREEN, GREEN, GREEN, 
        GREEN, GREEN, GREEN, GREEN, GREEN,
        YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 
        YELLOW, YELLOW, YELLOW, YELLOW, YELLOW,
        PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, 
        PURPLE, PURPLE, PURPLE, PURPLE, PURPLE
    };

    suit suits[] = {
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, 
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, 
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, 
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, 
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, 
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS
    };

    Game game = newGame(deck_size, values, colors, suits);

    printf("Checking if current player is 0\n\n");
    assert(currentPlayer(game) == 0);

    printf("Checking if current turn is 0\n\n");
    assert(currentTurn(game) == 0);

    printf("Checking if number of turns is 1\n\n");
    assert(numTurns(game) == 1);

    printf("Checking if current moves in turn is 0\n\n");
    assert(currentTurnMoves(game) == 0);

    printf("Checking if player from previous turn is -1\n\n");
    assert(getPreviousTurnPlayer(game) == -1);

    printf("Checking if the number of deck the number of cards in the initial deck of HEARTS is 10\n\n");
    assert(numOfSuit(game, HEARTS) == 10);

    printf("Checking if the number of deck the number of cards in the initial deck of RED is 10\n\n");
    assert(numOfColor(game, RED) == 10);

    printf("Checking if the number of deck the number of cards in the initial deck of ZERO is 4\n\n");
    assert(numOfValue(game, ZERO) == 4);

    check_deck(game, deck_size, values, colors, suits);
    printf("\n");

    check_discard(game, deck_size, values, colors, suits);
    printf("\n");

    check_hand(game, 0, values, colors, suits);
    printf("\n");

    check_hand(game, 1, values, colors, suits);
    printf("\n");

    check_hand(game, 2, values, colors, suits);
    printf("\n");

    check_hand(game, 3, values, colors, suits);
    printf("\n");


    printf("Trying to destroy the game\n\n");
    destroyGame(game);

    printf("All tests passed, nice work!\n");

}

/*  ----------------------------------------
        ... Helper functions below ...
*/

static void check_deck(Game game, int deck_size, value values[], 
                        color colors[], suit suits[]) {

    int deckStart = NUM_PLAYERS * STARTING_HAND_SIZE + 1;
    int expectedCards = deck_size - NUM_PLAYERS * STARTING_HAND_SIZE - 1;

    int i = 0;
    Card card = getDeckCard(game, i);
    while (card != NULL && i < expectedCards) {
        int originalDeckIndex = deckStart + i;

        int v = values[originalDeckIndex];
        int c = colors[originalDeckIndex];
        int s = suits[originalDeckIndex];

        printf("Checking card %d in deck is ", i);
        printCardByComponents(v, c, s);
        printf("\n");

        assert(cardMatchesComponents(card, v, c, s));

        i += 1;
        card = getDeckCard(game, i);
    }

    printf("Checking the deck has %d cards\n", expectedCards);
    assert(i == expectedCards);
}

static void check_discard(Game game, int deck_size, value values[], 
                            color colors[], suit suits[]) {
    Card card;
    int firstCard = NUM_PLAYERS * STARTING_HAND_SIZE;

    printf("Checking top of discard pile is ");
    printCardByComponents(values[firstCard], 
                            colors[firstCard], suits[firstCard]);
    printf("\n");

    card = getDiscardPileCard(game, 0);
    assert(cardMatchesComponents(card, values[firstCard], 
            colors[firstCard], suits[firstCard]));

    printf("Checking there are no other cards in the discard pile\n");

    card = getDiscardPileCard(game, 1);
    assert(card == NULL);
}


static void check_hand(Game game, int player, value values[], 
                        color colors[], suit suits[]) {
    int originalDeck_index = player;

    // Array to keep track of the index of cards in a player's hand we've already
    // matched against
    int alreadyMatched[STARTING_HAND_SIZE];
    for (int i = 0; i < STARTING_HAND_SIZE; i++) {
        alreadyMatched[i] = FALSE;
    }

    for (int k = 0; k < STARTING_HAND_SIZE; k++) {

        int v = values[originalDeck_index];
        int c = colors[originalDeck_index];
        int s = suits [originalDeck_index];
        originalDeck_index = originalDeck_index + NUM_PLAYERS;

        printf("\n---------- ----------------- ------------ \n");
        printf("Checking card ");
        printCardByComponents(v, c, s);
        printf(" is in player %d's hand\n", player);

        assert(cardIsInHand(game, player, alreadyMatched, v, c, s));
    }

    Card card = getHandCard(game, player, STARTING_HAND_SIZE);
    assert( card == NULL );

    printf("Finish checking hand for player %d , OK. ", player);

}


static int cardIsInHand(Game game, int player, int alreadyMatched[], 
                        value v, color c, suit s) {
    int i = 0;
    Card card = getHandCard(game, player, i);
    while (card != NULL) {
        if (cardMatchesComponents(card, v, c, s) 
            && !alreadyMatched[i]) {
            alreadyMatched[i] = TRUE;
            return TRUE;
        }

        i++;
        card = getHandCard(game, player, i);
    }

    return FALSE;
}

static int cardMatchesComponents(Card card, value v, color c, suit s) {
    return cardValue(card) == v && cardColor(card) == c && 
            cardSuit(card) == s;
}

static void printCardByComponents(value v, color c, suit s) {
    char* valueStrings[NUM_VALUES] = {
        "ZERO", "ONE", "DRAW_TWO", "THREE", "FOUR",
        "FIVE", "SIX", "SEVEN", "EIGHT", "NINE",
        "A", "B", "C", "D", "E", "F"
    };

    char* colorStrings[NUM_COLORS] = {
        "RED", "BLUE", "GREEN", "YELLOW", "PURPLE"
    };

    char* suitStrings[NUM_SUITS] = {
        "HEARTS", "DIAMONDS", "CLUBS", "SPADES", "QUESTIONS"
    };

    printf("%s %s of %s", colorStrings[c], valueStrings[v], suitStrings[s]);
}

//the start of test 2
static void test2_A(void) {
    int deckSize = 30;

    value values[] = {
        C, SIX, C, C,
        E, C, F, B,
        C, ZERO, E, F,
        ONE, SIX, FIVE, THREE,
        ONE, SEVEN, F, F,
        FOUR, ZERO, A, THREE,
        A, THREE, FOUR, ZERO,
        A, D
    };
    
    color colors[] = {
        PURPLE, PURPLE, YELLOW, PURPLE,
        YELLOW, YELLOW, YELLOW, GREEN,
        GREEN, YELLOW, PURPLE, YELLOW,
        PURPLE, PURPLE, GREEN, PURPLE,
        GREEN, GREEN, PURPLE, GREEN,
        GREEN, GREEN, YELLOW, YELLOW,
        PURPLE, YELLOW, PURPLE, PURPLE,
        RED, BLUE
    };

    suit suits[] = {
        SPADES, DIAMONDS, DIAMONDS, CLUBS,
        CLUBS, SPADES, SPADES, CLUBS,
        SPADES, SPADES, DIAMONDS, CLUBS,
        DIAMONDS, SPADES, SPADES, SPADES,
        CLUBS, DIAMONDS, SPADES, CLUBS,
        DIAMONDS, CLUBS, DIAMONDS,SPADES,
        CLUBS, CLUBS, DIAMONDS, SPADES,
        HEARTS, QUESTIONS
    };

    Game g = newGame(deckSize, values, colors, suits);
    
    checkPlayer1Turn1(g);
    end_2(g);
}

// test player 1 turn 1
static void checkPlayer1Turn1(Game game) {

    //check current turn is 0
    assert(currentTurn(game) == 0);
    printf(">Turn:   %d | Player 1\n", currentTurn(game));
    //check the top discard has A value
    assert(cardValue(topDiscard(game)) == A);
    //check current player is 1
    assert(currentPlayer(game) == 1);
    //check the turn number of the top discard pile 
    assert(getTopDiscardTurnNumber(game) == -1);
    //check the previous player 
    assert(getPreviousTurnPlayer(game) == -1);

    playerMove move;
    
    // check the hand card number
    int num = handCardCount(game);
    assert(num == 7);
    
    // check the correct card in hand
    move.action = DRAW_CARD;
    assert(isValidMove(game, move) == TRUE);
    
    playMove(game, move);
    
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);
    printf("Player 1  ends their turn\n");
}


// the end of test2
static void end_2(Game game) {
    //check NOT_FINISHED
    assert(gameWinner(game) == NO_WINNER);


    //check total_points
    assert(playerPoints(game, 0) == 54);
    assert(playerPoints(game, 1) == 47);
    assert(playerPoints(game, 2) == 75);
    assert(playerPoints(game, 3) == 59);


    printf("NO ONE WIN !!!!!!!!!!!!!!\n");
    printf("===========+=============\n");
    printf("| Player 0 |   %d points |\n", playerPoints(game, 0));
    printf("| Player 1 |   %d points |\n", playerPoints(game, 1));
    printf("| Player 2 |   %d points |\n", playerPoints(game, 2));
    printf("| Player 3 |   %d points |\n", playerPoints(game, 3));
    printf("===========+=============\n");
    printf("Trying to destroy the game\n");
    destroyGame(game);
    printf("PASS TEST_2\n");
    printf("\n=======================================================\n");
}

//the start of test 3
static void test3_TWOS(void) {

    int deckSize = 35;

    value values[] = {
        DRAW_TWO, ZERO, ZERO, DRAW_TWO,
        DRAW_TWO, C, C, B,
        B, ZERO, E, F,
        ONE, SIX, FIVE, THREE,
        ONE, SEVEN, F, F,
        FOUR, ZERO, ONE, THREE,
        SEVEN, THREE, FOUR, ZERO,
        DRAW_TWO, B, FIVE, THREE, 
        ONE, FIVE, SEVEN
    };
    
    color colors[] = {
        RED, RED, RED, RED, 
        RED, RED, RED, RED, 
        BLUE, BLUE, BLUE, BLUE, 
        BLUE, BLUE, BLUE, BLUE, 
        GREEN, GREEN, GREEN, GREEN, 
        GREEN, GREEN, GREEN, GREEN, 
        GREEN, GREEN, YELLOW, YELLOW, 
        YELLOW, YELLOW, YELLOW, YELLOW, 
        YELLOW, YELLOW, YELLOW
    };

    suit suits[] = {
        HEARTS, DIAMONDS, CLUBS, SPADES, 
        QUESTIONS, HEARTS, DIAMONDS, CLUBS, 
        SPADES, QUESTIONS, HEARTS, DIAMONDS, 
        CLUBS, SPADES, QUESTIONS, HEARTS, 
        DIAMONDS, CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, 
        QUESTIONS, HEARTS, DIAMONDS, CLUBS, 
        SPADES, QUESTIONS, HEARTS, DIAMONDS, 
        CLUBS, SPADES, QUESTIONS
    };
    
    //initialize the Game 
    Game g = newGame(deckSize, values, colors, suits);

    //check the deckSize
    assert(numCards(g) == 35);
    
    check3_player0Turn0(g);
    check3_player1Turn1(g);
    check3_player2Turn2(g);
    check3_player3Turn3(g);
    check3_player0Turn4(g);
    check3_player1Turn5(g);
    end_3(g);
}

//player 0 Turn 0
static void check3_player0Turn0(Game game) {


    playerMove move;
    // check current turn
    assert(currentTurn(game) == 0);


    //check number of turns
    assert(numTurns(game) == 1);


    //check current player
    assert(currentPlayer(game) == 0);


    //check top discard pile
    assert(cardColor(topDiscard(game)) == 3);
    assert(cardValue(topDiscard(game)) == 2);
    assert(cardSuit(topDiscard(game)) == 3);


    //check TWO
    assert(getNumberOfTwoCardsAtTop(game) == 1);
    assert(getActiveDrawTwos(game) == 1);
    printf("\n>Turn:   0 | Top Discard: YELLOW DRAW_TWO of QUESTIONS\n");
    printf("   Player: 0 | Move:\n");
    printf("Player 0 wants to play a RED DRAW_TWO of DIAMONDS\n");
    move.action = PLAY_CARD;


    //check card in hand 
    assert(cardColor(handCard(game, 6)) == 0);
    assert(cardValue(handCard(game, 6)) == 2);
    assert(cardSuit(handCard(game, 6)) == 0);

    // play the card
    move.card = handCard(game, 6);
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);

    //end the card
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);


    printf("Player 0 wants to end their turn\n");
    playMove(game, move);   
    printf(".....................................................\n");
}


//player 1 Turn 1
static void check3_player1Turn1(Game game) {


    playerMove move;

    // check current turn
    assert(currentTurn(game) == 1);


    //check number of turns
    assert(numTurns(game) == 2);


    //check current player
    assert(currentPlayer(game) == 1);


    //check the turn number of the top discard pile 
    assert(getTopDiscardTurnNumber(game) == 0);


    //check the previous player 
    assert(getPreviousTurnPlayer(game) == 0);

    //check top discard pile
    assert(cardColor(topDiscard(game)) == 0);
    assert(cardValue(topDiscard(game)) == 2);
    assert(cardSuit(topDiscard(game)) == 0);


    //check TWO
    assert(getNumberOfTwoCardsAtTop(game) == 2);
    assert(getActiveDrawTwos(game) == 2);


    printf("\n>Turn:   1 | Top Discard: RED DRAW_TWO of DIAMONDS\n");
    printf("   Player: 1 | Move:\n");
    printf("Player 1 wants to draw a card\n");

    // draw a card
    move.action = DRAW_CARD;
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);



    //check the drawn card YELLOW B of QUESTIONS
    assert(cardColor(handCard(game, 0)) == 3);
    assert(cardValue(handCard(game, 0)) == 11);
    assert(cardSuit(handCard(game, 0)) == 4);
    

        // draw a card
    move.action = DRAW_CARD;
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);


    //check the drawn card YELLOW FIVE of HEARTS
    assert(cardColor(handCard(game, 0)) == 3);
    assert(cardValue(handCard(game, 0)) == 5);
    assert(cardSuit(handCard(game, 0)) == 0);


        // draw a card
    move.action = DRAW_CARD;
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);


    //check the drawn card YELLOW THREE of DIAMONDS
    assert(cardColor(handCard(game, 0)) == 3);
    assert(cardValue(handCard(game, 0)) == 3);
    assert(cardSuit(handCard(game, 0)) == 1);


    // draw a card
    move.action = DRAW_CARD;
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);


    //check the drawn card YELLOW ONE of CLUBS
    assert(cardColor(handCard(game, 0)) == 3);
    assert(cardValue(handCard(game, 0)) == 1);
    assert(cardSuit(handCard(game, 0)) == 2);

    //end the turn
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    printf("Player 1 wants to end their turn\n");
    playMove(game, move);   
    printf(".....................................................\n");
}


//player 2 Turn 2
static void check3_player2Turn2(Game game) {


    playerMove move;


    // check current turn
    assert(currentTurn(game) == 2);


    //check number of turns
    assert(numTurns(game) == 3);


    //check current player
    assert(currentPlayer(game) == 2);


    //check the turn number of the top discard pile 
    assert(getTopDiscardTurnNumber(game) == 0);


    //check the previous player 
    assert(getPreviousTurnPlayer(game) == 1);


    //check top discard pile
    assert(cardColor(topDiscard(game)) == 0);
    assert(cardValue(topDiscard(game)) == 2);
    assert(cardSuit(topDiscard(game)) == 0);


    //check TWO
    assert(getNumberOfTwoCardsAtTop(game) == 2);
    assert(getActiveDrawTwos(game) == 0);

    printf("\n>Turn:   2 | Top Discard: YELLOW DRAW_TWO of QUESTIONS\n");
    printf("   Player: 2 | Move:\n");
    printf("Player 2 wants to play a RED ZERO of CLUBS\n");



    //check card in hand
    assert(cardColor(handCard(game, 6)) == 0);
    assert(cardValue(handCard(game, 6)) == 0);
    assert(cardSuit(handCard(game, 6)) == 2);
    
    //play a card
    move.action = PLAY_CARD;
    move.card = handCard(game, 6);
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);
    
    //end the turn 
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);


    printf("Player 2 wants to end their turn\n");
    playMove(game, move);   
    printf(".....................................................\n");
}

static void check3_player3Turn3(Game game) {


    playerMove move;

    // check current turn
    assert(currentTurn(game) == 3);


    //check number of turns
    assert(numTurns(game) == 4);


    //check current player
    assert(currentPlayer(game) == 3);



    //check top discard pile
    assert(cardColor(topDiscard(game)) == 0);
    assert(cardValue(topDiscard(game)) == 0);
    assert(cardSuit(topDiscard(game)) == 2);


    //check TWO
    assert(getNumberOfTwoCardsAtTop(game) == 0);
    assert(getActiveDrawTwos(game) == 0);


    printf("\n>Turn:   3 | Top Discard: RED ZERO of CLUBS\n");
    printf("   Player: 3 | Move:\n");
    printf("Player 3 wants to play a RED DRAW_TWO of SPADES\n");



    //check card in hand
    assert(cardColor(handCard(game, 6)) == 0);
    assert(cardValue(handCard(game, 6)) == 2);
    assert(cardSuit(handCard(game, 6)) == 3);


    //play a card
    move.action = PLAY_CARD;
    move.card = handCard(game, 6);
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);

    //end a turn 
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);


    printf("Player 3 wants to end their turn\n");
    playMove(game, move);   
    printf(".....................................................\n");
}

//player 0 Turn 4
static void check3_player0Turn4(Game game) {
    playerMove move;


    // check current turn
    assert(currentTurn(game) == 4);


    //check number of turns
    assert(numTurns(game) == 5);


    //check current player
    assert(currentPlayer(game) == 0);


    //check top discard pile
    assert(cardColor(topDiscard(game)) == 0);
    assert(cardValue(topDiscard(game)) == 2);
    assert(cardSuit(topDiscard(game)) == 3);


    //check TWO
    assert(getNumberOfTwoCardsAtTop(game) == 1);
    assert(getActiveDrawTwos(game) == 1);

    printf("\n>Turn:   4 | Top Discard: YELLOW DRAW_TWO of SPADES\n");
    printf("   Player: 0 | Move:\n");
    printf("Player 0 wants to play a RED DRAW_TWO of QUESTIONS\n");


    //check card in hand
    assert(cardColor(handCard(game, 5)) == 0);
    assert(cardValue(handCard(game, 5)) == 2);
    assert(cardSuit(handCard(game, 5)) == 4);

    //play a card
    move.action = PLAY_CARD;
    move.card = handCard(game, 5);
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);
    

    //end the turn
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    printf("Player 0 wants to end their turn\n");
    playMove(game, move);   
    printf(".....................................................\n");
}

//player 1 Turn 5
static void check3_player1Turn5(Game game) {

    playerMove move;


    // check current turn
    assert(currentTurn(game) == 5);


    //check number of turns
    assert(numTurns(game) == 6);


    //check current player
    assert(currentPlayer(game) == 1);


    //check top discard pile
    assert(cardColor(topDiscard(game)) == 0);
    assert(cardValue(topDiscard(game)) == 2);
    assert(cardSuit(topDiscard(game)) == 4);


    //check TWO
    assert(getNumberOfTwoCardsAtTop(game) == 2);
    assert(getActiveDrawTwos(game) == 2);

    printf("\n>Turn:   1 | Top Discard: RED DRAW_TWO of DIAMONDS\n");
    printf("   Player: 1 | Move:\n");
    printf("Player 1 wants to draw a card\n");


    //draw a card
    move.action = DRAW_CARD;
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);


    //check the drawn card 
    assert(cardColor(handCard(game, 0)) == 3);
    assert(cardValue(handCard(game, 0)) == 5);
    assert(cardSuit(handCard(game, 0)) == 3);


    //draw a card
    move.action = DRAW_CARD;
    assert(isValidMove(game, move) == TRUE);
    assert(getDeckCard(game, 1) == NULL);
    playMove(game, move);
    assert(getDeckCard(game, 0) == NULL);


    //check the drawn card Y
    assert(cardColor(handCard(game, 0)) == 3);
    assert(cardValue(handCard(game, 0)) == 7);
    assert(cardSuit(handCard(game, 0)) == 4);


    //draw a card
    move.action = DRAW_CARD;
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);


    //check the drawn card 
    assert(cardColor(handCard(game, 0)) == 3);
    assert(cardValue(handCard(game, 0)) == 2);
    assert(cardSuit(handCard(game, 0)) == 3);
    check_flip(game);


    //check the drawn card
    move.action = DRAW_CARD;
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);


    //check the drawn card 
    assert(cardColor(handCard(game, 0)) == 0);
    assert(cardValue(handCard(game, 0)) == 2);
    assert(cardSuit(handCard(game, 0)) == 0);


    //end the turn 
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    printf("Player 1 wants to end their turn\n");
    playMove(game, move);   
    printf(".....................................................\n");
}

//check flipping over the card pile
static void check_flip(Game game) {


    //check new discard pile
    assert(cardColor(topDiscard(game)) == 0);
    assert(cardValue(topDiscard(game)) == 2);
    assert(cardSuit(topDiscard(game)) == 4);
    
    //printf("\n%d\n",cardColor(getDiscardPileCard(game, 4)));
    assert(getDiscardPileCard(game, 1) == NULL);


    //check new deck
    assert(cardColor(getDeckCard(game, 0)) == 0);
    assert(cardValue(getDeckCard(game, 0)) == 2);
    assert(cardSuit(getDeckCard(game, 0)) == 0);
    printf("\nThe new card pile is correct\n!!! ");
}

//end of the end_
static void end_3(Game game) {
   
    printf("\nPlayer 1 gives up!!!!\n");
    printf("===========+=============\n");
    printf("| Player 0 |   %d points |\n", playerPoints(game, 0));
    printf("| Player 1 |   %d points |\n", playerPoints(game, 1));
    printf("| Player 2 |   %d points |\n", playerPoints(game, 2));
    printf("| Player 3 |   %d points |\n", playerPoints(game, 3));
    printf("===========+=============\n");
    printf("Trying to destroy the game\n");
    destroyGame(game);
    printf("PASS TEST_3\n");
    printf("\n=======================\n");
}

// the start of test 4
static void test4_D(void) {
    int deckSize = 50;
    value values[] = {
        D, ZERO, D, D,
        D, ZERO, D, D,
        D, ZERO, D, D,
        D, ZERO, D, D,
        D, ZERO, D, D,
        D, ZERO, D, D,
        ONE, THREE, FOUR, FIVE,
        D, D, EIGHT, NINE,
        A, B, C, E,
        ONE, THREE, FOUR, FIVE,
        SIX, SEVEN, EIGHT, NINE,
        A, B, C, E,
        SIX, SEVEN
    };

    color colors[] = {
        RED, RED, RED, RED, 
        RED, RED, RED, RED, 
        RED, RED, BLUE, BLUE, 
        BLUE, BLUE, BLUE, BLUE,
        BLUE, BLUE, BLUE, BLUE,
        GREEN, GREEN, GREEN, GREEN,
        YELLOW, GREEN, GREEN, GREEN,
        GREEN, GREEN, YELLOW, YELLOW,
        YELLOW, YELLOW, YELLOW, YELLOW,
        YELLOW, YELLOW, YELLOW, YELLOW,
        PURPLE, PURPLE, PURPLE, PURPLE, 
        PURPLE, PURPLE, PURPLE, PURPLE,
        PURPLE, PURPLE
    };

    suit suits[] = {
        HEARTS, DIAMONDS, CLUBS, SPADES, 
        QUESTIONS, HEARTS, DIAMONDS, CLUBS, 
        SPADES, QUESTIONS, HEARTS, DIAMONDS, 
        CLUBS, SPADES, QUESTIONS, HEARTS, 
        DIAMONDS, CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, 
        QUESTIONS, HEARTS, DIAMONDS, CLUBS, 
        SPADES, QUESTIONS, HEARTS, DIAMONDS, 
        CLUBS, SPADES, QUESTIONS, HEARTS, 
        DIAMONDS, CLUBS, SPADES, QUESTIONS, 
        HEARTS, DIAMONDS, CLUBS, SPADES, 
        QUESTIONS, HEARTS, DIAMONDS, CLUBS, 
        SPADES, QUESTIONS
    };
    
    //initialize the Game
    Game g = newGame(deckSize, values, colors, suits);
    
    //check the deckSize
    assert(numCards(g) == 50);

    player0Turn0(g);
    player1Turn1(g);
    player2Turn2(g);
    player3Turn3(g);
    player0Turn4(g);
    player1Turn5(g);
    end_4(g);
}

// player 0 turn 0
static void player0Turn0(Game game) {


    playerMove move;


    // check current turn
    assert(currentTurn(game) == 0);


    //check number of turns
    assert(numTurns(game) == 1);


    //check current player
    assert(currentPlayer(game) == 0);


    //check top discard pile GREEN D of SPADES
    assert(cardColor(topDiscard(game)) == 2);
    assert(cardValue(topDiscard(game)) == 13);
    assert(cardSuit(topDiscard(game)) == 3);


    //check TWO
    assert(getNumberOfTwoCardsAtTop(game) == 0);
    assert(getActiveDrawTwos(game) == 0);

    printf("\n>Turn:   0 | Top Discard: GREEN D of SPADES\n");
    printf("   Player: 0 | Move:\n");
    printf("Player 0 wants to play a RED D of HEARTS\n");




    //check card in hand 
    assert(cardColor(handCard(game, 6)) == 0);
    assert(cardValue(handCard(game, 6)) == 13);
    assert(cardSuit(handCard(game, 6)) == 0);


    //play a card
    move.action = PLAY_CARD;
    move.card = handCard(game, 6);
    move.nextColor = BLUE;
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);
    
    //end the turn
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    printf("Player 0 wants to end their turn\n");
    playMove(game, move);   
    printf(".....................................................\n");
}

// player 1 Turn 1
static void player1Turn1(Game game) {

    playerMove move;


    // check current turn
    assert(currentTurn(game) == 1);


    //check number of turns
    assert(numTurns(game) == 2);


    //check current player
    assert(currentPlayer(game) == 1);


    //check current color is 
    assert(getCurrentColor(game) == BLUE);


    //check the turn number of the top discard pile 
    assert(getTopDiscardTurnNumber(game) == 0);


    //check the previous player 
    assert(getPreviousTurnPlayer(game) == 0);


    //check top discard pile
    assert(cardColor(topDiscard(game)) == 0);
    assert(cardValue(topDiscard(game)) == 13);
    assert(cardSuit(topDiscard(game)) == 0);


    //check TWO
    assert(getNumberOfTwoCardsAtTop(game) == 0);
    assert(getActiveDrawTwos(game) == 0);

    printf("\n>Turn:   1 | Top Discard: RED D of HEARTS\n");
    printf("   Player: 1 | Move:\n");
    printf("Player 1 wants to play a RED ZERO of DIAMONDS\n");


    //check the card is in the hand
    assert(cardColor(handCard(game, 6)) == 0);
    assert(cardValue(handCard(game, 6)) == 0);
    assert(cardSuit(handCard(game, 6)) == 1);
    
    //play a card
    move.action = PLAY_CARD;
    move.card = handCard(game, 6);
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);

    //end the turn 
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    printf("Player 1 wants to end their turn\n");
    playMove(game, move);   
    printf(".....................................................\n");
}

// player 2 turn 2
static void player2Turn2(Game game) {


    playerMove move;


    // check current turn
    assert(currentTurn(game) == 2);


    //check number of turns
    assert(numTurns(game) == 3);


    //check current player
    assert(currentPlayer(game) == 2);


    //check current color is 
    assert(getCurrentColor(game) == RED);


    //check the turn number of the top discard pile 
    assert(getTopDiscardTurnNumber(game) == 1);


    //check the previous player 
    assert(getPreviousTurnPlayer(game) == 1);

    //check top discard pile
    assert(cardColor(topDiscard(game)) == 0);
    assert(cardValue(topDiscard(game)) == 0);
    assert(cardSuit(topDiscard(game)) == 1);


    //check TWO
    assert(getNumberOfTwoCardsAtTop(game) == 0);
    assert(getActiveDrawTwos(game) == 0);

    printf("\n>Turn:   2 | Top Discard: RED ZERO of DIAMONDS\n");
    printf("   Player: 2 | Move:\n");
    printf("Player 2 wants to play a RED D of CLUBS\n");


    //check the card is in the hand
    assert(cardColor(handCard(game, 6)) == 0);
    assert(cardValue(handCard(game, 6)) == 13);
    assert(cardSuit(handCard(game, 6)) == 2);
    
    //play a card
    move.action = PLAY_CARD;
    move.card = handCard(game, 6);
    move.nextColor = GREEN;
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);

    //end the turn
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    printf("Player 2 wants to end their turn\n");
    playMove(game, move);   
    printf(".....................................................\n");
}

//player 3 Turn 3
static void player3Turn3(Game game) {

    playerMove move;


    // check current turn
    assert(currentTurn(game) == 3);


    //check number of turns
    assert(numTurns(game) == 4);


    //check current player
    assert(currentPlayer(game) == 3);


    //check current color is 
    assert(getCurrentColor(game) == GREEN);


    //check the turn number of the top discard pile 
    assert(getTopDiscardTurnNumber(game) == 2);


    //check the previous player 
    assert(getPreviousTurnPlayer(game) == 2);

    //check top discard pile
    assert(cardColor(topDiscard(game)) == 0);
    assert(cardValue(topDiscard(game)) == 13);
    assert(cardSuit(topDiscard(game)) == 2);


    //check TWO
    assert(getNumberOfTwoCardsAtTop(game) == 0);
    assert(getActiveDrawTwos(game) == 0);

    printf("\n>Turn:   3 | Top Discard: RED D of CLUBS\n");
    printf("   Player: 3 | Move:\n");
    printf("Player 3 wants to play a BLUE D of HEARTS\n");


    //check the card is in the hand
    assert(cardColor(handCard(game, 3)) == 1);
    assert(cardValue(handCard(game, 3)) == 13);
    assert(cardSuit(handCard(game, 3)) == 0);
    

    //play a card
    move.action = PLAY_CARD;
    move.card = handCard(game, 3);
    move.nextColor = YELLOW;
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);

    //end the turn 
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    printf("Player 3 wants to end their turn\n");
    playMove(game, move);   
    printf(".....................................................\n");
}

//player 0 turn 4
static void player0Turn4(Game game) {

    playerMove move;

    // check current turn
    assert(currentTurn(game) == 4);


    //check number of turns
    assert(numTurns(game) == 5);


    //check current player
    assert(currentPlayer(game) == 0);


    //check current color is 
    assert(getCurrentColor(game) == YELLOW);


    //check the turn number of the top discard pile 
    assert(getTopDiscardTurnNumber(game) == 3);


    //check the previous player 
    assert(getPreviousTurnPlayer(game) == 3);


    //check number of cards in hand
    assert(handCardCount(game) == 6);

    //check top discard pile
    assert(cardColor(topDiscard(game)) == 1);
    assert(cardValue(topDiscard(game)) == 13);
    assert(cardSuit(topDiscard(game)) == 0);


    //check TWO
    assert(getNumberOfTwoCardsAtTop(game) == 0);
    assert(getActiveDrawTwos(game) == 0);

    printf("\n>Turn:   4 | Top Discard: BLUE D of HEARTS\n");
    
    printf("   Player: 0 | Move:\n");
    printf("Player 0 wants to play a YELLOW ONE of QUESTIONS\n");


    //check the card is in the hand
    assert(cardColor(handCard(game, 0)) == 3);
    assert(cardValue(handCard(game, 0)) == 1);
    assert(cardSuit(handCard(game, 0)) == 4);
    
    //play a card
    move.action = PLAY_CARD;
    move.card = handCard(game, 0);
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);

    //end the turn
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    //check number of cards in hand
    assert(handCardCount(game) == 5);

    printf("Player 0 wants to end their turn\n");
    playMove(game, move);   
    printf(".....................................................\n");
}

// player 1 Turn 5
static void player1Turn5(Game game) {


    playerMove move;


    // check current turn
    assert(currentTurn(game) == 5);


    //check number of turns
    assert(numTurns(game) == 6);


    //check current player
    assert(currentPlayer(game) == 1);


    //check current color is 
    assert(getCurrentColor(game) == 3);


    //check the turn number of the top discard pile 
    assert(getTopDiscardTurnNumber(game) == 4);


    //check the previous player 
    assert(getPreviousTurnPlayer(game) == 0);


    //check number of cards in hand
    assert(handCardCount(game) == 6);


    //check top discard pile
    assert(cardColor(topDiscard(game)) == 3);
    assert(cardValue(topDiscard(game)) == 1);
    assert(cardSuit(topDiscard(game)) == 4);


    //check TWO
    assert(getNumberOfTwoCardsAtTop(game) == 0);
    assert(getActiveDrawTwos(game) == 0);

    printf("\n>Turn:   5 | Top Discard: YELLOW ONE of QUESTIONS\n");
    printf("   Player: 1 | Move:\n");
    printf("Player 1 wants to play a RED ZERO of HEARTS\n");



    assert(cardColor(handCard(game, 5)) == 0);
    assert(cardValue(handCard(game, 5)) == 0);
    assert(cardSuit(handCard(game, 5)) == 0);

    //play a card
    move.action = PLAY_CARD;
    move.card = handCard(game, 5);
    assert(isValidMove(game, move) == TRUE);
    playMove(game, move);

    //end the turn 
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    //check number of cards in hand
    assert(handCardCount(game) == 5);


    printf("Player 1 wants to end their turn\n");
    playMove(game, move);   
    printf(".....................................................\n");
}

//the end of test4
static void end_4(Game game) {
   
    printf("\nWriting the testGame is annoying. I give up!!!\n");
    printf("===========+=============\n");
    printf("| Player 0 |   %d points |\n", playerPoints(game, 0));
    printf("| Player 1 |   %d points |\n", playerPoints(game, 1));
    printf("| Player 2 |   %d points |\n", playerPoints(game, 2));
    printf("| Player 3 |   %d points |\n", playerPoints(game, 3));
    printf("===========+=============\n");
    printf("Trying to destroy the game\n");
    destroyGame(game);
    printf("PASS TEST_4\n");
    printf("\n=======================\n");
}


