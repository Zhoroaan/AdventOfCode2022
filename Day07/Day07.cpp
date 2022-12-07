#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

struct Directory
{
    Directory(std::string InName)
    {
        Name = InName;
    }
    
    std::string Name;
    std::vector<std::pair<std::string, int64_t>> ContainingFiles;
    std::vector<std::shared_ptr<Directory>> Directories;

    int64_t GetDirectorySize()
    {
        int64_t size = 0;
        for(const auto& dir : Directories)
            size += dir->GetDirectorySize();
        for (const auto& val : ContainingFiles | std::views::values)
            size += val;
        return size;
    }
    
    std::vector<std::pair<Directory*, int64_t>> GetDirectoriesWithMaxSize(int64_t InMaxSize)
    {
        std::vector<std::pair<Directory*, int64_t>> returnVector;
        const int64_t directorySize = GetDirectorySize();
        if (directorySize < InMaxSize)
            returnVector.emplace_back(this, directorySize);
        
        for(const auto& dir : Directories)
        {
            auto foundSubDirectories = dir->GetDirectoriesWithMaxSize(InMaxSize);
            returnVector.insert(returnVector.end(), foundSubDirectories.begin(), foundSubDirectories.end());
        }
        return returnVector;
    }
};

std::shared_ptr<Directory> ParseDirectory(std::ifstream& inputFile, std::string InName = "")
{
    std::shared_ptr<Directory> returnPointer;
    if (!InName.empty())
        returnPointer = std::make_shared<Directory>(InName);
    
    std::string inputLine;
    while (!inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        std::vector<std::string> parts;
        std::istringstream stringParser(inputLine);
        std::string lineBuffer;
        while (std::getline(stringParser, lineBuffer, ' ')) {
            parts.push_back(lineBuffer);
        }
        if (parts[0] == "$")
        {
            if (parts[1] == "cd")
            {
                if (parts[2] == "..")
                    return returnPointer;
                
                if (returnPointer == nullptr)
                    returnPointer = ParseDirectory(inputFile, parts[2]);
                else
                    returnPointer->Directories.emplace_back(ParseDirectory(inputFile, parts[2]));
            }
        }
        else
        {
            if (parts[0] != "dir")
            {
                returnPointer->ContainingFiles.emplace_back(parts[1], std::atoll(parts[0].c_str()));
            }
        }
    }
    return returnPointer;
}

int main(int argc, char* argv[])
{
    std::ifstream inputFile;

    inputFile.open(Filename);
    std::shared_ptr<Directory> startDir;
    auto tree = ParseDirectory(inputFile);
    auto t = tree->GetDirectoriesWithMaxSize(100000);

    int64_t size = 0;
    for (auto& dir : t)
    {
        size += dir.second;
    }

    std::cout << "Total size: " << size << std::endl;
    
    return 0;
}
