TwentyOne for ArcaOS / eComStation / OS/2
=========================================
Version 1.6

DESCRIPTION
-----------
TwentyOne is a card game (Blackjack) for OS/2 Presentation Manager. You
try to get closer to 21 than the dealer without going over. You play
against the house with a bank of money.

Originally written by Michael G. Slack in 2001 (Sibyl, version 1.04).
Ported to C and OpenWatcom 2.0 by the OS2World community in 2026.

Features:
  - One to three decks, bet limits and start money you can set
  - Hit, Stay, Double down, Split and Insurance
  - 6-language interface and online help: English, Spanish, Dutch,
    German, French, Italian (switch at runtime in Options - Language)
  - Six card backs to choose from
  - Large cards, window about 960 x 640
  - Frame Controls toggle (Ctrl+F) for borderless play
  - Settings are saved to TwentyOne.cfg on exit (can be switched off)

RULES
-----
  - The dealer hits on 16 or less and stays on 17 or more.
  - A split plays one more card on each of the two hands.
  - Double down is only possible at the start and takes one more card.
  - Insurance can be bought when the dealer shows an ace. It costs 25%
    of the bet (nothing for a bet of 1).
  - If you or the dealer draw 5 cards without going over 21, that
    player wins automatically.
  - If the dealer or you have 21 to start, that one wins (unless
    insurance can be bought, or both have 21).
  - Aces count 1 or 11. The closest to 21 without going over wins.
  This game does not follow every rule of 21 and is not meant for real
  betting or gambling.

HOW TO PLAY
-----------
  Press Play (Ctrl+N), choose your bet and the cards are dealt. The first
  dealer card is face down.

    Play     Ctrl+N   deal a new hand (ignored while a hand is played)
    Hit      H        take one more card
    Stay     S        keep your cards, the dealer plays
    Double   D        double the bet, take one card, then the dealer plays
    Split    P        split a pair into two hands
    Insure   I        buy insurance when the dealer shows an ace

KEYBOARD SHORTCUTS
------------------
  Ctrl+N    New game (deal a hand)
  Ctrl+Q    Quit the hand (asks first, the bet is lost)
  Ctrl+X    Exit the program
  Ctrl+F    Toggle Frame Controls (hides title bar and menu)

SETTINGS (Options - Settings)
-----------------------------
  Number of decks (1-3), minimum bet, maximum bet, initial bank, bet the
  maximum by default, and the card back. A change of the number of decks
  takes effect with the next shuffle. If your bank is below the minimum
  bet you are asked whether to reset the game (the initial bank is added).

  Settings are stored in TwentyOne.cfg in the current directory.
  Delete TwentyOne.cfg to reset all settings to the defaults.

REQUIREMENTS
------------
  - OS/2 Warp 4, eComStation, or ArcaOS
  - 32-bit Presentation Manager

INSTALLATION
------------
  Copy the program folder to any place and run TwentyOne.exe. Keep the
  help\ folder (TwentyOne_en.hlp, ...) next to TwentyOne.exe for the
  online help.

COMPILING FROM SOURCE
---------------------
  Requirements: OpenWatcom 2.0 (wmake, wcc386, wlink, wrc, wipfc) and the
  OS/2 Toolkit 4.5.

  Build:    compile-wat.cmd
  Output:   bin\TwentyOne.exe and bin\help\TwentyOne_xx.hlp

CREDITS
-------
  Original author:  Michael G. Slack (2001)
  OS/2 port:        OS2World community (2026)
  Card pictures:    from the Compulsive Gambler cards. The original
                    program used the public domain QCard images by
                    Stephen Murphy and Daniel Di Bacco.
  OS2World site:    https://www.os2world.com

LICENSE
-------
  GNU General Public License v3 - see doc\LICENSE.txt
