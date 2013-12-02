#ifndef WAVE_H_
#define WAVE_H_


//Wave.h
//Reads / write RIFF Wave headers
//URL: https://ccrma.stanford.edu/courses/422/projects/WaveFormat/

#include <stdio.h>
#include <stdint.h>

/*
The RIFF header!
*/
typedef struct {
    uint32_t ChunkID; //"RIFF"
    uint32_t ChunkSize; //"36 + sizeof(wave_data_t) + data"
    uint32_t Format; // "WAVE"
} wave_riff_t;

/*
The FMT header!
*/
typedef struct {
    uint32_t Subchunk1ID; //"fmt "
    uint32_t Subchunk1Size; //16 (PCM)
    uint16_t AudioFormat; // 1 'cause PCM
    uint16_t NumChannels; // mono = 1; stereo = 2
    uint32_t SampleRate; // 8000, 44100, etc.
    uint32_t ByteRate; //== SampleRate * NumChannels * byte
    uint16_t BlockAlign; //== NumChannels * bytePerSample
    uint16_t BytesPerSample; //8 byte = 8, 16 byte = 16, etc.
} wave_fmt_t;

/*
The Data header!
*/
typedef struct {
    uint32_t Subchunk2ID; //"data"
    uint32_t Subchunk2Size; //== NumSamples * NumChannels * bytePerSample/8
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
* The sample struct
*/
typedef struct {
    unsigned int Channels;
    unsigned int BytesPerSample;//---size
    unsigned int DataSize;//        |
    void *sampleData; // [chan 1][chan 2][chan 3] ; Pure memory, but we're using char* for easier adressing
} wave_sample_t;

/*
Wave header size in bytes
*/
static const int WAVE_HEADER_SIZE = 44;

/*
Writes the wave header.
@param: FILE *stream - The file to write the header to
@param: unsigned int channels - The number of channels; Mono = 1 ; Stereo = 2
@param: unsigned int samplerate - The sample rate ; e.g 8000, 44000
@param: unsigned int sampleBytes - The byte per sample
@param: unsigned int samples - Number of the samples per channel that will be written to the file. 
*/
void write_wave_header(FILE *stream,unsigned int channels,unsigned int samplerate,unsigned int sampleBytes, unsigned int samples);

/*
Reads a wave header
@param: FILE *stream - the file to read the header from
@param: wave_header_t *dst - The wave_header_t structure to write the header to
*/
int read_wave_header(FILE *stream, wave_header_t *dst);

/*
Writes a sample to a file
@param: FILE *stream - The file to write the header to
@param: wave_sample_t *sample - The sample to write
*/
void write_wave_sample(FILE *stream, wave_sample_t *sample);

/*
Reads samples from a wave file
@param: FILE *stream - the file to read the samples from
@param: unsigned int retArr - 1D array where the samples will be stored
@param: unsigned int size - Amount of elements that can be stored in the array
@param: unsigned int sampleBytes - the number of byte per sample
@param: unsigned int offset - the offset to start to read from the file 
*/
int read_wave_samples(FILE *stream, unsigned int* retArr, unsigned int size, unsigned int sampleBytes, unsigned int offset);

#endif /* WAVE_H_ */