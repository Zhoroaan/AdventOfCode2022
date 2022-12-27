#include <algorithm>
#include <array>
#include <charconv>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>


std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

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
    Packet(PacketType InType, int InValue = 0, const std::string& InStringRespresentation = "")
        : Type(InType), Value(InValue), StringRepresentation(InStringRespresentation) {
        
    }
    std::string StringRepresentation;
    PacketType Type;
    std::vector<std::shared_ptr<Packet>> Packages;
    int Value;
    
    static std::shared_ptr<Packet> ParsePackage(const std::string& InInputData)
    {
        std::shared_ptr<Packet> package = std::make_shared<Packet>(PacketType::List, 0, InInputData);
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
            if( Value < InShared->Value)
                return PacketOrder::Correct;
            if (Value == InShared->Value)
                return PacketOrder::Unknown;

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
                return PacketOrder::Wrong;

            const PacketOrder compareResult = Packages[a]->IsSmallerThan(InShared->Packages[a]);
            if (compareResult == PacketOrder::Unknown)
                continue;
            
            return compareResult;
        }
        return Packages.size() == InShared->Packages.size() ? PacketOrder::Unknown : PacketOrder::Correct;
    }
};

int main(int /*InArgc*/, char* /*InArgv[]*/)
{
    std::ifstream inputFile;

    inputFile.open(Filename);
    std::string inputLine;
    std::vector<std::shared_ptr<Packet>> packets;
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        if (inputLine.empty())
            continue;
        packets.emplace_back(std::move(Packet::ParsePackage(inputLine)));
    }
    packets.emplace_back(std::move(Packet::ParsePackage("[[2]]")));
    packets.emplace_back(std::move(Packet::ParsePackage("[[6]]")));
    std::ranges::sort(packets, [] (const auto& InA, const auto& InB)
    {
       return InA->IsSmallerThan(InB) == PacketOrder::Correct; 
    });
    int counter = 1;
    for (size_t a = 0; a < packets.size(); ++a)
    {
        const auto& packet = packets[a];
        if (packet->StringRepresentation == "[[2]]"
            || packet->StringRepresentation == "[[6]]")
                counter *= (a + 1);
        //std::cout << packet->StringRepresentation << std::endl;
    }
    std::cout << "Answer is: " << counter << std::endl;
    return 0;
}
