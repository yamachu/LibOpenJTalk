CXX = g++
LINK = g++
CXXFLAGS = -O2 -Wall -finput-charset=UTF-8 -fexec-charset=UTF-8
LDFLAGS = -lstdc++ -lm
ARFLAGS = -cr
INCLUDE = \
	-I ../../mecab \
	-I ../../text2mecab \
	-I ../../mecab/src \
	-I ../../mecab2njd \
	-I ../../njd \
	-I ../../njd_set_pronunciation \
	-I ../../njd_set_digit \
	-I ../../njd_set_accent_phrase \
	-I ../../njd_set_accent_type \
	-I ../../njd_set_unvoiced_vowel \
	-I ../../njd_set_long_vowel \
	-I ../../njd2jpcommon \
	-I ../../jpcommon \
	-I ../../utfcpp/source
LIBS = \
	../../text2mecab/libtext2mecab.a \
	../../mecab/src/libmecab.a \
	../../mecab2njd/libmecab2njd.a \
	../../njd/libnjd.a \
	../../njd_set_pronunciation/libnjd_set_pronunciation.a \
	../../njd_set_digit/libnjd_set_digit.a \
	../../njd_set_accent_phrase/libnjd_set_accent_phrase.a \
	../../njd_set_accent_type/libnjd_set_accent_type.a \
	../../njd_set_unvoiced_vowel/libnjd_set_unvoiced_vowel.a \
	../../njd_set_long_vowel/libnjd_set_long_vowel.a \
	../../njd2jpcommon/libnjd2jpcommon.a \
	../../jpcommon/libjpcommon.a

all: libobj

libobj: libopenjtalk-lang.o

libopenjtalk-lang.o: libopenjtalk-lang.cpp
	$(CXX) $(CXXFLAGS) -fPIC $(INCLUDE) -D CHARSET_UTF_8 -DHAVE_COINFIG_H -o "$@" -c "$<" --std=c++11

mac_shared: libopenjtalk-lang.dylib

libopenjtalk-lang.dylib: libopenjtalk-lang.o
	$(LINK) -Wl,-no_compact_unwind $(CXXFLAGS) $(LDFLAGS) $(LIBS) -liconv -dynamiclib -o "$@" "$<"

linux_shared: libopenjtalk-lang.so

libopenjtalk-lang.so: libopenjtalk-lang.o
	$(LINK) $(CXXFLAGS) $(LDFLAGS) -Wl,--whole-archive $(LIBS) -shared -o "$@" "$<" -Wl,--no-whole-archive

wasm_static: wasm-libopenjtalk-lang.a

wasm-libopenjtalk-lang.a: libopenjtalk-lang.o
	$(LINK) $(CXXFLAGS) $(LDFLAGS) -r -o libs.o -Wl,--whole-archive $(LIBS)
	$(AR) $(ARFLAGS) "$@" "$<" libs.o

clean:
	$(RM) libopenjtalk-lang.o

.PHONY: clean all libobj
.DELETE_ON_ERRORS:
