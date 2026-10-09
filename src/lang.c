/*******************************************************************************
* LANG.C - translated texts and runtime language switching (plan.txt section 8)
*
* ASCII only: no accented letters in the strings.
*******************************************************************************/

#include <string.h>

#include "lang.h"

int current_lang = LANG_EN;

const char *lang_strings[LANG_COUNT][STR_COUNT] =
{
    /* ---- English ------------------------------------------------------- */
    {
        "~Game",
        "~New Game\tCtrl+N",
        "~Quit Game\tCtrl+Q",
        "E~xit\tCtrl+X",
        "~Options",
        "~Settings...",
        "~Language",
        "~Frame Controls\tCtrl+F",
        "~Save settings on exit",
        "~Help",
        "~Help for help...",
        "~Extended help...",
        "~Keys help...",
        "Help ~index...",
        "~About...",

        "Play",
        "Hit (H)",
        "Stay (S)",
        "Double (D)",
        "Split (P)",
        "Insure (I)",

        "Dealer",
        "Player",
        "Cards left",
        "Bank",
        "Bet",
        "Press Play (Ctrl+N) to deal a new hand.",

        "Push! Nobody wins this hand...",
        "The %s has won with %d.",
        "%s has drawn 5 cards without going over 21, %s wins.",
        "You don't have enough money for insurance (need %d).",
        "Dealer does not have 21.",
        "You don't have enough money to split.",
        "You don't have enough money to double down.",
        "You've exceeded 21.",
        "Dealer has exceeded 21.",
        "Scoring first hand of split.",
        "Scoring second hand of split.",
        "The deck was shuffled.",
        "You quit the hand, the bet is lost.",

        "You don't have enough money for a bet, reset game?",
        "Quit this hand? The bet will be lost.",

        "Place your bet",
        "Bet:",
        "Minimum bet",
        "Maximum bet",
        "Initial bank",
        " must be between %d and %d (without decimals)",

        "Settings",
        "Number of decks",
        "1 deck",
        "2 decks",
        "3 decks",
        "Bet the maximum by default",
        "Card back",
        "OK",
        "Cancel",

        "Can't open the help file '%s'. Put it in the 'help' folder next to TwentyOne.exe."
    },

    /* ---- Spanish ------------------------------------------------------- */
    {
        "~Juego",
        "~Nuevo Juego\tCtrl+N",
        "~Abandonar Juego\tCtrl+Q",
        "~Salir\tCtrl+X",
        "~Opciones",
        "~Ajustes...",
        "~Idioma",
        "~Controles de marco\tCtrl+F",
        "Guardar ajustes al sa~lir",
        "A~yuda",
        "~Como usar la ayuda...",
        "Ayuda ~extendida...",
        "A~yuda de teclas...",
        "~Indice de ayuda...",
        "~Acerca de...",

        "Jugar",
        "Carta (H)",
        "Plantarse (S)",
        "Doblar (D)",
        "Dividir (P)",
        "Seguro (I)",

        "Crupier",
        "Jugador",
        "Cartas",
        "Banco",
        "Apuesta",
        "Pulse Jugar (Ctrl+N) para repartir una mano nueva.",

        "Empate! Nadie gana esta mano...",
        "El %s ha ganado con %d.",
        "%s ha robado 5 cartas sin pasarse de 21, %s gana.",
        "No tiene dinero suficiente para el seguro (necesita %d).",
        "El crupier no tiene 21.",
        "No tiene dinero suficiente para dividir.",
        "No tiene dinero suficiente para doblar.",
        "Se ha pasado de 21.",
        "El crupier se ha pasado de 21.",
        "Puntuando la primera mano de la division.",
        "Puntuando la segunda mano de la division.",
        "Se ha barajado el mazo.",
        "Abandono la mano, la apuesta se pierde.",

        "No tiene dinero suficiente para apostar, reiniciar el juego?",
        "Abandonar esta mano? Se perdera la apuesta.",

        "Haga su apuesta",
        "Apuesta:",
        "Apuesta minima",
        "Apuesta maxima",
        "Banco inicial",
        " debe estar entre %d y %d (sin decimales)",

        "Ajustes",
        "Numero de mazos",
        "1 mazo",
        "2 mazos",
        "3 mazos",
        "Apostar el maximo por defecto",
        "Reverso de carta",
        "Aceptar",
        "Cancelar",

        "No se puede abrir el archivo de ayuda '%s'. Pongalo en la carpeta 'help' junto a TwentyOne.exe."
    },

    /* ---- Dutch --------------------------------------------------------- */
    {
        "~Spel",
        "~Nieuw Spel\tCtrl+N",
        "~Spel Stoppen\tCtrl+Q",
        "~Afsluiten\tCtrl+X",
        "~Opties",
        "~Instellingen...",
        "~Taal",
        "~Vensterbesturing\tCtrl+F",
        "Instellingen ~bewaren bij afsluiten",
        "~Help",
        "~Help bij help...",
        "~Uitgebreide help...",
        "~Toetsenhelp...",
        "Help~index...",
        "~Info...",

        "Spelen",
        "Kaart (H)",
        "Blijven (S)",
        "Verdubbel (D)",
        "Splits (P)",
        "Verzeker (I)",

        "Deler",
        "Speler",
        "Kaarten over",
        "Bank",
        "Inzet",
        "Druk op Spelen (Ctrl+N) voor een nieuwe hand.",

        "Gelijkspel! Niemand wint deze hand...",
        "De %s heeft gewonnen met %d.",
        "%s heeft 5 kaarten getrokken zonder boven 21 te komen, %s wint.",
        "U heeft niet genoeg geld voor de verzekering (nodig: %d).",
        "De deler heeft geen 21.",
        "U heeft niet genoeg geld om te splitsen.",
        "U heeft niet genoeg geld om te verdubbelen.",
        "U zit boven 21.",
        "De deler zit boven 21.",
        "Eerste hand van de splitsing wordt geteld.",
        "Tweede hand van de splitsing wordt geteld.",
        "De kaarten zijn geschud.",
        "U stopte de hand, de inzet is verloren.",

        "U heeft niet genoeg geld voor een inzet, spel opnieuw beginnen?",
        "Deze hand stoppen? De inzet gaat verloren.",

        "Doe uw inzet",
        "Inzet:",
        "Minimale inzet",
        "Maximale inzet",
        "Startbank",
        " moet tussen %d en %d liggen (zonder decimalen)",

        "Instellingen",
        "Aantal sets kaarten",
        "1 set",
        "2 sets",
        "3 sets",
        "Standaard maximaal inzetten",
        "Kaartrug",
        "OK",
        "Annuleren",

        "Kan het helpbestand '%s' niet openen. Zet het in de map 'help' naast TwentyOne.exe."
    },

    /* ---- German -------------------------------------------------------- */
    {
        "~Spiel",
        "~Neues Spiel\tCtrl+N",
        "Spiel ~beenden\tCtrl+Q",
        "~Verlassen\tCtrl+X",
        "~Optionen",
        "~Einstellungen...",
        "~Sprache",
        "~Fensterelemente\tCtrl+F",
        "Einstellungen beim Beenden ~speichern",
        "~Hilfe",
        "~Hilfe zur Hilfe...",
        "~Erweiterte Hilfe...",
        "~Tastenhilfe...",
        "Hilfe~index...",
        "~Info...",

        "Geben",
        "Karte (H)",
        "Halten (S)",
        "Verdoppeln (D)",
        "Teilen (P)",
        "Versichern (I)",

        "Geber",
        "Spieler",
        "Karten uebrig",
        "Bank",
        "Einsatz",
        "Zum Austeilen Geben (Ctrl+N) druecken.",

        "Unentschieden! Niemand gewinnt diese Hand...",
        "Der %s hat mit %d gewonnen.",
        "%s hat 5 Karten ohne ueber 21 zu kommen, %s gewinnt.",
        "Sie haben nicht genug Geld fuer die Versicherung (noetig: %d).",
        "Der Geber hat keine 21.",
        "Sie haben nicht genug Geld zum Teilen.",
        "Sie haben nicht genug Geld zum Verdoppeln.",
        "Sie haben 21 ueberschritten.",
        "Der Geber hat 21 ueberschritten.",
        "Erste Hand der Teilung wird gewertet.",
        "Zweite Hand der Teilung wird gewertet.",
        "Die Karten wurden gemischt.",
        "Sie haben die Hand beendet, der Einsatz ist verloren.",

        "Sie haben nicht genug Geld fuer einen Einsatz, Spiel zuruecksetzen?",
        "Diese Hand beenden? Der Einsatz geht verloren.",

        "Einsatz setzen",
        "Einsatz:",
        "Mindesteinsatz",
        "Hoechsteinsatz",
        "Startbank",
        " muss zwischen %d und %d liegen (ohne Dezimalstellen)",

        "Einstellungen",
        "Anzahl der Kartenspiele",
        "1 Spiel",
        "2 Spiele",
        "3 Spiele",
        "Standardmaessig Hoechsteinsatz",
        "Kartenrueckseite",
        "OK",
        "Abbrechen",

        "Die Hilfedatei '%s' kann nicht geoeffnet werden. Legen Sie sie in den Ordner 'help' neben TwentyOne.exe."
    },

    /* ---- French -------------------------------------------------------- */
    {
        "~Jeu",
        "~Nouvelle Partie\tCtrl+N",
        "~Quitter la Partie\tCtrl+Q",
        "Q~uitter\tCtrl+X",
        "~Options",
        "~Parametres...",
        "~Langue",
        "~Controles du cadre\tCtrl+F",
        "~Enregistrer les parametres en quittant",
        "~Aide",
        "~Aide sur l'aide...",
        "Aide ~etendue...",
        "Aide des ~touches...",
        "~Index de l'aide...",
        "A ~propos...",

        "Jouer",
        "Carte (H)",
        "Rester (S)",
        "Doubler (D)",
        "Separer (P)",
        "Assurer (I)",

        "Donneur",
        "Joueur",
        "Cartes restantes",
        "Banque",
        "Mise",
        "Appuyez sur Jouer (Ctrl+N) pour distribuer une main.",

        "Egalite ! Personne ne gagne cette main...",
        "Le %s a gagne avec %d.",
        "%s a tire 5 cartes sans depasser 21, %s gagne.",
        "Vous n'avez pas assez d'argent pour l'assurance (il faut %d).",
        "Le donneur n'a pas 21.",
        "Vous n'avez pas assez d'argent pour separer.",
        "Vous n'avez pas assez d'argent pour doubler.",
        "Vous avez depasse 21.",
        "Le donneur a depasse 21.",
        "Calcul de la premiere main separee.",
        "Calcul de la deuxieme main separee.",
        "Le jeu a ete melange.",
        "Vous avez quitte la main, la mise est perdue.",

        "Vous n'avez pas assez d'argent pour miser, reinitialiser le jeu ?",
        "Quitter cette main ? La mise sera perdue.",

        "Faites votre mise",
        "Mise :",
        "Mise minimum",
        "Mise maximum",
        "Banque initiale",
        " doit etre entre %d et %d (sans decimales)",

        "Parametres",
        "Nombre de jeux de cartes",
        "1 jeu",
        "2 jeux",
        "3 jeux",
        "Miser le maximum par defaut",
        "Dos de carte",
        "OK",
        "Annuler",

        "Impossible d'ouvrir le fichier d'aide '%s'. Placez-le dans le dossier 'help' a cote de TwentyOne.exe."
    },

    /* ---- Italian ------------------------------------------------------- */
    {
        "~Gioco",
        "~Nuova Partita\tCtrl+N",
        "~Abbandona Partita\tCtrl+Q",
        "~Esci\tCtrl+X",
        "~Opzioni",
        "~Impostazioni...",
        "~Lingua",
        "~Controlli della cornice\tCtrl+F",
        "~Salva impostazioni all'uscita",
        "~Aiuto",
        "~Aiuto sull'aiuto...",
        "Aiuto ~esteso...",
        "Aiuto sui ~tasti...",
        "~Indice dell'aiuto...",
        "~Informazioni...",

        "Gioca",
        "Carta (H)",
        "Stai (S)",
        "Raddoppia (D)",
        "Dividi (P)",
        "Assicura (I)",

        "Banco",
        "Giocatore",
        "Carte rimaste",
        "Cassa",
        "Puntata",
        "Premi Gioca (Ctrl+N) per distribuire una mano.",

        "Pari! Nessuno vince questa mano...",
        "Il %s ha vinto con %d.",
        "%s ha pescato 5 carte senza sballare, %s vince.",
        "Non hai abbastanza soldi per l'assicurazione (servono %d).",
        "Il banco non ha 21.",
        "Non hai abbastanza soldi per dividere.",
        "Non hai abbastanza soldi per raddoppiare.",
        "Hai superato 21.",
        "Il banco ha superato 21.",
        "Conteggio della prima mano divisa.",
        "Conteggio della seconda mano divisa.",
        "Il mazzo e stato mescolato.",
        "Hai abbandonato la mano, la puntata e persa.",

        "Non hai abbastanza soldi per puntare, azzerare la partita?",
        "Abbandonare questa mano? La puntata sara persa.",

        "Fai la tua puntata",
        "Puntata:",
        "Puntata minima",
        "Puntata massima",
        "Cassa iniziale",
        " deve essere tra %d e %d (senza decimali)",

        "Impostazioni",
        "Numero di mazzi",
        "1 mazzo",
        "2 mazzi",
        "3 mazzi",
        "Punta il massimo per impostazione predefinita",
        "Dorso della carta",
        "OK",
        "Annulla",

        "Impossibile aprire il file di aiuto '%s'. Mettilo nella cartella 'help' accanto a TwentyOne.exe."
    }
};


/*** Runtime switching ********************************************************/

static HWND get_submenu(HWND hMnu, USHORT id)
{
    MENUITEM mi;

    memset(&mi, 0, sizeof(mi));
    if( (BOOL)WinSendMsg(hMnu, MM_QUERYITEM, MPFROM2SHORT(id, FALSE), MPFROMP(&mi)) )
        return mi.hwndSubMenu;
    return NULLHANDLE;
}


static void menu_set_text(HWND hMnu, USHORT id, const char *text)
{
    WinSendMsg(hMnu, MM_SETITEMTEXT, MPFROMSHORT(id), MPFROMP((PSZ)text));
}


/* Re-label the menu bar for the current language */
void set_language(HWND hMenu, int lang)
{
    HWND hGame, hOptions, hHelp;
    int  i;

    if( lang < 0 || lang >= LANG_COUNT )
        lang = LANG_EN;
    current_lang = lang;

    if( hMenu == NULLHANDLE )
        return;

    hGame    = get_submenu(hMenu, IDM_SUBMENU_GAME);
    hOptions = get_submenu(hMenu, IDM_SUBMENU_OPTIONS);
    hHelp    = get_submenu(hMenu, IDM_SUBMENU_HELP);

    menu_set_text(hMenu, IDM_SUBMENU_GAME,    tr(STR_GAME));
    menu_set_text(hMenu, IDM_SUBMENU_OPTIONS, tr(STR_OPTIONS));
    menu_set_text(hMenu, IDM_SUBMENU_HELP,    tr(STR_HELP));

    if( hGame )
    {
        menu_set_text(hGame, IDM_PLAY, tr(STR_NEW));
        menu_set_text(hGame, IDM_QUIT, tr(STR_QUIT));
        menu_set_text(hGame, IDM_EXIT, tr(STR_EXIT));
    }

    if( hOptions )
    {
        menu_set_text(hOptions, IDM_SETTINGS,      tr(STR_SETTINGS));
        menu_set_text(hOptions, IDM_SUBMENU_LANG,  tr(STR_LANGUAGE));
        menu_set_text(hOptions, IDM_FRAME,         tr(STR_FRAME));
        menu_set_text(hOptions, IDM_SAVEONEXIT,    tr(STR_SAVEONEXIT));
    }

    if( hHelp )
    {
        menu_set_text(hHelp, IDM_HELPUSING, tr(STR_USINGHELP));
        menu_set_text(hHelp, IDM_HELPEXT,   tr(STR_EXTHELP));
        menu_set_text(hHelp, IDM_HELPKEYS,  tr(STR_KEYSHELP));
        menu_set_text(hHelp, IDM_HELPINDEX, tr(STR_HELPINDEX));
        menu_set_text(hHelp, IDM_ABOUT,     tr(STR_ABOUT));
    }

    for( i = 0; i < LANG_COUNT; i++ )
        WinCheckMenuItem(hMenu, (USHORT)(IDM_LANG_EN + i), (current_lang == i));
}
