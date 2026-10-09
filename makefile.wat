# Makefile for TwentyOne (Open Watcom C on OS/2 / ArcaOS)
# wmake 2.0.1 on ArcaOS - explicit per-file rules (no pattern rules)

# ============================================================================
# Configuration
# ============================================================================

NAME    = TwentyOne
SRCDIR  = src
BINDIR  = bin

!ifndef WATCOM
WATCOM  = C:\WATCOM
!endif

!ifndef WIPFC
WIPFC   = $(WATCOM)\wipfc
!endif

!ifndef OS2TK
OS2TK   = C:\OS2TK45
!endif

# ============================================================================
# Tools
# ============================================================================

CC      = wcc386
LINK    = wlink
RC      = wrc
IPFC    = wipfc

# ============================================================================
# Flags (per plan.txt section 2)
# ============================================================================

CFLAGS  = -bt=os2 -mf -5 -fpi -Oaxt -W3 -ze -d0
CFLAGS  = $(CFLAGS) -i=$(OS2TK)\h -i=$(SRCDIR)

RCFLAGS = -r -bt=os2 -i=$(OS2TK)\h -i=$(SRCDIR)

LFLAGS  = system os2v2_pm
LFLAGS  = $(LFLAGS) option stack=65536
LFLAGS  = $(LFLAGS) option map=$(BINDIR)\$(NAME).map

# ============================================================================
# Source files
# ============================================================================

GAME_OBJS = $(BINDIR)\main.obj $(BINDIR)\game.obj $(BINDIR)\lang.obj $(BINDIR)\prefs.obj $(BINDIR)\help.obj $(BINDIR)\dialogs.obj

OBJS    = $(GAME_OBJS)

HLPDIR  = $(BINDIR)\help
HELPS   = $(HLPDIR)\TwentyOne_en.hlp $(HLPDIR)\TwentyOne_es.hlp $(HLPDIR)\TwentyOne_nl.hlp $(HLPDIR)\TwentyOne_de.hlp $(HLPDIR)\TwentyOne_fr.hlp $(HLPDIR)\TwentyOne_it.hlp

ROBJ    = $(BINDIR)\$(NAME).res
RCFILE  = $(SRCDIR)\$(NAME).rc
DEFFILE = $(SRCDIR)\$(NAME).def

# ============================================================================
# Targets
# ============================================================================

all : $(BINDIR)\$(NAME).exe helpfiles .SYMBOLIC

helpfiles : $(HELPS) .SYMBOLIC
	@echo Help files done

$(BINDIR) :
	@if not exist $(BINDIR) mkdir $(BINDIR)

$(BINDIR)\$(NAME).exe : $(OBJS) $(ROBJ) $(DEFFILE)
	@echo Linking $(NAME).exe...
	@$(LINK) $(LFLAGS) name $(BINDIR)\$(NAME).exe file $(BINDIR)\*.obj library os2386.lib
	@echo Binding resources...
	@$(RC) -q -bt=os2 -fe=$(BINDIR)\$(NAME).exe $(ROBJ) $(BINDIR)\$(NAME).exe
	@if exist $(BINDIR)\$(NAME).exe echo BUILD OK

# ============================================================================
# Game objects
# ============================================================================

$(BINDIR)\main.obj : $(SRCDIR)\main.c $(SRCDIR)\twenty.h $(BINDIR)
	@echo Compiling src\main.c
	@$(CC) $(CFLAGS) -fo=$@ $(SRCDIR)\main.c

$(BINDIR)\game.obj : $(SRCDIR)\game.c $(SRCDIR)\twenty.h $(BINDIR)
	@echo Compiling src\game.c
	@$(CC) $(CFLAGS) -fo=$@ $(SRCDIR)\game.c

$(BINDIR)\lang.obj : $(SRCDIR)\lang.c $(SRCDIR)\twenty.h $(BINDIR)
	@echo Compiling src\lang.c
	@$(CC) $(CFLAGS) -fo=$@ $(SRCDIR)\lang.c

$(BINDIR)\prefs.obj : $(SRCDIR)\prefs.c $(SRCDIR)\twenty.h $(BINDIR)
	@echo Compiling src\prefs.c
	@$(CC) $(CFLAGS) -fo=$@ $(SRCDIR)\prefs.c

$(BINDIR)\help.obj : $(SRCDIR)\help.c $(SRCDIR)\twenty.h $(BINDIR)
	@echo Compiling src\help.c
	@$(CC) $(CFLAGS) -fo=$@ $(SRCDIR)\help.c

$(BINDIR)\dialogs.obj : $(SRCDIR)\dialogs.c $(SRCDIR)\twenty.h $(BINDIR)
	@echo Compiling src\dialogs.c
	@$(CC) $(CFLAGS) -fo=$@ $(SRCDIR)\dialogs.c

# ============================================================================
# Resources. The bitmap files are referenced relative to the working
# directory, so wmake must run from the project root (see compile-wat.cmd).
# ============================================================================

$(ROBJ) : $(RCFILE) $(SRCDIR)\twenty.h src\TwentyOne.ico $(BINDIR)
	@echo Compiling resources...
	@$(RC) $(RCFLAGS) -fo=$(ROBJ) $(RCFILE)

# ============================================================================
# Help files (one per language, wipfc)
# ============================================================================

$(HLPDIR) :
	@if not exist $(HLPDIR) mkdir $(HLPDIR)

$(HLPDIR)\TwentyOne_en.hlp : help\TwentyOne_en.ipf $(HLPDIR)
	@echo Compiling help\TwentyOne_en.ipf
	@set WIPFC=$(WIPFC)
	@$(IPFC) -l en_US -o $@ help\TwentyOne_en.ipf

$(HLPDIR)\TwentyOne_es.hlp : help\TwentyOne_es.ipf $(HLPDIR)
	@echo Compiling help\TwentyOne_es.ipf
	@set WIPFC=$(WIPFC)
	@$(IPFC) -l en_US -o $@ help\TwentyOne_es.ipf

$(HLPDIR)\TwentyOne_nl.hlp : help\TwentyOne_nl.ipf $(HLPDIR)
	@echo Compiling help\TwentyOne_nl.ipf
	@set WIPFC=$(WIPFC)
	@$(IPFC) -l en_US -o $@ help\TwentyOne_nl.ipf

$(HLPDIR)\TwentyOne_de.hlp : help\TwentyOne_de.ipf $(HLPDIR)
	@echo Compiling help\TwentyOne_de.ipf
	@set WIPFC=$(WIPFC)
	@$(IPFC) -l de_DE -o $@ help\TwentyOne_de.ipf

$(HLPDIR)\TwentyOne_fr.hlp : help\TwentyOne_fr.ipf $(HLPDIR)
	@echo Compiling help\TwentyOne_fr.ipf
	@set WIPFC=$(WIPFC)
	@$(IPFC) -l fr_FR -o $@ help\TwentyOne_fr.ipf

$(HLPDIR)\TwentyOne_it.hlp : help\TwentyOne_it.ipf $(HLPDIR)
	@echo Compiling help\TwentyOne_it.ipf
	@set WIPFC=$(WIPFC)
	@$(IPFC) -l en_US -o $@ help\TwentyOne_it.ipf

# ============================================================================
# Clean
# ============================================================================

clean : .SYMBOLIC
	@if exist $(BINDIR)\*.obj del $(BINDIR)\*.obj >nul
	@if exist $(BINDIR)\$(NAME).res del $(BINDIR)\$(NAME).res >nul
	@if exist $(BINDIR)\$(NAME).exe del $(BINDIR)\$(NAME).exe >nul
	@if exist $(BINDIR)\$(NAME).map del $(BINDIR)\$(NAME).map >nul
	@if exist $(HLPDIR)\*.hlp del $(HLPDIR)\*.hlp >nul
	@echo Clean complete