CXX = g++
LINK = g++
CXXFLAGS = -O2 -Wall -finput-charset=UTF-8 -fexec-charset=UTF-8
LDFLAGS = -lstdc++ -lm
INCLUDE = \
    -I ../mecab \
    -I ../text2mecab \
    -I ../mecab/src \
    -I ../mecab2njd \
    -I ../njd \
    -I ../njd_set_pronunciation \
    -I ../njd_set_digit \
    -I ../njd_set_accent_phrase \
    -I ../njd_set_accent_type \
    -I ../njd_set_unvoiced_vowel \
    -I ../njd_set_long_vowel \
    -I ../njd2jpcommon \
    -I ../jpcommon \
    -I ../hts_engine_API/include \
    -I ../utfcpp/source
LIBS = \
    ../text2mecab/libtext2mecab.a \
    ../mecab/src/libmecab.a \
    ../mecab2njd/libmecab2njd.a \
    ../njd/libnjd.a \
    ../njd_set_pronunciation/libnjd_set_pronunciation.a \
    ../njd_set_digit/libnjd_set_digit.a \
    ../njd_set_accent_phrase/libnjd_set_accent_phrase.a \
    ../njd_set_accent_type/libnjd_set_accent_type.a \
    ../njd_set_unvoiced_vowel/libnjd_set_unvoiced_vowel.a \
    ../njd_set_long_vowel/libnjd_set_long_vowel.a \
    ../njd2jpcommon/libnjd2jpcommon.a \
    ../jpcommon/libjpcommon.a \
    ../hts_engine_API/lib/libHTSEngine.a

all: libobj

libobj: libopenjtalk.o

libopenjtalk.o: libopenjtalk.cpp
	$(CXX) $(CXXFLAGS) -fPIC $(INCLUDE) -D CHARSET_UTF_8 -DHAVE_COINFIG_H -o "$@" -c "$<"

mac_shared: libopenjtalk.dylib

libopenjtalk.dylib: libopenjtalk.o
	$(LINK) -Wl,-no_compact_unwind $(CXXFLAGS) $(LDFLAGS) $(LIBS) -liconv -dynamiclib -o "$@" "$<"

linux_shared: libopenjtalk.so

libopenjtalk.so: libopenjtalk.o
	$(LINK) $(CXXFLAGS) $(LDFLAGS) -Wl,--whole-archive $(LIBS) -shared -o "$@" "$<" -Wl,--no-whole-archive

clean:
	$(RM) libopenjtalk.o

.PHONY: clean all libobj
.DELETE_ON_ERRORS:
