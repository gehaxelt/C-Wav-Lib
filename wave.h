#ifndef WAVE_H_
#define WAVE_H_


//Wave.h
//Reads / write RIFF Wave headers
//URL: https://ccrma.stanford.edu/courses/422/projects/WaveFormat/

#include <stdio.h>

/*
Wave header size in bytes
*/
static const int WAVE_HEADER_SIZE = 44;

/*
Writes the RIFF wave header.
@param: FILE stream - The file to write the header to
@param: unsigned int channels - The number of channels; Mono = 1 ; Stereo = 2
@param: unsigned int samplerate - The sample rate ; e.g 8000, 44000
@param: unsigned int sampleBits - The bits per sample
@param: unsigned int samples - Number of the samples that will be written to the file
*/
void write_wave_header(FILE *stream,unsigned int channels,unsigned int samplerate,unsigned int sampleBits,unsigned int samples);

void write_wave_sample(FILE *stream, unsigned int sample, unsigned int sampleBits);

#endif /* WAVE_H_ */