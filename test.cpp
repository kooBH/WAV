#include "WAV.h"

int main() {
  /* Print
  WAV target;
  target.OpenFile("something_wrong.wav");
  target.Print();
  target.Finish();
  */

  /* Normalize  */
  WAV target;
  target.OpenFile("../s.wav");
  target.Print();
  target.Normalize();
  target.Finish();


  return 0;
}