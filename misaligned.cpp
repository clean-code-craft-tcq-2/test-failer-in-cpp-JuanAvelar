#include <iostream>
#include <assert.h>
#include <sstream>
#include <array>
#include <vector>
#define MAJOR 0
#define MINOR 1
#define FIRST_COLUMN 0
#define SECOND_COLUMN 1
const char* majorColor[] = {"White", "Red", "Black", "Yellow", "Violet"};
const char* minorColor[] = {"Blue", "Orange", "Green", "Brown", "Slate"};
const int   numberOfMajorColors = sizeof(majorColor) / sizeof(majorColor[0]);
const int   numberOfMinorColors = sizeof(minorColor) / sizeof(minorColor[0]);

std::stringstream printColorMap() {
    std::stringstream ColorMap;
    int i = 0, j = 0;
    for(i = 0; i < 5; i++) {
        for(j = 0; j < 5; j++) {
            ColorMap << i * 5 + j << " | " << majorColor[i] << " | " << minorColor[i] << "\n";
            std::cout << i * 5 + j << " | " << majorColor[i] << " | " << minorColor[i] << "\n";
        }
    }
    return ColorMap;
}

std::array<std::string,2> PairNumber2Color(int Pair){
  std::array<std::string,2> Colors;
  if(Pair > 0 && Pair < numberOfMajorColors*numberOfMinorColors){
    Colors[0] = std::string(majorColor[(Pair-1)/numberOfMajorColors]);
    Colors[1] = std::string(minorColor[(Pair-1)%numberOfMinorColors]);
  }
  return Colors;
}

std::stringstream GetOutputLine(std::stringstream* ColorMapText, int LinePrintedNumber){
  int ColorIndex = 0;
  std::string Line;
  while(ColorIndex < LinePrintedNumber){
    ColorIndex++;
    if(!std::getline(*ColorMapText, Line)){//extract line by line
      std::cerr << "ERROR: Last Line of Color Map Reached\n";
      ColorIndex = 0;
      break;
    }
  }
  return std::stringstream(Line);
}

void TestLineContents(std::stringstream* ColorMapText, int LinePrintedNumber){
  int ColorIndex = 0;
  std::string IndexWord, MajorWord, MinorWord;
  if(!LinePrintedNumber) LinePrintedNumber--;//This lines fixes false positive when LinePrintedNumber = 0
  std::stringstream Linestream = GetOutputLine(ColorMapText, LinePrintedNumber);
  Linestream >> IndexWord;//extract word by word
  Linestream >> MajorWord;//extract word by word should be a separator '|'
  Linestream >> MajorWord;//extract word by word
  Linestream >> MinorWord;//extract word by word should be a separator '|'
  Linestream >> MinorWord;//extract word by word
  if(ColorIndex) ColorIndex = std::stoul(IndexWord);
  std::cout << "ColorIndex is >" << ColorIndex << ", when it should be>" << LinePrintedNumber << '\n';
  assert(ColorIndex == LinePrintedNumber);
  std::array<std::string, 2> colors = PairNumber2Color(LinePrintedNumber);
  std::cout << "Major:" << MajorWord << ", Expected:" << colors.at(MAJOR) << '\n';
  std::cout << "Major:" << MinorWord << ", Expected:" << colors.at(MINOR) << '\n';
  assert(MajorWord == colors.at(0));
  assert(MinorWord == colors.at(1));
}

void TestOutputAlignment(std::stringstream* ColorMapText, char separator){
  std::stringbuf NonFunctionalParameter;
  std::string word, line;
  std::vector<std::array<std::streamsize,2>> SeparatorPositions;
  while(std::getline(*ColorMapText, line)){//generate a table of positions
    std::stringstream Linestream = std::stringstream(line);
    Linestream.get(NonFunctionalParameter, separator);//look for separator character
    std::streamsize firstsize = Linestream.gcount();//count lines of last get() function
    Linestream.get();//advance 1 char forward
    Linestream.get(NonFunctionalParameter, separator);//count lines of last get() function
    SeparatorPositions.push_back({firstsize, Linestream.gcount()});
  }
  for(size_t LineIndex = 1; LineIndex < SeparatorPositions.size(); LineIndex++){
    //check if '|' separator aligns across all lines
    assert(SeparatorPositions.at(LineIndex).at(FIRST_COLUMN) == SeparatorPositions.at(LineIndex-1).at(FIRST_COLUMN));
    assert(SeparatorPositions.at(LineIndex).at(SECOND_COLUMN) == SeparatorPositions.at(LineIndex-1).at(SECOND_COLUMN));
  }
}

int main() {
    std::stringstream result = printColorMap();
    // TestLineContents(&result, 14);
    TestOutputAlignment(&result, '|');
    std::cout << "All is not well (maybe!)\n";
    return 0;
}
