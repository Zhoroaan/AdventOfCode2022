#include <algorithm>
#include <array>
#include <charconv>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>


//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

enum class PacketType
{
    Value,
    List
};
int CountDigits(int inNumber)
{
    if (inNumber == 0)
        return 1;
    for (int digitCount = 1, valueCount = 1; valueCount <= inNumber; digitCount++, valueCount *= 10)
    {
        return digitCount;
    }
    return -1;
}
enum class PacketOrder : uint8_t
{
    Correct,
    Unknown,
    Wrong
};

struct Packet
{
    Packet(PacketType InType, int InValue = 0)
        : Type(InType), Value(InValue) {
        
    }
    PacketType Type;
    std::vector<std::shared_ptr<Packet>> Packages;
    int Value;
    
    static std::shared_ptr<Packet> ParsePackage(const std::string& InInputData)
    {
        std::shared_ptr<Packet> package = std::make_shared<Packet>(PacketType::List);
        std::vector<std::shared_ptr<Packet>> packageStack;
        packageStack.emplace_back(package);
        for (size_t dataIndex = 0; dataIndex < InInputData.size(); ++dataIndex)
        {
            const char& data = InInputData[dataIndex];
            if (data == '[')
            {
                packageStack.back()->Packages.emplace_back(std::make_shared<Packet>(PacketType::List));
                packageStack.emplace_back(packageStack.back()->Packages.back());
            }
            else if (data == ']')
            {
                packageStack.pop_back();
            }
            else if (std::isdigit(data))
            {
                const int newNumber = std::atoi(InInputData.c_str() + dataIndex);
                const int numDigits = CountDigits(newNumber);
                dataIndex += numDigits - 1;
                packageStack.back()->Packages.emplace_back(std::make_shared<Packet>(PacketType::Value, newNumber));
            }
        }
        return package;
    }

    PacketOrder IsSmallerThan(const std::shared_ptr<Packet>& InShared)
    {
        if (InShared->Type == PacketType::Value && Type == PacketType::Value)
        {
            //std::cout << "Compare " << Value << " with " << InShared->Value << std::endl;
            if( Value < InShared->Value)
            {
            //    std::cout << "Left side is smaller so items are in the right order!" << std::endl;
                return PacketOrder::Correct;
            }
            if (Value == InShared->Value)
                return PacketOrder::Unknown;

            //std::cout << "Right side is smaller so items are in the wrong order!" << std::endl;
            return PacketOrder::Wrong;
        }
        if(InShared->Type == PacketType::Value)
        {
            InShared->Type = PacketType::List;
            InShared->Packages.emplace_back(std::make_shared<Packet>(PacketType::Value, InShared->Value));
        }

        if (Type == PacketType::Value)
        {
            Type = PacketType::List;
            Packages.emplace_back(std::make_shared<Packet>(PacketType::Value, Value));
        }

        for (size_t a = 0; a < Packages.size(); ++a)
        {
            if (InShared->Packages.size() <= a)
            {
                //std::cout << "Ran out of element on right side" << std::endl;
                return PacketOrder::Wrong;
            }

            const PacketOrder compareResult = Packages[a]->IsSmallerThan(InShared->Packages[a]);
            if (compareResult == PacketOrder::Unknown)
                continue;
            
            return compareResult;
        }
        //std::cout << "Ran out of element on left side" << std::endl;
        return Packages.size() == InShared->Packages.size() ? PacketOrder::Unknown : PacketOrder::Correct;
    }
};

int main(int /*InArgc*/, char* /*InArgv[]*/)
{
    std::ifstream inputFile;

    inputFile.open(Filename);
    std::string inputLineLeft;
    std::string inputLineRight;
    int readLines = 0;
    int32_t counter = 0;
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLineLeft);
        if (inputLineLeft.size() == 0)
            continue;
        std::shared_ptr<Packet> leftPacket = std::move(Packet::ParsePackage(inputLineLeft));
        std::getline(inputFile, inputLineRight);
        std::shared_ptr<Packet> rightPacket = std::move(Packet::ParsePackage(inputLineRight));
        readLines++;
        //std::cout << "Testing " << readLines << std::endl;

        auto packetOrder = leftPacket->IsSmallerThan(rightPacket);
        if (packetOrder == PacketOrder::Correct)
        {
            std::cout << "Smaller found at " <<  readLines << std::endl;
            counter+= readLines;
        }
        //std::cout << inputLineLeft << (leftSmallest ? " smaller than " : " bigger than ") << inputLineRight << std::endl;
    }
    std::cout << "Answer is " << counter << std::endl;
    return 0;
}
