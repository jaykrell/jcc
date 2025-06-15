# FIXME use cmake

# NOTE: This one Makefile works with Microsoft nmake and GNU make.
# They use different conditional syntax, but each can be nested and inverted within the other.

all: check

ifdef MAKEDIR:
!ifdef MAKEDIR

# Microsoft nmake on Windows. Visual C++.
CFLAGS=-MD -Gy -Z7 -EHsc -W4 -EHsc -WX
CPPFLAGS=-MD -Gy -Z7 -EHsc -std:c++20 -W4 -EHsc -WX
CXXFLAGS=-MD -Gy -Z7 -EHsc -std:c++20 -W4 -EHsc -WX
CXX=cl

CP_F=copy /y
UNIX_LN_F=@rem
WINDOWS_LN_F=mklink /h
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
CP_F=cp -f
UNIX_LN_F=ln -f
WINDOWS_LN_F=@echo >/dev/null
RM = rm 2>/dev/null
RM_F = rm -f 2>/dev/null
O=o
EXE=
CXX=g++

CFLAGS=-g
CLINK_FLAGS=-o $@

endif
!endif :

check:

clean:

exe:

# jarray.$O \

OBJS=\
 jbase.$O \
\
 jcc.$O \
 jcc_ctype.$O \
 jcc_preprocess_backtrack.$O \
 jcc_preprocess_commments.$O \
 jcc_preprocess_line_continations.$O \
 jcc_preprocess_newlines.$O \
 jcc_unget.$O \
\
 jdec.$O \
 jerr.$O \
 jfile.$O \
 jhash.$O \
 jlist.$O \
 jmalloc.$O \
 jmem.$O \
 jos.$O \
 jsprintf.$O \
 jstdio.$O \
 jstdio_file.$O \
 jstr.$O \
 jthread.$O \
 jvarint.$O \
 jvec1.$O \
 jvec.$O \
 camd64.$O \
 carm64.$O \
 ccheck.$O \
 celf.$O \
 clex.$O \
 cmacho.$O \
 copt.$O \
 cparse.$O \
 cpe.$O \
 cx86.$O \
\
 read_entire_file.$O \
\
 csv.$O \
 csv1.$O \
\
 test_dec.$O \
 test_hash.$O \
 test_list.$O \
 test_os.$O \
 test_thread.$O \
 test_varint.$O \
 test_vec.$O \
 test_vec1.$O \

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

check:

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

EXES=csv$(EXE) csv1$(EXE) dump_varuint$(EXE) \
 test_thread$(EXE) test_varint$(EXE) test_os$(EXE) \
 test_hash$(EXE) test_list$(EXE) test_vec$(EXE) test_vec1$(EXE)

all: \
	$(EXES) \
	csv_random_write$(EXE) \
	dump_varuint$(EXE) \

# TODO clang cross
#
#mac: w3.cpp
#	g++ -g -o $@
#

$(win): $(OBJS)
	-$(RM_F) $(@R).pdb $(@R).ilk $@
	$(CC) $(CFLAGS) $(Wall) $(Qspectre) cmain.c $(CLINK_FLAGS)

jmain$(EXE): $(OBJS) jmain.c
	-$(RM_F) $(@R).pdb $(@R).ilk $@
	$(CC) $(CFLAGS) $(Wall) $(Qspectre) jmain.c $(OBJS) $(CLINK_FLAGS)

$(EXES): jmain$(EXE)
	-$(RM_F) $(@R).pdb $(@R).ilk $@
	$(UNIX_LN_F) jmain$(EXE) $@
	$(WINDOWS_LN_F) $@ jmain$(EXE)

csv_random_write$(EXE): csv_random_write.c
	@-$(RM_F) $(@R).pdb $(@R).ilk
	$(CC) $(CFLAGS) $(Wall) $(Qspectre) csv_random_write.c $(CLINK_FLAGS)

clean:
	$(RM_F) *.h.gch $(EXES) main main.exe jmain jmain.exe
	$(RM_F) 1 2 genprimes 1.exe 2.exe genprimes.exe
	$(RM_F) csv1 csv1.exe test_os test_vec1 dump_varuint dump_varuint.exe a.out
	$(RM_F) csv csv_random_write csv.exe csv_random_write.exe
	$(RM_F) camd64.$(O) carm64.$(O) ccheck.$(O) ccpe.$(O) celf.$(O) clex.$(O)
	$(RM_F) cmacho.$(O) cmain.$(O) config.cpp config.mk config.$(O)
	$(RM_F) copt.$(O) cparse.$(O) cpe.$(O) cpre.$(O) cx86.$(O) jerr.$(O)
	$(RM_F) jhash.$(O) jlist.$(O) jpe.c jpe.h jstr.$(O) jvec1.$(O)  jvec.$(O) mscver.cpp
	$(RM_F) test1 test_vec test_list test_hash test1.exe test_vec.exe test_list.exe test_hash.exe
	$(RM_F) test1.$(O) test1.pdb typedenum.cpp typedenum.$(O) winamd64$(EXE) winamd64.pdb
	$(RM_F) mscver.cpp typedenum.cpp *.o *.obj w3rt.o w3rt.$(O) mac win32 win32$(EXE) win64 win64$(EXE) win win$(EXE) cyg cyg$(EXE) *.ilk lin win$(EXE) winarm$(EXE) winx86$(EXE) winamd64$(EXE)
	$(RM_F) test_thread test_thread$(EXE)
	$(RM_F) $(win) mscver.cpp typedenum.cpp *.o *.obj config.cpp config.mk w3rt.o w3rt.$(O) w3.$(O) *.ilk win32 win32$(EXE) win64 win64$(EXE) win win$(EXE) winarm$(EXE) winx86$(EXE) winamd64$(EXE) *.pdb lin *.i

realclean: clean
	$(RM_F) 1.csv 1.csv.index
