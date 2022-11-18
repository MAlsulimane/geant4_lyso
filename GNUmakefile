# $Id: GNUmakefile 338 2010-03-02 08:59:58Z gunter $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := PSTEP
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/architecture.gmk

#Add ROOT options for compilation
CPPFLAGS += `root-config --cflags`
LDFLAGS  += `root-config --libs`

include $(G4INSTALL)/config/binmake.gmk

