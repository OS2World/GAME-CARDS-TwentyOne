/*******************************************************************************
* LANG.H - translated texts (plan.txt section 8)
*
* All strings are plain ASCII: no accented letters.
*******************************************************************************/

#ifndef LANG_H
#define LANG_H

#include "twenty.h"

enum
{
    STR_GAME = 0,           /* menu bar and menus                            */
    STR_NEW,
    STR_QUIT,
    STR_EXIT,
    STR_OPTIONS,
    STR_SETTINGS,
    STR_LANGUAGE,
    STR_FRAME,
    STR_SAVEONEXIT,
    STR_HELP,
    STR_USINGHELP,
    STR_EXTHELP,
    STR_KEYSHELP,
    STR_HELPINDEX,
    STR_ABOUT,

    STR_BTN_PLAY,           /* buttons                                       */
    STR_BTN_HIT,
    STR_BTN_STAY,
    STR_BTN_DOUBLE,
    STR_BTN_SPLIT,
    STR_BTN_INSURE,

    STR_DEALER,             /* labels in the window                          */
    STR_PLAYER,
    STR_CARDSLEFT,
    STR_BANK,
    STR_BET,
    STR_IDLE,

    STR_PUSH,               /* messages of the rules engine                  */
    STR_WON,
    STR_WON_FIVE,
    STR_NOT_ENOUGH_INS,
    STR_NOT21,
    STR_NOT_ENOUGH_SPLIT,
    STR_NOT_ENOUGH_DOUBLE,
    STR_P_OVER21,
    STR_D_OVER21,
    STR_FIRST_HAND,
    STR_SECOND_HAND,
    STR_SHUFFLED,
    STR_FORFEIT,

    STR_NOT_ENOUGH_RESET,   /* questions                                     */
    STR_QUIT_CONFIRM,

    STR_BET_TITLE,          /* bet dialog                                    */
    STR_BET_LABEL,
    STR_MIN_BET,
    STR_MAX_BET,
    STR_INIT_BANK,
    STR_RANGE_ERR,

    STR_OPT_TITLE,          /* settings dialog                               */
    STR_OPT_DECKS,
    STR_OPT_DECK1,
    STR_OPT_DECK2,
    STR_OPT_DECK3,
    STR_OPT_BETMAX,
    STR_OPT_BACK,
    STR_OK,
    STR_CANCEL,

    STR_HELP_ERR,

    STR_COUNT
};

extern int         current_lang;
extern const char *lang_strings[LANG_COUNT][STR_COUNT];

#define tr(id)  ((char *)lang_strings[current_lang][(id)])

void set_language(HWND hMenu, int lang);

#endif /* LANG_H */
