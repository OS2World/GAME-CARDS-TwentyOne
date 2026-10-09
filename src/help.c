/*******************************************************************************
* HELP.C - online help, one IPF library per language (plan.txt section 17)
*
* The help files are looked for in <exe dir>\help\ first, then in the exe dir.
*******************************************************************************/

#include <stdio.h>
#include <string.h>

#include "twenty.h"
#include "lang.h"
#include "help.h"

extern HAB  hab;
extern HWND hwndFrame;

static HWND hwndHelp;

static char *szHelpFiles[LANG_COUNT] =
{
    "TwentyOne_en.hlp",
    "TwentyOne_es.hlp",
    "TwentyOne_nl.hlp",
    "TwentyOne_de.hlp",
    "TwentyOne_fr.hlp",
    "TwentyOne_it.hlp"
};

static char szLibName[CCHMAXPATH];
static char szHelpTitle[] = "TwentyOne Help";


/* Look for the help file next to the executable (help\ first) */
static void FindHelpFile(const char *name, char *out)
{
    char        szExe[CCHMAXPATH];
    char        szTry[CCHMAXPATH];
    FILESTATUS3 fs;
    char        *p;
    PTIB        ptib;
    PPIB        ppib;

    strcpy( out, name );
    DosGetInfoBlocks( &ptib, &ppib );
    if( DosQueryModuleName( ppib->pib_hmte, sizeof(szExe), szExe ) )
        return;
    p = strrchr( szExe, '\\' );
    if( p == NULL )
        return;
    p[1] = 0;

    sprintf( szTry, "%shelp\\%s", szExe, name );
    if( !DosQueryPathInfo( szTry, FIL_STANDARD, &fs, sizeof(fs) ) )
    {
        strcpy( out, szTry );
        return;
    }
    sprintf( szTry, "%s%s", szExe, name );
    if( !DosQueryPathInfo( szTry, FIL_STANDARD, &fs, sizeof(fs) ) )
        strcpy( out, szTry );
}


static void EnableHelpMenu(BOOL on)
{
    HWND hMenu = WinWindowFromID( hwndFrame, FID_MENU );

    if( hMenu == NULLHANDLE )
        return;
    WinEnableMenuItem( hMenu, IDM_HELPUSING, on );
    WinEnableMenuItem( hMenu, IDM_HELPEXT,   on );
    WinEnableMenuItem( hMenu, IDM_HELPKEYS,  on );
    WinEnableMenuItem( hMenu, IDM_HELPINDEX, on );
}


/* (Re)create the help instance for the current language */
BOOL SetHelpLanguage(void)
{
    HELPINIT hini;
    char     msg[CCHMAXPATH + 160];

    if( hwndHelp != NULLHANDLE )
        WinDestroyHelpInstance( hwndHelp );
    hwndHelp = NULLHANDLE;
    EnableHelpMenu( FALSE );

    memset( &hini, 0, sizeof(hini) );
    FindHelpFile( szHelpFiles[current_lang], szLibName );

    hini.cb                       = sizeof(HELPINIT);
    hini.phtHelpTable             = (PHELPTABLE)(0xFFFF0000 | HELP_TABLE_ID);
    hini.pszHelpWindowTitle       = (PSZ)szHelpTitle;
    hini.fShowPanelId             = CMIC_HIDE_PANEL_ID;
    hini.pszHelpLibraryName       = (PSZ)szLibName;

    hwndHelp = WinCreateHelpInstance( hab, &hini );
    if( hwndHelp == NULLHANDLE || hini.ulReturnCode )
    {
        hwndHelp = NULLHANDLE;
        sprintf( msg, tr(STR_HELP_ERR), szHelpFiles[current_lang] );
        WinMessageBox( HWND_DESKTOP, hwndFrame, msg, PRG_NAME, 0,
                       MB_OK | MB_WARNING | MB_MOVEABLE );
        return FALSE;
    }

    if( !WinAssociateHelpInstance( hwndHelp, hwndFrame ) )
        return FALSE;

    EnableHelpMenu( TRUE );
    return TRUE;
}


void CloseHelp(void)
{
    if( hwndHelp != NULLHANDLE )
        WinDestroyHelpInstance( hwndHelp );
    hwndHelp = NULLHANDLE;
}


void HelpPanel(long idPanel)
{
    if( hwndHelp )
        WinSendMsg( hwndHelp, HM_DISPLAY_HELP, MPFROMLONG(idPanel),
                    MPFROMSHORT(HM_RESOURCEID) );
}


void HelpForHelp(void)
{
    if( hwndHelp )
        WinSendMsg( hwndHelp, HM_DISPLAY_HELP, MPVOID, MPVOID );
}


void HelpContents(void)
{
    if( hwndHelp )
        WinSendMsg( hwndHelp, HM_HELP_CONTENTS, MPVOID, MPVOID );
}


void HelpIndex(void)
{
    if( hwndHelp )
        WinSendMsg( hwndHelp, HM_HELP_INDEX, MPVOID, MPVOID );
}
