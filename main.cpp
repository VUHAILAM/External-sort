#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <locale>
#include <codecvt>
#include <fstream>


class TempFileHandler {
    private:
        int _counter = 0;
        std::vector<std::string> _tempFileNames;
        std::vector<std::wifstream*> _tempFiles;
    public:
        TempFileHandler();
        ~TempFileHandler();
        void WriteToTempFile(const std::vector<std::wstring> &textLines);
        void OpenTempFiles();
        void CloseTempFiles(); 
};

TempFileHandler::TempFileHandler(){}
TempFileHandler::~TempFileHandler(){}

void TempFileHandler::WriteToTempFile(const std::vector<std::wstring> &textLines) {
    std::string fileName = "temp_file_" + std::to_string(_counter) +".txt";

    std::wofstream ftemp(fileName);

    for(int i = 0; i < textLines.size(); ++i) {
        ftemp << textLines[i] << "\n";
    }

    ++_counter;
    ftemp.close();
    _tempFileNames.push_back(fileName);
}

class Sorter {
    private:
        std::string _inputFile;
        std::string _outputFile;
        int _memoryLimit;
        TempFileHandler* _handler;
    public:
        Sorter();
        Sorter(const std::string& inputFile,
            const std::string& outputFile,
            int memoryLimit
        );
        ~Sorter();
        void Sort();
        void SplitAndSort();
        void Merge();
};

Sorter::Sorter() {
    _handler = new TempFileHandler();
}

Sorter::Sorter(const std::string& inputFile,
            const std::string& outputFile,
            int memoryLimit)
            : _inputFile(inputFile)
            , _outputFile(outputFile)
            , _memoryLimit(memoryLimit)
{
    _handler = new TempFileHandler();
}

Sorter::~Sorter() {}

void Sorter::Sort() {
    SplitAndSort();
    Merge();
}

void Sorter::SplitAndSort() {
    std::wifstream finput(_inputFile);

    if(!finput.good()) {
        exit(1);
    }

    std::vector<std::wstring> textLines;
    int totalSize = 0;

    std::wstring line;
    while (std::getline(finput, line)) {
        textLines.push_back(line);

        totalSize += sizeof(line);

        if (totalSize > _memoryLimit) {
            sort(textLines.begin(), textLines.end());
            _handler->WriteToTempFile(textLines);
            textLines.clear();
            totalSize = 0;
        }
    }

    if(!textLines.empty()) {
        sort(textLines.begin(), textLines.end());
        _handler->WriteToTempFile(textLines);
    }
    
    finput.close();
}

int main(int argc, char *argv[]) {
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    // std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
    // std::wifstream fin(argv[1]);
    // std::wofstream fout(argv[2]);
    // std::wstring s;
    // std::vector<std::wstring> a;
    // fin.imbue(loc);
    // while (std::getline(fin, s))
    // {
    //     /* code */
    //     a.push_back(s);
    // }
    // fin.close();
    // sort(a.begin(), a.end());
    // fout.imbue(loc);
    // for (std::wstring i : a) {
    //     fout << i << std::endl;
    // }
    // fout.close();
    return 0;
}