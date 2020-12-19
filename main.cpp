#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <locale>
#include <codecvt>
#include <fstream>
#include <queue>

typedef std::pair<std::wstring, std::wifstream*> linedata;

class FileHandler {
    private:
        int _counter = 0;
        std::vector<std::string> _fileNames;
        std::vector<std::wifstream*> _files;
    public:
        FileHandler();
        ~FileHandler();
        void WriteToFile(const std::vector<std::wstring> &textLines);
        void OpenFiles();
        void CloseFiles();
        std::vector<std::wifstream*> GetVectorFiles();      
};

FileHandler::FileHandler(){}

FileHandler::~FileHandler(){
    _fileNames.clear();
    _files.clear();
}

void FileHandler::WriteToFile(const std::vector<std::wstring> &textLines) {
    std::string fileName = "temp_file_" + std::to_string(_counter) +".txt";

    std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
    std::wofstream file(fileName);
    file.imbue(loc);

    if (!file.good()) {
        std::cerr << "Can not write to " << fileName << std::endl;
        exit(1);
    }

    for(int i = 0; i < textLines.size(); ++i) {
        file << textLines[i] << "\n";
    }

    ++_counter;
    file.close();
    _fileNames.push_back(fileName);
}

void FileHandler::OpenFiles() {
    for(int i = 0; i < _fileNames.size(); i++) {
        std::wifstream *file = new std::wifstream(_fileNames[i]);
        
        if (file->good()) {
            _files.emplace_back(file);
        } else {
            std::cerr << "Can not open " << _fileNames[i] << std::endl;
            exit(0);
        }
    }
}

void FileHandler::CloseFiles() {
    for(int i = 0; i < _files.size(); i++) {
        _files[i]->close();
        delete _files[i];
    }

    for(int i = 0; i < _fileNames.size(); i++) {
        remove(_fileNames[i].c_str());
    }
}

std::vector<std::wifstream*> FileHandler::GetVectorFiles() {
    return this->_files;
}

class Sorter {
    private:
        std::string _inputFile;
        std::string _outputFile;
        long long _memoryLimit = 8000000000;
        FileHandler* _fileHandler;
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
    _fileHandler = new FileHandler();
}

Sorter::Sorter(const std::string& inputFile,
            const std::string& outputFile,
            int memoryLimit)
            : _inputFile(inputFile)
            , _outputFile(outputFile)
            , _memoryLimit(memoryLimit)
{
    _fileHandler = new FileHandler();
}

Sorter::~Sorter() {}

void Sorter::Sort() {
    SplitAndSort();
    Merge();
}

void Sorter::SplitAndSort() {
    std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
    std::wifstream finput(_inputFile);
    finput.imbue(loc);

    if(!finput.good()) {
        std::cerr << "Can not open " << _inputFile << std::endl;
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
            _fileHandler->WriteToFile(textLines);
            textLines.clear();
            totalSize = 0;
        }
    }

    if(!textLines.empty()) {
        sort(textLines.begin(), textLines.end());
        _fileHandler->WriteToFile(textLines);
    }
    
    finput.close();
}

void Sorter::Merge() {
    _fileHandler->OpenFiles();
    std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
    std::priority_queue<linedata, std::vector<linedata>, std::greater<linedata> > dataQueue;

    auto files = _fileHandler->GetVectorFiles();

    std::wstring line;
    for(int i = 0; i < files.size(); i++) {
        files[i]->imbue(loc);
        std::getline(*files[i], line);
        dataQueue.emplace(linedata(line, files[i]));
    }
    
    std::wofstream foutput(_outputFile);
    foutput.imbue(loc);

    if(!foutput.good()) {
        std::cerr << "Can not open " << _outputFile << std::endl;
        exit(1);
    }
    while (!dataQueue.empty()) {
        linedata topData = dataQueue.top();
        dataQueue.pop();
        foutput << topData.first << "\n";
        topData.second->imbue(loc);
        if(std::getline(*(topData.second), line)) {
            dataQueue.emplace(linedata(line, topData.second));
        }
    }
    _fileHandler->CloseFiles();
}

int main(int argc, char *argv[]) {
    Sorter *sort = new Sorter(argv[1], argv[2], std::stoll(argv[3]));
    sort->Sort();
    return 0;
}