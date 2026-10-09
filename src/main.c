/*******************************************************************************
* MAIN.C - TwentyOne (Blackjack) for OS/2 Presentation Manager
*
* Window, drawing and commands. The rules are in GAME.C.
*
* Copyright (C) 2001 Michael G. Slack
* Copyright (C) 2026 OS2World
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "twenty.h"
#include "game.h"
#include "lang.h"
#include "prefs.h"
#include "help.h"
#include "dialogs.h"

/* BLDLEVEL record, readable with bldlevel.exe (plan.txt section 4) */
const char bldlevel[] =
    "@#Michael G. Slack:1.6#@##1## 09 Oct 2026 12:00:00      "
    "ARCAOS:::0::::@@TwentyOne - Blackjack card game for OS/2\r\n\x1a";

/*** Layout (client window coordinates, origin bottom left) *******************/

#define CLIENT_W        960
#define CLIENT_H        640

#define SRC_W           80              /* size of the bitmaps in the exe    */
#define SRC_H           120
#define CARD_W          120             /* drawn 1.5 times larger            */
#define CARD_H          180
#define CARD_X0         30
#define CARD_STEP       132

#define DEALER_Y        424             /* bottom of the dealer's cards      */
#define PLAYER_Y        200
#define LABEL_H         26

#define STATUS_Y        70
#define STATUS_H        120
#define LINE_H          20
#define MAX_LINES       6

#define BTN_Y           16
#define BTN_W           156
#define BTN_H           44
#define BTN_GAP         4
#define BTN_X0          2

#define INFO_X          706             /* right hand column                 */
#define INFO_W          240

/*** Variables ****************************************************************/

HAB         hab;
HWND        hwndFrame;
HBITMAP     hbmBack[NUM_BACKS];

static HMQ      hmq;
static HWND     hwndClient;
static HWND     hwndObject;             /* parking window for hidden controls */
static BOOL     bFrameHidden;
static HBITMAP  hbmCard[52];

static char     szLines[MAX_LINES][200];
static int      nLines;

static const USHORT BtnIds[6]  = { IDM_PLAY, IDM_HIT, IDM_STAY, IDM_DOUBLE, IDM_SPLIT, IDM_INSURE };
static const int    BtnText[6] = { STR_BTN_PLAY, STR_BTN_HIT, STR_BTN_STAY,
                                   STR_BTN_DOUBLE, STR_BTN_SPLIT, STR_BTN_INSURE };

static MRESULT EXPENTRY ClientWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);


/*** Helpers ******************************************************************/

/* The menu bar, also while the frame controls are hidden */
static HWND GetMenu(void)
{
    return WinWindowFromID( bFrameHidden ? hwndObject : hwndFrame, FID_MENU );
}

static void Money(char *buf, long amount)
{
    sprintf( buf, "$%ld.00", amount );
}


static void Say(const char *text)
{
    if( nLines < MAX_LINES )
    {
        strncpy( szLines[nLines], text, sizeof(szLines[0]) - 1 );
        szLines[nLines][sizeof(szLines[0]) - 1] = 0;
        nLines++;
    }
}


/* Show the messages of the rules engine in the status area */
static void ShowMessages(void)
{
    int  i;
    char buf[200];

    nLines = 0;

    for( i = 0; i < NumMsgs; i++ )
    {
        const char *who;

        switch( Msgs[i].code )
        {
            case GM_PUSH:
                Say( tr(STR_PUSH) );
                break;
            case GM_WON:
                who = Msgs[i].a ? tr(STR_DEALER) : tr(STR_PLAYER);
                sprintf( buf, tr(STR_WON), who, Msgs[i].b );
                Say( buf );
                break;
            case GM_WON_FIVE:
                who = Msgs[i].a ? tr(STR_DEALER) : tr(STR_PLAYER);
                sprintf( buf, tr(STR_WON_FIVE), who, who );
                Say( buf );
                break;
            case GM_NOT_ENOUGH_INS:
                sprintf( buf, tr(STR_NOT_ENOUGH_INS), Msgs[i].a );
                Say( buf );
                break;
            case GM_NOT21:           Say( tr(STR_NOT21) );             break;
            case GM_NOT_ENOUGH_SPLIT: Say( tr(STR_NOT_ENOUGH_SPLIT) ); break;
            case GM_NOT_ENOUGH_DOUBLE: Say( tr(STR_NOT_ENOUGH_DOUBLE) ); break;
            case GM_PLAYER_OVER21:   Say( tr(STR_P_OVER21) );          break;
            case GM_DEALER_OVER21:   Say( tr(STR_D_OVER21) );          break;
            case GM_FIRST_HAND:      Say( tr(STR_FIRST_HAND) );        break;
            case GM_SECOND_HAND:     Say( tr(STR_SECOND_HAND) );       break;
            case GM_SHUFFLED:        Say( tr(STR_SHUFFLED) );          break;
            case GM_FORFEIT:         Say( tr(STR_FORFEIT) );           break;
        }
    }

    if( nLines == 0 && !InHand && NumPlayer == 0 )
        Say( tr(STR_IDLE) );

    NumMsgs = 0;
}


/* Enable the buttons and menu items that make sense right now */
static void UpdateButtons(void)
{
    HWND hMenu = GetMenu();
    BOOL on[6];
    int  i;

    on[0] = !InHand;
    on[1] = InHand && CanHit;
    on[2] = InHand && CanStay;
    on[3] = InHand && CanDouble;
    on[4] = InHand && CanSplit;
    on[5] = InHand && CanInsure;

    for( i = 0; i < 6; i++ )
        WinEnableWindow( WinWindowFromID( hwndClient, BtnIds[i] ), on[i] );

    if( hMenu )
    {
        WinEnableMenuItem( hMenu, IDM_PLAY, !InHand );
        WinEnableMenuItem( hMenu, IDM_QUIT, InHand );
    }
}


static void UpdateButtonTexts(void)
{
    int i;

    for( i = 0; i < 6; i++ )
        WinSetWindowText( WinWindowFromID( hwndClient, BtnIds[i] ), tr(BtnText[i]) );
}


/* Everything changed: buttons, texts and the window */
static void Refresh(void)
{
    UpdateButtons();
    WinInvalidateRect( hwndClient, NULL, TRUE );
}


/*** Drawing ******************************************************************/

static void DrawLabel(HPS hps, LONG x, LONG y, LONG w, const char *text, ULONG flags)
{
    RECTL rcl;

    rcl.xLeft   = x;
    rcl.yBottom = y;
    rcl.xRight  = x + w;
    rcl.yTop    = y + LABEL_H - 2;
    WinDrawText( hps, -1, (PSZ)text, &rcl, CLR_WHITE, CLR_DARKGREEN,
                 flags | DT_VCENTER | DT_TEXTATTRS );
}


static void DrawSlot(HPS hps, LONG x, LONG y)
{
    POINTL ptl;

    GpiSetColor( hps, CLR_GREEN );
    ptl.x = x;                ptl.y = y;                GpiMove( hps, &ptl );
    ptl.x = x + CARD_W - 1;   ptl.y = y + CARD_H - 1;   GpiBox( hps, DRO_OUTLINE, &ptl, 0, 0 );
}


static void DrawCard(HPS hps, LONG x, LONG y, HBITMAP hbm)
{
    POINTL aptl[4];

    if( hbm == NULLHANDLE )
    {
        DrawSlot( hps, x, y );
        return;
    }

    aptl[0].x = x;                  aptl[0].y = y;                  /* target  */
    aptl[1].x = x + CARD_W - 1;     aptl[1].y = y + CARD_H - 1;
    aptl[2].x = 0;                  aptl[2].y = 0;                  /* source  */
    aptl[3].x = SRC_W;              aptl[3].y = SRC_H;
    GpiWCBitBlt( hps, hbm, 4, aptl, ROP_SRCCOPY, BBO_IGNORE );
}


static void PaintGame(HPS hps)
{
    RECTL  rcl;
    int    i;
    LONG   x;
    char   buf[96], num[32];
    HBITMAP hbm;

    /* The table */
    rcl.xLeft = 0; rcl.yBottom = 0; rcl.xRight = CLIENT_W; rcl.yTop = CLIENT_H;
    WinFillRect( hps, &rcl, CLR_DARKGREEN );

    /* Dealer */
    strcpy( buf, tr(STR_DEALER) );
    if( HoleShown && NumDealer )
        sprintf( buf + strlen(buf), "  %d", ScoreOfHand( &Dealer[1], NumDealer ) );
    DrawLabel( hps, CARD_X0, DEALER_Y + CARD_H + 4, 600, buf, DT_LEFT );

    for( i = 1; i <= MAX_DRAW_CARDS; i++ )
    {
        x = CARD_X0 + (i - 1) * CARD_STEP;
        hbm = NULLHANDLE;
        if( i <= NumDealer )
            hbm = (i == 1 && !HoleShown) ? hbmBack[Prefs.cardback] : hbmCard[Dealer[i]];
        DrawCard( hps, x, DEALER_Y, hbm );
    }

    /* Player */
    strcpy( buf, tr(STR_PLAYER) );
    if( NumPlayer )
    {
        if( SplitHand )
        {
            int h2[2];

            h2[0] = Player[4]; h2[1] = Player[5];
            sprintf( buf + strlen(buf), "  %d  /  %d",
                     ScoreOfHand( &Player[1], 2 ), ScoreOfHand( h2, 2 ) );
        }
        else
            sprintf( buf + strlen(buf), "  %d", ScoreOfHand( &Player[1], NumPlayer ) );
    }
    DrawLabel( hps, CARD_X0, PLAYER_Y + CARD_H + 4, 600, buf, DT_LEFT );

    for( i = 1; i <= MAX_DRAW_CARDS; i++ )
    {
        x = CARD_X0 + (i - 1) * CARD_STEP;
        hbm = NULLHANDLE;
        if( SplitHand )
        {
            if( i != 3 )
                hbm = hbmCard[Player[i]];
        }
        else if( i <= NumPlayer )
            hbm = hbmCard[Player[i]];
        DrawCard( hps, x, PLAYER_Y, hbm );
    }

    /* The deck and the money */
    DrawCard( hps, INFO_X + (INFO_W - CARD_W) / 2, DEALER_Y, hbmBack[Prefs.cardback] );

    sprintf( buf, "%s: %d", tr(STR_CARDSLEFT), CardsLeft() );
    DrawLabel( hps, INFO_X, PLAYER_Y + CARD_H - 24, INFO_W, buf, DT_CENTER );
    Money( num, Bank );
    sprintf( buf, "%s: %s", tr(STR_BANK), num );
    DrawLabel( hps, INFO_X, PLAYER_Y + CARD_H - 60, INFO_W, buf, DT_CENTER );
    Money( num, InHand ? Bet : 0 );
    sprintf( buf, "%s: %s", tr(STR_BET), num );
    DrawLabel( hps, INFO_X, PLAYER_Y + CARD_H - 96, INFO_W, buf, DT_CENTER );

    /* Status lines */
    for( i = 0; i < nLines; i++ )
    {
        rcl.xLeft   = CARD_X0;
        rcl.xRight  = CLIENT_W - CARD_X0;
        rcl.yTop    = STATUS_Y + STATUS_H - i * LINE_H;
        rcl.yBottom = rcl.yTop - LINE_H;
        WinDrawText( hps, -1, (PSZ)szLines[i], &rcl, CLR_WHITE, CLR_DARKGREEN,
                     DT_LEFT | DT_VCENTER | DT_TEXTATTRS );
    }
}


/*** Commands *****************************************************************/

static void DoPlay(void)
{
    long bet;

    if( InHand )                            /* Ctrl+N is ignored while playing */
        return;

    if( !CanAffordBet() )
    {
        if( WinMessageBox( HWND_DESKTOP, hwndFrame, tr(STR_NOT_ENOUGH_RESET), PRG_NAME,
                           0, MB_YESNO | MB_QUERY | MB_MOVEABLE ) != MBID_YES )
            return;
        GameResetBank();
        Refresh();
    }

    if( !BetDialog( hwndFrame, &bet ) )
        return;

    NumMsgs = 0;
    GameDeal( bet );
    ShowMessages();
    Refresh();
}


static void DoQuit(void)
{
    if( !InHand )                           /* Ctrl+Q is ignored without a hand */
        return;

    if( WinMessageBox( HWND_DESKTOP, hwndFrame, tr(STR_QUIT_CONFIRM), PRG_NAME,
                       0, MB_YESNO | MB_QUERY | MB_MOVEABLE ) != MBID_YES )
        return;

    GameForfeit();
    ShowMessages();
    Refresh();
}


static void DoAction(USHORT id)
{
    switch( id )
    {
        case IDM_HIT:    GameHit();    break;
        case IDM_STAY:   GameStay();   break;
        case IDM_DOUBLE: GameDouble(); break;
        case IDM_SPLIT:  GameSplit();  break;
        case IDM_INSURE: GameInsure(); break;
    }

    ShowMessages();
    Refresh();
}


static void DoSettings(void)
{
    int oldBack = Prefs.cardback;

    if( OptionsDialog( hwndFrame ) )
    {
        Prefs.numdecks = NumOfDecks;
        Prefs.minbet   = MinimumBet;
        Prefs.maxbet   = MaximumBet;
        Prefs.initbank = InitialBank;
        Prefs.betmax   = BetMax;
    }
    else
        Prefs.cardback = oldBack;           /* the dialog only edits on OK     */

    Refresh();
}


/* Hide or show title bar, system menu, minimize button and menu */
static void DoFrame(void)
{
    HWND  hwndNew = bFrameHidden ? hwndFrame : hwndObject;
    HWND  hMenu;
    RECTL rcl;
    ULONG flChange = FCF_TITLEBAR | FCF_SYSMENU | FCF_MINBUTTON | FCF_MENU;

    WinSetParent( WinWindowFromID( bFrameHidden ? hwndObject : hwndFrame, FID_TITLEBAR ), hwndNew, FALSE );
    WinSetParent( WinWindowFromID( bFrameHidden ? hwndObject : hwndFrame, FID_SYSMENU ),  hwndNew, FALSE );
    WinSetParent( WinWindowFromID( bFrameHidden ? hwndObject : hwndFrame, FID_MINMAX ),   hwndNew, FALSE );
    WinSetParent( WinWindowFromID( bFrameHidden ? hwndObject : hwndFrame, FID_MENU ),     hwndNew, FALSE );

    bFrameHidden = !bFrameHidden;

    WinSendMsg( hwndFrame, WM_UPDATEFRAME, MPFROMLONG(flChange), 0 );

    /* keep the client size: the frame loses or regains the decorations */
    rcl.xLeft = 0; rcl.yBottom = 0; rcl.xRight = CLIENT_W; rcl.yTop = CLIENT_H;
    WinCalcFrameRect( hwndFrame, &rcl, FALSE );
    WinSetWindowPos( hwndFrame, HWND_TOP, 0, 0,
                     rcl.xRight - rcl.xLeft, rcl.yTop - rcl.yBottom, SWP_SIZE );

    WinInvalidateRect( hwndFrame, NULL, TRUE );
    WinUpdateWindow( hwndFrame );

    /* the menu may be parked: find it where it is now */
    hMenu = WinWindowFromID( bFrameHidden ? hwndObject : hwndFrame, FID_MENU );
    if( hMenu )
        WinCheckMenuItem( hMenu, IDM_FRAME, bFrameHidden );
}


static void DoSaveOnExit(void)
{
    HWND hMenu = GetMenu();

    Prefs.saveonexit = Prefs.saveonexit ? 0 : 1;
    if( hMenu )
        WinCheckMenuItem( hMenu, IDM_SAVEONEXIT, Prefs.saveonexit );
}


static void DoLanguage(USHORT id)
{
    HWND hMenu = GetMenu();

    set_language( hMenu, id - IDM_LANG_EN );
    Prefs.current_lang = current_lang;

    UpdateButtonTexts();
    SetHelpLanguage();

    nLines = 0;
    if( !InHand && NumPlayer == 0 )
        Say( tr(STR_IDLE) );
    UpdateButtons();
    WinInvalidateRect( hwndClient, NULL, TRUE );
}


static void Command(USHORT id)
{
    switch( id )
    {
        case IDM_PLAY:      DoPlay();                        break;
        case IDM_QUIT:      DoQuit();                        break;
        case IDM_EXIT:      WinPostMsg( hwndClient, WM_CLOSE, 0, 0 ); break;

        case IDM_HIT:
        case IDM_STAY:
        case IDM_DOUBLE:
        case IDM_SPLIT:
        case IDM_INSURE:    DoAction( id );                  break;

        case IDM_SETTINGS:  DoSettings();                    break;
        case IDM_FRAME:     DoFrame();                       break;
        case IDM_SAVEONEXIT: DoSaveOnExit();                 break;

        case IDM_LANG_EN: case IDM_LANG_ES: case IDM_LANG_NL:
        case IDM_LANG_DE: case IDM_LANG_FR: case IDM_LANG_IT:
                            DoLanguage( id );                break;

        case IDM_HELPUSING: HelpForHelp();                   break;
        case IDM_HELPEXT:   HelpContents();                  break;
        case IDM_HELPKEYS:  HelpPanel( HELP_KEYS );          break;
        case IDM_HELPINDEX: HelpIndex();                     break;
        case IDM_ABOUT:     AboutDialog( hwndFrame );        break;
    }
}


/*** Owner drawn buttons: a small picture and the text ************************/

#define RGBC(r, g, b)   (((LONG)(r) << 16) | ((LONG)(g) << 8) | (LONG)(b))

static void FillPoly(HPS hps, const POINTL *pt, int n, LONG fill, LONG edge)
{
    GpiSetColor( hps, fill );
    GpiBeginArea( hps, BA_NOBOUNDARY | BA_ALTERNATE );
    GpiMove( hps, (PPOINTL)&pt[0] );
    GpiPolyLine( hps, n - 1, (PPOINTL)&pt[1] );
    GpiEndArea( hps );

    if( edge >= 0 )
    {
        GpiSetColor( hps, edge );
        GpiMove( hps, (PPOINTL)&pt[n - 1] );
        GpiPolyLine( hps, n, (PPOINTL)pt );
    }
}


static void Disc(HPS hps, LONG cx, LONG cy, LONG r, LONG fill, LONG edge)
{
    POINTL c;

    c.x = cx; c.y = cy;
    GpiMove( hps, &c );
    GpiSetColor( hps, fill );
    GpiFullArc( hps, DRO_FILL, MAKEFIXED(r, 0) );
    GpiMove( hps, &c );
    GpiSetColor( hps, edge );
    GpiFullArc( hps, DRO_OUTLINE, MAKEFIXED(r, 0) );
}


/* The pictures are 28 x 28 pixels, origin (x, y) bottom left. dis = grey. */
static void DrawButtonIcon(HPS hps, int idx, LONG x, LONG y, BOOL dis)
{
    POINTL p[8];
    LONG   edge = dis ? RGBC(120, 120, 120) : RGBC(20, 20, 20);
    int    i;

#define PT(i, a, b)  p[i].x = x + (a); p[i].y = y + (b)
#define COL(r, g, b) (dis ? RGBC(170, 170, 170) : RGBC(r, g, b))

    switch( idx )
    {
        case 0:                                     /* Play: green arrow      */
            PT(0, 6, 3);  PT(1, 6, 25);  PT(2, 25, 14);
            FillPoly( hps, p, 3, COL(40, 170, 60), edge );
            break;

        case 1:                                     /* Hit: a pencil         */
            PT(0, 9, 5);  PT(1, 5, 9);   PT(2, 21, 25);  PT(3, 25, 21);
            FillPoly( hps, p, 4, COL(210, 40, 40), edge );
            PT(0, 5, 9);  PT(1, 9, 5);   PT(2, 3, 3);
            FillPoly( hps, p, 3, COL(240, 210, 160), edge );
            PT(0, 21, 25); PT(1, 25, 21); PT(2, 27, 23); PT(3, 23, 27);
            FillPoly( hps, p, 4, COL(230, 150, 160), edge );
            break;

        case 2:                                     /* Stay: red stop sign   */
            for( i = 0; i < 8; i++ )
            {
                double a = 3.14159265 * (2 * i + 1) / 8.0;
                p[i].x = x + 14 + (LONG)(13.0 * cos(a));
                p[i].y = y + 14 + (LONG)(13.0 * sin(a));
            }
            FillPoly( hps, p, 8, COL(200, 30, 30), edge );
            PT(0, 6, 11);  PT(1, 22, 11);  PT(2, 22, 17);  PT(3, 6, 17);
            FillPoly( hps, p, 4, dis ? RGBC(235, 235, 235) : RGBC(255, 255, 255), -1 );
            break;

        case 3:                                     /* Double: a yellow face */
            Disc( hps, x + 14, y + 14, 12, COL(250, 220, 40), edge );
            Disc( hps, x + 10, y + 17, 2, edge, edge );
            Disc( hps, x + 18, y + 17, 2, edge, edge );
            GpiSetColor( hps, edge );
            PT(0, 7, 12);  GpiMove( hps, &p[0] );
            PT(1, 10, 8);  PT(2, 14, 6);  PT(3, 18, 8);  PT(4, 21, 12);
            GpiPolyLine( hps, 4, &p[1] );
            break;

        case 4:                                     /* Split: two cards      */
            PT(0, 2, 4);  PT(1, 14, 4);  PT(2, 14, 24);  PT(3, 2, 24);
            FillPoly( hps, p, 4, dis ? RGBC(225, 225, 225) : RGBC(255, 255, 255), edge );
            PT(0, 15, 4); PT(1, 27, 4);  PT(2, 27, 24);  PT(3, 15, 24);
            FillPoly( hps, p, 4, dis ? RGBC(225, 225, 225) : RGBC(255, 255, 255), edge );
            Disc( hps, x + 8, y + 14, 3, COL(200, 30, 30), edge );
            Disc( hps, x + 21, y + 14, 3, COL(200, 30, 30), edge );
            break;

        case 5:                                     /* Insure: a shield      */
            PT(0, 14, 26); PT(1, 3, 22);  PT(2, 3, 12);  PT(3, 14, 2);
            PT(4, 25, 12); PT(5, 25, 22);
            FillPoly( hps, p, 6, COL(40, 90, 200), edge );
            GpiSetColor( hps, dis ? RGBC(235, 235, 235) : RGBC(255, 255, 255) );
            PT(0, 14, 7);  GpiMove( hps, &p[0] );
            PT(1, 14, 21); GpiLine( hps, &p[1] );
            PT(0, 8, 15);  GpiMove( hps, &p[0] );
            PT(1, 20, 15); GpiLine( hps, &p[1] );
            break;
    }
#undef PT
#undef COL
}


static void DrawButton(PUSERBUTTON pub, int idx)
{
    HPS    hps = pub->hps;
    RECTL  rcl;
    BOOL   down = (pub->fsState & BDS_HILITED) != 0;
    BOOL   dis  = (pub->fsState & BDS_DISABLED) != 0;
    BOOL   def  = (pub->fsState & BDS_DEFAULT) != 0;
    POINTL pt;
    char   text[64];
    RECTL  rt;
    LONG   h;

    WinQueryWindowRect( pub->hwnd, &rcl );
    h = rcl.yTop - rcl.yBottom;

    GpiCreateLogColorTable( hps, 0, LCOLF_RGB, 0, 0, NULL );

    /* face */
    GpiSetColor( hps, RGBC(204, 204, 204) );
    pt.x = rcl.xLeft;  pt.y = rcl.yBottom;  GpiMove( hps, &pt );
    pt.x = rcl.xRight - 1;  pt.y = rcl.yTop - 1;
    GpiBox( hps, DRO_FILL, &pt, 0, 0 );

    /* border: raised, or pressed in */
    GpiSetColor( hps, def ? RGBC(0, 0, 0) : RGBC(80, 80, 80) );
    pt.x = rcl.xLeft;  pt.y = rcl.yBottom;  GpiMove( hps, &pt );
    pt.x = rcl.xRight - 1;  pt.y = rcl.yTop - 1;
    GpiBox( hps, DRO_OUTLINE, &pt, 0, 0 );

    GpiSetColor( hps, down ? RGBC(110, 110, 110) : RGBC(255, 255, 255) );
    pt.x = rcl.xLeft + 1;  pt.y = rcl.yBottom + 1;  GpiMove( hps, &pt );
    pt.y = rcl.yTop - 2;   GpiLine( hps, &pt );
    pt.x = rcl.xRight - 2; GpiLine( hps, &pt );
    GpiSetColor( hps, down ? RGBC(255, 255, 255) : RGBC(110, 110, 110) );
    pt.x = rcl.xRight - 2; pt.y = rcl.yBottom + 1;  GpiLine( hps, &pt );
    pt.x = rcl.xLeft + 1;  GpiLine( hps, &pt );

    /* picture and text (moved a little when pressed) */
    if( idx >= 0 )
        DrawButtonIcon( hps, idx, rcl.xLeft + 7 + (down ? 1 : 0),
                        rcl.yBottom + (h - 28) / 2 - (down ? 1 : 0), dis );

    WinQueryWindowText( pub->hwnd, sizeof(text), text );
    rt.xLeft   = rcl.xLeft + 38 + (down ? 1 : 0);
    rt.xRight  = rcl.xRight - 2;
    rt.yBottom = rcl.yBottom - (down ? 1 : 0);
    rt.yTop    = rcl.yTop - (down ? 1 : 0);
    WinDrawText( hps, -1, (PSZ)text, &rt, dis ? RGBC(120, 120, 120) : RGBC(0, 0, 0),
                 RGBC(204, 204, 204), DT_VCENTER | DT_CENTER | DT_TEXTATTRS );
}


/* A click on a button can reach us as WM_CONTROL (BN_CLICKED) and as
   WM_COMMAND: run it only once. */
static void ButtonClick(USHORT id)
{
    static ULONG  lastTime;
    static USHORT lastId;
    ULONG         now = WinGetCurrentTime( hab );

    if( id == lastId && now - lastTime < 400 )
        return;
    lastId   = id;
    lastTime = now;
    Command( id );
}


/*** Window procedure *********************************************************/

static MRESULT EXPENTRY ClientWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    HPS   hps;
    RECTL rcl;
    int   i;

    switch( msg )
    {
        case WM_CREATE:
            for( i = 0; i < 6; i++ )
            {
                WinCreateWindow( hwnd, WC_BUTTON, BtnText[i] >= 0 ? tr(BtnText[i]) : "",
                                 WS_VISIBLE | BS_USERBUTTON,
                                 BTN_X0 + i * (BTN_W + BTN_GAP), BTN_Y, BTN_W, BTN_H,
                                 hwnd, HWND_TOP, BtnIds[i], NULL, NULL );
            }
            return 0;

        case WM_ERASEBACKGROUND:
            return 0;

        case WM_PAINT:
            hps = WinBeginPaint( hwnd, NULLHANDLE, &rcl );
            PaintGame( hps );
            WinEndPaint( hps );
            return 0;

        case WM_CONTROL:
            for( i = 0; i < 6; i++ )
                if( BtnIds[i] == SHORT1FROMMP(mp1) )
                    break;
            if( i < 6 )
            {
                if( SHORT2FROMMP(mp1) == BN_PAINT )
                    DrawButton( (PUSERBUTTON)PVOIDFROMMP(mp2), i );
                else if( SHORT2FROMMP(mp1) == BN_CLICKED )
                    ButtonClick( SHORT1FROMMP(mp1) );
            }
            return 0;

        case WM_COMMAND:
            if( SHORT1FROMMP(mp2) == CMDSRC_ACCELERATOR || SHORT1FROMMP(mp2) == CMDSRC_MENU )
                Command( SHORT1FROMMP(mp1) );
            else
                ButtonClick( SHORT1FROMMP(mp1) );       /* from a button */
            return 0;
    }

    return WinDefWindowProc( hwnd, msg, mp1, mp2 );
}


/*** Start up and shut down ***************************************************/

static BOOL LoadBitmaps(void)
{
    HPS hps = WinGetPS( hwndClient );
    int i;

    for( i = 0; i < 52; i++ )
        hbmCard[i] = GpiLoadBitmap( hps, NULLHANDLE, BMP_CARD_BASE + i, 0, 0 );
    for( i = 0; i < NUM_BACKS; i++ )
        hbmBack[i] = GpiLoadBitmap( hps, NULLHANDLE, BMP_BACK_BASE + i, 0, 0 );

    WinReleasePS( hps );

    for( i = 0; i < 52; i++ )
        if( hbmCard[i] == NULLHANDLE )
            return FALSE;
    return hbmBack[0] != NULLHANDLE;
}


static void FreeBitmaps(void)
{
    int i;

    for( i = 0; i < 52; i++ )
        if( hbmCard[i] ) GpiDeleteBitmap( hbmCard[i] );
    for( i = 0; i < NUM_BACKS; i++ )
        if( hbmBack[i] ) GpiDeleteBitmap( hbmBack[i] );
}


int main(void)
{
    QMSG    qmsg;
    ULONG   flFrame = FCF_TITLEBAR | FCF_SYSMENU | FCF_MINBUTTON | FCF_BORDER |
                      FCF_TASKLIST | FCF_MENU | FCF_ACCELTABLE | FCF_ICON;
    RECTL   rcl;
    LONG    cxScreen, cyScreen, w, h;
    HWND    hMenu;

    hab = WinInitialize( 0 );
    hmq = WinCreateMsgQueue( hab, 0 );

    LoadPrefs();
    NumOfDecks  = Prefs.numdecks;
    MinimumBet  = Prefs.minbet;
    MaximumBet  = Prefs.maxbet;
    InitialBank = Prefs.initbank;
    BetMax      = Prefs.betmax;
    current_lang = Prefs.current_lang;

    GameSeed();
    GameInit();

    if( !WinRegisterClass( hab, PRG_NAME, ClientWndProc,
                           CS_SIZEREDRAW | CS_SYNCPAINT | CS_CLIPCHILDREN, 0 ) )
        return 1;

    /* created invisible (style 0), shown after it is sized */
    hwndFrame = WinCreateStdWindow( HWND_DESKTOP, 0, &flFrame, PRG_NAME, PRG_NAME,
                                    0, NULLHANDLE, ID_MAIN, &hwndClient );
    if( !hwndFrame )
        return 1;

    hwndObject = WinCreateWindow( HWND_OBJECT, WC_FRAME, "", 0L, 0, 0, 0, 0,
                                  NULLHANDLE, HWND_TOP, 0, NULL, NULL );

    if( !LoadBitmaps() )
    {
        WinMessageBox( HWND_DESKTOP, HWND_DESKTOP, "Can't load the card bitmaps.",
                       PRG_NAME, 0, MB_OK | MB_ERROR | MB_MOVEABLE );
        return 1;
    }

    hMenu = WinWindowFromID( hwndFrame, FID_MENU );
    set_language( hMenu, current_lang );
    WinCheckMenuItem( hMenu, IDM_SAVEONEXIT, Prefs.saveonexit );
    UpdateButtonTexts();
    UpdateButtons();
    Say( tr(STR_IDLE) );

    /* size the frame around the client area and center it */
    rcl.xLeft = 0; rcl.yBottom = 0; rcl.xRight = CLIENT_W; rcl.yTop = CLIENT_H;
    WinCalcFrameRect( hwndFrame, &rcl, FALSE );
    w = rcl.xRight - rcl.xLeft;
    h = rcl.yTop - rcl.yBottom;
    cxScreen = WinQuerySysValue( HWND_DESKTOP, SV_CXSCREEN );
    cyScreen = WinQuerySysValue( HWND_DESKTOP, SV_CYSCREEN );
    WinSetWindowPos( hwndFrame, HWND_TOP, (cxScreen - w) / 2, (cyScreen - h) / 2, w, h,
                     SWP_SIZE | SWP_MOVE | SWP_ACTIVATE | SWP_SHOW );

    SetHelpLanguage();

    while( WinGetMsg( hab, &qmsg, NULLHANDLE, 0, 0 ) )
        WinDispatchMsg( hab, &qmsg );

    if( Prefs.saveonexit )
        SavePrefs();

    CloseHelp();
    FreeBitmaps();
    if( hwndObject )
        WinDestroyWindow( hwndObject );
    WinDestroyWindow( hwndFrame );
    WinDestroyMsgQueue( hmq );
    WinTerminate( hab );

    return 0;
}
