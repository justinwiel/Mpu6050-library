#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
# 
#############################################################################

# source files in this project (main.cpp is automatically assumed)
SOURCES := src/pong/entities.cpp src/pong/enemy.cpp src/pong/player.cpp lib/MPU.cpp  src/dodge/obstacle.cpp

# Playerer files in this project
PlayerERS := src/pong/entities.hpp src/pong/enemy.hpp src/pong/player.hpp lib/MPU.hpp  src/dodge/obstacle.hpp

# other places to look for files for this project
SEARCH  := 

# set RELATIVE to the next higher directory 
# and defer to the appropriate Makefile.* there
RELATIVE := ..
include $(RELATIVE)/Makefile.due