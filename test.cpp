#include "WAV.h"

int main() {
  /* Print
  WAV target;
  target.OpenFile("something_wrong.wav");
  target.Print();
  target.Finish();
  */

  WAV target;

  /* int16 */
  WAV out(1,16000,WAV::int16);
  short* buf_int16 = new short[512];

  
  target.OpenFile("mono_16kHz_int16.wav");
  target.Print();
  out.NewFile("mono_16kHz_int16_out.wav");

  while (!target.IsEOF()) {
    target.ReadUnit(buf_int16, 512);
    out.Append(buf_int16, 512);
  }
  out.Print();
  out.Finish();
  delete[] buf_int16;


  /* float32 */
  float* buf_float32 = new float[512];
  out.Clear();
  out.SetFmtType(WAV::float32);
  out.NewFile("mono_16kHz_float32_out.wav");
  target.OpenFile("mono_16kHz_float32.wav");
  target.Print();

  while (!target.IsEOF()) {
    target.ReadUnit(buf_float32, 512);
    out.Append(buf_float32, 512);
  }
  out.Print();

  out.Finish();
  delete[] buf_float32;
  


  return 0;
}