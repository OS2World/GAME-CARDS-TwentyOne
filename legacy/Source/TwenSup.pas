UNIT TwenSup;

(*                                                                      *)
(* AUTHOR: Michael G. Slack                    DATE WRITTEN: 2001/05/18 *)
(* ENVIRONMENT: Sibyl                                                   *)
(*                                                                      *)
(* Unit defines the global constants, variables, types and functions    *)
(* used by the twenty-one program.                                      *)
(*                                                                      *)
(* -------------------------------------------------------------------- *)
(*                                                                      *)
(* REVISED: 2001/05/31 - Removed 'CenterDialog' and 'DisplayMsg'        *)
(*                       procedures to see if the memory leak is taken  *)
(*                       care of.                                       *)
(*          2001/06/02 - Added back the 'CenterDialog' procedure (did   *)
(*                       not clear up memory leak and was useful).      *)
(*                                                                      *)

INTERFACE

 USES Forms, Graphics, ExtCtrls, Messages, QCardU;

 CONST wm_21Start = wm_User + 101;
       wm_Shuffle = wm_User + 102;
       {number constants}
       Min_Bet_Amt    = 1;
       Start_Max      = 5;
       Max_Bet_Amt    = 10000;
       Start_IBank    = 100;
       Max_IBank_Amt  = 100000;
       Max_Draw_Cards = 5; // if draw this number of cards, you win...
       {ini string constants (application/key)}
       Ini_Pos       : STRING[14] = 'WindowPosition';
       Ini_PTop      : STRING[3]  = 'Top';
       Ini_PLeft     : STRING[4]  = 'Left';
       Ini_Opt       : STRING[7]  = 'Options';
       Ini_OCPic     : STRING[11] = 'CardBackPic';
       Ini_ONDecks   : STRING[13] = 'NumberOfDecks';
       Ini_OMinBet   : STRING[10] = 'MinimumBet';
       Ini_OMaxBet   : STRING[10] = 'MaximumBet';
       Ini_OInitBank : STRING[11] = 'InitialBank';
       Ini_OBetMax   : STRING[12] = 'BetMaxAmount';
       {global options with default values}
       CardBackPic : LONGWORD = Min_CardBackNum;
       NumOfDecks  : INTEGER  = qcn_1Deck;
       MinimumBet  : LONGINT  = Min_Bet_Amt;
       MaximumBet  : LONGINT  = Start_Max;
       InitialBank : LONGINT  = Start_IBank;
       Bet_Max     : BOOLEAN  = FALSE;
       {message constants}
       mShuffling    : STRING[25] = 'Shuffling, please wait...';
       mMin          : STRING[11] = 'Minimum bet';
       mMax          : STRING[11] = 'Maximum bet';
       mInitBank     : STRING[12] = 'Initial bank';
       mMM_Error     : STRING[45] = ' must be between %d and %d (without decimals)';
       mNotEnough    : STRING[48] = 'You don''t have enough money for bet, reset game?';
       mPlayer       : STRING[6]  = 'Player';
       mDealer       : STRING[6]  = 'Dealer';
       mPushMsg      : STRING[30] = 'Push! Nobody wins this hand...';
       mWonMsg       : STRING[23] = 'The %s has won with %d.';
       mWonFive      : STRING[52] = '%s has drawn 5 cards without going over 21, %s wins.';
       mNotEnoughIns : STRING[52] = 'You don''t have enough money for insurance (need %d).';
       mNot21        : STRING[24] = 'Dealer does not have 21.';
       mNotEnoughSpt : STRING[37] = 'You don''t have enough money to split.';
       mNotEnoughDbl : STRING[48] = 'You don''t have enough money to double down with.';
       mPOver21      : STRING[19] = 'You''ve exceeded 21.';
       mDOver21      : STRING[23] = 'Dealer has exceeded 21.';
       mFirstHand    : STRING[28] = 'Scoring first hand of split.';
       mSecondHand   : STRING[29] = 'Scoring second hand of split.';

 TYPE THANDS = ARRAY[1..5] OF LONGWORD;

 VAR CardDeck : Q_CARDDECK;

(************************************************************************)

 PROCEDURE PutCardInImage(VAR Img : TIMAGE; Card : LONGWORD);
     (* procedure used to put a specific card into a timage instance *)

 PROCEDURE DoTheShuffle;
     (* procedure used to execute the card deck shuffle *)

 FUNCTION  GetMoneyDisplayString(Amount : LONGINT) : STRING;
     (* procedure to return an integer amount formatted to display as money *)

 PROCEDURE CenterDialog(Par : TFORM; VAR Dlg : TFORM);
     (* procedure used to center a dialog in a parent form *)
     (*  - Dlg must have Align set to alNone -             *)
     (*  - Assumes Dlg is smaller than parent -            *)

(************************************************************************)

IMPLEMENTATION

 USES SysUtils;

(************************************************************************)

 PROCEDURE PutCardInImage(VAR Img : TIMAGE; Card : LONGWORD);
     (* procedure used to put a specific card into a timage object *)

    VAR T : TBITMAP;

  BEGIN (*putcardinimage*)
   // get card image from DLL/Unit (based on what kind it is)
   IF (Card = Red_Joker) OR (Card = Black_Joker)
    THEN T := QC_GetJoker(Card)
   ELSE IF (Card >= Min_SymbolNum) AND (Card <= Max_SymbolNum)
         THEN T := QC_GetCardSymbol(Card)
        ELSE IF (Card >= Min_CardBackNum) AND (Card <= Max_CardBackNum)
              THEN T := QC_GetCardBack(Card)
             ELSE T := QC_GetCard(Card);
   Img.Bitmap := T; // assign to image
   T.Free; // free reference returned from DLL/unit
  END; (*putcardinimage*)

(************************************************************************)

 PROCEDURE DoTheShuffle;
     (* procedure used to execute the card deck shuffle *)

  BEGIN (*dotheshuffle*)
   QC_ShuffleDeck(CardDeck, NumOfDecks);
  END; (*dotheshuffle*)

(************************************************************************)

 FUNCTION GetMoneyDisplayString(Amount : LONGINT) : STRING;
     (* procedure to return an integer amount formatted to display as money *)

  BEGIN (*getmoneydiplaystring*)
   Result := CurrencyString + IntToStr(Amount) + DecimalSeparator + '00';
  END; (*getmoneydiplaystring*)

(************************************************************************)

 PROCEDURE CenterDialog(Par : TFORM; VAR Dlg : TFORM);
     (* procedure used to center a dialog in a parent form *)

  BEGIN (*centerdialog*)
   Dlg.Top := Par.Top + ((Par.Height - Dlg.Height) DIV 2);
   Dlg.Left := Par.Left + ((Par.Width - Dlg.Width) DIV 2);
  END; (*centerdialog*)

(************************************************************************)

INITIALIZATION
END. (*of unit*)
