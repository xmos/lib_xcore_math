#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wav.h"
#include "xmath/xmath.h"

#define BLOCK_SIZE_LOG2 9
#define BLOCK_SIZE (1 << BLOCK_SIZE_LOG2)



/*

Here is an example of a low pass filter. It simply multiplies the 
high frequnecies by 0 and the lower ones by 1. Remember that
there are BLOCK_SIZE bins in the frequency domain(in this program)
where DC is in the lower bins and NQ is in the higher bins.
*/
void process_freq_domain_example(bfp_complex_s32_t* A ){

  //let's chop off everything above BLOCK_SIZE / 8
  //which is 44100 / 8 = 5.5KHz
  int cut_off = BLOCK_SIZE / 8;
  const exponent_t exp = 0;
  bfp_s32_t filter;
  int32_t filter_buffer[BLOCK_SIZE];
  for (int i = 0; i <cut_off ; i++) filter_buffer[i] = 1;
  for (int i = cut_off; i < BLOCK_SIZE; i++) filter_buffer[i] = 0;

  bfp_s32_init(&filter, filter_buffer, exp, BLOCK_SIZE, 1);

  bfp_complex_s32_real_mul(A, A, &filter);
}

/*
Here's a starting point for inspecting the frequency domain of the current 
block. It takes A and converts it to the magnitude of each bin.
This is a very useful starting point for a noise suppressor.
*/
void process_freq_domain(bfp_complex_s32_t* A ){
  bfp_s32_t mag;
  const exponent_t exp = 0;
  int32_t mag_buffer[BLOCK_SIZE];
  bfp_s32_init(&mag, mag_buffer, exp, BLOCK_SIZE, 0);

  bfp_complex_s32_mag(&mag, A);


}

/*
Here's a starting point for adjusting the
*/
void process_time_domain(bfp_s32_t* td_samples ){

  for(int i=0;i<BLOCK_SIZE*2;i++){
    //modify td_samples
  }
}


void add_new_samples(int16_t buffer[BLOCK_SIZE],
                     int16_t prev_buffer[BLOCK_SIZE],
                     int16_t overlap_buffer[BLOCK_SIZE]) {
  int32_t buffer32[BLOCK_SIZE * 2];

  for (int i = 0; i < BLOCK_SIZE; i++)  // old samples in prev_buffer
    buffer32[i] = (int32_t)prev_buffer[i];
  for (int i = 0; i < BLOCK_SIZE; i++)  // freshest samples in buffer
    buffer32[i + BLOCK_SIZE] = (int32_t)buffer[i];

  // save the latest samples to prev_buffer
  memcpy(prev_buffer, buffer, sizeof(int16_t) * BLOCK_SIZE);

  bfp_s32_t a;
  const exponent_t exp = 0;
  const unsigned length = BLOCK_SIZE * 2;
  const unsigned calc_hr = 1;  // Boolean indicating whether the HR of the BFP
                               // vector should be calculated

  bfp_s32_init(&a, buffer32, exp, length, calc_hr);

  // fft the buffer
  bfp_complex_s32_t* A = bfp_fft_forward_mono(&a);

  process_freq_domain_example(A);

  // ifft the buffer
  bfp_s32_t* b = bfp_fft_inverse_mono(A);

  process_time_domain(b);

  // return to the input exponent
  bfp_s32_use_exponent(b, exp);

  // clipping!
  for (int i = 0; i < BLOCK_SIZE; i++) {
    int32_t v = buffer32[i];
    if (v > INT16_MAX) v = INT16_MAX;
    if (v < INT16_MIN) v = INT16_MIN;
    buffer32[i] = v;
  }

  // overlap and add the old half of the sample to the overlap_buffer
  int16_t output_buffer[BLOCK_SIZE];
  for (int i = 0; i < BLOCK_SIZE; i++) {
    // simple linear fade
    buffer[i] =
        (int16_t)((i * buffer32[i] + (BLOCK_SIZE - i) * overlap_buffer[i]) >>
                  BLOCK_SIZE_LOG2);
    overlap_buffer[i] = buffer32[i + BLOCK_SIZE];
  }
}

int main(void) {
  const char* inputPath =
      "/Users/andrewstanford-jason/lib_xcore_math/example/wav_demo/"
      "untitled.wav";
  const char* outputPath =
      "/Users/andrewstanford-jason/lib_xcore_math/example/wav_demo/output.wav";

  WavFile* read_fp = wav_open(inputPath, WAV_OPEN_READ);
  WavU16 n_channels = wav_get_num_channels(read_fp);

  if (n_channels != 1) {
    printf("Error: make sure that the input is single channel\n");
    return 1;
  }

  int totalNumSamples = wav_get_length(read_fp);
  int sample_rate = wav_get_sample_rate(read_fp);

  WavFile* write_fp = wav_open(outputPath, WAV_OPEN_WRITE);
  wav_set_format(write_fp, WAV_FORMAT_PCM);
  wav_set_num_channels(write_fp, n_channels);
  wav_set_sample_rate(write_fp, sample_rate);

  int16_t overlap_buffer[BLOCK_SIZE];
  int16_t prev_buffer[BLOCK_SIZE];

  memset(overlap_buffer, 0, sizeof(int16_t) * BLOCK_SIZE);
  memset(prev_buffer, 0, sizeof(int16_t) * BLOCK_SIZE);
  int samplesProcessed = 0;
  while (samplesProcessed < totalNumSamples) {
    int16_t buffer[BLOCK_SIZE];
    memset(buffer, 0, sizeof(int16_t) * BLOCK_SIZE);

    int samplesRead = wav_read(read_fp, buffer, BLOCK_SIZE);
    add_new_samples(buffer, prev_buffer, overlap_buffer);
    wav_write(write_fp, buffer, samplesRead);

    samplesProcessed += samplesRead * n_channels;
  }

  wav_close(write_fp);
  wav_close(read_fp);
  return 0;
}
