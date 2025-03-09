# FIXME use cmake

# NOTE: This one Makefile works with Microsoft nmake and GNU make.
# They use different conditional syntax, but each can be nested and inverted within the other.

all: check

ifdef MAKEDIR:
!ifdef MAKEDIR

# Microsoft nmake on Windows. Visual C++.
CFLAGS=-MD -Gy -Z7 -GX
CPPFLAGS=-MD -Gy -Z7 -GX

RM_F = del 2>nul /f
#ILDASM = ildasm /nobar /out:$@
#ILASM = ilasm /quiet
#RUN_EACH=for %%a in (
#RUN_EACH_END=) do @$Q$(MONO)$Q %%a
O=obj

!else
else

# GNU/Posix make on Unix with gcc, clang, etc.
RM_F = rm -f
O=o

endif
!endif :

check:

clean:

exe:

# jarray.$O \

OBJS=\
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
win=winamd64.exe
386=0
ARM=0
!elseif $(386)
win=winx86.exe
AMD64=0
ARM=0
!elseif $(ARM)
win=winarm.exe
AMD64=0
386=0
!endif

!ifndef win
win=win.exe
!endif

all: $(win) test_vec.exe test_list.exe test_hash.exe

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

# TODO clang cross
#
#mac: w3.cpp
#	g++ -g -o $@
#

$(win): $(OBJS)
	@-del $(@R).pdb $(@R).ilk
	@rem TODO /GX on old, /EHsc on new
	rem cl -MD -Gy -Z7 /O2s $(Wall) $(Qspectre) -W4 -GX $** /link /out:$@ /incremental:no /opt:ref,icf
	cl -MD -Gy -Z7 $(Wall) $(Qspectre) -W4 /GX cmain.c /link /out:$@ /incremental:no /opt:ref /pdb:$(@B).pdb

test_vec.exe: $(OBJS)
	@-del $(@R).pdb $(@R).ilk
	@rem TODO /GX on old, /EHsc on new
	rem cl -MD -Gy -Z7 /O2s $(Wall) $(Qspectre) -W4 -GX $** /link /out:$@ /incremental:no /opt:ref,icf
	cl -MD -Gy -Z7 $(Wall) $(Qspectre) -W4 /GX test_vec.c $** /link /out:$@ /incremental:no /opt:ref /pdb:$(@B).pdb

test_list.exe: $(OBJS)
	@-del $(@R).pdb $(@R).ilk
	@rem TODO /GX on old, /EHsc on new
	rem cl -MD -Gy -Z7 /O2s $(Wall) $(Qspectre) -W4 -GX $** /link /out:$@ /incremental:no /opt:ref,icf
	cl -MD -Gy -Z7 $(Wall) $(Qspectre) -W4 /GX test_list.c $** /link /out:$@ /incremental:no /opt:ref /pdb:$(@B).pdb

test_hash.exe: $(OBJS)
	@-del $(@R).pdb $(@R).ilk
	@rem TODO /GX on old, /EHsc on new
	rem cl -MD -Gy -Z7 /O2s $(Wall) $(Qspectre) -W4 -GX $** /link /out:$@ /incremental:no /opt:ref,icf
	cl -MD -Gy -Z7 $(Wall) $(Qspectre) -W4 /GX test_hash.c $** /link /out:$@ /incremental:no /opt:ref /pdb:$(@B).pdb

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
all: $(NativeTarget) win32.exe win64.exe

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

win32.exe: $(OBJS)
	i686-w64-mingw32-g++ -g $(OBJS) -o $@ -Bsymbolic

win64.exe: $(OBJS)
	x86_64-w64-mingw32-g++ -g $(OBJS) -o $@ -Bsymbolic

test:

endif
!endif :

clean:
	$(RM_F) camd64.obj carm64.obj ccheck.obj ccpe.obj celf.obj clex.obj
	$(RM_F) cmacho.obj cmain.obj config.cpp config.mk config.obj
	$(RM_F) copt.obj cparse.obj cpe.obj cpre.obj cx86.obj jerr.obj
	$(RM_F) jhash.obj jlist.obj jpe.c jpe.h jstr.obj jvec.obj mscver.cpp test1.exe test_vec.exe test_list.exe test_hash.exe
	$(RM_F) test1.obj test1.pdb typedenum.cpp typedenum.obj winamd64.exe winamd64.pdb
	$(RM_F) mscver.cpp typedenum.cpp *.o *.obj w3rt.o w3rt.obj mac win32 win32.exe win64 win64.exe win win.exe cyg cyg.exe *.ilk lin win.exe winarm.exe winx86.exe winamd64.exe
	$(RM_F) $(win) mscver.cpp typedenum.cpp *.o *.obj config.cpp config.mk w3rt.o w3rt.obj w3.obj *.ilk win32 win32.exe win64 win64.exe win win.exe winarm.exe winx86.exe winamd64.exe *.pdb lin *.i
