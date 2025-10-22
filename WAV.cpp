#include "WAV.h"


void WAV::Init() {
  fp = nullptr;
  buf = nullptr;
  isRead = false;

  riff_id[0] = 'R';
  riff_id[1] = 'I';
  riff_id[2] = 'F';
  riff_id[3] = 'F';

  wave_id[0] = 'W';
  wave_id[1] = 'A';
  wave_id[2] = 'V';
  wave_id[3] = 'E';

  chunk_id[0] = 'f';
  chunk_id[1] = 'm';
  chunk_id[2] = 't';
  chunk_id[3] = ' ';

  chunk_size = 16;

  // 1- PCM
  fmt_type = 1;

  channels = 0;

  // bit per sample, 8 or 16. not sure
  // Presume short = 16 bit
  // short 16bit ->2 bytes
  bit_per_sample = 16;
  size_unit = bit_per_sample/8;

  // have to be Optional
  sample_rate = 0;
  // smaple_rate * channels * fmt_size / 8
  byte_rate = sample_rate * channels * bit_per_sample/ 8;

  block_align = bit_per_sample * channels / 8;

  data_id[0] = 'd';
  data_id[1] = 'a';
  data_id[2] = 't';
  data_id[3] = 'a';

  // Number of Samples * Number of Channels * Bit_per_sample / 8
  data_size = 0 * channels * bit_per_sample / 8;

  riff_size = data_size + head_size;

  IsOpen = false;
  use_buf = false;
  non_pcm = false;
  extensible = false;

  frame_size = 512;
  shift_size = 512;

}

/* default WAV format */
WAV::WAV() {
#ifndef NDEBUG
//  printf("WAV::contsructor\n");
#endif
  Init();
}

WAV::WAV(short _ch, uint32_t _rate) : WAV() {
#ifndef NDEBUG
//  printf("WAV::constructor (ch,rate)\n");
#endif

  channels = _ch;
  // have to be Optional
  sample_rate = _rate;

  // have to be Optional
  // smaple_rate * channels * bit_per_sample / 8
  byte_rate = sample_rate * channels * bit_per_sample/ 8;

  block_align = bit_per_sample * channels / 8;

  // smaple_rate * channels * bit_per_sample/ 8
  byte_rate = sample_rate * channels * bit_per_sample/ 8;
  // bit_per_sample*channels /8
  block_align = bit_per_sample * channels / 8;
  // Number of Samples * Number of Channels * Bit_per_sample / 8
  data_size = 0 * channels * bit_per_sample / 8;
  riff_size = data_size + head_size;
}

WAV::WAV(short _ch, uint32_t _rate, int _frame_size, int _shift_size)
    : WAV(_ch, _rate) {
#ifndef NDEBUG
//  printf("WAV::constructor(ch,rate,frame,shift)\n");
#endif

  frame_size = _frame_size;
  shift_size = _shift_size;
  
}

WAV::WAV(short _ch, uint32_t _rate, int fmt_type_):WAV(_ch,_rate) {
  this->SetFmtType(fmt_type_);
}

WAV::~WAV() {

  if (IsOpen) {
#ifndef NDEBUG
    //printf("WAV::%s destructor\n", file_name);
#endif
    Finish();
  }
  if (use_buf){
    switch(fmt_type){
      case 3:
    delete[] (float*)buf;
    break;
      default:
    delete[] (short*)buf;
    break;
    }
  }
}

void WAV::WriteHeader() {
  if (isRead)return;
  if (!fp) {
    printf("ERROR::File doesn't exist\n");
  }
  fseek(fp, 0, SEEK_SET);
#ifndef NDEBUG
//  printf("WriteHeader::ftell %ld\n",ftell(fp));
#endif
  riff_size = data_size + head_size;

  fwrite(riff_id, sizeof(char), 4, fp);
  fwrite(&(riff_size), sizeof(uint32_t), 1, fp);
  fwrite((wave_id), sizeof(char), 4, fp);
  fwrite((chunk_id), sizeof(char), 4, fp);
  fwrite(&(chunk_size), sizeof(uint32_t), 1, fp);
  fwrite(&(fmt_type), sizeof(short), 1, fp);
  fwrite(&(channels), sizeof(short), 1, fp);
  fwrite(&(sample_rate), sizeof(uint32_t), 1, fp);
  fwrite(&(byte_rate), sizeof(uint32_t), 1, fp);
  fwrite(&(block_align), sizeof(short), 1, fp);
  fwrite(&(bit_per_sample), sizeof(short), 1, fp);
  fwrite(data_id, sizeof(char), 4, fp);
  fwrite(&(data_size), sizeof(uint32_t), 1, fp);
}

int WAV::NewFile(const char *_file_name) {
  Clear();
  fp = fopen(_file_name, "wb");
  if (fp == NULL) {
    printf("WAV::NewFile::Failed to Open : %s\n", _file_name);
    exit(-1);
  }
  WriteHeader();
  strcpy(file_name,_file_name);
  IsOpen = true;
  isRead = false;


  return 0;
};

int WAV::NewFile(std::string file_name_) {
  return NewFile(file_name_.c_str());
}

int WAV::Append(short *app_data, unsigned int app_size) {
  fseek(fp, 0, SEEK_END);
#ifndef NDEBUG
// printf("Append::ftell %ld | size %d\n",ftell(fp),app_size);
#endif

  if (!fwrite(reinterpret_cast<void*>( app_data), size_unit, app_size, fp)){
    printf("ERROR::Append<short>\n");
  }
  data_size += app_size *size_unit ;
  WriteHeader();
  return 0;
};

int WAV::Append(float*app_data, unsigned int app_size) {
  fseek(fp, 0, SEEK_END);
#ifndef NDEBUG
// printf("Append::ftell %ld | size %d\n",ftell(fp),app_size);
#endif
  if (!fwrite(reinterpret_cast<void*>(app_data), size_unit, app_size, fp)){
    printf("ERROR::Append<float>\n");
  }
  data_size += app_size *size_unit ;
  WriteHeader();
  return 0;
};


int WAV::OpenFile(const char *_file_name) {
  fp = fopen(_file_name, "rb");
  strcpy(file_name,_file_name);
  if (fp == NULL) {
    printf("WAV::OpenFile::Failed to Open : '%s'\n", _file_name);
    return 1;
  }
  ReadHeader();
  IsOpen = true;
  isRead = true;

  UseBuf(frame_size,shift_size);

  return 0;
};

int WAV::OpenFile(std::string file_name_) {
  return OpenFile(file_name_.c_str());
}

void WAV::ReadHeader() {
  head_size = 44;
  unsigned char buffer4[4];
  unsigned char buffer2[2];
  unsigned char buffer16[16];
  unsigned int temp;
  /*
   * http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
   * */

  if (!fp) {
    printf("ERROR::File doesn't exist\n");
  }

  fread(riff_id, sizeof(riff_id), 1, fp);

  fread(buffer4, sizeof(buffer4), 1, fp);
  // convert little endial to big endian 4 bytes int;
  riff_size =
      buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
  // fread(riff_size,sizeof(riff_size),1,fp);

  fread(wave_id, sizeof(wave_id), 1, fp);

  fread(chunk_id, sizeof(chunk_id), 1, fp);

  fread(buffer4, sizeof(buffer4), 1, fp);
  // convert little endial to big endian 4 bytes int;
  //          | pcm  | non-pcm
  // fmt_size |  16  |     18
  //
  //Chunk size: 16, 18 or 40
  //
  //
  chunk_size =
      buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

  if(chunk_size==18)
     non_pcm = true;

  if (chunk_size == 16 || chunk_size == 18);
  // Unkown chunk_size;
  else if (chunk_size == 40)
      extensible = true;
  else chunk_size=16;

  fread(buffer2, sizeof(buffer2), 1, fp);
  // convert little endial to big endian 2 bytes int;
  fmt_type = buffer2[0] | (buffer2[1] << 8);
  if(fmt_type==1 || fmt_type == 3)
    ;
  // for undefined type
  else
    fmt_type = 1;


  // convert little endian to big endian 2 bytes int;
  
  fread(buffer2, sizeof(buffer2), 1, fp);
  //Check
  if(channels!=0){
  temp = buffer2[0] | (buffer2[1] << 8);
    if(temp!=channels){
      printf("ERROR::WAV channels is not expected (%d != %d)\n",channels,temp);
      exit(-1);
    }
  }
  else
    channels = buffer2[0] | (buffer2[1] << 8);

  fread(buffer4, sizeof(buffer4), 1, fp);
  // convert little endial to big endian 4 bytes int;
  // Check
  if(sample_rate!=0){
    temp
      =  buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
    if(temp != sample_rate){
      printf("ERROR::WAV sampe_rate is not expected (%d != %d)\n",sample_rate,temp);  
      exit(-1);
    }

  }   
  else
    sample_rate =
      buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

  fread(buffer4, sizeof(buffer4), 1, fp);
  // convert little endial to big endian 4 bytes int;
  byte_rate =
      buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

  fread(buffer2, sizeof(buffer2), 1, fp);
  // convert little endial to big endian 2 bytes int;
  block_align = buffer2[0] | (buffer2[1] << 8);

  fread(buffer2, sizeof(buffer2), 1, fp);
  // convert little endial to big endian 2 bytes int;
  bit_per_sample = buffer2[0] | (buffer2[1] << 8);

  size_unit = bit_per_sample/8;


  /* non_pcm format has more elements */
  if(non_pcm){
    fread(buffer2, sizeof(buffer2), 1, fp);
    cb_size= buffer2[0] | (buffer2[1] << 8);
    
    fread(fact_id, sizeof(fact_id), 1, fp);

    fread(buffer4, sizeof(buffer4), 1, fp);
    fact_size =
      buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

    fread(buffer4, sizeof(buffer4), 1, fp);
    dwSampleLength =
      buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
  }

  /* extensible format has more elements */
  if (extensible) {
      fread(buffer2, sizeof(buffer2), 1, fp);
      cb_size = buffer2[0] | (buffer2[1] << 8);

      if(cb_size!=22){
          printf("ERROR::WAV size of extension is not correct (%d != %d)\n", cb_size, 22);
          exit(-1);
      }
      
      fread(buffer2, sizeof(buffer2), 1, fp);
      w_valid_bits_per_sample = buffer2[0] | (buffer2[1] << 8);
      
      fread(buffer4, sizeof(buffer4), 1, fp);
      w_valid_bits_per_sample = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

      fread(buffer16, sizeof(buffer16), 1, fp);
      //need subFormat reading

      fread(fact_id, sizeof(fact_id), 1, fp);

      fread(buffer4, sizeof(buffer4), 1, fp);
      fact_size =
          buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

      fread(buffer4, sizeof(buffer4), 1, fp);
      dwSampleLength =
          buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

      head_size += 7;
  }
  fread(data_id, sizeof(data_id), 1, fp);

  fread(buffer4, sizeof(buffer4), 1, fp);
  // convert little endial to big endian 4 bytes int;
  data_size =
      buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
}

size_t WAV::ReadUnit(short*dest,int unit){
  return fread(dest,size_unit,unit,fp);
}
size_t WAV::ReadUnit(float*dest,int unit){
  return fread(dest,size_unit,unit,fp);
}

void WAV::Finish() {

  if (fp) {
    WriteHeader();
    fclose(fp);
    IsOpen = false;
  }
}

void WAV::Print()const {
  int t;
  if (!IsOpen) {
    printf("ERROR::file is not opened.\n");
    return;
  }
  printf("-------- WAV HEADER INFOMATION of [ %s ] ----------\n", file_name);
  printf("riff_id        : %c%c%c%c\n", riff_id[0], riff_id[1], riff_id[2],
         riff_id[3]);
  printf("riff_size      : %u \n", riff_size);
  printf("wave_id        : %c%c%c%c\n", wave_id[0], wave_id[1], wave_id[2],
         wave_id[3]);
  printf("chunk_id       : %c%c%c%c\n", chunk_id[0], chunk_id[1], chunk_id[2],
         chunk_id[3]);
  printf("chunk_size     : %u\n", chunk_size);
  switch (fmt_type) {
  case 1:
    printf("fmt_type       : %u - PCM\n", fmt_type);
    break;
  case 3:
    printf("fmt_type       : %u - IEEE float\n", fmt_type);
    break;
  case 6:
    printf("fmt_type       : %u - 8bit A law\n", fmt_type);
    break;
  case 7:
    printf("fmt_type       : %u - 8 bit U law\n", fmt_type);
    break;
  default:
    printf("fmt_type       : %u - Unknown\n", fmt_type);
    break;
  }
  printf("channels       : %u\n", channels);
  printf("sample_rate    : %u \n", sample_rate);
  printf("byte_rate      : %u\n", byte_rate);
  printf("block_align    : %u\n", block_align);
  printf("bit_per_sample : %u\n", bit_per_sample);
  if(non_pcm){
    printf("cbSize         : %u\n", cb_size);
  printf("fact_id        : %c%c%c%c\n", fact_id[0], fact_id[1], fact_id[2],
         fact_id[3]);
  printf("fact_size      : %u\n",fact_size);
  printf("dwSampleLength : %u\n", dwSampleLength);
  }
  printf("data_id        : %c%c%c%c\n", data_id[0], data_id[1], data_id[2],
         data_id[3]);
  printf("data_size      : %u\n", data_size);
  printf("duration       : %.3lf sec\n", (double)riff_size / byte_rate);
}

bool WAV::checkValidHeader() {

  int t;
  if(riff_id[0]!='R' || riff_id[1]!='I'|| riff_id[2]!='F'|| riff_id[3]!='F')
    return false; 
  if(wave_id[0]!='W' || wave_id[1]!='A'|| wave_id[2]!='V'|| wave_id[3]!='E')
    return false; 
  if(!(fmt_type == 1 || fmt_type ==3 || fmt_type ==6 || fmt_type ==7))
    return false;
  if(!(chunk_size == 16 || chunk_size == 18))
    return false;
  if(byte_rate!=(block_align*sample_rate))
    return false;
  if(data_id[0]!='d' || data_id[1]!='a'|| data_id[2]!='t'|| data_id[3]!='a')
    return false; 
  
  return true;  
}


int WAV::IsEOF() const { return feof(fp); }

void WAV::Rewind() {
#ifndef NDEBUG
  printf("INFO::Rewind\n");
#endif
  rewind(fp);
  ReadHeader();
}

void WAV::SplitBy2(const char* f1,const char* f2){
  if(!IsOpen){
    printf("ERROR::WAV must be opened\n");
    exit(-1);
  }
  if(channels != 2){
    printf("ERROR::WAV must be 2 channels\n");
    exit(-1);
  }
  WAV w1(1,sample_rate);
  WAV w2(1,sample_rate);

  w1.NewFile(f1);
  w2.NewFile(f2);

  short temp;

  while(!feof(fp)){
    fread(&temp, size_unit, 1, fp);
    w1.Append(&temp,1);

    fread(&temp, size_unit, 1, fp);
    w2.Append(&temp,1);
 
  }

  w1.Finish();
  w2.Finish();

}

int WAV::GetChannels(){
  return channels;
}


bool WAV::GetIsOpen(){
  return IsOpen;
}

uint32_t WAV::GetSize(){
  return riff_size;
}
uint32_t WAV::GetSizeUnit(){
  return size_unit;
}

uint32_t WAV::GetNumOfSamples(){
  return riff_size / (2 * channels);
}

void WAV::UseBuf(int _frame_size,int _shift_size){

  if(!use_buf){
    frame_size = _frame_size;
    shift_size = _shift_size;
    use_buf = true;
    switch(fmt_type){
      /* 3 - IEEE float*/
      case 3:
      buf = new float[channels * shift_size];
      break;
      /* 1 - PCM*/
      default:
      buf = new short[channels * shift_size];
      break;
    }
  }
}

uint32_t WAV::GetSampleRate(){
  return sample_rate;
}


int WAV::Convert2ShiftedArray(double **raw) {
  int i, j,read;
  read=fread(buf, size_unit, channels * shift_size, fp);

  /*

  printf("READ : %d\n",read);
  for(int q=0;q<shift_size;q++)
    printf("read %d %d\n",q,reinterpret_cast<T*>(buf)[q]);
    */

  if(read == channels*shift_size){
      // SHIFT
      for (j = 0; j < channels; j++) {
        for (i = 0; i < (frame_size - shift_size); i++) {
          raw[j][i] = raw[j][i + shift_size];
        }
      }
    // COPY as doulbe
    //  memcpy(arr,data.buffer+read_offset,shift_size);
    switch(fmt_type){
      case 3:
       for (i = 0; i < shift_size; i++) {
        for (j = 0; j < channels; j++){
          raw[j][i + (frame_size - shift_size)] 
            = static_cast<double>(reinterpret_cast<float*>(buf)[i * channels + j]);
        }
       }
        break;
      default:
        for (i = 0; i < shift_size; i++) {
          for (j = 0; j < channels; j++){
            raw[j][i + (frame_size - shift_size)] 
              = static_cast<double>(reinterpret_cast<short*>(buf)[i * channels + j]);
          }
        }
    break;
    }
    return 1;
  // Not enough buf to read
  }else{
    read = read/channels;
      for (j = 0; j < channels; j++) {
        for (i = 0; i < (frame_size - shift_size); i++) {
          raw[j][i] = raw[j][i + shift_size];
        }
      }
    switch(fmt_type){
      case 3:
        for (i = 0; i < read; i++) {
          for (j = 0; j < channels; j++)
          raw[j][i + (frame_size - shift_size)]
             =  static_cast<double>(reinterpret_cast<float*>(buf)[i * channels + j]);
          }
      break;
      default:
        for (i = 0; i < read; i++) {
          for (j = 0; j < channels; j++)
          raw[j][i + (frame_size - shift_size)]
             =  static_cast<double>(reinterpret_cast<short*>(buf)[i * channels + j]);
          }
    break;
    }
    for (i = read; i < shift_size; i++) {
      for (j = 0; j < channels; j++)
        raw[j][i + (frame_size - shift_size)] = 0;
    }
    return 0;
  }
    return 0;
}


int WAV::Convert2ShiftedArray(double *raw) {
  int i, j,read;
  read=fread(buf, size_unit, channels * shift_size, fp);

  /*

  printf("READ : %d\n",read);
  for(int q=0;q<shift_size;q++)
    printf("read %d %d\n",q,reinterpret_cast<T*>(buf)[q]);
    */

  if(read == channels*shift_size){
      // SHIFT
      for (j = 0; j < channels; j++) {
        for (i = 0; i < (frame_size - shift_size); i++) {
          raw[j*frame_size + i] = raw[j*frame_size + i + shift_size ];
        }
      }
    // COPY as doulbe
    //  memcpy(arr,data.buffer+read_offset,shift_size);
    switch(fmt_type){
      case 3:
       for (i = 0; i < shift_size; i++) {
        for (j = 0; j < channels; j++){
          raw[j*frame_size + i + (frame_size - shift_size)] 
            = static_cast<double>(reinterpret_cast<float*>(buf)[i * channels + j]);
        }
       }
        break;
      default:
        for (i = 0; i < shift_size; i++) {
          for (j = 0; j < channels; j++){
            raw[j*frame_size + i + (frame_size - shift_size)] 
              = static_cast<double>(reinterpret_cast<short*>(buf)[i * channels + j]);
          }
        }
    break;

    }
    return 1;
  // Not enough buf to read
  }else{
    read = read/channels;
      for (j = 0; j < channels; j++) {
        for (i = 0; i < (frame_size - shift_size); i++) {
          raw[j*frame_size + i] = raw[j*frame_size + i + shift_size];
        }
      }
    switch(fmt_type){
      case 3:
        for (i = 0; i < read; i++) {
          for (j = 0; j < channels; j++)
          raw[j*frame_size + i + (frame_size - shift_size)]
             =  static_cast<double>(reinterpret_cast<float*>(buf)[i * channels + j]);
          }
      break;
      default:
        for (i = 0; i < read; i++) {
          for (j = 0; j < channels; j++)
          raw[j* frame_size + i + (frame_size - shift_size)]
             =  static_cast<double>(reinterpret_cast<short*>(buf)[i * channels + j]);
          }
    break;
    }
    for (i = read; i < shift_size; i++) {
      for (j = 0; j < channels; j++)
        raw[j * frame_size+  i + (frame_size - shift_size)] = 0;
    }
    return 0;
  }

    return 0;
}

/* Note :: There is no padding for edge! 
   TOOD :: need to add padding
*/
int WAV::Convert2Array(double **raw) {
  int i, j,read;
  read=fread(buf, size_unit, channels * shift_size, fp);
  //printf("read : %d\n",read);
  if(read == channels*shift_size){
    // COPY as doulbe
    //  memcpy(arr,data.buffer+read_offset,shift_size);
    switch(fmt_type){
      case 3:
       for (i = 0; i < shift_size; i++) {
        for (j = 0; j < channels; j++){
          raw[j][i] 
            = static_cast<double>(reinterpret_cast<float*>(buf)[i * channels + j]);
        }
       }
        break;
      default:
        for (i = 0; i < shift_size; i++) {
          for (j = 0; j < channels; j++){
            raw[j][i] 
              = static_cast<double>(reinterpret_cast<short*>(buf)[i * channels + j]);
          }
        }
    break;
    }
    return 1;
  // Not enough buf to read
  }else{
    read = read/channels;
    switch(fmt_type){
      case 3:
        for (i = 0; i < read; i++) {
          for (j = 0; j < channels; j++)
          raw[j][i]
             =  static_cast<double>(reinterpret_cast<float*>(buf)[i * channels + j]);
          }
      break;
      default:
        for (i = 0; i < read; i++) {
          for (j = 0; j < channels; j++)
          raw[j][i ]
             =  static_cast<double>(reinterpret_cast<short*>(buf)[i * channels + j]);
          }
    break;
    }
    for (i = read; i < shift_size; i++) {
      for (j = 0; j < channels; j++)
        raw[j][i] = 0;
    }
  }
    return 0;
}




void WAV::SetSizes(int frame,int shift){
  frame_size = frame;
  shift_size = shift;
}

void WAV::SetFmtType(int fmt_size){
  if (fmt_size == WAV::float32) {
    bit_per_sample = 32;
    size_unit = bit_per_sample / 8;
    byte_rate = sample_rate * channels * bit_per_sample / 8;
    block_align = bit_per_sample * channels / 8;
  }
  //int16
  else {
    bit_per_sample = 16;
    size_unit = bit_per_sample / 8;
    byte_rate = sample_rate * channels * bit_per_sample / 8;
    block_align = bit_per_sample * channels / 8;

  
  }


}

short WAV::GetFmtSize(){
  return bit_per_sample/8;
}

void WAV::Split(char* _file_name ) {
  const int fr = 512;
  char temp_file_name[512];

  WAV input;
  input.OpenFile(_file_name);
  int ch = input.GetChannels();
  int sr = input.GetSampleRate();
  short *temp;


  std::vector<WAV> splited(ch,WAV(1,sr));

  temp  = new short[fr * ch];

  for(int i=0;i<ch;i++){
  char *temp_str = strtok(_file_name, ".");
  sprintf(temp_file_name,"%s_%d.wav",temp_str,i+1);
  splited[i].NewFile(temp_file_name);
  }

  // Distribute
  while(!input.IsEOF()){
    input.ReadUnit(temp,fr*ch);
    for(int i=0;i<fr;i++){
      for(int j=0;j<ch;j++)
        splited[j].Append(temp + i*ch + j,1);
    }
  }

  splited.clear();
  delete[] temp;
  
}

FILE* WAV::GetFilePointer() {
  return fp;
}

const char* WAV::GetFileName() {
  return file_name;
}

/* channel wise normalization */
void WAV::Normalize() {
  int cnt;
  short *max;
  float *rate;

  if (fmt_type != 1) {
    printf("SORRY::Currently, I do not support Normalize() for other than short type.");
    return;
  }
  if (fp)fclose(fp);
  OpenFile(file_name);
 // Print();

  int n_sample = data_size / size_unit;

  max = new short[channels];
  memset(max, 0, sizeof(short) * channels);
  rate = new float[channels];
  short* temp_buf = new short[n_sample];
  cnt = ReadUnit(temp_buf, n_sample);

  if (cnt != n_sample) {
    printf("WARNNING::Read Sample Num : %d != Sample Num %d\n",cnt,n_sample);
  }
  Finish();
  NewFile(file_name);
  data_size = 0;
  riff_size = data_size + head_size;

  // Get Max
  for (int i = 0; i < cnt; i++) {
    if (std::abs(temp_buf[i]) > max[i%channels])
      max[i%channels] = std::abs(temp_buf[i]);
  }
  // Normalize
  for(int i=0;i<channels;i++)
    rate[i] = (float)((32767.0 /(float)max[i]));

#pragma omp parallel for
    for (int i = 0; i < cnt; i++) {
        temp_buf[i] = (short)(temp_buf[i] * rate[i%channels]);
    }
    Append(temp_buf, cnt);
   // Print();

    delete[] temp_buf;
    delete[] max;
    delete[] rate;
}

void WAV::Clear() {
  data_size = 0;
  riff_size = data_size + head_size;
}
