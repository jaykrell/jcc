# FIXME use cmake

# NOTE: This one Makefile works with Microsoft nmake and GNU make.
# They use different conditional syntax, but each can be nested and inverted within the other.

all: check

ifdef MAKEDIR:
!ifdef MAKEDIR

# Microsoft nmake on Windows. Visual C++.
CFLAGS=-MD -Gy -Z7 -GX -W4 -GX
CPPFLAGS=-MD -Gy -Z7 -GX -std:c++20 -W4 -GX
CXXFLAGS=-MD -Gy -Z7 -GX -std:c++20 -W4 -GX
CXX=cl

RM = del 2>nul
RM_F = $(RM) /f
#ILDASM = ildasm /nobar /out:$@
#ILASM = ilasm /quiet
#RUN_EACH=for %%a in (
#RUN_EACH_END=) do @$Q$(MONO)$Q %%a
O=obj
EXE=.exe

CLINK_FLAGS=/link /out:$@ /incremental:no /opt:ref /pdb:$(@B).pdb

!else
else

# GNU/Posix make on Unix with gcc, clang, etc.
RM = rm
RM_F = $(RM) -f
O=o
EXE=
CXX=g++

CLINK_FLAGS=-o $@

endif
!endif :

check:

clean:

exe:

# jarray.$O \

OBJS=\
 read_entire_file.$O \
 jstdio.$O \
 jmalloc.$O \
 jerr.$O \
 jhash.$O \
 jlist.$O \
 jvec.$O \
 jstr.$O \
 camd64.$O \
 carm64.$O \
 ccheck.$O \
 ccpe.$O \
 celf.$O \
 clex.$O \
 cmacho.$O \
 copt.$O \
 cparse.$O \
 cpe.$O \
 cpre.$O \
 cx86.$O \

ifdef MAKEDIR:
!ifdef MAKEDIR

!if !defined (AMD64) && !defined (386) && !defined (ARM)
AMD64=1
386=0
ARM=0
!endif

!if $(AMD64)
win=winamd64$(EXE)
386=0
ARM=0
!elseif $(386)
win=winx86$(EXE)
AMD64=0
ARM=0
!elseif $(ARM)
win=winarm$(EXE)
AMD64=0
386=0
!endif

!ifndef win
win=win$(EXE)
!endif

all:

#all: $(win) \
#	test_vec$(EXE) \
#	test_list$(EXE) \
#	test_hash$(EXE) \
#	csv$(EXE) \
#	csv_random_write$(EXE) \

config:
	.\config.cmd

check:

run: $(win)
	.\$(win) hello.wasm

debug: $(win)
!if $(AMD64)
	\bin\amd64\cdb .\$(win) hello.wasm
!elseif $(386)
	\bin\x86\cdb .\$(win) hello.wasm
!endif

!else
else

UNAME_S = $(shell uname -s)

ifeq ($(UNAME_S), Cygwin)
Cygwin=1
NativeTarget=cyg
else
Cygwin=0
Linux=0
endif

ifeq ($(UNAME_S), Linux)
Linux=1
NativeTarget=lin
else
Cygwin=0
endif

# TODO Darwin, Linux, etc.

# FIXME winarm64 etc.
#all: $(NativeTarget) win32$(EXE) win64$(EXE)
all:

run: $(NativeTarget)
	./$(NativeTarget) /s/mono/mcs/class/lib/build-macos/mscorlib.dll

debug: mac
	lldb -- ./$(NativeTarget) /s/mono/mcs/class/lib/build-macos/mscorlib.dll

mac: $(OBJS)
	g++ -g $(OBJS) -o $@ -Bsymbolic -bind_at_load

cyg: $(OBJS)
	g++ -g $(OBJS) -o $@ -Bsymbolic -znow -zrelro

lin: $(OBJS)
	g++ -Wall -g $(OBJS) -o $@ -Bsymbolic -znow -zrelro

win32$(EXE): $(OBJS)
	i686-w64-mingw32-g++ -g $(OBJS) -o $@ -Bsymbolic

win64$(EXE): $(OBJS)
	x86_64-w64-mingw32-g++ -g $(OBJS) -o $@ -Bsymbolic

test:

endif
!endif :

all: test_vec$(EXE) \
	test_list$(EXE) \
	test_hash$(EXE) \
	csv$(EXE) \
	csv_random_write$(EXE) \

# TODO clang cross
#
#mac: w3.cpp
#	g++ -g -o $@
#

$(win): $(OBJS)
	-$(RM) $(@R).pdb $(@R).ilk
	$(CXX) $(CXXFLAGS) $(Wall) $(Qspectre) cmain.c $(CLINK_FLAGS)

test_vec$(EXE): $(OBJS)
	-$(RM) $(@R).pdb $(@R).ilk
	$(CXX) $(CXXFLAGS) $(Wall) $(Qspectre) test_vec.c $(OBJS) $(CLINK_FLAGS)

test_list$(EXE): $(OBJS)
	-$(RM) $(@R).pdb $(@R).ilk
	$(CXX) $(CXXFLAGS) $(Wall) $(Qspectre) test_list.c $(OBJS) $(CLINK_FLAGS)

test_hash$(EXE): $(OBJS)
	-$(RM) $(@R).pdb $(@R).ilk
	$(CXX) $(CXXFLAGS) $(Wall) $(Qspectre) test_hash.c $(OBJS) $(CLINK_FLAGS)

csv$(EXE): $(OBJS)
	-$(RM) $(@R).pdb $(@R).ilk
	$(CXX) $(CXXFLAGS) $(Wall) $(Qspectre) csv.cpp $(OBJS) $(CLINK_FLAGS)

csv_random_write$(EXE): $(OBJS)
	-$(RM) $(@R).pdb $(@R).ilk
	$(CXX) $(CXXFLAGS) $(Wall) $(Qspectre) csv_random_write.cpp $(OBJS) $(CLINK_FLAGS)

clean:
	$(RM_F) *.h.gch
	$(RM_F) 1$(EXE) 2$(EXE) genprimes$(EXE)
	$(RM_F) csv$(EXE) csv_random_write$(EXE)
	$(RM_F) camd64.$(O) carm64.$(O) ccheck.$(O) ccpe.$(O) celf.$(O) clex.$(O)
	$(RM_F) cmacho.$(O) cmain.$(O) config.cpp config.mk config.$(O)
	$(RM_F) copt.$(O) cparse.$(O) cpe.$(O) cpre.$(O) cx86.$(O) jerr.$(O)
	$(RM_F) jhash.$(O) jlist.$(O) jpe.c jpe.h jstr.$(O) jvec.$(O) mscver.cpp test1$(EXE) test_vec$(EXE) test_list$(EXE) test_hash$(EXE)
	$(RM_F) test1.$(O) test1.pdb typedenum.cpp typedenum.$(O) winamd64$(EXE) winamd64.pdb
	$(RM_F) mscver.cpp typedenum.cpp *.o *.$(O) w3rt.o w3rt.$(O) mac win32 win32$(EXE) win64 win64$(EXE) win win$(EXE) cyg cyg$(EXE) *.ilk lin win$(EXE) winarm$(EXE) winx86$(EXE) winamd64$(EXE)
	$(RM_F) $(win) mscver.cpp typedenum.cpp *.o *.$(O) config.cpp config.mk w3rt.o w3rt.$(O) w3.$(O) *.ilk win32 win32$(EXE) win64 win64$(EXE) win win$(EXE) winarm$(EXE) winx86$(EXE) winamd64$(EXE) *.pdb lin *.i

realclean: clean
	$(RM_F) 1.csv 1.csv.index
