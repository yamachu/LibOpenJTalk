#ifndef LIB_OPENJTALK_H_
#define LIB_OPENJTALK_H_

#include "mecab.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "njd.h"
#include "jpcommon.h"
#include "HTS_engine.h"

#define MAXBUFLEN 1024

typedef struct _Open_JTalk {
    Mecab mecab;
    NJD njd;
    JPCommon jpcommon;
    HTS_Engine engine;
} Open_JTalk;


Open_JTalk* Open_JTalk_initialize();
void Open_JTalk_clear(Open_JTalk **);
int Open_JTalk_load(Open_JTalk * open_jtalk, char *dn_mecab, char *fn_voice);
void Open_JTalk_destroy_buffer(Open_JTalk * open_jtalk, short ** data);
int Open_JTalk_synthesis_buffer(Open_JTalk * open_jtalk, const char *txt,
    short ** data);
int Open_JTalk_synthesis(Open_JTalk * open_jtalk, const char *txt,
    const char *wav_file_path, const char *log_file_path);
int Open_JTalk_synthesis_labels(Open_JTalk * open_jtalk, const char *txt,
    const char *wav_file_path,
    const char *text_anal_file_path,
    const char *context_label_file_path);
int Open_JTalk_synthesis_buffer_WORLD(Open_JTalk * open_jtalk, const char *txt,
    short ** data);
int Open_JTalk_synthesis_WORLD(Open_JTalk * open_jtalk, const char *txt,
    const char *wav_file_path, const char *log_file_path);
int Open_JTalk_synthesis_labels_WORLD(Open_JTalk * open_jtalk, const char *txt,
    const char *wav_file_path,
    const char *text_anal_file_path,
    const char *context_label_file_path);
int Open_JTalk_resynthesis_buffer(Open_JTalk * open_jtalk, short ** data);
int Open_JTalk_resynthesis(Open_JTalk * open_jtalk, const char *wav_file_path);
int Open_JTalk_resynthesis_buffer_WORLD(Open_JTalk * open_jtalk, short ** data);
int Open_JTalk_resynthesis_WORLD(Open_JTalk * open_jtalk, const char *wav_file_path);

void Open_JTalk_set_sampling_frequency(Open_JTalk * open_jtalk, size_t i);
void Open_JTalk_set_fperiod(Open_JTalk * open_jtalk, size_t i);
void Open_JTalk_set_alpha(Open_JTalk * open_jtalk, double f);
void Open_JTalk_set_beta(Open_JTalk * open_jtalk, double f);
void Open_JTalk_set_speed(Open_JTalk * open_jtalk, double f);
void Open_JTalk_add_half_tone(Open_JTalk * open_jtalk, double f);
void Open_JTalk_set_msd_threshold(Open_JTalk * open_jtalk, size_t i, double f);
void Open_JTalk_set_gv_weight(Open_JTalk * open_jtalk, size_t i, double f);
void Open_JTalk_set_volume(Open_JTalk * open_jtalk, double f);
void Open_JTalk_set_audio_buff_size(Open_JTalk * open_jtalk, size_t i);

void Open_JTalk_set_lf0_array(Open_JTalk * open_jtalk, double *lf0_buffer, size_t buffer_length);
void Open_JTalk_set_lf0(Open_JTalk * open_jtalk, double lf0, size_t frame_index);

int Open_JTalk_get_lf0_length(Open_JTalk * open_jtalk);
void Open_JTalk_get_lf0_array(Open_JTalk * open_jtalk, double *lf0_buffer, size_t buffer_length);

#ifdef __cplusplus
}
#endif

#endif  // LIB_OPENJTAKL_H_
