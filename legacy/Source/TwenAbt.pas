UNIT TwenAbt;

(*                                                                      *)
(* AUTHOR: Michael G. Slack                    DATE WRITTEN: 2001/05/19 *)
(* ENVIRONMENT: Sibyl                                                   *)
(*                                                                      *)
(* Unit defines the about dialog used by the twenty-one program.        *)
(*                                                                      *)
(* -------------------------------------------------------------------- *)
(*                                                                      *)
(* REVISED: 2001/05/27 - Changed version number.                        *)
(*          2001/05/28 -  " " " " " " " " " " "                         *)
(*          2001/05/31 -  " " " " " " " " " " "                         *)
(*          2001/06/02 -  " " " " " " " " " " "                         *)
(*                                                                      *)

INTERFACE

 USES Classes, Forms, Graphics, Buttons, ExtCtrls, StdCtrls;

 TYPE TAboutDlg = CLASS(TFORM)
                   Image1        : TImage;
                   Lbl_Title     : TLabel;
                   Lbl_Version   : TLabel;
                   Lbl_Copyright : TLabel;
                   Bevel1        : TBevel;
                   OkButton      : TBitBtn;
                  PRIVATE
                   {Insert private declarations here}
                  PUBLIC
                   {Insert public declarations here}
                  END;

 VAR AboutDlg : TAboutDlg;

(************************************************************************)

IMPLEMENTATION

(************************************************************************)

INITIALIZATION
  RegisterClasses([TAboutDlg, TBitBtn, TImage, TLabel, TBevel]);
END. (*of unit*)
