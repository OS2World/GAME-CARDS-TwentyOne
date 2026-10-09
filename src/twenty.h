/*******************************************************************************
* TWENTY.H - TwentyOne (Blackjack) for OS/2 Presentation Manager
*
* Copyright (C) 2001 Michael G. Slack
* Copyright (C) 2026 OS2World
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 3 of the License, or (at your
* option) any later version.
*******************************************************************************/

#ifndef TWENTY_H
#define TWENTY_H

#define INCL_WIN
#define INCL_WINHELP
#define INCL_GPI
#define INCL_DOS
#define INCL_DOSPROCESS
#define INCL_DOSMODULEMGR
#define INCL_DOSFILEMGR
#include <os2.h>

#define PRG_NAME        "TwentyOne"
#define PRG_VERSION     "1.6"

/*** Resource ids *************************************************************/

#define ID_MAIN             555         /* menu, accelerator table and icon  */

/* Game menu 100-199 */
#define IDM_PLAY            100
#define IDM_QUIT            102
#define IDM_EXIT            107
#define IDM_HIT             110         /* these are also the button ids     */
#define IDM_STAY            111
#define IDM_DOUBLE          112
#define IDM_SPLIT           113
#define IDM_INSURE          114

/* Options menu 200-299 */
#define IDM_SETTINGS        200
#define IDM_FRAME           205
#define IDM_SAVEONEXIT      206

/* Language menu 300-399 */
#define IDM_LANG_EN         300
#define IDM_LANG_ES         301
#define IDM_LANG_NL         302
#define IDM_LANG_DE         303
#define IDM_LANG_FR         304
#define IDM_LANG_IT         305

/* Help menu 900-999 */
#define IDM_HELPUSING       901
#define IDM_HELPEXT         902
#define IDM_HELPKEYS        903
#define IDM_HELPINDEX       904
#define IDM_ABOUT           999
#define IDM_ESCAPE          998

/* Cascade ids 1000-1099 */
#define IDM_SUBMENU_GAME    1000
#define IDM_SUBMENU_OPTIONS 1001
#define IDM_SUBMENU_LANG    1002
#define IDM_SUBMENU_HELP    1003

/* Dialogs */
#define DLG_OPTIONS         500
#define DLG_ABOUT           501
#define DLG_BET             502

#define IDD_BET_LABEL       510
#define IDD_BET_SPIN        511
#define IDD_BET_BANKTXT     512
#define IDD_BET_BANK        513
#define IDD_BET_MINTXT      514
#define IDD_BET_MIN         515
#define IDD_BET_MAXTXT      516
#define IDD_BET_MAX         517

#define IDD_OPT_DECKS       520
#define IDD_OPT_DECK1       521
#define IDD_OPT_DECK2       522
#define IDD_OPT_DECK3       523
#define IDD_OPT_MINTXT      524
#define IDD_OPT_MIN         525
#define IDD_OPT_MAXTXT      526
#define IDD_OPT_MAX         527
#define IDD_OPT_BANKTXT     528
#define IDD_OPT_BANK        529
#define IDD_OPT_BETMAX      530
#define IDD_OPT_BACKTXT     531
#define IDD_OPT_BACK        532
#define IDD_OPT_PREVIEW     533

/* Bitmaps */
#define BMP_CARD_BASE       1000        /* +0..51, rank-major: 2C 2D 2H 2S ... AS */
#define BMP_BACK_BASE       1100        /* +0..NUM_BACKS-1                    */
#define NUM_BACKS           6

/* Help panels (same numbers in every language file) */
#define HELP_GENERAL        1000
#define HELP_RULES          1100
#define HELP_KEYS           1200
#define HELP_OPTIONS        1300
#define HELP_ABOUT          1400
#define HELP_TABLE_ID       555

/*** Game constants ***********************************************************/

#define MIN_BET_AMT         1
#define START_MAX_BET       5
#define MAX_BET_AMT         10000
#define START_BANK          100
#define MAX_BANK_AMT        100000
#define MAX_DRAW_CARDS      5           /* 5 cards without busting wins      */
#define MAX_DECKS           3

/* Language ids (plan.txt section 8) */
#define LANG_EN             0
#define LANG_ES             1
#define LANG_NL             2
#define LANG_DE             3
#define LANG_FR             4
#define LANG_IT             5
#define LANG_COUNT          6

#endif /* TWENTY_H */
