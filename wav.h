#ifndef WAV_H_
#define WAV_H_


//Wav.h
//Reads / write RIFF Wav headers
//URL: https://ccrma.stanford.edu/courses/422/projects/WavFormat/

#include <stdio.h>
#include <stdint.h>

/*
The RIFF header!
*/
typedef struct {
    uint32_t ChunkID; //"RIFF"
    uint32_t ChunkSize; //"36 + sizeof(wav_data_t) + data"
    uint32_t Format; // "WAV"
} wav_riff_t;

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
} wav_fmt_t;

/*
The Data header!
*/
typedef struct {
    uint32_t Subchunk2ID; //"data"
    uint32_t Subchunk2Size; //== NumSamples * NumChannels * bytePerSample/8
} wav_data_t;

/*
The complete header!
*/
typedef struct {
    wav_riff_t Riff;
    wav_fmt_t Fmt;
    wav_data_t Data;
} wav_header_t;

/*
* The sample struct
*/
typedef struct {
    unsigned int Channels;
    unsigned int BytesPerSample;//---size
    unsigned int DataSize;//        |
    void *sampleData; // [chan 1][chan 2][chan 3] ; Pure memory, every channel value needs to be in little endian format
} wav_sample_t;

/*
Wav header size in bytes
*/
static const int WAV_HEADER_SIZE = 44;

/*
Writes the wav header.
@param: FILE *stream - The file to write the header to
@param: unsigned int channels - The number of channels; Mono = 1 ; Stereo = 2
@param: unsigned int samplerate - The sample rate ; e.g 8000, 44000
@param: unsigned int sampleBytes - The byte per sample
@param: unsigned int samples - Number of the samples per channel that will be written to the file. 
*/
void write_wav_header(FILE *stream,unsigned int channels,unsigned int samplerate,unsigned int sampleBytes, unsigned int samples);

/*
Reads a wav header
@param: FILE *stream - the file to read the header from
@param: wav_header_t *dst - The wav_header_t structure to write the header to
*/
int read_wav_header(FILE *stream, wav_header_t *dst);

/*
Writes a sample to a file
@param: FILE *stream - The file to write the header to
@param: wav_sample_t *sample - The sample to write
*/
void write_wav_sample(FILE *stream, wav_sample_t *sample);

/*
Reads samples from a wav file
@param: FILE *stream - the file to read the samples from
@param: unsigned int retArr - 1D array where the samples will be stored
@param: unsigned int size - Amount of elements that can be stored in the array
@param: unsigned int sampleBytes - the number of byte per sample
@param: unsigned int offset - the offset to start to read from the file 
*/
int read_wav_samples(FILE *stream, unsigned int* retArr, unsigned int size, unsigned int sampleBytes, unsigned int offset);

#endif /* WAV_H_ */