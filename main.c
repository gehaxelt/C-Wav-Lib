#include "wav.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

int main() {

    char *file2 = "test2.wav";

    FILE *f = fopen(file2,"wb");
    unsigned int test_channels = 2;
    unsigned int test_sampleRate = 44000;
    unsigned int test_sampleByte = 2;
    unsigned int test_samplesPerChannel = 44000;
    unsigned int test_seconds = 60;
    unsigned int test_hertz = 440;


    write_wav_header(f,test_channels,test_sampleRate,test_sampleByte,test_samplesPerChannel*test_seconds);

    void *sampleData = malloc(sizeof(char)*test_channels*test_sampleByte*test_seconds);
    if(sampleData==NULL)
        return;

    wav_sample_t sample = {
        test_channels,
        test_sampleByte,
        test_channels*test_sampleByte,
        NULL
    };

    int i,j;
    double high,low;
    for(i=0;i<test_samplesPerChannel*test_seconds;i++) {
        for(j=0;j<test_channels;j++) {
            //                                             sin(2*Pi*(Hz / 44000)*i)
            high = sin( (double) i * ( ((double) (test_hertz + ((double)(i % 1000))/440)  * 2 * M_PI) / (double)test_sampleRate));
            if((i%(22000))<1100*test_seconds) {
                low = sin( (double) i * ( ((double) (test_hertz + (50)  * 2 * M_PI) / (double)test_sampleRate)));
            } else {
                low = sin( (double) i * ( ((double) (test_hertz + (75)  * 2 * M_PI) / (double)test_sampleRate)));
            }
            *(((short *)sampleData) + j) = to_le16((short)(( high/2 + low*4 )*0xFFF));
        }
        sample.sampleData = sampleData;
        write_wav_sample(f,&sample);
    }

    free(sampleData);
    fclose(f);

    wav_header_t *readHeader = malloc(sizeof(wav_header_t));

    if(readHeader==NULL) {
        printf("Something failed :(\n");
        return 0;
    }

    FILE *f2 = fopen(file2,"rb");

    read_wav_header(f2,readHeader);

    int samplePerChannel = readHeader->Data.Subchunk2Size / (readHeader->Fmt.NumChannels * (readHeader->Fmt.BytesPerSample));
    printf("samplePerChannel = %d\n",samplePerChannel);

    fclose(f2);
    free(readHeader);
    
    return 0;

}
