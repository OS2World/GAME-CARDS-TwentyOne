UNIT TwenWin;

(*                                                                      *)
(* AUTHOR: Michael G. Slack                    DATE WRITTEN: 2001/05/18 *)
(* ENVIRONMENT: Sibyl                                                   *)
(*                                                                      *)
(* Unit defines the main window used by the twenty-one program.         *)
(* Simple Rules:                                                        *)
(*  - Dealer hits on 16 or less, stays on 17+                           *)
(*  - Splits will play with one card on each one.                       *)
(*  - Double can only be done at the start and will hit one card.       *)
(*  - Insurance can be bought when the dealer has an ace showing.       *)
(*    (costs 25% of bet (1 dollar bets cost nothing))                   *)
(*  - If player or dealer hit to 5 cards without going over 21, they    *)
(*    are the automatic winner.                                         *)
(*  - If dealer or player has twentyone to start, they are the winner   *)
(*    (unless insurance can be bought or both have 21).                 *)
(*  - Closest to 21 without going over is the winner.                   *)
(*                                                                      *)
(* -------------------------------------------------------------------- *)
(*                                                                      *)
(* REVISED: 2001/05/28 - Fixed a bug in the 'double down' function (was *)
(*                       not checking to see if player got over 21      *)
(*                       after doubling down).                          *)
(*          2001/06/02 - Moved the display of the players bank to after *)
(*                       the 'win' or 'lose' message so that the bet    *)
(*                       winnings would appear after the message.       *)
(*                                                                      *)

INTERFACE

 USES Classes, Forms, Graphics, ExtCtrls, StdCtrls, Buttons, Menus, Messages,
      TwenSup;

 TYPE TTwenO_Win = CLASS(TFORM)
                    CardBackImg    : TImage;
                    Label1         : TLabel;
                    Label2         : TLabel;
                    Lbl_CardsLeft  : TLabel;
                    Label3         : TLabel;
                    Label4         : TLabel;
                    Lbl_PlayerBank : TLabel;
                    Label5         : TLabel;
                    Lbl_Bet        : TLabel;
                    ExitBtn        : TBitBtn;
                    PlayBtn        : TBitBtn;
                    HitBtn         : TBitBtn;
                    StayBtn        : TBitBtn;
                    DoubleBtn      : TBitBtn;
                    SplitBtn       : TBitBtn;
                    InsBtn         : TBitBtn;
                    D_Card1        : TImage;
                    D_Card2        : TImage;
                    D_Card3        : TImage;
                    D_Card4        : TImage;
                    D_Card5        : TImage;
                    P_Card1        : TImage;
                    P_Card2        : TImage;
                    P_Card3        : TImage;
                    P_Card4        : TImage;
                    P_Card5        : TImage;
                    PopupMenu1     : TPopupMenu;
                    MI_Options     : TMenuItem;
                    MI_About       : TMenuItem;
                    Procedure TwenO_WinOnCreate(Sender : TObject);
                    Procedure TwenO_WinOnShow(Sender : TObject);
                    Procedure TwenO_WinOnDestroy(Sender : TObject);
                    Procedure PlayBtnOnClick(Sender : TObject);
                    Procedure HitBtnOnClick(Sender : TObject);
                    Procedure StayBtnOnClick(Sender : TObject);
                    Procedure DoubleBtnOnClick(Sender : TObject);
                    Procedure SplitBtnOnClick(Sender : TObject);
                    Procedure InsBtnOnClick(Sender : TObject);
                    Procedure MI_AboutOnClick(Sender : TObject);
                    Procedure MI_OptionsOnClick(Sender : TObject);
                   PRIVATE
                    OldTop, OldLeft,            // saved window position
                    NumCards,
                    NumC_D, NumC_P   : INTEGER;
                    Ini_Fn           : STRING[160];
                    D_Cards          : ARRAY[1..Max_Draw_Cards] OF TIMAGE;
                    P_Cards          : ARRAY[1..Max_Draw_Cards] OF TIMAGE;
                    Bet, AmountWon   : LONGINT;
                    DealerC, PlayerC : THANDS;
                    DownShowed,
                    DoubleHand,
                    SplitHand        : BOOLEAN;
                   PROTECTED
                    PROCEDURE WM21Start(VAR Msg : TMESSAGE);
                     MESSAGE wm_21Start;
                   PUBLIC
                    PROCEDURE Read_INI;
                    PROCEDURE Write_INI;
                    PROCEDURE ShuffleCards(InDlg : BOOLEAN);
                    PROCEDURE DisplayPlayersBank;
                    FUNCTION  GetBet : TMSGDLGRETURN;
                    FUNCTION  DealCard : LONGWORD;
                    PROCEDURE StartHand;
                    FUNCTION  ScoreOfHand(Hnd : THANDS; NumC : INTEGER) : INTEGER;
                    PROCEDURE CheckHands;
                    PROCEDURE ScoreIt(Pts : INTEGER;
                                      Player, Push, FiveC, ResetG : BOOLEAN);
                    PROCEDURE FinishOffDealer;
                   END;

 VAR TwenO_Win : TTwenO_Win;

(************************************************************************)

IMPLEMENTATION

 USES SysUtils, IniFiles, Dialogs, QCardU, TwenSDlg, TwenAbt, TwenBet, TwenOpts;

(************************************************************************)

 Procedure TTwenO_Win.TwenO_WinOnCreate(Sender : TObject);
  Begin
   AmountWon := InitialBank; Bet := 0;
  End;

(************************************************************************)

 Procedure TTwenO_Win.TwenO_WinOnShow(Sender : TObject);
  Begin
   Top := (Screen.Height DIV 2) - (Height DIV 2); {default position}
   Left := (Screen.Width DIV 2) - (Width DIV 2);
   {read and setup options}
   Ini_Fn := GetDefaultINI;
   Read_INI;
   OldTop := Top; OldLeft := Left; {save old window position}
   {assign timages to arrays}
   D_Cards[1] := D_Card1;
   D_Cards[2] := D_Card2;
   D_Cards[3] := D_Card3;
   D_Cards[4] := D_Card4;
   D_Cards[5] := D_Card5;
   P_Cards[1] := P_Card1;
   P_Cards[2] := P_Card2;
   P_Cards[3] := P_Card3;
   P_Cards[4] := P_Card4;
   P_Cards[5] := P_Card5;
   {do the final setup now}
   PostMsg(Handle,wm_21Start,0,0);
  End;

(************************************************************************)

 Procedure TTwenO_Win.TwenO_WinOnDestroy(Sender : TObject);

    VAR Ini : TINIFILE;

  Begin
   {check if position needs to be saved or not}
   IF (Top <> OldTop) OR (Left <> OldLeft)
    THEN BEGIN {only save if changed}
          Ini := TINIFILE.Create(Ini_Fn);
          Ini.WriteInteger(Ini_Pos,Ini_PTop,Top);
          Ini.WriteInteger(Ini_Pos,Ini_PLeft,Left);
          Ini.Free;
         END; {then}
  End;

(************************************************************************)

 Procedure TTwenO_Win.PlayBtnOnClick(Sender : TObject);

    VAR Ret : TMSGDLGRETURN;

  Begin
   Ret := GetBet;
   IF Ret = mrOK
    THEN BEGIN {play through}
          Lbl_Bet.Caption := GetMoneyDisplayString(Bet);
          AmountWon := AmountWon - Bet;
          DisplayPlayersBank;
          StartHand;
          CheckHands;
         END; {then}
  End;

(************************************************************************)

 Procedure TTwenO_Win.HitBtnOnClick(Sender : TObject);
    VAR P : INTEGER;
  Begin
   // disable a few buttons now (no longer have some choices...)
   DoubleBtn.Enabled := FALSE;
   InsBtn.Enabled := FALSE;
   SplitBtn.Enabled := FALSE;
   // draw a card
   Inc(NumC_P); PlayerC[NumC_P] := DealCard;
   PutCardInImage(P_Cards[NumC_P], PlayerC[NumC_P]);
   P := ScoreOfHand(PlayerC, NumC_P);
   IF (NumC_P = Max_Draw_Cards) AND (P <= 21)
    THEN ScoreIt(0, TRUE, FALSE, TRUE, TRUE)
   ELSE BEGIN {check to see if over 21...}
         IF P > 21
          THEN BEGIN {whoops}
                MessageBox(mPOver21, mtInformation, [mbOK]);
                ScoreIt(ScoreOfHand(DealerC, NumC_D), FALSE, FALSE, FALSE, TRUE);
               END; {then}
        END; {else}
  End;

(************************************************************************)

 Procedure TTwenO_Win.StayBtnOnClick(Sender : TObject);
  Begin
   FinishOffDealer;
  End;

(************************************************************************)

 Procedure TTwenO_Win.DoubleBtnOnClick(Sender : TObject);
    VAR P : INTEGER;
  Begin
   IF AmountWon < Bet
    THEN MessageBox(mNotEnoughDbl, mtInformation, [mbOK])
   ELSE BEGIN {do double down}
         AmountWon := AmountWon - Bet;
         DisplayPlayersBank; Application.ProcessMessages;
         PlayerC[3] := DealCard; Inc(NumC_P);
         PutCardInImage(P_Cards[3], PlayerC[3]);
         P := ScoreOfHand(PlayerC, NumC_P);
         IF P > 21
          THEN BEGIN {whoops}
                MessageBox(mPOver21, mtInformation, [mbOK]);
                ScoreIt(ScoreOfHand(DealerC, NumC_D), FALSE, FALSE, FALSE, TRUE);
               END {then}
         ELSE BEGIN {ok - finish the dealer off}
               DoubleHand := TRUE;
               FinishOffDealer;
              END; {else}
        END; {else}
  End;

(************************************************************************)

 Procedure TTwenO_Win.SplitBtnOnClick(Sender : TObject);
  Begin
   IF AmountWon < Bet
    THEN MessageBox(mNotEnoughSpt, mtInformation, [mbOK])
   ELSE BEGIN {do it then}
         AmountWon := AmountWon - Bet; // subtract off bet again
         DisplayPlayersBank; Application.ProcessMessages;
         PlayerC[4] := PlayerC[2];
         PlayerC[2] := DealCard;
         PlayerC[5] := DealCard;
         PutCardInImage(P_Cards[2], PlayerC[2]);
         PutCardInImage(P_Cards[4], PlayerC[4]);
         PutCardInImage(P_Cards[5], PlayerC[5]);
         SplitHand := TRUE;
         FinishOffDealer;
        END; {else}
  End;

(************************************************************************)

 Procedure TTwenO_Win.InsBtnOnClick(Sender : TObject);
    VAR InsCost : LONGINT;
  Begin
   InsCost := Round(Bet * 0.25);
   IF InsCost > AmountWon
    THEN MessageBox(Format(mNotEnoughIns, [InsCost]), mtInformation, [mbOK])
   ELSE BEGIN {pay for it, then check it}
         AmountWon := AmountWon - InsCost;
         DisplayPlayersBank; Application.ProcessMessages;
         IF QC_GetCV_Face10(DealerC[1]) = 10
          THEN BEGIN {good call - dealer has 21}
                AmountWon := AmountWon + Bet; // give the player his bet back
                ScoreIt(21, FALSE, FALSE, FALSE, TRUE);
               END {then}
         ELSE BEGIN {nope}
               MessageBox(mNot21, mtInformation, [mbOK]);
               StayBtn.Focus;
               InsBtn.Enabled := FALSE;
              END; {else}
        END; {else}
  End;

(************************************************************************)

 Procedure TTwenO_Win.MI_AboutOnClick(Sender : TObject);
  Begin
   AboutDlg := TABOUTDLG.Create(Self);
   CenterDialog(Self, AboutDlg);
   AboutDlg.ShowModal;
   AboutDlg.Free;
  End;

(************************************************************************)

 Procedure TTwenO_Win.MI_OptionsOnClick(Sender : TObject);

    VAR Ret   : TMSGDLGRETURN;
        OldCB : LONGWORD;

  Begin
   OldCB := CardBackPic; // save this
   ConfDlg := TCONFDLG.Create(Self);
   CenterDialog(Self, ConfDlg);
   Ret := ConfDlg.ShowModal;
   ConfDlg.Free;
   IF (Ret = mrOK)
    THEN BEGIN {save settings}
          Write_INI;
          IF OldCB <> CardBackPic
           THEN PutCardInImage(CardBackImg, CardBackPic);
         END; {then}
  End;

(************************************************************************)

 PROCEDURE TTwenO_Win.WM21Start(VAR Msg : TMESSAGE);
     (* procedure used to display the card back image at program startup *)

  BEGIN (*ttweno_win.wm21start*)
   ShuffleCards(FALSE); // setup initial deck(s) to use
   DisplayPlayersBank;
   // this is needed when using the DLL to store cards (timing issue I think)
   PutCardInImage(CardBackImg, CardBackPic);
  END; (*ttweno_win.wm21start*)

(************************************************************************)

 PROCEDURE TTwenO_Win.Read_INI;
     (* procedure used to read the ini file and pull program options out *)

    VAR Ini : TINIFILE;

  BEGIN (*ttweno_win.read_ini*)
   IF NOT(FileExists(Ini_Fn)) THEN Exit; // no ini to check...
   Ini := TINIFILE.Create(Ini_Fn);
   {read in window position (just top and left needed)}
   Top := Ini.ReadInteger(Ini_Pos, Ini_PTop, Top);
   Left := Ini.ReadInteger(Ini_Pos, Ini_PLeft, Left);
   {read in options now}
   CardBackPic := Ini.ReadInteger(Ini_Opt, Ini_OCPic, CardBackPic);
   IF (CardBackPic < Min_CardBackNum) OR (CardBackPic > Max_CardBackNum)
    THEN CardBackPic := Min_CardBackNum;
   NumOfDecks := Ini.ReadInteger(Ini_Opt, Ini_ONDecks, NumOfDecks);
   IF (NumOfDecks < qcn_1Deck) OR (NumOfDecks > qcn_3Deck)
    THEN NumOfDecks := qcn_1Deck;
   MinimumBet := Ini.ReadInteger(Ini_Opt, Ini_OMinBet, MinimumBet);
   IF (MinimumBet < Min_Bet_Amt) OR (MinimumBet > Max_Bet_Amt)
    THEN MinimumBet := Min_Bet_Amt;
   MaximumBet := Ini.ReadInteger(Ini_Opt, Ini_OMaxBet, MaximumBet);
   IF (MaximumBet < MinimumBet) OR (MaximumBet > Max_Bet_Amt)
    THEN MaximumBet := MinimumBet;
   InitialBank := Ini.ReadInteger(Ini_Opt, Ini_OInitBank, InitialBank);
   IF (InitialBank < MinimumBet) OR (InitialBank > Max_IBank_Amt)
    THEN InitialBank := MinimumBet;
   IF (InitialBank < (MinimumBet * 2)) THEN InitialBank := MinimumBet * 2;
   AmountWon := InitialBank;
   Bet_Max := Ini.ReadBool(Ini_Opt, Ini_OBetMax, Bet_Max);
   {close it}
   Ini.Free;
  END; (*ttweno_win.read_ini*)

(************************************************************************)

 PROCEDURE TTwenO_Win.Write_INI;
     (* procedure to write out to the ini file the options *)

    VAR Ini : TINIFILE;

  BEGIN (*ttweno_win.writeini*)
   Ini := TINIFILE.Create(Ini_Fn);
   {write out options}
   Ini.WriteInteger(Ini_Opt, Ini_OCPic, CardBackPic);
   Ini.WriteInteger(Ini_Opt, Ini_ONDecks, NumOfDecks);
   Ini.WriteInteger(Ini_Opt, Ini_OMinBet, MinimumBet);
   Ini.WriteInteger(Ini_Opt, Ini_OMaxBet, MaximumBet);
   Ini.WriteInteger(Ini_Opt, Ini_OInitBank, InitialBank);
   Ini.WriteBool(Ini_Opt, Ini_OBetMax, Bet_Max);
   {close it down}
   Ini.Free;
  END; (*ttweno_win.writeini*)

(************************************************************************)

 PROCEDURE TTwenO_Win.ShuffleCards(InDlg : BOOLEAN);
     (* procedure used to execute the shuffle card routine *)

  BEGIN (*ttweno_win.shufflecards*)
   IF InDlg
    THEN BEGIN {execute dialog to shuffle cards}
          ShuffleDlg := TSHUFFLEDLG.Create(Self);
          CenterDialog(Self, ShuffleDlg);
          ShuffleDlg.ShowModal;
          ShuffleDlg.Free;
         END {then}
   ELSE DoTheShuffle;
   NumCards := QC_GetNumCardsLeft(CardDeck);
   Lbl_CardsLeft.Caption := IntToStr(NumCards);
  END; (*ttweno_win.shufflecards*)

(************************************************************************)

 PROCEDURE TTwenO_Win.DisplayPlayersBank;
     (* procedure used to show the amount in the players bank *)

  BEGIN (*ttweno_win.displayplayersbank*)
   Lbl_PlayerBank.Caption := GetMoneyDisplayString(AmountWon);
  END; (*ttweno_win.displayplayersbank*)

(************************************************************************)

 FUNCTION TTwenO_Win.GetBet : TMSGDLGRETURN;
     (* function used to get the bet from the user for the hand *)

    VAR Ret : TMSGDLGRETURN;

  BEGIN (*TTwenO_Win.getbet*)
   Result := mrCancel;
   IF AmountWon < MinimumBet
    THEN BEGIN {reset??}
          Ret := MessageBox(mNotEnough,mtConfirmation,[mbYes,mbNo]);
          IF Ret = mrYes
           THEN BEGIN {reset values and go on}
                 AmountWon := AmountWon + InitialBank;
                 DisplayPlayersBank;
                END {then}
          ELSE Exit;
         END; {then}
   BetDlg := TBETDLG.Create(Self);
   CenterDialog(Self, BetDlg);
   BetDlg.Bank := AmountWon;
   Ret := BetDlg.ShowModal;
   IF Ret = mrOK THEN Bet := BetDlg.Bet;
   BetDlg.Free;
   Result := Ret;
  END; (*TTwenO_Win.getbet*)

(************************************************************************)

 FUNCTION TTwenO_Win.DealCard : LONGWORD;
     (* function used to deal a card to the player or dealer *)

  BEGIN (*ttweno_win.dealcard*)
   IF NumCards <= 0 THEN ShuffleCards(TRUE);
   Dec(NumCards); Lbl_CardsLeft.Caption := IntToStr(NumCards);
   Result := QC_GetNextCard(CardDeck);
  END; (*ttweno_win.dealcard*)

(************************************************************************)

 PROCEDURE TTwenO_Win.StartHand;
     (* procedure used to start off the hand *)

    VAR I : INTEGER;

  BEGIN (*ttweno_win.starthand*)
   NumC_D := 2; NumC_P := 2;
   SplitHand := FALSE; DoubleHand := FALSE; DownShowed := FALSE;
   FOR I := 1 TO 2 DO
    BEGIN {deal...}
     PlayerC[I] := DealCard;
     PutCardInImage(P_Cards[I], PlayerC[I]);
     DealerC[I] := DealCard;
     IF I = 1
      THEN PutCardInImage(D_Cards[I], CardBackPic)
     ELSE PutCardInImage(D_Cards[I], DealerC[I]);
    END; {i for}
  END; (*ttweno_win.starthand*)

(************************************************************************)

 FUNCTION TTwenO_Win.ScoreOfHand(Hnd : THANDS; NumC : INTEGER) : INTEGER;
     (* function to score hand and pass back value *)

    VAR I, C, T : INTEGER;

  BEGIN (*ttweno_win.scoreofhand*)
   T := 0;
   FOR I := 1 TO NumC DO
    BEGIN {get value of each card}
     C := QC_GetCV_Face10(Hnd[I]);
     T := T + C;
    END; {i for}
   FOR I := 1 TO NumC DO
    BEGIN {check for aces - add 10 if one found and less than/equal to 21}
     C := QC_GetCardValue(Hnd[I]);
     IF C = 1
      THEN BEGIN {maybe add 10 and leave (won't have two aces be 11)}
            IF (T + 10) <= 21 THEN T := T + 10;
            Break;
           END; {then}
    END; {i for}
   Result := T;
  END; (*ttweno_win.scoreofhand*)

(************************************************************************)

 PROCEDURE TTwenO_Win.CheckHands;
     (* procedure to do initial check of hands and setup for rest of it *)

    VAR D, P : INTEGER;

  BEGIN (*ttweno_win.checkhands*)
   D := ScoreOfHand(DealerC, NumC_D);
   P := ScoreOfHand(PlayerC, NumC_P);
   IF (D = 21) AND (P = 21)
    THEN BEGIN {push - both have 21}
          ScoreIt(P, FALSE, TRUE, FALSE, TRUE);
         END {then}
   ELSE IF P = 21
         THEN BEGIN {player wins}
               ScoreIt(P, TRUE, FALSE, FALSE, TRUE);
              END {then}
        ELSE IF (D = 21) AND (QC_GetCardValue(DealerC[2]) <> 1)
              THEN BEGIN {dealer wins (if his up card is not an ace)}
                    ScoreIt(D, FALSE, FALSE, FALSE, TRUE);
                   END {then}
             ELSE BEGIN {finish initializing this game}
                   PlayBtn.Enabled := FALSE;
                   HitBtn.Enabled := TRUE;
                   StayBtn.Enabled := TRUE;
                   DoubleBtn.Enabled := TRUE;
                   IF QC_GetCardValue(DealerC[2]) = 1
                    THEN InsBtn.Enabled := TRUE;
                   IF QC_GetCardValue(PlayerC[1]) = QC_GetCardValue(PlayerC[2])
                    THEN SplitBtn.Enabled := TRUE;
                   StayBtn.Focus;
                  END; {else}
  END; (*ttweno_win.checkhands*)

(************************************************************************)

 PROCEDURE TTwenO_Win.ScoreIt(Pts : INTEGER; Player, Push, FiveC, ResetG : BOOLEAN);
     (* procedure to do final tally and reset game for next hand *)

    VAR I : INTEGER;
        W : STRING[20];
        S : STRING[200];

  BEGIN (*ttweno_win.scoreit*)
   IF NOT(DownShowed)
    THEN PutCardInImage(D_Cards[1], DealerC[1]); // show down card (if not already)
   DownShowed := TRUE;

   // create message and display
   IF Push
    THEN BEGIN {push - no one won...}
          S := mPushMsg;
          AmountWon := AmountWon + Bet;
          IF DoubleHand THEN AmountWon := AmountWon + Bet;
         END {then}
   ELSE BEGIN {somebody won...}
         IF Player
          THEN BEGIN {player did}
                AmountWon := AmountWon + Bet + Bet; // add back bet plus winnings
                IF DoubleHand
                 THEN AmountWon := AmountWon + Bet + Bet; // double it
                W := mPlayer;
               END {then}
         ELSE W := mDealer;
         IF FiveC
          THEN S := Format(mWonFive, [W, W])
         ELSE S := Format(mWonMsg, [W, Pts]);
        END;
   MessageBox(S, mtInformation, [mbOK]);
   DisplayPlayersBank; // show amount now

   IF ResetG
    THEN BEGIN {reset game window to start}
          // reset buttons
          DoubleBtn.Enabled := FALSE;
          InsBtn.Enabled := FALSE;
          SplitBtn.Enabled := FALSE;
          HitBtn.Enabled := FALSE;
          StayBtn.Enabled := FALSE;
          PlayBtn.Enabled := TRUE;
          PlayBtn.Focus;

          // reset card images now
          FOR I := 1 TO Max_Draw_Cards DO
           BEGIN {reset images}
            PutCardInImage(P_Cards[I], 0);
            PutCardInImage(D_Cards[I], 0);
           END; {i for}

          // reset bet display
          Lbl_Bet.Caption := GetMoneyDisplayString(0);
         END; {then}
  END; (*ttweno_win.scoreit*)

(************************************************************************)

 PROCEDURE TTwenO_Win.FinishOffDealer;
     (* procedure to finish out the dealers hand *)

    VAR D, P, P2 : INTEGER;

  BEGIN (*ttweno_win.finishoffdealer*)
   PutCardInImage(D_Cards[1], DealerC[1]); // show down card
   DownShowed := TRUE;
   D := ScoreOfHand(DealerC, NumC_D);
   P := ScoreOfHand(PlayerC, NumC_P); // score first hand if split
   IF SplitHand
    THEN BEGIN {score second hand if split}
          PlayerC[1] := PlayerC[4];
          PlayerC[2] := PlayerC[5];
          P2 := ScoreOfHand(PlayerC, NumC_P);
         END; {then}
   WHILE D <= 16 DO
    BEGIN {draw cards}
     Inc(NumC_D); DealerC[NumC_D] := DealCard;
     PutCardInImage(D_Cards[NumC_D], DealerC[NumC_D]);
     D := ScoreOfHand(DealerC, NumC_D);
    END; {end}
   IF D <= 21
    THEN BEGIN {figure out who won}
          IF NumC_D = Max_Draw_Cards
           THEN ScoreIt(0, FALSE, FALSE, TRUE, TRUE) // dealer did
          ELSE BEGIN {check points}
                IF SplitHand THEN MessageBox(mFirstHand, mtInformation, [mbOK]);
                IF D = P
                 THEN ScoreIt(P, FALSE, TRUE, FALSE, NOT(SplitHand)) // push
                ELSE IF D < P
                      THEN ScoreIt(P, TRUE, FALSE, FALSE, NOT(SplitHand))
                     ELSE ScoreIt(D, FALSE, FALSE, FALSE, NOT(SplitHand));
                IF SplitHand
                 THEN BEGIN {work up score of second hand}
                       MessageBox(mSecondHand, mtInformation, [mbOK]);
                       IF D = P2
                        THEN ScoreIt(P, FALSE, TRUE, FALSE, TRUE) // push
                       ELSE IF D < P2
                             THEN ScoreIt(P2, TRUE, FALSE, FALSE, TRUE)
                            ELSE ScoreIt(D, FALSE, FALSE, FALSE, TRUE);
                      END;
               END; {else}
         END {then}
   ELSE BEGIN {player won}
         MessageBox(mDOver21, mtInformation, [mbOK]);
         IF SplitHand THEN MessageBox(mFirstHand, mtInformation, [mbOK]);
         ScoreIt(P, TRUE, FALSE, FALSE, NOT(SplitHand));
         IF SplitHand
          THEN BEGIN {do second}
                MessageBox(mSecondHand, mtInformation, [mbOK]);
                ScoreIt(P2, TRUE, FALSE, FALSE, TRUE);
               END; {then}
        END; {else}
  END; (*ttweno_win.finishoffdealer*)

(************************************************************************)

INITIALIZATION
  RegisterClasses([TTwenO_Win, TImage, TLabel, TPopupMenu,
    TMenuItem, TBitBtn]);
END. (*of unit*)
