#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "libopenjtalk-lang.h"

#include "text2mecab.h"
#include "mecab2njd.h"
#include "njd_set_pronunciation.h"
#include "njd_set_digit.h"
#include "njd_set_accent_phrase.h"
#include "njd_set_accent_type.h"
#include "njd_set_unvoiced_vowel.h"
#include "njd_set_long_vowel.h"
#include "njd2jpcommon.h"

#include <fstream>
#include "utf8.h"

namespace
{
std::string conv_u16_u8(char16_t* org_u16)
{
    std::u16string source(org_u16);
    std::string utf8string; 

    utf8::utf16to8(source.begin(), source.end(), std::back_inserter(utf8string));

    return utf8string;
}
}

Open_JTalk* Open_JTalk_initialize()
{
    Open_JTalk *open_jtalk;
    open_jtalk = (Open_JTalk*)malloc(sizeof(Open_JTalk));
    Mecab_initialize(&open_jtalk->mecab);
    NJD_initialize(&open_jtalk->njd);
    JPCommon_initialize(&open_jtalk->jpcommon);

    return open_jtalk;
}

void Open_JTalk_clear(Open_JTalk **open_jtalk)
{
    Mecab_clear(&(*open_jtalk)->mecab);
    NJD_clear(&(*open_jtalk)->njd);
    JPCommon_clear(&(*open_jtalk)->jpcommon);

    free(*open_jtalk);
}

int Open_JTalk_load(Open_JTalk * open_jtalk, char *dn_mecab)
{
    if (Mecab_load(&open_jtalk->mecab, dn_mecab) != TRUE) {
        Open_JTalk_clear(&open_jtalk);
        return 0;
    }
    return 1;
}

int Open_JTalk_load_u16(Open_JTalk * open_jtalk, char16_t *dn_mecab)
{
    std::string mecab_str = conv_u16_u8(dn_mecab);
    const char *mecab_str_c = mecab_str.c_str();
    char *mecab = const_cast<char*>(mecab_str_c);

    return Open_JTalk_load(open_jtalk, mecab);
}

int Open_JTalk_extract_label(Open_JTalk * open_jtalk, const char *txt,
    char ***labels, int *labelLength)
{
    int result = 0;
    char buff[MAXBUFLEN];

    text2mecab(buff, txt);
    Mecab_analysis(&open_jtalk->mecab, buff);
    mecab2njd(&open_jtalk->njd, Mecab_get_feature(&open_jtalk->mecab),
        Mecab_get_size(&open_jtalk->mecab));
    njd_set_pronunciation(&open_jtalk->njd);
    njd_set_digit(&open_jtalk->njd);
    njd_set_accent_phrase(&open_jtalk->njd);
    njd_set_accent_type(&open_jtalk->njd);
    njd_set_unvoiced_vowel(&open_jtalk->njd);
    njd_set_long_vowel(&open_jtalk->njd);
    njd2jpcommon(&open_jtalk->jpcommon, &open_jtalk->njd);
    JPCommon_make_label(&open_jtalk->jpcommon);
    if (JPCommon_get_label_size(&open_jtalk->jpcommon) > 2) {
        *labelLength = JPCommon_get_label_size(&open_jtalk->jpcommon);
        char **tmp = (char**)malloc(sizeof(char*) * (*labelLength));
        char **tmp2 = JPCommon_get_label_feature(&open_jtalk->jpcommon);
        for (int i = 0; i < *labelLength; i++)
        {
            std::string label = std::string(tmp2[i]);
            tmp[i] = (char*)malloc(sizeof(char) * label.length() + 1);
            strcpy(tmp[i], label.c_str());
        }

        *labels = tmp;
        result = 1;
    }
    JPCommon_refresh(&open_jtalk->jpcommon);
    NJD_refresh(&open_jtalk->njd);
    Mecab_refresh(&open_jtalk->mecab);

    return result;
}

int Open_JTalk_extract_label_u16(Open_JTalk * open_jtalk, char16_t *txt,
    char ***labels, int *labelLength)
{
    return Open_JTalk_extract_label(open_jtalk, conv_u16_u8(txt).c_str(),
        labels, labelLength);
}
