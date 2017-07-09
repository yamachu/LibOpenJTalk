#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "libopenjtalk.h"

#include "text2mecab.h"
#include "mecab2njd.h"
#include "njd_set_pronunciation.h"
#include "njd_set_digit.h"
#include "njd_set_accent_phrase.h"
#include "njd_set_accent_type.h"
#include "njd_set_unvoiced_vowel.h"
#include "njd_set_long_vowel.h"
#include "njd2jpcommon.h"


Open_JTalk* Open_JTalk_initialize()
{
    Open_JTalk *open_jtalk;
    open_jtalk = (Open_JTalk*)malloc(sizeof(Open_JTalk));
    Mecab_initialize(&open_jtalk->mecab);
    NJD_initialize(&open_jtalk->njd);
    JPCommon_initialize(&open_jtalk->jpcommon);
    HTS_Engine_initialize(&open_jtalk->engine);

    return open_jtalk;
}

void Open_JTalk_clear(Open_JTalk **open_jtalk)
{
    Mecab_clear(&(*open_jtalk)->mecab);
    NJD_clear(&(*open_jtalk)->njd);
    JPCommon_clear(&(*open_jtalk)->jpcommon);
    HTS_Engine_clear(&(*open_jtalk)->engine);

    free(*open_jtalk);
}

int Open_JTalk_load(Open_JTalk * open_jtalk, char *dn_mecab, char *fn_voice)
{
    if (Mecab_load(&open_jtalk->mecab, dn_mecab) != TRUE) {
        Open_JTalk_clear(&open_jtalk);
        return 0;
    }
    if (HTS_Engine_load(&open_jtalk->engine, &fn_voice, 1) != TRUE) {
        Open_JTalk_clear(&open_jtalk);
        return 0;
    }
    if (strcmp(HTS_Engine_get_fullcontext_label_format(&open_jtalk->engine), "HTS_TTS_JPN") != 0) {
        Open_JTalk_clear(&open_jtalk);
        return 0;
    }
    return 1;
}

void Open_JTalk_set_sampling_frequency(Open_JTalk * open_jtalk, size_t i)
{
    HTS_Engine_set_sampling_frequency(&open_jtalk->engine, i);
}

void Open_JTalk_set_fperiod(Open_JTalk * open_jtalk, size_t i)
{
    HTS_Engine_set_fperiod(&open_jtalk->engine, i);
}

void Open_JTalk_set_alpha(Open_JTalk * open_jtalk, double f)
{
    HTS_Engine_set_alpha(&open_jtalk->engine, f);
}

void Open_JTalk_set_beta(Open_JTalk * open_jtalk, double f)
{
    HTS_Engine_set_beta(&open_jtalk->engine, f);
}

void Open_JTalk_set_speed(Open_JTalk * open_jtalk, double f)
{
    HTS_Engine_set_speed(&open_jtalk->engine, f);
}

void Open_JTalk_add_half_tone(Open_JTalk * open_jtalk, double f)
{
    HTS_Engine_add_half_tone(&open_jtalk->engine, f);
}

void Open_JTalk_set_msd_threshold(Open_JTalk * open_jtalk, size_t i, double f)
{
    HTS_Engine_set_msd_threshold(&open_jtalk->engine, i, f);
}

void Open_JTalk_set_gv_weight(Open_JTalk * open_jtalk, size_t i, double f)
{
    HTS_Engine_set_gv_weight(&open_jtalk->engine, i, f);
}

void Open_JTalk_set_volume(Open_JTalk * open_jtalk, double f)
{
    HTS_Engine_set_volume(&open_jtalk->engine, f);
}

void Open_JTalk_set_audio_buff_size(Open_JTalk * open_jtalk, size_t i)
{
    HTS_Engine_set_audio_buff_size(&open_jtalk->engine, i);
}

int Open_JTalk_synthesis(Open_JTalk * open_jtalk, const char *txt,
    const char *wav_file_path, const char* log_file_path)
{
    int result = 0;
    char buff[MAXBUFLEN];
    FILE *wavfp = NULL;
    FILE *logfp = NULL;
    if (wav_file_path != NULL) {
        // Do Error Handling
        wavfp = fopen(wav_file_path, "wb");
    }
    if (log_file_path != NULL) {
        // Do Error Handling
        logfp = fopen(log_file_path, "w");
    }

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
        if (HTS_Engine_synthesize_from_strings
            (&open_jtalk->engine, JPCommon_get_label_feature(&open_jtalk->jpcommon),
            JPCommon_get_label_size(&open_jtalk->jpcommon)) == TRUE)
            result = 1;
        if (wavfp != NULL)
            HTS_Engine_save_riff(&open_jtalk->engine, wavfp);
        if (logfp != NULL) {
            fprintf(logfp, "[Text analysis result]\n");
            NJD_fprint(&open_jtalk->njd, logfp);
            fprintf(logfp, "\n[Output label]\n");
            HTS_Engine_save_label(&open_jtalk->engine, logfp);
            fprintf(logfp, "\n");
            HTS_Engine_save_information(&open_jtalk->engine, logfp);
        }
        HTS_Engine_refresh(&open_jtalk->engine);
    }
    JPCommon_refresh(&open_jtalk->jpcommon);
    NJD_refresh(&open_jtalk->njd);
    Mecab_refresh(&open_jtalk->mecab);

    if (wavfp != NULL) {
        fclose(wavfp);
    }
    if (logfp != NULL) {
        fclose(logfp);
    }

    return result;
}
