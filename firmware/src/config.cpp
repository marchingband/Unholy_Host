#include "config.h"
#include <FlashStorage.h>
#include "calibration.h"
#include "config.h"
#include "handlers.h"

struct config_t config_default;
struct config_t _config;
struct config_t *config;

void build_default_config (void)
{
    config_default.calibration_mode =         false;
    
    config_default.POLYPHONY_MODE =           MONOPHONIC_MODE;
    config_default.CV_1_SOURCE =              NOTE;
    config_default.CV_2_SOURCE =              NOTE;
    config_default.CV_3_SOURCE =              NOTE;
    config_default.CV_1_AND_2_MODE =          LAST;
    config_default.CV_1_AND_2_AND_3_MODE =    LAST;
    config_default.CV_1_SCALE =               V_OCT;
    config_default.CV_2_SCALE =               V_OCT;
    config_default.CV_3_SCALE =               V_OCT;
    config_default.PITCH_BEND_RANGE =         12;
    config_default.CC1_COMMAND =              7;
    config_default.CC2_COMMAND =              10;
    config_default.CC3_COMMAND =              11;
    config_default.CLOCK_1_DIVIDER =          2;
    config_default.CLOCK_2_DIVIDER =          3;
    config_default.CLOCK_3_DIVIDER =          6;
    config_default.CLOCK_4_DIVIDER =          12;
    config_default.CLOCK_5_DIVIDER =          24;
    config_default.CLOCK_6_DIVIDER =          48;
    config_default.CLOCK_7_DIVIDER =          96;
    config_default.CLOCK_8_DIVIDER =          96;
    config_default.RESET_1_BEATS =            4;
    config_default.RESET_2_BEATS =            16;
    config_default.RESET_3_BEATS =            32;
    config_default.GATE_1_SOURCE =            MONOPHONIC_ON_OFF;
    config_default.GATE_1_NOTE =              C_6;
    config_default.GATE_1_INVERT =            0;
    config_default.GATE_2_SOURCE =            MONOPHONIC_ON_OFF;
    config_default.GATE_2_NOTE =              C_6;
    config_default.GATE_2_INVERT =            0;
    config_default.GATE_3_SOURCE =            MONOPHONIC_ON_OFF;
    config_default.GATE_3_NOTE =              C_6;
    config_default.GATE_3_INVERT =            0;
    config_default.GATE_4_SOURCE =            MONOPHONIC_ON_OFF;
    config_default.GATE_4_NOTE =              C_6;
    config_default.GATE_4_INVERT =            0;
    config_default.GATE_5_SOURCE =            MONOPHONIC_ON_OFF;
    config_default.GATE_5_NOTE =              C_6;
    config_default.GATE_5_INVERT =            0;
    config_default.GATE_6_SOURCE =            MONOPHONIC_ON_OFF;
    config_default.GATE_6_NOTE =              C_6;
    config_default.GATE_6_INVERT =            0;
    config_default.GATE_7_SOURCE =            MONOPHONIC_ON_OFF;
    config_default.GATE_7_NOTE =              C_6;
    config_default.GATE_7_INVERT =            0;
    config_default.GATE_8_SOURCE =            MONOPHONIC_ON_OFF;
    config_default.GATE_8_NOTE =              C_6;
    config_default.GATE_8_INVERT =            0;
    config_default.CAL_1_0 =                  0.002;
    config_default.CAL_1_1 =                  0.077;
    config_default.CAL_1_2 =                  1.074;
    config_default.CAL_1_3 =                  2.066;
    config_default.CAL_1_4 =                  3.061;
    config_default.CAL_1_5 =                  4.053;
    config_default.CAL_1_6 =                  4.877;
    config_default.CAL_2_0 =                  0.001;
    config_default.CAL_2_1 =                  0.084;
    config_default.CAL_2_2 =                  1.071;
    config_default.CAL_2_3 =                  2.065;
    config_default.CAL_2_4 =                  3.056;
    config_default.CAL_2_5 =                  4.047;
    config_default.CAL_2_6 =                  4.874;
    config_default.CAL_3_0 =                  0.022;
    config_default.CAL_3_1 =                  0.080;
    config_default.CAL_3_2 =                  1.067;
    config_default.CAL_3_3 =                  2.051;
    config_default.CAL_3_4 =                  3.037;
    config_default.CAL_3_5 =                  4.074;
    config_default.CAL_3_6 =                  5.148;
}

FlashStorage(config_store, struct config_t);

void init_config( void )
{
    build_default_config();
    config = &_config;
    _config = config_store.read();
    if(_config.valid == false)
    {
        Serial1.println("initializing flash storage");
        _config = config_default;
        _config.valid = true;
        config_store.write(_config);
        Serial1.println("initializing flash storage done");
    }
    else
    {
        Serial1.println("flash storage found");
    }
}

void save_config( void )
{
    config_store.write(_config);
    Serial1.println("update flash storage done");
}

void encode_config(uint8_t *dest)
{
    dest[0] = config->POLYPHONY_MODE;
    dest[1] = config->CV_1_SOURCE;
    dest[2] = config->CV_2_SOURCE;
    dest[3] = config->CV_3_SOURCE;
    dest[4] = config->CV_1_AND_2_MODE;
    dest[5] = config->CV_1_AND_2_AND_3_MODE;
    dest[6] = config->CV_1_SCALE;
    dest[7] = config->CV_2_SCALE;
    dest[8] = config->CV_3_SCALE;
    dest[9] = config->PITCH_BEND_RANGE;
    dest[10] = config->CC1_COMMAND;
    dest[11] = config->CC2_COMMAND;
    dest[12] = config->CC3_COMMAND;
    dest[13] = config->CLOCK_1_DIVIDER;
    dest[14] = config->CLOCK_2_DIVIDER;
    dest[15] = config->CLOCK_3_DIVIDER;
    dest[16] = config->CLOCK_4_DIVIDER;
    dest[17] = config->CLOCK_5_DIVIDER;
    dest[18] = config->CLOCK_6_DIVIDER;
    dest[19] = config->CLOCK_7_DIVIDER;
    dest[20] = config->CLOCK_8_DIVIDER;
    dest[21] = config->RESET_1_BEATS;
    dest[22] = config->RESET_2_BEATS;
    dest[23] = config->RESET_3_BEATS;
    dest[24] = config->GATE_1_SOURCE;
    dest[25] = config->GATE_1_NOTE;
    dest[26] = config->GATE_1_INVERT;
    dest[27] = config->GATE_2_SOURCE;
    dest[28] = config->GATE_2_NOTE;
    dest[29] = config->GATE_2_INVERT;
    dest[30] = config->GATE_3_SOURCE;
    dest[31] = config->GATE_3_NOTE;
    dest[32] = config->GATE_3_INVERT;
    dest[33] = config->GATE_4_SOURCE;
    dest[34] = config->GATE_4_NOTE;
    dest[35] = config->GATE_4_INVERT;
    dest[36] = config->GATE_5_SOURCE;
    dest[37] = config->GATE_5_NOTE;
    dest[38] = config->GATE_5_INVERT;
    dest[39] = config->GATE_6_SOURCE;
    dest[40] = config->GATE_6_NOTE;
    dest[41] = config->GATE_6_INVERT;
    dest[42] = config->GATE_7_SOURCE;
    dest[43] = config->GATE_7_NOTE;
    dest[44] = config->GATE_7_INVERT;
    dest[45] = config->GATE_8_SOURCE;
    dest[46] = config->GATE_8_NOTE;
    dest[47] = config->GATE_8_INVERT;

    int i = 48;
    encode_float(&dest[i], config->CAL_1_0);
    i += 4;
    encode_float(&dest[i], config->CAL_1_1);
    i += 4;
    encode_float(&dest[i], config->CAL_1_2);
    i += 4;
    encode_float(&dest[i], config->CAL_1_3);
    i += 4;
    encode_float(&dest[i], config->CAL_1_4);
    i += 4;
    encode_float(&dest[i], config->CAL_1_5);
    i += 4;
    encode_float(&dest[i], config->CAL_1_6);
    i += 4;
    encode_float(&dest[i], config->CAL_2_0);
    i += 4;
    encode_float(&dest[i], config->CAL_2_1);
    i += 4;
    encode_float(&dest[i], config->CAL_2_2);
    i += 4;
    encode_float(&dest[i], config->CAL_2_3);
    i += 4;
    encode_float(&dest[i], config->CAL_2_4);
    i += 4;
    encode_float(&dest[i], config->CAL_2_5);
    i += 4;
    encode_float(&dest[i], config->CAL_2_6);
    i += 4;
    encode_float(&dest[i], config->CAL_3_0);
    i += 4;
    encode_float(&dest[i], config->CAL_3_1);
    i += 4;
    encode_float(&dest[i], config->CAL_3_2);
    i += 4;
    encode_float(&dest[i], config->CAL_3_3);
    i += 4;
    encode_float(&dest[i], config->CAL_3_4);
    i += 4;
    encode_float(&dest[i], config->CAL_3_5);
    i += 4;
    encode_float(&dest[i], config->CAL_3_6);
    i += 4;

    // add the checksum
    dest[SYSEX_CONFIG_MSG_LEN] = calculate_checksum(dest, SYSEX_CONFIG_MSG_LEN);
}

void decode_config(uint8_t *src)
{
    uint8_t checksum = src[SYSEX_CONFIG_MSG_LEN];
    uint8_t check = calculate_checksum(src, SYSEX_CONFIG_MSG_LEN);

    if(check != checksum){
        Serial1.println("checksum failed!");
        return;
    }
    Serial1.println("checksum passed");

    config->POLYPHONY_MODE = src[0];
    config->CV_1_SOURCE = src[1];
    config->CV_2_SOURCE = src[2];
    config->CV_3_SOURCE = src[3];
    config->CV_1_AND_2_MODE = src[4];
    config->CV_1_AND_2_AND_3_MODE = src[5];
    config->CV_1_SCALE = src[6];
    config->CV_2_SCALE = src[7];
    config->CV_3_SCALE = src[8];
    config->PITCH_BEND_RANGE = src[9];
    config->CC1_COMMAND = src[10];
    config->CC2_COMMAND = src[11];
    config->CC3_COMMAND = src[12];
    config->CLOCK_1_DIVIDER = src[13];
    config->CLOCK_2_DIVIDER = src[14];
    config->CLOCK_3_DIVIDER = src[15];
    config->CLOCK_4_DIVIDER = src[16];
    config->CLOCK_5_DIVIDER = src[17];
    config->CLOCK_6_DIVIDER = src[18];
    config->CLOCK_7_DIVIDER = src[19];
    config->CLOCK_8_DIVIDER = src[20];
    config->RESET_1_BEATS = src[21];
    config->RESET_2_BEATS = src[22];
    config->RESET_3_BEATS = src[23];
    config->GATE_1_SOURCE = src[24];
    config->GATE_1_NOTE = src[25];
    config->GATE_1_INVERT = src[26];
    config->GATE_2_SOURCE = src[27];
    config->GATE_2_NOTE = src[28];
    config->GATE_2_INVERT = src[29];
    config->GATE_3_SOURCE = src[30];
    config->GATE_3_NOTE = src[31];
    config->GATE_3_INVERT = src[32];
    config->GATE_4_SOURCE = src[33];
    config->GATE_4_NOTE = src[34];
    config->GATE_4_INVERT = src[35];
    config->GATE_5_SOURCE = src[36];
    config->GATE_5_NOTE = src[37];
    config->GATE_5_INVERT = src[38];
    config->GATE_6_SOURCE = src[39];
    config->GATE_6_NOTE = src[40];
    config->GATE_6_INVERT = src[41];
    config->GATE_7_SOURCE = src[42];
    config->GATE_7_NOTE = src[43];
    config->GATE_7_INVERT = src[44];
    config->GATE_8_SOURCE = src[45];
    config->GATE_8_NOTE = src[46];
    config->GATE_8_INVERT = src[47];
    int i = 48;
    
    config->CAL_1_0 = decode_float(&src[i]);
    i += 4;
    config->CAL_1_1 = decode_float(&src[i]);
    i += 4;
    config->CAL_1_2 = decode_float(&src[i]);
    i += 4;
    config->CAL_1_3 = decode_float(&src[i]);
    i += 4;
    config->CAL_1_4 = decode_float(&src[i]);
    i += 4;
    config->CAL_1_5 = decode_float(&src[i]);
    i += 4;
    config->CAL_1_6 = decode_float(&src[i]);
    i += 4;
    config->CAL_2_0 = decode_float(&src[i]);
    i += 4;
    config->CAL_2_1 = decode_float(&src[i]);
    i += 4;
    config->CAL_2_2 = decode_float(&src[i]);
    i += 4;
    config->CAL_2_3 = decode_float(&src[i]);
    i += 4;
    config->CAL_2_4 = decode_float(&src[i]);
    i += 4;
    config->CAL_2_5 = decode_float(&src[i]);
    i += 4;
    config->CAL_2_6 = decode_float(&src[i]);
    i += 4;
    config->CAL_3_0 = decode_float(&src[i]);
    i += 4;
    config->CAL_3_1 = decode_float(&src[i]);
    i += 4;
    config->CAL_3_2 = decode_float(&src[i]);
    i += 4;
    config->CAL_3_3 = decode_float(&src[i]);
    i += 4;
    config->CAL_3_4 = decode_float(&src[i]);
    i += 4;
    config->CAL_3_5 = decode_float(&src[i]);
    i += 4;
    config->CAL_3_6 = decode_float(&src[i]);
    i += 4;

    // update all the handlers so it actually changes immidiately
    init_handlers();

    // save to disk
    save_config();
}

void encode_float(uint8_t *dest, double in)
{
    char buf[6];

    char a[2], b[2], c[2], d[2];
    int  w, x, y, z;    

    sprintf(buf, "%5.3f", in);

    // Serial1.println(buf);

    a[0] = buf[0]; a[1]=0;
    //skip decimal point
    b[0] = buf[2]; b[1]=0;
    c[0] = buf[3]; c[1]=0;
    d[0] = buf[4]; d[1]=0;  

    //convert back into numeric discretes if necessary
    w = atoi(a);
    x = atoi(b);
    y = atoi(c);
    z = atoi(d);

    // char log[50];
    // sprintf(log, "encoded %d%d%d%d", w, x, y, z);
    // Serial1.println(log);

    dest[0] = w;
    dest[1] = x;
    dest[2] = y;
    dest[3] = z;
}

double decode_float(uint8_t *src)
{
    // char log[50];
    // sprintf(log, "%d,%d,%d,%d", src[0], src[1], src[2], src[3]);
    // Serial1.println(log);
    
    double out = 0.0;
    out += src[0];
    out += (((double)src[1]) * 0.1);
    out += (((double)src[2]) * 0.01);
    out += (((double)src[3]) * 0.001);

    // sprintf(log, "parsed %f", out);
    // Serial1.println(log);

    return out;
}

uint8_t calculate_checksum(uint8_t *data, int len)
{
  uint8_t res = 0x7f;
  for(int i = 0; i < len; i++) {
    res ^= data[i];
  }
  return res;
}