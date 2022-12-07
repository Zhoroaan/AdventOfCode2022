#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

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
    
    std::vector<std::pair<Directory*, int64_t>> GetDirectoriesWithMinSize(int64_t InMinSize)
    {
        std::vector<std::pair<Directory*, int64_t>> returnVector;
        const int64_t directorySize = GetDirectorySize();
        if (directorySize >= InMinSize)
            returnVector.emplace_back(this, directorySize);
        
        for(const auto& dir : Directories)
        {
            auto foundSubDirectories = dir->GetDirectoriesWithMinSize(InMinSize);
            returnVector.insert(returnVector.end(), foundSubDirectories.begin(), foundSubDirectories.end());
        }
        return returnVector;
    }
};

std::shared_ptr<Directory> ParseDirectory(std::ifstream& InInputFile, std::string InName = "")
{
    std::shared_ptr<Directory> returnPointer;
    if (!InName.empty())
        returnPointer = std::make_shared<Directory>(InName);
    
    std::string inputLine;
    while (!InInputFile.eof())
    {
        std::getline(InInputFile, inputLine);
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
                    returnPointer = ParseDirectory(InInputFile, parts[2]);
                else
                    returnPointer->Directories.emplace_back(ParseDirectory(InInputFile, parts[2]));
            }
        }
        else if (parts[0] != "dir")
        {
            returnPointer->ContainingFiles.emplace_back(parts[1], std::atoll(parts[0].c_str()));
        }
    }
    return returnPointer;
}

int main(int /*InArgc*/, char* /*InArgv[]*/)
{
    std::ifstream inputFile;

    inputFile.open(Filename);
    std::shared_ptr<Directory> startDir;
    const int64_t diskSize = 70000000;
    
    const auto tree = ParseDirectory(inputFile);
    const auto amountToFree = tree->GetDirectorySize() + 30000000 - diskSize;

    auto foundDirectories = tree->GetDirectoriesWithMinSize(amountToFree);
    std::ranges::sort(foundDirectories, [](const auto& InA, const auto& InB)
    {
        return InA.second < InB.second;
    });

    std::cout << "Remove dir: " << foundDirectories[0].first->Name << " of size " << foundDirectories[0].second << std::endl;
    
    return 0;
}
