# TwentyOne for ArcaOS / eComStation / OS/2

TwentyOne is a Blackjack card game for the OS/2 Presentation Manager: get closer to 21 than the dealer without going over.
Originally written by Michael G. Slack in 2001 (Sibyl, version 1.04). Ported to C and Open Watcom 2.0 in 2026.

## Version

1.6

## License

GNU General Public License v3 - see `doc/LICENSE.txt`

## Features

- One to three decks; bet limits and start money are settings
- Hit, Stay, Double down, Split and Insurance
- 6-language interface **and** online help: English, Spanish, Dutch, German, French, Italian (switch at runtime in Options - Language)
- Six card backs to choose from
- Large cards (window about 960 x 640)
- Frame Controls toggle (Ctrl+F) for borderless play
- Settings saved to `TwentyOne.cfg` on exit (can be switched off)

## Rules

Dealer hits on 16 or less and stays on 17+. A split plays one more card on each hand. Double down only at the start (one card). Insurance (25% of the bet) when the dealer shows an ace. Five cards without going over 21 win automatically. Closest to 21 without going over wins.

## Keys

| Key | Action |
|-----|--------|
| Ctrl+N | Play (deal a new hand) |
| H / S / D / P / I | Hit / Stay / Double / Split / Insure |
| Ctrl+Q | Quit the hand (asks first, the bet is lost) |
| Ctrl+X | Exit |
| Ctrl+F | Frame Controls on/off |

## Installation

Run `TwentyOne.exe`. Keep the `help\` folder (one `.hlp` per language) next to it. `TwentyOne.cfg` is created in the folder you start the program from.

## Compile Tools

- OpenWatcom 2.0 (`wmake`, `wcc386`, `wlink`, `wrc`, `wipfc`)
- OS/2 Toolkit 4.5

## Build

```
compile-wat.cmd
```

Output: `bin\TwentyOne.exe` and `bin\help\TwentyOne_xx.hlp`

## Requirements

- OS/2 Warp 4, eComStation, or ArcaOS
- 32-bit Presentation Manager

## Authors

- Original: Michael G. Slack - 2001
- OS/2 port: OS2World community - 2026
- Card pictures: from the Compulsive Gambler cards (the original used the public domain QCard images by Stephen Murphy and Daniel Di Bacco)

## Links

- OS2World: https://www.os2world.com/games/index.php/native-games/cards-dice/153-twentyone
