/*******************************************************************************
* HELP.H - online help
*******************************************************************************/

#ifndef HELP_H
#define HELP_H

BOOL SetHelpLanguage(void);     /* (re)create the help instance              */
void CloseHelp(void);
void HelpPanel(long idPanel);
void HelpForHelp(void);
void HelpContents(void);
void HelpIndex(void);

#endif /* HELP_H */
