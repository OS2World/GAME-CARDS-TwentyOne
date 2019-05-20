PROGRAM TwentyOne;

(*                                                                      *)
(* AUTHOR: Michael G. Slack                    DATE WRITTEN: 2001/05/18 *)
(* ENVIRONMENT: Sibyl                                                   *)
(*                                                                      *)
(* Program runs a game of twenty-one.  The game tries to stay true to   *)
(* the rules as I understand them.  Some may not be entirely correct,   *)
(* some may be made up and some may be wrong.  Player plays against the *)
(* computer (who is the house).  Like all games of 21, house has to     *)
(* draw on 16 and lower/stand on 17+.                                   *)
(* NOTE: Program relies on the QCard2/U system for card images/backs.   *)
(*                                                                      *)
(* -------------------------------------------------------------------- *)
(*                                                                      *)
(* REVISED: 2001/05/27 - Fixed a 'bug' in the bet dialog.  (1.01)       *)
(*          2001/05/28 - Fixed a 'bug' in the double routine. (1.02)    *)
(*          2001/05/31 - Attempt to fix memory leak.  (1.03)            *)
(*          2001/06/02 - Minor cleanup, add proc back.  (1.04)          *)
(*                                                                      *)

 USES Forms, Graphics, TwenWin, TwenSDlg, TwenAbt, TwenBet, TwenOpts;

{$r TwentyOne.scu}

BEGIN (*main*)
 Application.Create;
 Application.CreateForm(TTwenO_Win, TwenO_Win);
 Application.Run;
 Application.Destroy;
END. (*main*)
