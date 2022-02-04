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

namespace Utils{
    std::string Spacing(const char* word, size_t length){
        std::string WordString(word);
        if(WordString.size() > length){
          std::cerr << "We got a problem, we will loose characters\t";
        }
        WordString.resize(length, ' ' );
        return WordString;
    }
    std::array<std::string,2> PairNumber2Color(int Pair){
        std::array<std::string,2> Colors;
        if(Pair > 0 && Pair < numberOfMajorColors*numberOfMinorColors){
          Colors[MAJOR] = std::string(majorColor[(Pair-1)/numberOfMajorColors]);
          Colors[MINOR] = std::string(minorColor[(Pair-1)%numberOfMinorColors]);
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
}

namespace Testing {
    void TestLineContents(std::stringstream* ColorMapText, int LinePrintedNumber){
        int ColorIndex = 0;
        std::string IndexWord, MajorWord, MinorWord;
        if(!LinePrintedNumber) LinePrintedNumber--;//This lines fixes false positive when LinePrintedNumber = 0
        std::stringstream Linestream = Utils::GetOutputLine(ColorMapText, LinePrintedNumber);
        Linestream >> IndexWord;//extract word by word
        Linestream >> MajorWord;//extract word by word should be a separator '|'
        Linestream >> MajorWord;//extract word by word
        Linestream >> MinorWord;//extract word by word should be a separator '|'
        Linestream >> MinorWord;//extract word by word
        if(LinePrintedNumber > 0) ColorIndex = std::stoul(IndexWord);
        std::cout << "\n\tTesting::ColorIndex is >" << ColorIndex << ", when it should be >" << LinePrintedNumber << '\n';
        assert(ColorIndex == LinePrintedNumber);
        std::array<std::string, 2> colors = Utils::PairNumber2Color(LinePrintedNumber);
        std::cout << "\tTesting::Major:" << MajorWord << ", Expected:" << colors.at(MAJOR) << '\n';
        std::cout << "\tTesting::Major:" << MinorWord << ", Expected:" << colors.at(MINOR) << '\n';
        assert(MajorWord == colors.at(MAJOR));
        assert(MinorWord == colors.at(MINOR));
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
}

std::stringstream printColorMap() {
  std::stringstream ColorMap;
  int i = 0, j = 0;
  for(i = 0; i < 5; i++) {
    for(j = 0; j < 5; j++) {
      ColorMap  << Utils::Spacing(std::to_string(i * 5 + j + 1).c_str(), 5) << " | " << Utils::Spacing(majorColor[i], 10) << " | " << Utils::Spacing(minorColor[j], 10) << "\n";
      std::cout << Utils::Spacing(std::to_string(i * 5 + j + 1).c_str(), 5) << " | " << Utils::Spacing(majorColor[i], 10) << " | " << Utils::Spacing(minorColor[j], 10) << "\n";
    }
  }
  return ColorMap;
}

int main() {
    std::stringstream result = printColorMap();
    Testing::TestLineContents   (&result, 14 );
    Testing::TestOutputAlignment(&result, '|');
    std::cout << "\nAll is well (maybe!)\n";
    return 0;
}
