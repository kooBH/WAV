#ifndef _H_WAV_
#define _H_WAV_

#include <stdlib.h>
#include <stdio.h>
//for types
#include <stdint.h>


#include <cstdlib>
#include <string.h>
#include <string>
#include <vector>

// Data Type is fixed as short

class WAV {
  /*
   * http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
   * */
private:

  // ---- Header Info ---- //
  uint32_t head_size=44;
  bool non_pcm;
  // 4bytes fixed size header infomation -> uint32_t
  char riff_id[4];    // riff string
  uint32_t riff_size; // overall size of fp in bytes
  char wave_id[4];    // wave string
  char chunk_id[4];     // fmt string with trailing null char

  //          | pcm  | non-pcm
  // chunk_size |  16  |     18
  //
  uint32_t chunk_size;  // format chunk size 16,18,40
  short
      fmt_type; // format type 1-PCM 3-IEEE float 6- 8bit A law, 7- 8bit ,u law
  unsigned short channels;       // no of channel
  uint32_t sample_rate; // SampleRate(blocks per second)
  uint32_t byte_rate;   // ByteRate = SampleRate * NumChannels * BitsPerSample/8
  short block_align;    // NumChannels * BitsPerSample/8
  short bit_per_sample; // bits per sample, 8 - 8bits, 16-16bits etc

  /*   (if non -pcm )*/
  uint32_t cb_size;     //size of the extension
  char fact_id[4];
  uint32_t fact_size;
  uint32_t num_samples;
  uint32_t dwSampleLength; 

  char data_id[4];      // DATA string or FLLR string
  uint32_t data_size;   // NumSamples * NumChannels * BitsPerSample/8 - size of
                        // the nex chunk that will be read

public:
  enum FMT{int16,int32,float32,float64};
private : 
  enum fmt_types{PCM=1,IEEE_float=3};
  FMT fmt_code;

  FILE *fp;
  bool IsOpen;
  char file_name[1024];
  // For Input usage only
  bool use_buf;
  int frame_size;
  int shift_size;

  int size_unit;

  void* buf; 
  bool isRead;
  
  /*for extensible format*/
  bool extensible;
  int w_valid_bits_per_sample;
  int dw_channel_mask;
  int sub_format;

//  short* buf; 

public:
   WAV();
   WAV(short _ch, uint32_t _rate);
   WAV(short _ch, uint32_t _rate, int frame_size, int shift_size);
   WAV(short _ch, uint32_t _rate, int fmt_type);
   ~WAV();
   int NewFile(const char *_file_name);
   int NewFile(std::string file_name_);
   int OpenFile(const char *_file_name);
   int OpenFile(std::string file_name_);
   int Append(short *app_data, unsigned int app_size);
   int Append(int *app_data, unsigned int app_size);
   int Append(float*app_data, unsigned int app_size);
   int Append(double*app_data, unsigned int app_size);
   void WriteHeader();
  // set default
   void Init();
  // reset data;
   void Clear();
   void Finish();

   void ReadHeader();

  /* There might be compile error for ReadUnit() in Visual Studio.
   * in this case, try to update your VS to most recent version. */
   size_t ReadUnit(short*dest,int unit);
   size_t ReadUnit(int*dest,int unit);
   size_t ReadUnit(float*dest,int unit);
   size_t ReadUnit(double*dest,int unit);
   int IsEOF() const;

   void Print() const;
   void Rewind();

   int Convert2ShiftedArray(double **raw);
   int Convert2ShiftedArray(double *raw);

   int Convert2Array(double **raw);

  // Split 2 channel Wav into two 1 channel wav files.
   void SplitBy2(const char* f1,const char* f2);
   void SetChannels(int ch);
   void SetSamplerate(int sr);
   void SetSizes(int frame,int shift);
   void SetFmtCode(int fmt);

   int GetChannels();
   bool GetIsOpen();
   uint32_t GetSize(); 
   uint32_t GetSizeUnit(); 
   uint32_t GetSampleRate();
   uint32_t GetNumOfSamples();
   const char* GetFileName();
   short GetFmtSize();
   void UseBuf(int frame_size,int shift_size);
   bool checkValidHeader();
   FILE* GetFilePointer();

  // Normalize WAV
   void Normalize();


  /*Split Wav fp into each channel */
   static void Split(char* );
};
#endif
