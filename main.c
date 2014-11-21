#include "wav.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

int main() {

    char *file2 = "test2.wav";

    FILE *f = fopen(file2,"wb");
    FILE *random = fopen("/dev/urandom","r");

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
    unsigned short note;
    for(i=0;i<test_samplesPerChannel*test_seconds;i++) {
        if(i % (test_samplesPerChannel/(i % 5 + 2)) == 0) {
            note = 0;
            fread(&note, sizeof(note),1,random);
            note = note % 600 + 50;
            printf("Hertz: %d\n",note);

        }
        for(j=0;j<test_channels;j++) {
            test_hertz = (unsigned int) note;
            low = sin( (double) i * ( ((double) (test_hertz * 2 * M_PI) / (double)test_sampleRate)));
            *(((short *)sampleData) + j) = to_le16((short)(( low*4 )*0xFFF));
        }
        sample.sampleData = sampleData;
        write_wav_sample(f,&sample);
    }

    free(sampleData);
    fclose(f);
    fclose(random);
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
