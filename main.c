#include "wave.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main() {

    char *file2 = "test2.wave";

    FILE *f = fopen(file2,"wb");
    unsigned int test_channels = 2;
    unsigned int test_samplerate = 44000;
    unsigned int test_samplebits = 16;
    unsigned int test_samples = 0xff-1;


    write_wave_header(f,test_channels,test_samplerate,test_samplebits,test_samples);
    int i;
    for(i=0;i < 0xff; i++) {
        write_wave_sample(f,(i % 100) + 50,test_samplebits);
    }
    fclose(f);
    
    

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
    */
    fclose(fh);
    free(samples);
    free(readHeader);
    

}