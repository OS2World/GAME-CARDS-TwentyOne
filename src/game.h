/*******************************************************************************
* GAME.H - rules of TwentyOne, no user interface in here
*
* Rules (from the original program by Michael G. Slack):
*  - Dealer hits on 16 or less, stays on 17+.
*  - A split plays with one more card on each of the two hands.
*  - Double down is only possible at the start and takes one card.
*  - Insurance can be bought when the dealer shows an ace (25% of the bet).
*  - 5 cards without going over 21 win automatically (player or dealer).
*  - 21 to start wins (unless the dealer shows an ace or both have 21).
*  - Closest to 21 without going over wins.
*******************************************************************************/

#ifndef GAME_H
#define GAME_H

#include "twenty.h"

/* Messages the rules engine asks the user interface to show. Each carries up
   to two numbers, see the formats in lang.c. */
enum
{
    GM_NONE = 0,
    GM_PUSH,                    /* nobody wins                               */
    GM_WON,                     /* a = 0 player / 1 dealer, b = points       */
    GM_WON_FIVE,                /* a = 0 player / 1 dealer                   */
    GM_NOT_ENOUGH_INS,          /* a = cost                                  */
    GM_NOT21,                   /* dealer has no 21 (insurance lost)         */
    GM_NOT_ENOUGH_SPLIT,
    GM_NOT_ENOUGH_DOUBLE,
    GM_PLAYER_OVER21,
    GM_DEALER_OVER21,
    GM_FIRST_HAND,
    GM_SECOND_HAND,
    GM_SHUFFLED,
    GM_FORFEIT                  /* the player quit the hand                  */
};

#define MAX_MSGS    12

struct GameMsg
{
    int code;
    int a;
    int b;
};

/* Settings used by the rules (loaded and saved by the program) */
extern int  NumOfDecks;
extern int  MinimumBet;
extern int  MaximumBet;
extern int  InitialBank;
extern int  BetMax;

/* State of the hand, read by the user interface */
extern long Bank;                       /* money of the player               */
extern long Bet;                        /* bet of the running hand           */
extern int  Dealer[MAX_DRAW_CARDS + 1]; /* 1-based, card ids 0..51           */
extern int  Player[MAX_DRAW_CARDS + 1];
extern int  NumDealer;
extern int  NumPlayer;
extern int  HoleShown;                  /* dealer's first card face up?      */
extern int  SplitHand;
extern int  InHand;                     /* a hand is being played            */
extern int  CanHit, CanStay, CanDouble, CanSplit, CanInsure;

extern struct GameMsg Msgs[MAX_MSGS];
extern int  NumMsgs;

/* Card helpers: id = rank * 4 + suit, rank 0 = deuce ... 12 = ace */
int  CardFace10(int card);              /* 1..10, ace = 1, J/Q/K = 10        */
int  CardValue(int card);               /* 1..13, ace = 1                    */
int  ScoreOfHand(const int *hand, int num);

void GameInit(void);                    /* new bank, new shuffled deck       */
void GameShuffle(void);
int  CardsLeft(void);
void GameSeed(void);

int  CanAffordBet(void);                /* Bank >= MinimumBet                */
void GameResetBank(void);               /* Bank += InitialBank               */
int  GameMaxBet(void);                  /* highest bet the player may place  */

void GameDeal(long bet);                /* start a hand (bet already valid)  */
void GameHit(void);
void GameStay(void);
void GameDouble(void);
void GameSplit(void);
void GameInsure(void);
void GameForfeit(void);                 /* quit the hand: the bet is lost    */

#endif /* GAME_H */
