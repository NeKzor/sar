BINARY=sar.so
STEAM=/home/nekz/.steam/steam/steamapps/common/
SDIR=src/
ODIR=obj/
PROC=revolution

SRCS=$(wildcard $(SDIR)*.cpp)
SRCS+=$(wildcard $(SDIR)Features/*.cpp)
SRCS+=$(wildcard $(SDIR)Features/Demo/*.cpp)
SRCS+=$(wildcard $(SDIR)Features/Hud/*.cpp)
SRCS+=$(wildcard $(SDIR)Features/Routing/*.cpp)
SRCS+=$(wildcard $(SDIR)Features/Speedrun/*.cpp)
SRCS+=$(wildcard $(SDIR)Features/Speedrun/Rules/*.cpp)
SRCS+=$(wildcard $(SDIR)Features/Stats/*.cpp)
SRCS+=$(wildcard $(SDIR)Features/ReplaySystem/*.cpp)
SRCS+=$(wildcard $(SDIR)Features/Tas/*.cpp)
SRCS+=$(wildcard $(SDIR)Features/Timer/*.cpp)
SRCS+=$(wildcard $(SDIR)Games/Linux/*.cpp)
SRCS+=$(wildcard $(SDIR)Modules/*.cpp)
SRCS+=$(wildcard $(SDIR)Utils/*.cpp)
OBJS=$(patsubst $(SDIR)%.cpp, $(ODIR)%.o, $(SRCS))

CXX=g++-11
STFU=-Wno-unused-function -Wno-unused-variable -Wno-parentheses -Wno-unknown-pragmas -Wno-attributes
CFLAGS=-std=c++17 -m64 -fPIC -static-libstdc++ -shared -Wall -s $(STFU) -I$(SDIR)
#CFLAGS+=-D _DEBUG
EXPORT=cp -fu
PRINT=echo
DELETE=rm -rf
CREATE=mkdir -p

-include config.mk

all: pre sar post

sar: $(OBJS)
	@$(CXX) $(CFLAGS) -o $(BINARY) $^ -lstdc++fs

$(ODIR)%.o: $(SDIR)%.cpp $(SDIR)%.hpp
	@$(PRINT) $@
	@$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	@$(DELETE) $(OBJS) $(BINARY)

pre:
	@$(CREATE) $(ODIR)
	@$(CREATE) $(ODIR)Features/
	@$(CREATE) $(ODIR)Features/Demo/
	@$(CREATE) $(ODIR)Features/Hud/
	@$(CREATE) $(ODIR)Features/Routing/
	@$(CREATE) $(ODIR)Features/Speedrun/
	@$(CREATE) $(ODIR)Features/Speedrun/Rules
	@$(CREATE) $(ODIR)Features/Stats/
	@$(CREATE) $(ODIR)Features/ReplaySystem/
	@$(CREATE) $(ODIR)Features/Tas/
	@$(CREATE) $(ODIR)Features/Timer/
	@$(CREATE) $(ODIR)Games/Linux/
	@$(CREATE) $(ODIR)Modules/
	@$(CREATE) $(ODIR)Utils/

post:
#	@$(EXPORT) "$(BINARY)" "$(STEAM)Portal 2/$(BINARY)"
#	@$(EXPORT) "$(BINARY)" "$(STEAM)Aperture Tag/bin/$(BINARY)"
#	@$(EXPORT) "$(BINARY)" "$(STEAM)Portal Stories Mel/bin/$(BINARY)"
#	@$(EXPORT) "$(BINARY)" "$(STEAM)The Stanley Parable/$(BINARY)"
#	@$(EXPORT) "$(BINARY)" "$(STEAM)The Beginners Guide/$(BINARY)"
#	@$(EXPORT) "$(BINARY)" "$(STEAM)Half-Life 2/$(BINARY)"
#	@$(EXPORT) "$(BINARY)" "$(STEAM)Portal/$(BINARY)"

get-deps:
	sudo apt-get install -y g++-8-multilib

cvars:
	node cvars.js "$(STEAM)Portal 2"

load:
	sudo ./sar.sh load $(PROC) $(BINARY)

unload:
	sudo ./sar.sh unload $(PROC) $(BINARY)
