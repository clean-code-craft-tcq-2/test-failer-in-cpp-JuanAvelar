#include <iostream>
#include <assert.h>
#include <sstream>

std::stringstream printColorMap() {
    std::stringstream ColorMap;
    const char* majorColor[] = {"White", "Red", "Black", "Yellow", "Violet"};
    const char* minorColor[] = {"Blue", "Orange", "Green", "Brown", "Slate"};
    int i = 0, j = 0;
    for(i = 0; i < 5; i++) {
        for(j = 0; j < 5; j++) {
            ColorMap << i * 5 + j << " | " << majorColor[i] << " | " << minorColor[i] << "\n";
            std::cout << i * 5 + j << " | " << majorColor[i] << " | " << minorColor[i] << "\n";
        }
    }
    return ColorMap;
}

void TestLine(std::stringstream* ColorMapText, int LinePrintedNumber){
  int ColorIndex = 0;
  if(!LinePrintedNumber) LinePrintedNumber--;
  std::string Line, Word;
  while(ColorIndex < LinePrintedNumber){
    ColorIndex++;
    if(!std::getline(*ColorMapText, Line)){//extract line by line
      std::cerr << "ERROR: Last Line of Color Map Reached\n";
      ColorIndex = 0;
      break;
    }
  }
  std::stringstream Linestream = std::stringstream(Line);
  Linestream >> Word;//extract word by word
  if(ColorIndex) ColorIndex = std::stoul(Word);
  std::cout << "ColorIndex is >" << ColorIndex << ", when it should be>" << LinePrintedNumber << '\n';
  assert(ColorIndex == LinePrintedNumber);
}

int main() {
    std::stringstream result = printColorMap();
    TestLine(&result, 14);
    std::cout << "All is not well (maybe!)\n";
    return 0;
}
