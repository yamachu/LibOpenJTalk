#ifndef LIB_OPENJTALK_LANG_H_
#define LIB_OPENJTALK_LANG_H_

#include "mecab.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "njd.h"
#include "jpcommon.h"

#define MAXBUFLEN 1024

typedef struct _Open_JTalk {
    Mecab mecab;
    NJD njd;
    JPCommon jpcommon;
} Open_JTalk;


Open_JTalk* Open_JTalk_initialize();
void Open_JTalk_clear(Open_JTalk **);
int Open_JTalk_load(Open_JTalk * open_jtalk, char *dn_mecab, char *dn_user);
int Open_JTalk_load_u16(Open_JTalk * open_jtalk, char16_t *dn_mecab, char16_t *dn_user);
int Open_JTalk_extract_label(Open_JTalk * open_jtalk, const char *txt,
    char ***labels, int *labelLength);
int Open_JTalk_extract_label_u16(Open_JTalk * open_jtalk, char16_t *txt,
    char ***labels, int *labelLength);
int Open_JTalk_dict_gen(const char *dicdir, const char *usrdiccsv, const char *usrdicpath);
int Open_JTalk_dict_gen_u16(char16_t *dicdir, char16_t *usrdiccsv, char16_t *usrdicpath);

#ifdef __cplusplus
}
#endif

#endif  // LIB_OPENJTALK_LANG_H_
