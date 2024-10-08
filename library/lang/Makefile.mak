
CC = cl
CL = link

CFLAGS = /O2 /Ob2 /Oi /Ot /Oy /GT /GL /TP /I ..\..\text2mecab /I ..\..\mecab\src /I ..\..\mecab2njd /I ..\..\njd /I ..\..\njd_set_pronunciation /I ..\..\njd_set_digit /I ..\..\njd_set_accent_phrase /I ..\..\njd_set_accent_type /I ..\..\njd_set_unvoiced_vowel /I ..\..\njd_set_long_vowel /I ..\..\njd2jpcommon /I ..\..\jpcommon /I ..\..\utfcpp\source
CPPFLAGS = $(CFLAGS)
LFLAGS = /LTCG

LIBS = ..\..\text2mecab\text2mecab.lib ..\..\mecab\src\mecab.lib ..\..\mecab2njd\mecab2njd.lib ..\..\njd\njd.lib ..\..\njd_set_pronunciation\njd_set_pronunciation.lib ..\..\njd_set_digit\njd_set_digit.lib ..\..\njd_set_accent_phrase\njd_set_accent_phrase.lib ..\..\njd_set_accent_type\njd_set_accent_type.lib ..\..\njd_set_unvoiced_vowel\njd_set_unvoiced_vowel.lib ..\..\njd_set_long_vowel\njd_set_long_vowel.lib ..\..\njd2jpcommon\njd2jpcommon.lib ..\..\jpcommon\jpcommon.lib winmm.lib Advapi32.lib

all: open_jtalk_lang.dll

open_jtalk_lang.dll : libopenjtalk-lang.obj
	$(CL) $(LFLAGS) /dll /def:open_jtalk_lang.def /OUT:$@ $(LIBS) libopenjtalk-lang.obj

clean:	
	del *.dll
	del *.obj
