#include "wave.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main() {

    char *file = "test.wave";

    FILE *f = fopen(file,"wb");
    unsigned int test_channels = 2;
    unsigned int test_samplerate = 44000;
    unsigned int test_samplebits = 16;
    unsigned int test_samples = 2*2*2*2*2*2*2*2-1;


    /* write wave header */
    write_wave_header(f,test_channels,test_samplerate,test_samplebits,test_samples);
    int i;
    for(i=0;i < 2*2*2*2*2*2*2*2; i++) {
        write_wave_sample(f,(i % 100) + 40,test_samplebits);
    }
    fclose(f);
}