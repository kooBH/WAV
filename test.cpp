#include "WAV.h"

int main() {
  /* Print
  WAV target;
  target.OpenFile("something_wrong.wav");
  target.Print();
  target.Finish();
  */

  WAV target;
  short* buf_int16 = new short[512];
  int * buf_int32 = new int[512];
  float* buf_float32 = new float[512];
  double* buf_float64 = new double[512];

  /* int16 */
  WAV out(1,16000,WAV::int16);

  target.OpenFile("mono_16kHz_int16.wav");
  target.Print();
  out.NewFile("mono_16kHz_int16_out.wav");

  while (!target.IsEOF()) {
    target.ReadUnit(buf_int16, 512);
    out.Append(buf_int16, 512);
  }
  out.Print();
  out.Finish();

  /* int32 */
  out.Clear();
  out.SetFmtCode(WAV::int32);
  target.OpenFile("mono_16kHz_int32.wav");
  target.Print();
  out.NewFile("mono_16kHz_int32_out.wav");

  while (!target.IsEOF()) {
    target.ReadUnit(buf_int32, 512);
    out.Append(buf_int32, 512);
  }
  out.Print();
  out.Finish();

  /* float32 */
  out.Clear();
  out.SetFmtCode(WAV::float32);
  out.SetChannels(1);
  out.SetSamplerate(16000);

  out.NewFile("mono_16kHz_float32_out.wav");
  target.OpenFile("mono_16kHz_float32.wav");
  target.Print();

  while (!target.IsEOF()) {
    target.ReadUnit(buf_float32, 512);
    out.Append(buf_float32, 512);
  }
  out.Print();
  out.Finish();

  /* float64 */
  out.Clear();
  out.SetFmtCode(WAV::float64);
  out.SetChannels(1);
  out.SetSamplerate(16000);

  out.NewFile("mono_16kHz_float64_out.wav");
  target.OpenFile("mono_16kHz_float64.wav");
  target.Print();

  while (!target.IsEOF()) {
    target.ReadUnit(buf_float64, 512);
    out.Append(buf_float64, 512);
  }
  out.Print();
  out.Finish();

  /* float64 */
  out.Clear();
  out.SetFmtCode(WAV::float64);
  out.SetChannels(2);
  out.SetSamplerate(16000);

  out.NewFile("stereo_16kHz_float64_out.wav");
  target.OpenFile("stereo_16kHz_float64.wav");
  target.Print();

  while (!target.IsEOF()) {
    target.ReadUnit(buf_float64, 512);
    out.Append(buf_float64, 512);
  }
  out.Print();
  out.Finish();


  WAV test;
  test.OpenFile("test.wav");
  test.Print();

  delete[] buf_int16;
  delete[] buf_int32;
  delete[] buf_float32;
  delete[] buf_float64;
  return 0;
}