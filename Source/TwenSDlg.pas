UNIT TwenSDlg;

(*                                                                      *)
(* AUTHOR: Michael G. Slack                    DATE WRITTEN: 2001/05/19 *)
(* ENVIRONMENT: Sibyl                                                   *)
(*                                                                      *)
(* Unit defines the dialog used to shuffle the deck(s) when needed.     *)
(*                                                                      *)
(* -------------------------------------------------------------------- *)
(*                                                                      *)
(* REVISED: yyyy/mm/dd -                                                *)
(*                                                                      *)

INTERFACE

 USES Classes, Forms, Graphics, Buttons, ExtCtrls, StdCtrls, Messages, TwenSup;

 TYPE TShuffleDlg = CLASS(TFORM)
                     Bevel1: TBevel;
                     Label1: TLabel;
                     PauseTimer : TTimer;
                     Procedure ShuffleDlgOnShow(Sender : TObject);
                     Procedure PauseTimerOnTimer(Sender : TObject);
                    PRIVATE
                     Pausing : BOOLEAN;
                    PROTECTED
                     PROCEDURE WMShuffle(VAR Msg : TMESSAGE);
                      MESSAGE wm_Shuffle;
                    PUBLIC
                     {Insert public declarations here}
                    END;

 VAR ShuffleDlg : TShuffleDlg;

(************************************************************************)

IMPLEMENTATION

(************************************************************************)

 Procedure TShuffleDlg.ShuffleDlgOnShow(Sender : TObject);
  Begin
   Label1.Caption := mShuffling; Pausing := TRUE;
   {post message to do the rest of it}
   PostMsg(Handle, wm_Shuffle, 0, 0);
  End;

(************************************************************************)

 Procedure TShuffleDlg.PauseTimerOnTimer(Sender : TObject);
  Begin
   PauseTimer.Stop;
   Pausing := FALSE;
  End;

(************************************************************************)

 PROCEDURE TShuffleDlg.WMShuffle(VAR Msg : TMESSAGE);
     (* procedure used to do the shuffle (quits the dialog also) *)

  BEGIN (*tshuffledlg.wmshuffle*)
   DoTheShuffle;
   PauseTimer.Start;
   REPEAT
    Application.ProcessMessages;
   UNTIL NOT(Pausing);
   DismissDlg(mrOK);
  END; (*tshuffledlg.wmshuffle*)

(************************************************************************)

INITIALIZATION
  RegisterClasses([TShuffleDlg, TBevel, TLabel, TTimer]);
END.
