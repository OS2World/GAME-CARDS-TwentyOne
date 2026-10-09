:userdoc.
:title.Ayuda de TwentyOne
:docprof toc=12.
:h1 res=1000.General
:p.TwentyOne es un juego de cartas en el que intenta acercarse a 21 mas que el crupier (la casa) sin pasarse. Es una version del Blackjack para OS/2 Presentation Manager, escrita por Michael G. Slack en 2001.
:p.Juegue una mano con el boton Jugar (Ctrl+N). Tras hacer su apuesta se reparten dos cartas a usted y dos al crupier; la primera carta del crupier queda boca abajo.
:p.:link reftype=hd res=1100.Reglas:elink.
:p.:link reftype=hd res=1200.Teclas y botones:elink.
:p.:link reftype=hd res=1300.Ajustes:elink.
:p.:link reftype=hd res=1400.Acerca de TwentyOne:elink.
:h1 res=1100.Reglas
:ul.
:li.El crupier pide carta con 16 o menos y se planta con 17 o mas.
:li.Al dividir, cada una de las dos manos recibe una carta mas.
:li.Doblar solo es posible al comienzo de la mano y se recibe exactamente una carta mas.
:li.El seguro se puede comprar cuando el crupier muestra un as. Cuesta el 25% de la apuesta (nada para una apuesta de 1).
:li.Si usted o el crupier roban 5 cartas sin pasarse de 21, ese jugador gana automaticamente.
:li.Si el crupier o usted tienen 21 al empezar, gana ese (salvo que se pueda comprar seguro, o que ambos tengan 21).
:li.Los ases valen 1 u 11. Gana la mano mas cercana a 21 sin pasarse.
:li.Solo se pueden dividir dos cartas del mismo valor, y cada mano recibe exactamente una carta mas.
:eul.
:p.Este juego no sigue todas las reglas del 21 y no esta pensado para apostar de verdad.
:h1 res=1200.Teclas y botones
:p.Estos botones y teclas se usan mientras se juega una mano&colon.
:table cols='22 12 46' rules=both frame=box.
:row.:c.:hp2.Accion:ehp2.:c.:hp2.Tecla:ehp2.:c.:hp2.Que hace:ehp2.
:row.:c.Jugar:c.Ctrl+N:c.Repartir una mano nueva (se ignora durante una mano)
:row.:c.Carta:c.H:c.Pedir una carta mas
:row.:c.Plantarse:c.S:c.Quedarse con sus cartas, juega el crupier
:row.:c.Doblar:c.D:c.Doblar la apuesta, recibir una carta y juega el crupier
:row.:c.Dividir:c.P:c.Dividir una pareja en dos manos
:row.:c.Seguro:c.I:c.Comprar seguro cuando el crupier muestra un as
:row.:c.Abandonar Juego:c.Ctrl+Q:c.Abandonar la mano (se pierde la apuesta)
:row.:c.Salir:c.Ctrl+X:c.Cerrar el programa
:row.:c.Controles de marco:c.Ctrl+F:c.Ocultar o mostrar la barra de titulo y el menu
:etable.
:h1 res=1300.Ajustes
:p.Opciones - Ajustes cambia el juego. Los ajustes se guardan al salir si Guardar ajustes al salir esta marcado.
:parml tsize=24 break=none.
:pt.Numero de mazos
:pd.Use de uno a tres mazos. Un cambio vale desde la siguiente barajada.
:pt.Apuesta minima
:pd.La apuesta mas pequena, de 1 a 10000. Por defecto 1.
:pt.Apuesta maxima
:pd.La apuesta mas grande, desde la apuesta minima hasta 10000. Por defecto 5.
:pt.Banco inicial
:pd.El dinero que recibe al empezar y con cada reinicio, desde la apuesta minima hasta 100000. Por defecto 100.
:pt.Apostar el maximo por defecto
:pd.El dialogo de apuesta empieza con la apuesta mas grande posible en lugar de la minima.
:pt.Reverso de carta
:pd.Elija el dibujo del reverso de las cartas.
:pt.Idioma
:pd.Idioma de la interfaz y de la ayuda (ingles, espanol, neerlandes, aleman, frances, italiano).
:pt.Controles de marco
:pd.Oculta o muestra la barra de titulo y el menu (Ctrl+F).
:pt.Guardar ajustes al salir
:pd.Guarda los ajustes en TwentyOne.cfg al salir.
:eparml.
:p.Si su banco es menor que la apuesta minima al empezar una mano, se le pregunta si desea reiniciar el juego. Un reinicio suma el banco inicial a lo que tiene.
:h1 res=1400.Acerca de TwentyOne
:p.TwentyOne 1.6 para OS/2, ArcaOS y eComStation.
:p.Autor original&colon. Michael G. Slack (2001).
:p.Port a Open Watcom 2.0&colon. comunidad OS2World (2026).
:p.Las imagenes de las cartas proceden de las cartas de Compulsive Gambler. El juego original usaba las imagenes QCard de dominio publico de Stephen Murphy y Daniel Di Bacco.
:p.Licencia&colon. GNU General Public License v3.
:euserdoc.
