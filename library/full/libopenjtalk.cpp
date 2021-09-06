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

int Open_JTalk_load_u16(Open_JTalk * open_jtalk, char16_t *dn_mecab, char16_t *fn_voice)
{
    std::string mecab_str = conv_u16_u8(dn_mecab);
    const char *mecab_str_c = mecab_str.c_str();
    char *mecab = const_cast<char*>(mecab_str_c);
    std::string voice_str = conv_u16_u8(fn_voice);
    const char *voice_str_c = voice_str.c_str();
    char *voice = const_cast<char*>(voice_str_c);

    return Open_JTalk_load(open_jtalk, mecab, voice);
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

void Open_JTalk_destroy_buffer(Open_JTalk * open_jtalk, short ** data)
{
    HTS_Engine_free_generated_speech(&open_jtalk->engine, data);
}

int Open_JTalk_synthesis_buffer(Open_JTalk * open_jtalk, const char *txt,
    short ** data)
{
    int result = 0;
    char buff[MAXBUFLEN];

    HTS_Engine_refresh(&open_jtalk->engine);
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
        if (data != NULL && result == 1)
            result = HTS_Engine_allocate_generated_speech(&open_jtalk->engine, data);
        HTS_Engine_weak_refresh(&open_jtalk->engine);
    }
    JPCommon_refresh(&open_jtalk->jpcommon);
    NJD_refresh(&open_jtalk->njd);
    Mecab_refresh(&open_jtalk->mecab);

    return result;
}

int Open_JTalk_synthesis_buffer_u16(Open_JTalk * open_jtalk, char16_t *txt,
    short ** data)
{
    return Open_JTalk_synthesis_buffer(open_jtalk, conv_u16_u8(txt).c_str(), data);
}

int Open_JTalk_synthesis(Open_JTalk * open_jtalk, const char *txt,
    const char *wav_file_path, const char *log_file_path)
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

    HTS_Engine_refresh(&open_jtalk->engine);
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
        HTS_Engine_weak_refresh(&open_jtalk->engine);
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

int Open_JTalk_synthesis_u16(Open_JTalk * open_jtalk, char16_t *txt,
    char16_t *wav_file_path, char16_t *log_file_path)
{
    return Open_JTalk_synthesis(open_jtalk, conv_u16_u8(txt).c_str(),
        conv_u16_u8(wav_file_path).c_str(), conv_u16_u8(log_file_path).c_str());
}

int Open_JTalk_extract_label(Open_JTalk * open_jtalk, const char *txt,
    char ***labels, int *labelLength)
{
    int result = 0;
    char buff[MAXBUFLEN];

    HTS_Engine_refresh(&open_jtalk->engine);
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

        HTS_Engine_weak_refresh(&open_jtalk->engine);
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

int Open_JTalk_synthesis_labels(Open_JTalk * open_jtalk, const char *txt,
    const char *wav_file_path,
    const char *text_anal_file_path,
    const char *context_label_file_path)
{
    int result = 0;
    char buff[MAXBUFLEN];
    FILE *wavfp = NULL;
    FILE *textfp = NULL;
    FILE *contextfp = NULL;
    if (wav_file_path != NULL) {
        // Do Error Handling
        wavfp = fopen(wav_file_path, "wb");
    }
    if (text_anal_file_path != NULL) {
        // Do Error Handling
        textfp = fopen(text_anal_file_path, "w");
    }
    if (context_label_file_path != NULL) {
        // Do Error Handling
        contextfp = fopen(context_label_file_path, "w");
    }

    HTS_Engine_refresh(&open_jtalk->engine);
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
        if (textfp != NULL)
            NJD_fprint(&open_jtalk->njd, textfp);
        if (contextfp != NULL)
            HTS_Engine_save_label(&open_jtalk->engine, contextfp);
        HTS_Engine_weak_refresh(&open_jtalk->engine);
    }
    JPCommon_refresh(&open_jtalk->jpcommon);
    NJD_refresh(&open_jtalk->njd);
    Mecab_refresh(&open_jtalk->mecab);

    if (wavfp != NULL) {
        fclose(wavfp);
    }
    if (textfp != NULL) {
        fclose(textfp);
    }
    if (contextfp != NULL) {
        fclose(contextfp);
    }

    return result;
}

int Open_JTalk_synthesis_labels_u16(Open_JTalk * open_jtalk, char16_t *txt,
    char16_t *wav_file_path,
    char16_t *text_anal_file_path,
    char16_t *context_label_file_path)
{
    return Open_JTalk_synthesis_labels(open_jtalk, conv_u16_u8(txt).c_str(),
    conv_u16_u8(wav_file_path).c_str(), conv_u16_u8(text_anal_file_path).c_str(),
    conv_u16_u8(context_label_file_path).c_str());
}

int Open_JTalk_resynthesis_buffer(Open_JTalk * open_jtalk, short ** data)
{
    int result = 0;

    if (HTS_Engine_resynthesize(&open_jtalk->engine) == TRUE)
        result = 1;

    if (data != NULL && result == 1)
        result = HTS_Engine_allocate_generated_speech(&open_jtalk->engine, data);
    
    return result;
}

int Open_JTalk_resynthesis(Open_JTalk * open_jtalk, const char *wav_file_path)
{
    int result = 0;
    FILE *wavfp = NULL;

    if (wav_file_path != NULL) {
        // Do Error Handling
        wavfp = fopen(wav_file_path, "wb");
    }

    if (HTS_Engine_resynthesize(&open_jtalk->engine) == TRUE)
        result = 1;

    if (wavfp != NULL)
        HTS_Engine_save_riff(&open_jtalk->engine, wavfp);
    
    if (wavfp != NULL)
        fclose(wavfp);

    return result;
}

int Open_JTalk_resynthesis_u16(Open_JTalk * open_jtalk, char16_t *wav_file_path)
{
    return Open_JTalk_resynthesis(open_jtalk, conv_u16_u8(wav_file_path).c_str());
}

int Open_JTalk_get_lf0_length(Open_JTalk * open_jtalk)
{
    return HTS_Engine_get_total_frame(&open_jtalk->engine);
}

void Open_JTalk_get_lf0_array(Open_JTalk * open_jtalk, double *lf0_buffer, size_t buffer_length)
{
    for (size_t i = 0; i < buffer_length; i++)
        lf0_buffer[i] = HTS_Engine_get_generated_parameter(&open_jtalk->engine, 1 /* lf0 stream */, i, 0 /* lf0 is 1-dim */);
}

void Open_JTalk_set_lf0_array(Open_JTalk * open_jtalk, double *lf0_buffer, size_t buffer_length)
{
    for (size_t i = 0; i < buffer_length; i++)
        open_jtalk->engine.gss.gstream[1].par[i][0] = lf0_buffer[i];
}

void Open_JTalk_set_lf0(Open_JTalk * open_jtalk, double lf0, size_t frame_index)
{
    open_jtalk->engine.gss.gstream[1].par[frame_index][0] = lf0;
}
