//By Terry(z5191493)
//10/13/2018
//Tutor Helena & Steven

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "Card.h"
#include "Game.h"

#define START_HAND_SIZE 7
#define NUM_PLAYERS 4
#define HAND_CARDS 28

typedef struct node *Node;
typedef struct state *State;

typedef struct node {
    Card cards;
    Node next;
}node;

typedef struct player{
    Node hand;
    int total_points;
    int num_card;
}Player;

typedef struct state{
    int order;
    int moveCount;
    int turnNumber;
    int discard_turn;
    int two;
    int top_dis_player;
    color color;
    action action;
}state;    
    

typedef struct _game {
    int active_D;
    int num_turn;
    int size;
    int pre_player;
    Node start_deck;
    Node deck;
    Node discard;
    Player players[NUM_PLAYERS];
    State state;
    action pre_action;
}game;

static Node start_deck(int i, value values[], color colors[], suit suits[]);
static void start_hand(Game game);
static Node start_draw(int i, Node node);
static State newstate(State new);
static Player start_player(Player player, int i);
static int check_same(Card c1, Card c2);
static int check_two(Game game, int ply);
static void flip(Game game);
static void update(Game game, int player, playerMove move);
static void remove_card(Game game, int player, playerMove move, int i);
static int checkValidCard(Game game, int ply, Card card);
static int checkcards(Card cur, Card pre, Game game);


//intialize the discard pile
static Node new_node(Card cards) {
    Node new = malloc(sizeof(struct node));
    new->cards = cards;
    new->next = NULL; 
    return new;
}

//intialize the deck
static Node start_deck(int i, value values[], color colors[], 
                        suit suits[]) {
    Node head = NULL;
    
    while (i >= 0) {
        Node n = new_node(newCard(values[i], colors[i], suits[i]));
        n->next = head;
        head = n;
        i--;
    }
    return head;

}

//intialize the hand[]
static void start_hand_discard(Game game) {
    int order = 0;
    int count = 0;
    int i = 0;
    while (i < HAND_CARDS) {
        if (order == 0) {
            count++;
        }
        game->players[order].num_card = count;
        Node p = game->deck;
        game->deck = game->deck->next;
        p->next = game->players[order].hand;
        game->players[order].hand = p;
        i++;
        order = i % NUM_PLAYERS;
    }



}

static State newstate(State new) {
    State s = malloc(sizeof(state));
    s->action = END_TURN;
    s->turnNumber = 0;
    s->order = 0;
    s->moveCount = 0;
    s->discard_turn = -1;
    s->color = 0;
    s->two = 0;
    new = s;
    return s;
}

static Player start_player(Player player, int i) {
    player.hand = malloc(sizeof(struct node) * i);
    player.hand = NULL;
    player.num_card = 0;
    player.total_points = 0;
    return player;
}



Game newGame(int deckSize, value values[], color colors[], 
             suit suits[]) {
    Game game1 = malloc(sizeof(game));

    game1->state = newstate(game1->state);

    game1->discard = malloc(sizeof(struct node) * deckSize);

    game1->start_deck = start_deck(deckSize - 1, values, colors, suits);
    
    game1->deck = start_deck(deckSize - 1, values, colors, suits);
    
    int i = 0;
    while (i < NUM_PLAYERS) {
        game1->players[i] = start_player(game1->players[i], deckSize);
        i++;
    }

    game1->num_turn = 1;
    game1->pre_action = END_TURN;


    
    start_hand_discard(game1);    

    game1->discard = game1->deck;
    game1->deck = game1->deck->next;
    game1->discard->next = NULL;
    game1->pre_action = END_TURN;
    
    
    game1->state = newstate(game1->state);

    game1->size = deckSize;
    
    game1->pre_player = -1;
    game1->active_D = 0;
    game1->state->color = cardColor(game1->discard->cards);

    if (cardValue(game1->discard->cards) == A) {
        game1->state->order = 1;
    } else if (cardValue(game1->discard->cards) == DRAW_TWO) {
        game1->state->two = 1;
    }   
    return game1;
}

void destroyGame(Game game) {
    free(game);
}

int numCards(Game game) {
    return game->size;
}

int numOfSuit(Game game, suit suit) {
    Node new = game->start_deck; 
    int i = 0;
    while (new != NULL) {
        if (cardSuit(new->cards) == suit) {
            i++;
        }
        new = new->next;
    }
    return i;
}

int numOfColor(Game game, color color) {
    Node new = game->start_deck; 
    int i = 0;
    while (new != NULL) {
        if (cardColor(new->cards) == color) {
            i++;
        }
        new = new->next;
    }
    return i;
}

int numOfValue(Game game, value value) {
    Node new = game->start_deck; 	
    int i = 0;	
    while (new != NULL) {
        if (cardValue(new->cards) == value) {
            i++;
        }
        new = new->next;
    }

    return i;
}



int currentPlayer(Game game) {
    return game->state->order;
}

int currentTurn(Game game) {
    return game->state->turnNumber;
}

int playerPoints(Game game, int player) {
    int sum = 0;
    Node new = game->players[player].hand;
    while (new != NULL) {
        sum = sum + cardValue(new->cards);
        new = new->next;
    }

    return sum;
}

Card topDiscard(Game game) {
    return game->discard->cards;
}
        
int numTurns(Game game) {
    return game->num_turn;
}

int currentTurnMoves(Game game) {
    return game->state->moveCount;
}




int getNumberOfTwoCardsAtTop(Game game) {
    value val = 0;
    int count = 0;
    Node new = game->discard;
    val = cardValue(new->cards);
    while (val == DRAW_TWO && new != NULL) {
        count++;
        new = new->next;
        if (new != NULL) {
            val = cardValue(new->cards);
        } 
    }
    return count;
}




int getCurrentColor(Game game) {
    return game->state->color;
}




int getTopDiscardTurnNumber(Game game) {
    return game->state->discard_turn;
}




int getActiveDrawTwos(Game game) {
    return game->state->two;
}




int getPreviousTurnPlayer(Game game) {
    int ply = 0;
    int curply = game->state->order;
    
    if (game->state->turnNumber == 0) {
        return -1;
    } 
    int act = 0;
    act = game->state->order - game->state->top_dis_player;
    if (cardValue(game->discard->cards) == A && act == 2) {
        ply = (curply + 2) % NUM_PLAYERS;
    } else {
        ply = (curply + 3) % NUM_PLAYERS;
    }
    return ply;
}


int playerCardCount(Game game, int player) {
    return game->players[player].num_card;
}

int handCardCount(Game game) {
    int count = game->players[game->state->order].num_card;
    return count;
}

Card handCard(Game game, int card) {
    int currentplayer = game->state->order;
    if (game->players[currentplayer].num_card < card) {
        return NULL;
    } else {
        int index = 0;
        Node handcard = game->players[currentplayer].hand;
        while (handcard != NULL && index < card) {
            handcard = handcard->next;
            index++;
        
        }
        return handcard->cards;
    }
}

//check whether two cards are matched
static int check_same(Card c1, Card c2) {
    int check = FALSE;
    if (c1 == c2) {
        check = TRUE;
    } else {
        return FALSE;
    }
    return check;
}


// check whether the card is in the player's hand
static int checkValidCard(Game game, int ply, Card card) {
    Node h = game->players[ply].hand;
    while (h != NULL) {
        check_same(h->cards, card);
        if (check_same(h->cards, card) == TRUE) {
            return TRUE;
        }
        h = h->next;
    }
    return FALSE;

}

// check whether the card is matched
static int checkcards(Card cur, Card pre, Game game) {

    int result = TRUE;

    //ZERO is a special card
    if (cardValue(cur) == ZERO) {
        return TRUE;
    }

    //regular check whether the card played is matched 
    if (cardValue(cur) == cardValue(pre)) {
        result = TRUE;
    } else if (cardColor(cur) == game->state->color) {
        result = TRUE;
    } else if (cardSuit(cur) == cardSuit(pre)) {
        result = TRUE;
    } else {
        result = FALSE;
    }
    return result;
}



//find a DRAW_TWO is player's hand 
static int check_two(Game game, int ply) {
    Node h = game->players[ply].hand;
    while (h != NULL) {
        if (cardValue(h->cards) == DRAW_TWO) {
            return TRUE;
        }
        h = h->next;
    }
    return FALSE;
}



int isValidMove(Game game, playerMove move) {

    int cur = game->state->order;
    Card dis = game->discard->cards;
    Node handcard = game->players[cur].hand;


    if (move.action == PLAY_CARD) {
        if (checkValidCard(game, cur, move.card) != TRUE) {
            return FALSE;
        }
        if (game->pre_action != END_TURN && game->num_turn > 1) {
            return FALSE;
        }
        if (getActiveDrawTwos(game) != 0) {
            if (check_two(game, cur) != TRUE) {
                return FALSE;
            }
            if (cardValue(move.card) == DRAW_TWO) {
                return TRUE;
            } else {
                return FALSE;
            }
        } else {
            return checkcards(move.card, dis, game);
        }
    } else if (move.action == DRAW_CARD) {
        if (cardValue(topDiscard(game)) == DRAW_TWO) {
            if (currentTurnMoves(game) <= 2*getActiveDrawTwos(game)) {
                return TRUE;
            } else {
                return FALSE;
            }
        }
        if (game->pre_action == DRAW_CARD) {
            return FALSE;
        }
        return TRUE;
    } else {
        int i = getActiveDrawTwos(game);
        if (i > 0) {
            if (game->state->moveCount >= 2*i) {
                return TRUE;
            } else if (game->pre_action == PLAY_CARD) {
                return TRUE;
            } else {
                return FALSE;   
            }
        } else if (game->pre_action == END_TURN) {
            return FALSE;
        } else {
            return TRUE;
        }
    }
    return TRUE;
}


//flip over the card pile
static void flip(Game game) {
    game->deck = NULL;
    Node dis = game->discard->next;
    if (dis != NULL) {
        game->discard->next = NULL;
        while (dis != NULL) {
            Node temp = dis;
            dis = dis->next;
            temp->next = game->deck;
            game->deck = temp;
        }   
    }
}

// update only when ending the turn
static void update(Game game, int player, playerMove move) {
    game->pre_action = END_TURN;
    int count = 0;
    if (game->state->moveCount == 2 * game->state->two) {
        game->state->two = 0;
    }
    game->state->moveCount = 0;
    count = game->state->turnNumber - getTopDiscardTurnNumber(game);
    if (cardValue(game->discard->cards) == A && count == 0) {
        game->state->order = (currentPlayer(game) + 2) % NUM_PLAYERS;
    } else {
        game->state->order = (currentPlayer(game) + 1) % NUM_PLAYERS;    
    }
    game->state->turnNumber++;
    game->num_turn++;
}

// update when drawing card
static void update_hand(Game game, int player, int currpoints) {
    game->state->moveCount++;
    Node draw = game->deck;;
    game->deck = game->deck->next; 
    draw->next = game->players[player].hand;
    game->players[player].hand = draw;
    game->players[player].num_card++;
    if (game->deck != NULL) { 
        currpoints = currpoints + cardValue(game->deck->cards);
    }

}



// update when playing card
static void play_card(Game game, int curpl, playerMove move, int p) {
    game->state->moveCount++;
    game->state->discard_turn = game->state->turnNumber;
    game->state->top_dis_player = game->state->order;
    game->players[curpl].num_card--;
    p = p - cardValue(game->discard->cards); 
    Node currcard = game->players[curpl].hand;
    Node prevcard = NULL;
    while (currcard != NULL && check_same(move.card, currcard->cards) == FALSE) {
        prevcard = currcard;
        currcard = currcard->next;
    }
    if (currcard != NULL) {     
        if (prevcard == NULL) {
            game->players[curpl].hand = game->players[curpl].hand->next;
            currcard->next = game->discard;
            game->discard = currcard;
        } else {
            prevcard->next = currcard->next;
            currcard->next = game->discard;
            game->discard = currcard;
        }       
    } else {
        game->discard = game->discard;
    }
    if (cardValue(move.card) == D) {
        game->state->color = move.nextColor;
        game->state->two = 0;
    } else if (cardValue(move.card) == DRAW_TWO) {
        game->state->two++;
    } else {
        game->state->two = 0;
    }
}




void playMove(Game game, playerMove move) {
    int curpl = game->state->order;
    int currpoints = game->players[curpl].total_points;
    int moves = game->state->moveCount;
    game->state->action = move.action;
    if (game->state->turnNumber == 1 && moves == 1) {
        game->pre_action = move.action;
    } else {
        game->pre_action = game->state->action;
        game->state->action = move.action;
    }
    if (game->deck == NULL) {
        flip(game);
    }
    if (move.action == END_TURN) {
        update(game, curpl, move);
    } else if (move.action == DRAW_CARD) {
        update_hand(game, curpl, currpoints);
    } else if (move.action == PLAY_CARD) {
        game->state->color = cardColor(move.card);
        play_card(game, curpl, move, currpoints);
    }
}


int gameWinner(Game game) {
    int win = 0;
    if (game->players[0].num_card == 0) {
        win = 1;
    } else if (game->players[1].num_card == 0) {
        win = 1;
    } else if (game->players[2].num_card == 0) {
        win = 1;
    } else if (game->players[3].num_card == 0) {
        win = 1;
    }
    
    Card card = getDiscardPileCard(game, 1);
    int cur;
    cur = currentPlayer(game);
    if (win == 1) {
        return cur;
    } else if (game->deck == NULL && card == NULL) {
        return NO_WINNER;
    } else {
        return NOT_FINISHED;
    }
    
}



Card getDeckCard (Game game, int n) {
    int i = 0;
    Node topCards = game->deck;
    while (topCards != NULL && i < n) {
        topCards = topCards->next;
        i++;
    }
    if (topCards != NULL && i == n) {
        return topCards->cards;
    }
    return NULL;
}


Card getDiscardPileCard (Game game, int n) {
    int i = 0;
    Node topCards = game->discard;
    while (topCards != NULL && i < n) {
        topCards = topCards->next;
        i++;
    }
    if (topCards != NULL && i == n) {
        return topCards->cards;
    }
    return NULL;
}


Card getHandCard (Game game, int player, int n) {
    int i = 0;
    int num = game->players[player].num_card;
    if (num <= n) {
        return NULL;
    }
    if (game->players[player].hand == NULL) {
        return NULL;
    }
    Node topCards = game->players[player].hand;
    Card nthcard = NULL;
    while (topCards != NULL && i < n) {
        topCards = topCards->next;
        i++;
    }
    if (topCards != NULL && i == n) {
        nthcard = topCards->cards;
    }
    return nthcard;
}

// count total points in one specific player
static int pointscount(Game game, int player) {
    struct player cur = game->players[player];
    if (cur.hand == NULL) {
        return 0;
    }
    Node handcards = cur.hand;
    int sum = 0;
    int i = 0;
    while (i < cur.num_card) {
        sum = sum + cardValue(handcards->cards);
        i++;
        handcards = handcards->next;
    }
    return sum;
}









