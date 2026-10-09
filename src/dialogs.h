/*******************************************************************************
* DIALOGS.H
*******************************************************************************/

#ifndef DIALOGS_H
#define DIALOGS_H

void AboutDialog(HWND owner);
int  BetDialog(HWND owner, long *bet);     /* TRUE + *bet when OK            */
int  OptionsDialog(HWND owner);            /* TRUE when changed with OK      */

#endif /* DIALOGS_H */
