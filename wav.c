#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "endianness.h"
#include "wav.h"

static const uint32_t RIFF = 0x52494646;//"RIFF"
static const uint32_t WAV = 0x57415645;//"WAV"
static const uint32_t FMT = 0x666d7420; //"fmt "
static const uint32_t DATA = 0x64617461; //"data"


void write_wav_sample(FILE *stream, wav_sample_t *sample) {
    if(stream==NULL)
        return;

    if(sample==NULL)
        return;

    //printf("mem: %08x-%08x\n",*((int*) sample->sampleData),*(((int*) sample->sampleData)+1));
    //printf("bytes: %x\n",sample->DataSize);
    fseek(stream,0,SEEK_END);
    fwrite(sample->sampleData,sample->DataSize,1,stream);

    return;
}

void write_wav_header(FILE *stream,unsigned int channels,unsigned int samplerate, unsigned int sampleBytes,unsigned int samples) {

    if(stream==NULL)
        return;
    samples--;

    wav_riff_t RiffHeader = {
        to_be32(RIFF), 
        to_le32(36 + sizeof(wav_data_t) + (samples*channels*sampleBytes)),
        to_be32(WAV)
    };

    wav_fmt_t FmtHeader = {
        to_be32(FMT),
        to_le32(16), //we'll use PCM
        to_le16(1), //we'll use 1 for PCM
        to_le16(channels),
        to_le32(samplerate),
        to_le32(samplerate * channels * sampleBytes),
        to_le16(channels * sampleBytes),
        to_le16(sampleBytes*8)
    };

    wav_data_t DataHeader = {
        to_be32(DATA),
        to_le32(samples * channels * sampleBytes)
    };

    wav_header_t WavHeader = {
        RiffHeader,
        FmtHeader,
        DataHeader
    };

    fwrite(&WavHeader,sizeof(wav_header_t),1,stream);
}

int ret_read_header(int status,char* buffer) {
    free(buffer);
    return status;
}

int read_wav_header(FILE *stream, wav_header_t *dst) {

    if(dst == NULL) 
        return -1;

    if(stream == NULL)
        return -2;

    char *buffer = malloc(WAV_HEADER_SIZE);

    if(buffer==NULL)
        return ret_read_header(-3,buffer);

    fread(buffer,WAV_HEADER_SIZE,1,stream);

    dst->Riff.ChunkID = to_be32( *((uint32_t*)buffer));

    if(dst->Riff.ChunkID != RIFF)
        return ret_read_header(-4,buffer);

    dst->Riff.ChunkSize = to_le32( *((uint32_t*)(buffer + 4 )));
    dst->Riff.Format = to_be32( *((uint32_t*) (buffer + 8 )));

    if(dst->Riff.Format != WAV)
        return ret_read_header(-5,buffer);

    dst->Fmt.Subchunk1ID = to_be32( *((uint32_t*) (buffer + 12 )));

    if(dst->Fmt.Subchunk1ID != FMT)
        return ret_read_header(-6,buffer);

    dst->Fmt.Subchunk1Size = to_le32( *((uint32_t*) (buffer + 16 )));

    if(dst->Fmt.Subchunk1Size!= 16)
        return ret_read_header(-7,buffer);

    dst->Fmt.AudioFormat = to_le16( *((uint16_t*) (buffer + 20 )));

    if(dst->Fmt.AudioFormat != 1)
        return ret_read_header(-8,buffer);

    dst->Fmt.NumChannels = to_le16( *((uint16_t*) (buffer + 22)));
    dst->Fmt.SampleRate = to_le32( *((uint32_t*) (buffer + 24)));
    dst->Fmt.ByteRate = to_le32( *((uint32_t*) (buffer + 28)));
    dst->Fmt.BlockAlign = to_le16( *((uint16_t*) (buffer + 32)));
    dst->Fmt.BytesPerSample = to_le16( *((uint16_t*) (buffer + 34)));

    dst->Data.Subchunk2ID = to_be32( *((uint32_t*) (buffer + 36)));

    if(dst->Data.Subchunk2ID != DATA)
        return ret_read_header(-9,buffer);

    dst->Data.Subchunk2Size = to_le32( *((uint32_t*) (buffer + 40)));

    return ret_read_header(0,buffer);
}

int read_wav_samples(FILE *stream, unsigned int* retArr, unsigned int size, unsigned int sampleBytes, unsigned int offset) {
    if(stream==NULL)
        return -1;

    if(retArr==NULL)
        return -2;

    char *buffer = malloc(size*(sampleBytes));

    if(buffer==NULL)
        return -3;

    fseek(stream, WAV_HEADER_SIZE + offset*sizeof(int),SEEK_CUR);
    fread(buffer,size*(sampleBytes),1,stream);

    int i;
    for(i = 0; i <= size*(sampleBytes); i+= (sampleBytes) ) {
        *(retArr+i) = to_le32( *(uint32_t*) (buffer + i));
        printf("i: %d, data: %x\n",i,to_le32( *(uint32_t*) (buffer + i)));
    }

    free(buffer);

    return 0;
}