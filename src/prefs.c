/*******************************************************************************
* PREFS.C - load and save the settings
*******************************************************************************/

#include <stdio.h>
#include <string.h>

#include "twenty.h"
#include "game.h"
#include "prefs.h"

struct PrefsStruct Prefs;


void InitPrefs(void)
{
    memset( &Prefs, 0, sizeof(Prefs) );

    Prefs.saveonexit   = 1;
    Prefs.detaillevel  = 1;
    Prefs.current_lang = LANG_EN;
    Prefs.cardback     = 0;
    Prefs.numdecks     = 1;
    Prefs.minbet       = MIN_BET_AMT;
    Prefs.maxbet       = START_MAX_BET;
    Prefs.initbank     = START_BANK;
    Prefs.betmax       = 0;
}


/* Reset everything that is outside its range */
static void Validate(void)
{
    if( Prefs.saveonexit != 0 && Prefs.saveonexit != 1 )
        Prefs.saveonexit = 1;
    if( Prefs.detaillevel < 0 || Prefs.detaillevel > 2 )
        Prefs.detaillevel = 1;
    if( Prefs.current_lang < 0 || Prefs.current_lang >= LANG_COUNT )
        Prefs.current_lang = LANG_EN;
    if( Prefs.cardback < 0 || Prefs.cardback >= NUM_BACKS )
        Prefs.cardback = 0;
    if( Prefs.numdecks < 1 || Prefs.numdecks > MAX_DECKS )
        Prefs.numdecks = 1;
    if( Prefs.minbet < MIN_BET_AMT || Prefs.minbet > MAX_BET_AMT )
        Prefs.minbet = MIN_BET_AMT;
    if( Prefs.maxbet < Prefs.minbet || Prefs.maxbet > MAX_BET_AMT )
        Prefs.maxbet = Prefs.minbet;
    if( Prefs.initbank < Prefs.minbet || Prefs.initbank > MAX_BANK_AMT )
        Prefs.initbank = Prefs.minbet;
    if( Prefs.initbank < Prefs.minbet * 2 )
        Prefs.initbank = Prefs.minbet * 2;
    Prefs.betmax = (Prefs.betmax != 0);
}


void LoadPrefs(void)
{
    FILE *f;

    InitPrefs();

    f = fopen( CFG_FILE, "rb" );
    if( f )
    {
        fread( &Prefs, 1, sizeof(Prefs), f );   /* a shorter file is fine    */
        fclose( f );
    }

    Validate();
}


int SavePrefs(void)
{
    FILE *f = fopen( CFG_FILE, "wb" );

    if( !f )
        return 0;

    fwrite( &Prefs, 1, sizeof(Prefs), f );
    fclose( f );
    return 1;
}
