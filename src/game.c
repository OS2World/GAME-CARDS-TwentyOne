/*******************************************************************************
* GAME.C - rules of TwentyOne, no user interface in here
*
* Ported from the Pascal units TwenWin.pas and TwenSup.pas by Michael G. Slack.
*
* Notes about the port:
*  - The dealer stops drawing after 5 cards (the Pascal version could write
*    past its 5 element array when the dealer drew small cards).
*  - A split is scored without touching the displayed cards.
*******************************************************************************/

#include <stdlib.h>
#include <time.h>

#include "game.h"

/*** Variables ****************************************************************/

int     NumOfDecks  = 1;
int     MinimumBet  = MIN_BET_AMT;
int     MaximumBet  = START_MAX_BET;
int     InitialBank = START_BANK;
int     BetMax      = 0;

long    Bank;
long    Bet;
int     Dealer[MAX_DRAW_CARDS + 1];
int     Player[MAX_DRAW_CARDS + 1];
int     NumDealer;
int     NumPlayer;
int     HoleShown;
int     SplitHand;
int     InHand;
int     CanHit, CanStay, CanDouble, CanSplit, CanInsure;

struct GameMsg Msgs[MAX_MSGS];
int     NumMsgs;

static  int     DoubleHand;
static  int     Deck[MAX_DECKS * 52];
static  int     DeckLeft;                       /* cards not yet dealt       */
static  int     DeckSize;


/*** Helpers ******************************************************************/

static void AddMsg(int code, int a, int b)
{
    if( NumMsgs < MAX_MSGS )
    {
        Msgs[NumMsgs].code = code;
        Msgs[NumMsgs].a    = a;
        Msgs[NumMsgs].b    = b;
        NumMsgs++;
    }
}


int CardValue(int card)
{
    int rank = card / 4;                /* 0 = deuce ... 12 = ace            */

    if( rank == 12 )
        return 1;
    return rank + 2;                    /* 2..10, J = 11, Q = 12, K = 13     */
}


int CardFace10(int card)
{
    int v = CardValue(card);

    return (v > 10) ? 10 : v;
}


/* Score of a hand: aces count 1, one of them 11 when that does not bust */
int ScoreOfHand(const int *hand, int num)
{
    int i, total = 0, ace = 0;

    for( i = 0; i < num; i++ )
    {
        total += CardFace10(hand[i]);
        if( CardValue(hand[i]) == 1 )
            ace = 1;
    }

    if( ace && total + 10 <= 21 )
        total += 10;

    return total;
}


static int ScorePlayer(void)
{
    return ScoreOfHand(&Player[1], NumPlayer);
}


static int ScoreDealer(void)
{
    return ScoreOfHand(&Dealer[1], NumDealer);
}


/*** Deck *********************************************************************/

void GameSeed(void)
{
    srand( (unsigned)time(NULL) );
}


void GameShuffle(void)
{
    int i, j, tmp;

    DeckSize = NumOfDecks * 52;
    for( i = 0; i < DeckSize; i++ )
        Deck[i] = i % 52;

    for( i = DeckSize - 1; i > 0; i-- )
    {
        j = rand() % (i + 1);
        tmp = Deck[i]; Deck[i] = Deck[j]; Deck[j] = tmp;
    }

    DeckLeft = DeckSize;
}


int CardsLeft(void)
{
    return DeckLeft;
}


static int DealCard(void)
{
    if( DeckLeft <= 0 )
    {
        GameShuffle();
        AddMsg( GM_SHUFFLED, 0, 0 );
    }

    DeckLeft--;
    return Deck[DeckLeft];
}


/*** Bank *********************************************************************/

void GameInit(void)
{
    Bank = InitialBank;
    Bet  = 0;
    InHand = 0;
    NumDealer = NumPlayer = 0;
    CanHit = CanStay = CanDouble = CanSplit = CanInsure = 0;
    GameShuffle();
}


int CanAffordBet(void)
{
    return Bank >= MinimumBet;
}


void GameResetBank(void)
{
    Bank += InitialBank;
}


int GameMaxBet(void)
{
    return (Bank < MaximumBet) ? (int)Bank : MaximumBet;
}


/*** Scoring ******************************************************************/

static void EndHand(void)
{
    InHand = 0;
    CanHit = CanStay = CanDouble = CanSplit = CanInsure = 0;
}


/* Final tally of one hand. playerWins: 1 player, 0 dealer. */
static void ScoreIt(int pts, int playerWins, int push, int five, int endHand)
{
    int who = playerWins ? 0 : 1;

    HoleShown = 1;

    if( push )
    {
        AddMsg( GM_PUSH, 0, 0 );
        Bank += Bet;
        if( DoubleHand )
            Bank += Bet;
    }
    else
    {
        if( playerWins )
        {
            Bank += Bet + Bet;                  /* the bet back plus winnings */
            if( DoubleHand )
                Bank += Bet + Bet;
        }

        if( five )
            AddMsg( GM_WON_FIVE, who, 0 );
        else
            AddMsg( GM_WON, who, pts );
    }

    if( endHand )
        EndHand();
}


/* Result of one player hand against the dealer's final score */
static void ScoreAgainst(int d, int p, int endHand)
{
    if( d == p )
        ScoreIt( p, 0, 1, 0, endHand );
    else if( d < p )
        ScoreIt( p, 1, 0, 0, endHand );
    else
        ScoreIt( d, 0, 0, 0, endHand );
}


static void FinishOffDealer(void)
{
    int d, p, p2 = 0, hand2[2];

    HoleShown = 1;
    CanHit = CanStay = CanDouble = CanSplit = CanInsure = 0;

    if( SplitHand )
    {
        p       = ScoreOfHand( &Player[1], 2 );         /* first hand        */
        hand2[0] = Player[4];
        hand2[1] = Player[5];
        p2      = ScoreOfHand( hand2, 2 );
    }
    else
        p = ScorePlayer();

    d = ScoreDealer();
    while( d <= 16 && NumDealer < MAX_DRAW_CARDS )
    {
        NumDealer++;
        Dealer[NumDealer] = DealCard();
        d = ScoreDealer();
    }

    if( d <= 21 )
    {
        if( NumDealer == MAX_DRAW_CARDS )
            ScoreIt( 0, 0, 0, 1, 1 );                   /* 5 cards: dealer   */
        else
        {
            if( SplitHand )
                AddMsg( GM_FIRST_HAND, 0, 0 );
            ScoreAgainst( d, p, !SplitHand );
            if( SplitHand )
            {
                AddMsg( GM_SECOND_HAND, 0, 0 );
                ScoreAgainst( d, p2, 1 );
            }
        }
    }
    else
    {
        AddMsg( GM_DEALER_OVER21, 0, 0 );
        if( SplitHand )
            AddMsg( GM_FIRST_HAND, 0, 0 );
        ScoreIt( p, 1, 0, 0, !SplitHand );
        if( SplitHand )
        {
            AddMsg( GM_SECOND_HAND, 0, 0 );
            ScoreIt( p2, 1, 0, 0, 1 );
        }
    }
}


/*** Playing ******************************************************************/

static void CheckHands(void)
{
    int d = ScoreDealer();
    int p = ScorePlayer();

    if( d == 21 && p == 21 )
        ScoreIt( p, 0, 1, 0, 1 );                       /* both: push        */
    else if( p == 21 )
        ScoreIt( p, 1, 0, 0, 1 );                       /* player wins       */
    else if( d == 21 && CardValue(Dealer[2]) != 1 )
        ScoreIt( d, 0, 0, 0, 1 );                       /* dealer wins       */
    else
    {
        CanHit = CanStay = CanDouble = 1;
        CanInsure = (CardValue(Dealer[2]) == 1);
        CanSplit  = (CardValue(Player[1]) == CardValue(Player[2]));
    }
}


void GameDeal(long bet)
{
    int i;

    NumMsgs    = 0;
    Bet        = bet;
    Bank      -= bet;
    NumDealer  = NumPlayer = 2;
    SplitHand  = DoubleHand = 0;
    HoleShown  = 0;
    InHand     = 1;

    for( i = 1; i <= 2; i++ )
    {
        Player[i] = DealCard();
        Dealer[i] = DealCard();
    }

    CheckHands();
}


void GameHit(void)
{
    int p;

    if( !InHand || !CanHit )
        return;

    NumMsgs = 0;
    CanDouble = CanInsure = CanSplit = 0;

    NumPlayer++;
    Player[NumPlayer] = DealCard();
    p = ScorePlayer();

    if( NumPlayer == MAX_DRAW_CARDS && p <= 21 )
        ScoreIt( 0, 1, 0, 1, 1 );                       /* 5 cards: player   */
    else if( p > 21 )
    {
        AddMsg( GM_PLAYER_OVER21, 0, 0 );
        ScoreIt( ScoreDealer(), 0, 0, 0, 1 );
    }
}


void GameStay(void)
{
    if( !InHand || !CanStay )
        return;

    NumMsgs = 0;
    FinishOffDealer();
}


void GameDouble(void)
{
    int p;

    if( !InHand || !CanDouble )
        return;

    NumMsgs = 0;

    if( Bank < Bet )
    {
        AddMsg( GM_NOT_ENOUGH_DOUBLE, 0, 0 );
        return;
    }

    Bank -= Bet;
    NumPlayer++;
    Player[NumPlayer] = DealCard();
    p = ScorePlayer();

    if( p > 21 )
    {
        AddMsg( GM_PLAYER_OVER21, 0, 0 );
        ScoreIt( ScoreDealer(), 0, 0, 0, 1 );
    }
    else
    {
        DoubleHand = 1;
        FinishOffDealer();
    }
}


void GameSplit(void)
{
    if( !InHand || !CanSplit )
        return;

    NumMsgs = 0;

    if( Bank < Bet )
    {
        AddMsg( GM_NOT_ENOUGH_SPLIT, 0, 0 );
        return;
    }

    Bank -= Bet;                                        /* the bet again     */
    Player[4] = Player[2];
    Player[2] = DealCard();
    Player[5] = DealCard();
    SplitHand = 1;
    FinishOffDealer();
}


void GameInsure(void)
{
    long cost;

    if( !InHand || !CanInsure )
        return;

    NumMsgs = 0;
    cost = (Bet + 2) / 4;                               /* 25%, rounded      */

    if( cost > Bank )
    {
        AddMsg( GM_NOT_ENOUGH_INS, (int)cost, 0 );
        return;
    }

    Bank -= cost;

    if( CardFace10(Dealer[1]) == 10 )
    {
        Bank += Bet;                                    /* bet back          */
        ScoreIt( 21, 0, 0, 0, 1 );
    }
    else
    {
        AddMsg( GM_NOT21, 0, 0 );
        CanInsure = 0;
    }
}


void GameForfeit(void)
{
    if( !InHand )
        return;

    NumMsgs = 0;
    HoleShown = 1;
    AddMsg( GM_FORFEIT, 0, 0 );
    EndHand();
}
