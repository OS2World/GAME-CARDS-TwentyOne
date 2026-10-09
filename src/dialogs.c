/*******************************************************************************
* DIALOGS.C - About, bet and settings dialogs
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "twenty.h"
#include "game.h"
#include "lang.h"
#include "prefs.h"
#include "dialogs.h"

extern HBITMAP hbmBack[NUM_BACKS];


/*** Helpers ******************************************************************/

/* Center a dialog over its owner */
static void CenterDialog(HWND hwnd)
{
    HWND  hwndOwner = WinQueryWindow( hwnd, QW_OWNER );
    SWP   swpDlg, swpOwner;
    LONG  x, y;

    if( hwndOwner == NULLHANDLE )
        return;

    WinQueryWindowPos( hwnd,       &swpDlg );
    WinQueryWindowPos( hwndOwner,  &swpOwner );

    x = swpOwner.x + (swpOwner.cx - swpDlg.cx) / 2;
    y = swpOwner.y + (swpOwner.cy - swpDlg.cy) / 2;
    if( x < 0 ) x = 0;
    if( y < 0 ) y = 0;

    WinSetWindowPos( hwnd, HWND_TOP, x, y, 0, 0, SWP_MOVE );
}


static void SetMoney(HWND hwnd, USHORT id, long amount)
{
    char buf[32];

    sprintf( buf, "$%ld.00", amount );
    WinSetDlgItemText( hwnd, id, buf );
}


/* Read a whole, positive number from an entry field; -1 if it is not one */
static long ReadNumber(HWND hwnd, USHORT id)
{
    char  buf[32];
    char *end;
    long  v;

    WinQueryDlgItemText( hwnd, id, sizeof(buf), buf );
    if( buf[0] == 0 )
        return -1;

    v = strtol( buf, &end, 10 );
    while( *end == ' ' )
        end++;
    if( *end != 0 || v < 0 )
        return -1;
    return v;
}


/*** About ********************************************************************/

static MRESULT EXPENTRY AboutDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    switch( msg )
    {
        case WM_INITDLG:
            CenterDialog( hwnd );
            return 0;

        case WM_COMMAND:
            switch( COMMANDMSG(&msg)->cmd )
            {
                case DID_OK:
                case DID_CANCEL:
                    WinDismissDlg( hwnd, TRUE );
                    return 0;
            }
            break;
    }

    return WinDefDlgProc( hwnd, msg, mp1, mp2 );
}


void AboutDialog(HWND owner)
{
    WinDlgBox( HWND_DESKTOP, owner, AboutDlgProc, NULLHANDLE, DLG_ABOUT, NULL );
}


/*** Bet **********************************************************************/

static long BetResult;

static MRESULT EXPENTRY BetDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    long value;
    HWND hSpin;

    switch( msg )
    {
        case WM_INITDLG:
            CenterDialog( hwnd );
            WinSetWindowText( hwnd, tr(STR_BET_TITLE) );
            WinSetDlgItemText( hwnd, IDD_BET_LABEL,   tr(STR_BET_LABEL) );
            WinSetDlgItemText( hwnd, IDD_BET_BANKTXT, tr(STR_BANK) );
            WinSetDlgItemText( hwnd, IDD_BET_MINTXT,  tr(STR_MIN_BET) );
            WinSetDlgItemText( hwnd, IDD_BET_MAXTXT,  tr(STR_MAX_BET) );
            WinSetDlgItemText( hwnd, DID_OK,          tr(STR_OK) );
            WinSetDlgItemText( hwnd, DID_CANCEL,      tr(STR_CANCEL) );
            SetMoney( hwnd, IDD_BET_BANK, Bank );
            SetMoney( hwnd, IDD_BET_MIN,  MinimumBet );
            SetMoney( hwnd, IDD_BET_MAX,  MaximumBet );

            hSpin = WinWindowFromID( hwnd, IDD_BET_SPIN );
            WinSendMsg( hSpin, SPBM_SETLIMITS,
                        MPFROMLONG(GameMaxBet()), MPFROMLONG(MinimumBet) );
            WinSendMsg( hSpin, SPBM_SETCURRENTVALUE,
                        MPFROMLONG(BetMax ? GameMaxBet() : MinimumBet), 0 );
            return 0;

        case WM_COMMAND:
            switch( COMMANDMSG(&msg)->cmd )
            {
                case DID_OK:
                    hSpin = WinWindowFromID( hwnd, IDD_BET_SPIN );
                    value = MinimumBet;
                    WinSendMsg( hSpin, SPBM_QUERYVALUE, MPFROMP(&value),
                                MPFROM2SHORT(0, SPBQ_UPDATEIFVALID) );
                    if( value < MinimumBet )     value = MinimumBet;
                    if( value > GameMaxBet() )   value = GameMaxBet();
                    BetResult = value;
                    WinDismissDlg( hwnd, TRUE );
                    return 0;

                case DID_CANCEL:
                    WinDismissDlg( hwnd, FALSE );
                    return 0;
            }
            break;
    }

    return WinDefDlgProc( hwnd, msg, mp1, mp2 );
}


/* Ask for the bet: TRUE and *bet when the user pressed OK */
int BetDialog(HWND owner, long *bet)
{
    if( WinDlgBox( HWND_DESKTOP, owner, BetDlgProc, NULLHANDLE, DLG_BET, NULL ) )
    {
        *bet = BetResult;
        return 1;
    }
    return 0;
}


/*** Settings *****************************************************************/

static HWND hwndOptOwner;

/* The preview is a plain static whose paint we take over. A SS_BITMAP static
   would own (and delete) the bitmap handle we give it. */
static PFNWP pfnStatic;
static int   previewBack;

static MRESULT EXPENTRY PreviewProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    if( msg == WM_PAINT )
    {
        HPS    hps = WinBeginPaint( hwnd, NULLHANDLE, NULL );
        RECTL  rcl;
        POINTL aptl[4];

        WinQueryWindowRect( hwnd, &rcl );
        WinFillRect( hps, &rcl, CLR_PALEGRAY );

        if( hbmBack[previewBack] != NULLHANDLE )
        {
            aptl[0].x = 0;               aptl[0].y = 0;
            aptl[1].x = rcl.xRight - 1;  aptl[1].y = rcl.yTop - 1;
            aptl[2].x = 0;               aptl[2].y = 0;
            aptl[3].x = 80;              aptl[3].y = 120;
            GpiWCBitBlt( hps, hbmBack[previewBack], 4, aptl, ROP_SRCCOPY, BBO_IGNORE );
        }
        WinEndPaint( hps );
        return 0;
    }

    return pfnStatic( hwnd, msg, mp1, mp2 );
}


static void ShowBackPreview(HWND hwnd, int back)
{
    HWND hwndPrev = WinWindowFromID( hwnd, IDD_OPT_PREVIEW );

    if( back < 0 || back >= NUM_BACKS )
        return;
    previewBack = back;
    WinInvalidateRect( hwndPrev, NULL, FALSE );
}


static void RangeError(HWND hwnd, const char *what, long lo, long hi, USHORT idFocus)
{
    char msg[160];

    sprintf( msg, "%s", what );
    sprintf( msg + strlen(msg), tr(STR_RANGE_ERR), (int)lo, (int)hi );
    WinMessageBox( HWND_DESKTOP, hwnd, msg, PRG_NAME, 0, MB_OK | MB_INFORMATION | MB_MOVEABLE );
    WinSetFocus( HWND_DESKTOP, WinWindowFromID( hwnd, idFocus ) );
}


static MRESULT EXPENTRY OptionsDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    long mn, mx, bnk, back = 1;
    HWND hSpin;

    switch( msg )
    {
        case WM_INITDLG:
            CenterDialog( hwnd );
            WinSetWindowText( hwnd, tr(STR_OPT_TITLE) );
            WinSetDlgItemText( hwnd, IDD_OPT_DECKS,   tr(STR_OPT_DECKS) );
            WinSetDlgItemText( hwnd, IDD_OPT_DECK1,   tr(STR_OPT_DECK1) );
            WinSetDlgItemText( hwnd, IDD_OPT_DECK2,   tr(STR_OPT_DECK2) );
            WinSetDlgItemText( hwnd, IDD_OPT_DECK3,   tr(STR_OPT_DECK3) );
            WinSetDlgItemText( hwnd, IDD_OPT_MINTXT,  tr(STR_MIN_BET) );
            WinSetDlgItemText( hwnd, IDD_OPT_MAXTXT,  tr(STR_MAX_BET) );
            WinSetDlgItemText( hwnd, IDD_OPT_BANKTXT, tr(STR_INIT_BANK) );
            WinSetDlgItemText( hwnd, IDD_OPT_BETMAX,  tr(STR_OPT_BETMAX) );
            WinSetDlgItemText( hwnd, IDD_OPT_BACKTXT, tr(STR_OPT_BACK) );
            WinSetDlgItemText( hwnd, DID_OK,          tr(STR_OK) );
            WinSetDlgItemText( hwnd, DID_CANCEL,      tr(STR_CANCEL) );

            WinCheckButton( hwnd, IDD_OPT_DECK1 + (NumOfDecks - 1), TRUE );
            {
                char buf[16];

                sprintf( buf, "%d", MinimumBet );  WinSetDlgItemText( hwnd, IDD_OPT_MIN,  buf );
                sprintf( buf, "%d", MaximumBet );  WinSetDlgItemText( hwnd, IDD_OPT_MAX,  buf );
                sprintf( buf, "%d", InitialBank ); WinSetDlgItemText( hwnd, IDD_OPT_BANK, buf );
            }
            WinCheckButton( hwnd, IDD_OPT_BETMAX, BetMax ? TRUE : FALSE );

            hSpin = WinWindowFromID( hwnd, IDD_OPT_BACK );
            WinSendMsg( hSpin, SPBM_SETLIMITS, MPFROMLONG(NUM_BACKS), MPFROMLONG(1) );
            WinSendMsg( hSpin, SPBM_SETCURRENTVALUE, MPFROMLONG(Prefs.cardback + 1), 0 );
            pfnStatic = WinSubclassWindow( WinWindowFromID( hwnd, IDD_OPT_PREVIEW ), PreviewProc );
            ShowBackPreview( hwnd, Prefs.cardback );
            return 0;

        case WM_CONTROL:
            if( SHORT1FROMMP(mp1) == IDD_OPT_BACK && SHORT2FROMMP(mp1) == SPBN_CHANGE )
            {
                hSpin = WinWindowFromID( hwnd, IDD_OPT_BACK );
                WinSendMsg( hSpin, SPBM_QUERYVALUE, MPFROMP(&back),
                            MPFROM2SHORT(0, SPBQ_DONOTUPDATE) );
                ShowBackPreview( hwnd, (int)back - 1 );
            }
            return 0;

        case WM_COMMAND:
            switch( COMMANDMSG(&msg)->cmd )
            {
                case DID_OK:
                    mn  = ReadNumber( hwnd, IDD_OPT_MIN );
                    mx  = ReadNumber( hwnd, IDD_OPT_MAX );
                    bnk = ReadNumber( hwnd, IDD_OPT_BANK );

                    if( mn < MIN_BET_AMT || mn > MAX_BET_AMT )
                    {
                        RangeError( hwnd, tr(STR_MIN_BET), MIN_BET_AMT, MAX_BET_AMT, IDD_OPT_MIN );
                        return 0;
                    }
                    if( mx < mn || mx > MAX_BET_AMT )
                    {
                        RangeError( hwnd, tr(STR_MAX_BET), mn, MAX_BET_AMT, IDD_OPT_MAX );
                        return 0;
                    }
                    if( bnk < mn || bnk > MAX_BANK_AMT )
                    {
                        RangeError( hwnd, tr(STR_INIT_BANK), mn, MAX_BANK_AMT, IDD_OPT_BANK );
                        return 0;
                    }

                    NumOfDecks = 1;
                    if( WinQueryButtonCheckstate( hwnd, IDD_OPT_DECK2 ) ) NumOfDecks = 2;
                    if( WinQueryButtonCheckstate( hwnd, IDD_OPT_DECK3 ) ) NumOfDecks = 3;
                    MinimumBet  = (int)mn;
                    MaximumBet  = (int)mx;
                    InitialBank = (int)bnk;
                    BetMax      = WinQueryButtonCheckstate( hwnd, IDD_OPT_BETMAX ) ? 1 : 0;

                    hSpin = WinWindowFromID( hwnd, IDD_OPT_BACK );
                    WinSendMsg( hSpin, SPBM_QUERYVALUE, MPFROMP(&back),
                                MPFROM2SHORT(0, SPBQ_UPDATEIFVALID) );
                    if( back < 1 || back > NUM_BACKS )
                        back = 1;
                    Prefs.cardback = (int)back - 1;

                    WinDismissDlg( hwnd, TRUE );
                    return 0;

                case DID_CANCEL:
                    WinDismissDlg( hwnd, FALSE );
                    return 0;
            }
            break;
    }

    return WinDefDlgProc( hwnd, msg, mp1, mp2 );
}


/* TRUE when the settings were changed with OK */
int OptionsDialog(HWND owner)
{
    hwndOptOwner = owner;
    return WinDlgBox( HWND_DESKTOP, owner, OptionsDlgProc, NULLHANDLE,
                      DLG_OPTIONS, NULL ) == TRUE ? 1 : 0;
}
