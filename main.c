#include "wave.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main() {

    char *file2 = "test2.wave";

    FILE *f = fopen(file2,"wb");
    unsigned int test_channels = 3;
    unsigned int test_sampleRate = 44000;
    unsigned int test_sampleByte = 2;
    unsigned int test_samplesPerChannel = 0xf;


    write_wave_header(f,test_channels,test_sampleRate,test_sampleByte,test_samplesPerChannel);

    void *sampleData = malloc(sizeof(char)*test_channels*test_sampleByte);
    if(sampleData==NULL)
        return;

    wave_sample_t sample = {
        test_channels,
        test_sampleByte,
        test_channels*test_sampleByte,
        NULL
    };


    int i,j;
    for(i=0;i<test_samplesPerChannel;i++) {
        for(j=0;j<test_channels;j++) {
            *(((short *)sampleData) + j*test_sampleByte) = to_le16((i % 100 + 5000) & 0xFFFF);
            //printf("data: %x\n",(i  % 100 + 5000) & 0xFFFF);
            //printf("mem: %x%x%x\n",*((short*) sampleData),*((short*) sampleData+2),*((short*) sampleData+4));
        }
        //printf("NEXT###\n");
        sample.sampleData = sampleData;
        write_wave_sample(f,&sample);
        exit(0);
    }

    free(sampleData);
    fclose(f);

    wave_header_t *readHeader = malloc(sizeof(wave_header_t));

    if(readHeader==NULL) {
        printf("Something failed :(\n");
        return 0;
    }

    FILE *f2 = fopen(file2,"rb");

    read_wave_header(f2,readHeader);

    int samplePerChannel = readHeader->Data.Subchunk2Size / (readHeader->Fmt.NumChannels * (readHeader->Fmt.BytesPerSample));
    printf("samplePerChannel = %d\n",samplePerChannel);

    fclose(f2);
    free(readHeader);
    


    /*int i,j;

    char* buf = malloc(sizeof(char)*test_channels*test_samplebits/8)
    for(i=0;i <= test_samples; i++) {
        for(j=0;j <= test_channels;j++) {
            
        }
        write_wave_sample(f,(i % 100) + 50,test_samplebits,test_channels);
    }
    fclose(f);
    */
    
    
    /*
    wave_header_t *readHeader = malloc(sizeof(wave_header_t));

    if(readHeader==NULL) {
        printf("Something failed :(\n");
        return 0;
    }

    char *file = "test.wave";

    FILE *fh = fopen(file,"rb");

    read_wave_header(fh,readHeader);
    

    int sampleCount = readHeader->Data.Subchunk2Size / (readHeader->Fmt.NumChannels * (readHeader->Fmt.BitsPerSample/8));
    printf("samples = %d\n",sampleCount);

    int *samples = malloc(sizeof(int)*sampleCount+1);

    read_wave_samples(fh,samples,sampleCount-1,readHeader->Fmt.BitsPerSample,0);

    /*int i;
    for(i=0; i < sampleCount-1; i++)
        printf("Sample %d: %x\n",i,*(samples + 4*i));

    fclose(fh);
    free(samples);
    free(readHeader);*/
    
    return 0;

}