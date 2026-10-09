:userdoc.
:title.Aide de TwentyOne
:docprof toc=12.
:h1 res=1000.General
:p.TwentyOne est un jeu de cartes dans lequel vous essayez de vous rapprocher de 21 plus que le donneur (la banque) sans le depasser. C'est une version du Blackjack pour OS/2 Presentation Manager, ecrite par Michael G. Slack en 2001.
:p.Jouez une main avec le bouton Jouer (Ctrl+N). Apres votre mise, deux cartes vous sont donnees et deux au donneur; la premiere carte du donneur reste face cachee.
:p.:link reftype=hd res=1100.Regles:elink.
:p.:link reftype=hd res=1200.Touches et boutons:elink.
:p.:link reftype=hd res=1300.Parametres:elink.
:p.:link reftype=hd res=1400.A propos de TwentyOne:elink.
:h1 res=1100.Regles
:ul.
:li.Le donneur tire a 16 ou moins et reste a 17 ou plus.
:li.Apres une separation, chacune des deux mains recoit une carte de plus.
:li.Doubler n'est possible qu'au debut de la main et donne exactement une carte de plus.
:li.L'assurance peut etre achetee quand le donneur montre un as. Elle coute 25% de la mise (rien pour une mise de 1).
:li.Si vous ou le donneur tirez 5 cartes sans depasser 21, ce joueur gagne automatiquement.
:li.Si le donneur ou vous avez 21 au depart, celui-la gagne (sauf si l'assurance est possible, ou si les deux ont 21).
:li.Les as valent 1 ou 11. La main la plus proche de 21 sans depasser gagne.
:li.On ne peut separer que deux cartes de meme valeur, et chaque main recoit exactement une carte de plus.
:eul.
:p.Ce jeu ne suit pas toutes les regles du 21 et n'est pas fait pour parier pour de vrai.
:h1 res=1200.Touches et boutons
:p.Ces boutons et touches servent pendant une main&colon.
:table cols='22 12 46' rules=both frame=box.
:row.:c.:hp2.Action:ehp2.:c.:hp2.Touche:ehp2.:c.:hp2.Effet:ehp2.
:row.:c.Jouer:c.Ctrl+N:c.Distribuer une nouvelle main (ignore pendant une main)
:row.:c.Carte:c.H:c.Prendre une carte de plus
:row.:c.Rester:c.S:c.Garder vos cartes, le donneur joue
:row.:c.Doubler:c.D:c.Doubler la mise, une carte, puis le donneur joue
:row.:c.Separer:c.P:c.Separer une paire en deux mains
:row.:c.Assurer:c.I:c.Acheter l'assurance quand le donneur montre un as
:row.:c.Quitter la Partie:c.Ctrl+Q:c.Abandonner la main (la mise est perdue)
:row.:c.Quitter:c.Ctrl+X:c.Fermer le programme
:row.:c.Controles du cadre:c.Ctrl+F:c.Masquer ou afficher la barre de titre et le menu
:etable.
:h1 res=1300.Parametres
:p.Options - Parametres modifie le jeu. Les parametres sont enregistres en quittant si Enregistrer les parametres en quittant est coche.
:parml tsize=24 break=none.
:pt.Nombre de jeux de cartes
:pd.Un a trois jeux. Un changement vaut des le prochain melange.
:pt.Mise minimum
:pd.La plus petite mise, de 1 a 10000. Par defaut 1.
:pt.Mise maximum
:pd.La plus grande mise, de la mise minimum a 10000. Par defaut 5.
:pt.Banque initiale
:pd.L'argent recu au debut et a chaque reinitialisation, de la mise minimum a 100000. Par defaut 100.
:pt.Miser le maximum par defaut
:pd.La boite de mise commence avec la plus grande mise possible au lieu de la minimum.
:pt.Dos de carte
:pd.Choisissez l'image du dos des cartes.
:pt.Langue
:pd.Langue de l'interface et de l'aide (anglais, espagnol, neerlandais, allemand, francais, italien).
:pt.Controles du cadre
:pd.Masquer ou afficher la barre de titre et le menu (Ctrl+F).
:pt.Enregistrer les parametres en quittant
:pd.Enregistre les parametres dans TwentyOne.cfg en quittant.
:eparml.
:p.Si votre banque est inferieure a la mise minimum au debut d'une main, on vous demande s'il faut reinitialiser le jeu. Une reinitialisation ajoute la banque initiale a ce que vous avez.
:h1 res=1400.A propos de TwentyOne
:p.TwentyOne 1.6 pour OS/2, ArcaOS et eComStation.
:p.Auteur original&colon. Michael G. Slack (2001).
:p.Portage vers Open Watcom 2.0&colon. communaute OS2World (2026).
:p.Les images des cartes viennent des cartes de Compulsive Gambler. Le jeu original utilisait les images QCard du domaine public de Stephen Murphy et Daniel Di Bacco.
:p.Licence&colon. GNU General Public License v3.
:euserdoc.
