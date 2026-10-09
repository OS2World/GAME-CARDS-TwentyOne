/*******************************************************************************
* PREFS.H - settings kept in TwentyOne.cfg (plan.txt section 10)
*
* Binary file, sequential int fields. Older, shorter files are fine: the
* missing fields keep their defaults. Never reorder the fields, only append.
*******************************************************************************/

#ifndef PREFS_H
#define PREFS_H

#define CFG_FILE        "TwentyOne.cfg"

struct PrefsStruct
{
    int saveonexit;             /* 0 or 1                                    */
    int detaillevel;            /* kept for the common file layout           */
    int current_lang;           /* LANG_EN .. LANG_IT                        */

    /* game specific */
    int cardback;               /* 0 .. NUM_BACKS-1                          */
    int numdecks;               /* 1 .. 3                                    */
    int minbet;
    int maxbet;
    int initbank;
    int betmax;                 /* bet the maximum by default                */
};

extern struct PrefsStruct Prefs;

void InitPrefs(void);
void LoadPrefs(void);
int  SavePrefs(void);

#endif /* PREFS_H */
