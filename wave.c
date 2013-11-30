#include <stdio.h>
#include <stdint.h>

#include "wave.h"

static const uint32_t RIFF = 0x52494646;//"RIFF"
static const uint32_t WAVE = 0x57415645;//"WAVE"
static const uint32_t FMT = 0x666d7420; //"fmt "
static const uint32_t DATA = 0x64617461; //"data"

/*
The RIFF header!
*/
typedef struct {
    uint32_t ChunkID;
    uint32_t ChunkSize;
    uint32_t Format;
} wave_riff_t;

/*
The FMT header!
*/
typedef struct {
    uint32_t Subchunk1ID;
    uint32_t Subchunk1Size;
    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;
} wave_fmt_t;

/*
The Data header!
*/
typedef struct {
    uint32_t Subchunk2ID;
    uint32_t Subchunk2Size;
} wave_data_t;

/*
The complete header!
*/
typedef struct {
    wave_riff_t Riff;
    wave_fmt_t Fmt;
    wave_data_t Data;
} wave_header_t;

/*
 * True if system is little-endian, false otherwise.
 */
int is_little_endian()
{
    int one = 1;
    char *ptr = (char *) &one;
    return *ptr == 1;
}

/*
 * True if system is big-endian, false otherwise.
 */
int is_big_endian()
{
    return (int) ! is_little_endian();
}

/*
 * Converts the 16-bit value to little-endian representation.
 */
uint16_t to_le16(uint16_t value)
{
    return is_little_endian() ?
            value : ((value & 0x00ff) >> 8) | ((value & 0xff00) << 8);
}

/*
 * Converts the 16-bit value to big-endian representation.
 */
uint16_t to_be16(uint16_t value)
{
    return is_big_endian() ?
            value : ((value & 0xff00) << 8) | ((value & 0x00ff) >> 8);
}

/*
 * Converts the 32-bit value to big-endian representation.
 */
uint32_t to_be32(uint32_t value)
{
    return is_big_endian() ?
            value :
            ((value & 0xff000000) >> 24) | ((value & 0x00ff0000) >> 8)
                    | ((value & 0x0000ff00) << 8) | ((value & 0x000000ff) << 24);
}

/*
 * Converts the 32-bit value to little-endian representation.
 */
uint32_t to_le32(uint32_t value)
{
    return is_little_endian() ?
            value :
            ((value & 0x000000ff) << 24) | ((value & 0x0000ff00) << 8)
                    | ((value & 0x00ff0000) >> 8) | ((value & 0xff000000) >> 24);
}

void write_wave_sample(FILE *stream, unsigned int sample, unsigned int sampleBits) {
    fwrite(&sample,sampleBits / 8,1,stream);
}

void write_wave_header(FILE *stream,unsigned int channels,unsigned int samplerate,unsigned int sampleBits,unsigned int samples) {

    wave_riff_t RiffHeader = {
        to_be32(RIFF), 
        to_le32(36 + sizeof(wave_data_t) + (samples*channels*samples/8)),
        to_be32(WAVE)
    };

    wave_fmt_t FmtHeader = {
        to_be32(FMT),
        to_le32(16), //we'll use PCM
        to_le16(1), //we'll use 1 for PCM
        to_le16(channels),
        to_le32(samplerate),
        to_le32(samplerate * channels * sampleBits / 8),
        to_le16(channels * sampleBits / 8),
        to_le16(sampleBits)
    };

    wave_data_t DataHeader = {
        to_be32(DATA),
        to_le32(samples * channels * sampleBits/8)
    };

    wave_header_t WaveHeader = {
        RiffHeader,
        FmtHeader,
        DataHeader
    };

    fwrite(&WaveHeader,sizeof(wave_header_t),1,stream);
}