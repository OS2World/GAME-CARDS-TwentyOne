UNIT TwenOpts;

(*                                                                      *)
(* AUTHOR: Michael G. Slack                    DATE WRITTEN: 2001/05/20 *)
(* ENVIRONMENT: Sibyl                                                   *)
(*                                                                      *)
(* Unit defines the options dialog used by the twenty-one program.      *)
(*                                                                      *)
(* -------------------------------------------------------------------- *)
(*                                                                      *)
(* REVISED: yyyy/mm/dd -                                                *)
(*                                                                      *)

INTERFACE

 USES Classes, Forms, Graphics, TabCtrls, Buttons, ExtCtrls, StdCtrls, BmpList;

 TYPE TConfDlg = CLASS(TFORM)
                  TN_Opts       : TTabbedNotebook;
                  RG_Decks      : TRadioGroup;
                  Label1        : TLabel;
                  Label2        : TLabel;
                  E_MinBet      : TEdit;
                  Label3        : TLabel;
                  E_MaxBet      : TEdit;
                  Label4        : TLabel;
                  E_Bank        : TEdit;
                  Label5        : TLabel;
                  CB_BetMax     : TCheckBox;
                  BLB_CardBacks : TBitmapListBox;
                  OkButton      : TBitBtn;
                  CancelButton  : TBitBtn;
                  Procedure ConfDlgOnShow(Sender : TObject);
                  Procedure TN_OptsOnSetupShow(Sender : TObject);
                  Procedure OkButtonOnClick(Sender : TObject);
                 PRIVATE
                  {Insert private declarations here}
                 PUBLIC
                  {Insert public declarations here}
                 END;

 VAR ConfDlg : TConfDlg;

(************************************************************************)

IMPLEMENTATION

 USES SysUtils, Dialogs, QCardU, TwenSup;

(************************************************************************)

 Procedure TConfDlg.ConfDlgOnShow(Sender : TObject);
  Begin
   // set values into the fields in the dialog (except list box)
   RG_Decks.ItemIndex := NumOfDecks - 1;
   E_MinBet.Text := IntToStr(MinimumBet);
   E_MaxBet.Text := IntToStr(MaximumBet);
   E_Bank.Text := IntToStr(InitialBank);
   CB_BetMax.Checked := Bet_Max;
  End;

(************************************************************************)

 Procedure TConfDlg.TN_OptsOnSetupShow(Sender : TObject);

    VAR I      : LONGWORD;
        Sel, J : LONGINT;
        T      : TBITMAP;

  Begin
   Sel := 0; J := 0;
   FOR I := Min_CardBackNum TO Max_CardBackNum DO
    BEGIN {add bitmaps to list control}
     T := QC_GetCardBack(I);
     IF T <> NIL
      THEN BEGIN {apply it to list box}
            J := BLB_CardBacks.AddBitmap(IntToStr(I), T);
            IF I = CardBackPic THEN Sel := J;
            T.Free;
           END; {then}
    END; {i for}
   BLB_CardBacks.ItemIndex := Sel;
  End;

(************************************************************************)

 Procedure TConfDlg.OkButtonOnClick(Sender : TObject);
    VAR Mn, Mx, Bnk : LONGINT;
  Begin
   TRY {to get min bet}
    Mn := StrToInt(E_MinBet.Text);
   EXCEPT
    Mn := 0;
   END;
   TRY {to get max bet}
    Mx := StrToInt(E_MaxBet.Text);
   EXCEPT
    Mx := 0;
   END;
   TRY {to get init bank}
    Bnk := StrToInt(E_Bank.Text);
   EXCEPT
    Bnk := 0;
   END;
   // check values
   IF (Mn < Min_Bet_Amt) OR (Mn > Max_Bet_Amt)
    THEN BEGIN {minimum value out of range}
          MessageBox(Format(mMin+mMM_Error, [Min_Bet_Amt, Max_Bet_Amt]),
                     mtInformation, [mbOK]);
          E_MinBet.Focus;
          Exit;
         END; {then}
   IF (Mx < Mn) OR (Mx > Max_Bet_Amt)
    THEN BEGIN {maximum value out of range}
          MessageBox(Format(mMax+mMM_Error, [Mn, Max_Bet_Amt]),
                     mtInformation, [mbOK]);
          E_MaxBet.Focus;
          Exit;
         END; {then}
   IF (Bnk < Mn) OR (Bnk > Max_IBank_Amt)
    THEN BEGIN {initial bank value out of range}
          MessageBox(Format(mInitBank+mMM_Error, [Mn, Max_IBank_Amt]),
                     mtInformation, [mbOK]);
          E_Bank.Focus;
          Exit;
         END; {then}
   {save values now}
   NumOfDecks := RG_Decks.ItemIndex + 1;
   IF (NumOfDecks < qcn_1Deck) OR (NumOfDecks > qcn_3Deck)
    THEN NumOfDecks := qcn_1Deck;
   MinimumBet := Mn;
   MaximumBet := Mx;
   InitialBank := Bnk;
   CardBackPic := StrToInt(BLB_CardBacks.Items[BLB_CardBacks.ItemIndex]);
   Bet_Max := CB_BetMax.Checked;
   DismissDlg(mrOK);
  End;

(************************************************************************)

INITIALIZATION
  RegisterClasses([TConfDlg, TTabbedNotebook, TBitBtn, TRadioGroup, TLabel,
   TEdit, TBitmapListBox, TCheckBox]);
END. (*of unit*)
