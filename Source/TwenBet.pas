UNIT TwenBet;

(*                                                                      *)
(* AUTHOR: Michael G. Slack                    DATE WRITTEN: 2001/05/19 *)
(* ENVIRONMENT: Sibyl                                                   *)
(*                                                                      *)
(* Unit defines the dialog used to get the bet for the player for the   *)
(* game of twenty-one.                                                  *)
(*                                                                      *)
(* -------------------------------------------------------------------- *)
(*                                                                      *)
(* REVISED: 2001/05/27 - Fixed a problem with player being able to bet  *)
(*                       more than they had (went to -bank).            *)
(*                                                                      *)

INTERFACE

 USES Classes, Forms, Graphics, Buttons, ExtCtrls, StdCtrls, Spin;

 TYPE TBetDlg = CLASS(TFORM)
                 Bevel1       : TBevel;
                 Label1       : TLabel;
                 SE_Bet       : TSpinEdit;
                 Label2       : TLabel;
                 Lbl_Bank     : TLabel;
                 Label3       : TLabel;
                 Lbl_MinBet   : TLabel;
                 Label4       : TLabel;
                 Lbl_MaxBet   : TLabel;
                 OkButton     : TBitBtn;
                 CancelButton : TBitBtn;
                 Procedure BetDlgOnShow(Sender : TObject);
                 Procedure OkButtonOnClick(Sender : TObject);
                PRIVATE
                 {Insert private declarations here}
                PUBLIC
                 Bank, Bet : LONGINT;
                END;

 VAR BetDlg : TBetDlg;

(************************************************************************)

IMPLEMENTATION

 USES SysUtils, TwenSup;

(************************************************************************)

 Procedure TBetDlg.BetDlgOnShow(Sender : TObject);
  Begin
   IF MaximumBet > Bank
    THEN SE_Bet.MaxValue := Bank
   ELSE SE_Bet.MaxValue := MaximumBet;
   SE_Bet.MinValue := MinimumBet;
   IF Bet_Max
    THEN SE_Bet.Value := SE_Bet.MaxValue
   ELSE SE_Bet.Value := MinimumBet;
   Lbl_Bank.Caption := GetMoneyDisplayString(Bank);
   Lbl_MinBet.Caption := GetMoneyDisplayString(MinimumBet);
   Lbl_MaxBet.Caption := GetMoneyDisplayString(MaximumBet);
  End;

(************************************************************************)

 Procedure TBetDlg.OkButtonOnClick(Sender : TObject);
  Begin
   Bet := SE_Bet.Value;
   DismissDlg(mrOK);
  End;

(************************************************************************)

INITIALIZATION
  RegisterClasses([TBetDlg, TBitBtn, TBevel, TLabel, TSpinEdit]);
END. (*of unit*)
