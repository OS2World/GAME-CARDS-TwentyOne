:userdoc.
:title.TwentyOne Help
:docprof toc=12.
:h1 res=1000.General
:p.TwentyOne is a card game in which you try to get closer to 21 than the dealer (the house) without going over. It is a version of Blackjack for OS/2 Presentation Manager, written by Michael G. Slack in 2001.
:p.Play a hand with the Play button (Ctrl+N). After you place your bet two cards are dealt to you and two to the dealer; the first dealer card stays face down.
:p.:link reftype=hd res=1100.Rules:elink.
:p.:link reftype=hd res=1200.Keys and buttons:elink.
:p.:link reftype=hd res=1300.Settings:elink.
:p.:link reftype=hd res=1400.About TwentyOne:elink.
:h1 res=1100.Rules
:ul.
:li.The dealer hits on 16 or less and stays on 17 or more.
:li.A split plays one more card on each of the two hands.
:li.Double down is only possible at the start of a hand and takes exactly one more card.
:li.Insurance can be bought when the dealer shows an ace. It costs 25% of the bet (nothing for a bet of 1).
:li.If you or the dealer draw 5 cards without going over 21, that player wins automatically.
:li.If the dealer or you have 21 to start, that one wins (unless insurance can be bought, or both have 21).
:li.Aces count as 1 or 11. The closest hand to 21 without going over wins.
:li.You can only split two cards of the same value, and each hand gets exactly one more card.
:eul.
:p.This game does not follow every little rule of 21 and is not meant for real gambling.
:h1 res=1200.Keys and buttons
:p.These buttons and keys are used while a hand is played&colon.
:table cols='22 12 46' rules=both frame=box.
:row.:c.:hp2.Action:ehp2.:c.:hp2.Key:ehp2.:c.:hp2.What it does:ehp2.
:row.:c.Play:c.Ctrl+N:c.Deal a new hand (ignored while a hand is played)
:row.:c.Hit:c.H:c.Take one more card
:row.:c.Stay:c.S:c.Keep your cards, the dealer plays
:row.:c.Double:c.D:c.Double the bet, take one card, then the dealer plays
:row.:c.Split:c.P:c.Split a pair into two hands
:row.:c.Insure:c.I:c.Buy insurance when the dealer shows an ace
:row.:c.Quit Game:c.Ctrl+Q:c.Give up the hand (the bet is lost)
:row.:c.Exit:c.Ctrl+X:c.Close the program
:row.:c.Frame Controls:c.Ctrl+F:c.Hide or show the title bar and menu
:etable.
:h1 res=1300.Settings
:p.Options - Settings changes the game. The settings are saved when you exit if Save settings on exit is checked.
:parml tsize=24 break=none.
:pt.Number of decks
:pd.Use one to three decks. A change takes effect with the next shuffle.
:pt.Minimum bet
:pd.The smallest bet, from 1 to 10000. Default 1.
:pt.Maximum bet
:pd.The largest bet, from the minimum bet to 10000. Default 5.
:pt.Initial bank
:pd.The money you get at the start and with each reset, from the minimum bet to 100000. Default 100.
:pt.Bet the maximum by default
:pd.The bet dialog starts with the largest bet you can place instead of the minimum.
:pt.Card back
:pd.Choose the picture on the back of the cards.
:pt.Language
:pd.User interface and help language (English, Spanish, Dutch, German, French, Italian).
:pt.Frame Controls
:pd.Hide or show the title bar and the menu (Ctrl+F).
:pt.Save settings on exit
:pd.Save the settings in TwentyOne.cfg when you exit.
:eparml.
:p.If your bank is lower than the minimum bet when you start a hand, you are asked whether to reset the game. A reset adds the initial bank to what you have.
:h1 res=1400.About TwentyOne
:p.TwentyOne 1.6 for OS/2, ArcaOS and eComStation.
:p.Original author&colon. Michael G. Slack (2001).
:p.Port to Open Watcom 2.0&colon. OS2World community (2026).
:p.The card pictures come from the Compulsive Gambler cards. The original game used the public domain QCard images by Stephen Murphy and Daniel Di Bacco.
:p.Licence&colon. GNU General Public License v3.
:euserdoc.
