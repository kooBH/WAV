#include "WAV.h"

int main() {
  WAV target;
  target.OpenFile("something_wrong.wav");
  target.Print();
  target.Finish();

  return 0;
}