#include <algorithm>
#include <charconv>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

enum class MonkeyOperation
{
    OldAdd,
    OldMult,
    OldOldMult
};

class Monkey
{
public:
    void AddItem(int64_t InItemWorry)
    {
        ItemWorries.emplace_back(InItemWorry);
    }

    void SetOperation(MonkeyOperation InMonkeyOperation)
    {
        MonkeyOperation = InMonkeyOperation;
    }

    void SetValue(int InOperationValue)
    {
        OperationValue = InOperationValue;
    }

    void SetTestValue(int InTestValue)
    {
        TestValue = InTestValue;
    }

    void SetMonkeyThrow(bool InIfTrue, int InMonkeyIndex)
    {
        MonkeyThrowToo[InIfTrue ? 0 : 1] = InMonkeyIndex;
    }

    [[nodiscard]] int64_t GetItemInspectionCount() const { return ItemInspections; }

    void RunIteration(std::vector<Monkey>& InMonkeys, int64_t InCommonBase)
    {
        for (auto itemWorry : ItemWorries)
        {
            ItemInspections++;
            if (MonkeyOperation == MonkeyOperation::OldAdd)
                itemWorry += OperationValue;
            else if (MonkeyOperation == MonkeyOperation::OldMult)
                itemWorry *= OperationValue;
            else
                itemWorry *= itemWorry;

            itemWorry %= InCommonBase;

            const int targetMonkey = itemWorry % TestValue == 0 ? MonkeyThrowToo[0] : MonkeyThrowToo[1];
            InMonkeys[targetMonkey].AddItem(itemWorry);
                
        }
        ItemWorries.clear();
    }

protected:
    MonkeyOperation MonkeyOperation = MonkeyOperation::OldAdd;
    int32_t OperationValue = 0;
    int32_t TestValue = 1;
    int32_t MonkeyThrowToo[2] = {-1, -1};
    std::vector<int64_t> ItemWorries;
    int64_t ItemInspections = 0;
};

int main(int /*InArgc*/, char* /*InArgv[]*/)
{
    std::ifstream inputFile;

    inputFile.open(Filename);
    std::string inputLine;
    std::vector<Monkey> monkeys;
    int64_t commonBase = 1;
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        if (inputLine.length() == 0)
            continue;
        
        if (inputLine[0] == 'M')
            monkeys.emplace_back();
        else if (inputLine[2] == 'S')
        {
            auto items = inputLine.substr(18, inputLine.length() - 18);
            constexpr std::string_view delim{", "};
            for (const auto word : std::ranges::views::split(items, delim))
            {
                int32_t itemWorry;
                std::from_chars(word.data(), word.data() + word.size(), itemWorry);
                monkeys.back().AddItem(itemWorry);
            }
        }
        else if (inputLine[2] == 'O')
        {
            auto& lastMonkey = monkeys.back();
            if (inputLine.ends_with("* old"))
                lastMonkey.SetOperation(MonkeyOperation::OldOldMult);
            else if (const auto addIndex = inputLine.find('+'); addIndex != std::string::npos)
            {
                lastMonkey.SetOperation(MonkeyOperation::OldAdd);
                lastMonkey.SetValue(std::atoi(inputLine.c_str() + addIndex + 2));
            }
            else if (const auto multIndex = inputLine.find('*'); multIndex != std::string::npos)
            {
                lastMonkey.SetOperation(MonkeyOperation::OldMult);
                lastMonkey.SetValue(std::atoi(inputLine.c_str() + multIndex + 2));
            }
        }
        else if (inputLine[2] == 'T')
        {
             if (const auto index = inputLine.find_last_of(' '); index != std::string::npos)
             {
                 int testValue = std::atoi(inputLine.c_str() + index + 1);
                 commonBase *= testValue;
                 monkeys.back().SetTestValue(testValue);
             }
        }
        else if (const char trueM = inputLine[7]; trueM  == 't' || trueM == 'f')
        {
            if (const auto index = inputLine.find_last_of(' '); index != std::string::npos)
                monkeys.back().SetMonkeyThrow(trueM == 't', std::atoi(inputLine.c_str() + index + 1));
        }
    }

    for (int rounds = 0; rounds < 10000; ++rounds)
    {
        for (auto& monkey : monkeys)
        {
            monkey.RunIteration(monkeys, commonBase);
        }
    }

    std::ranges::sort(monkeys, [](const Monkey& InA, const Monkey& InB)
    {
        return InA.GetItemInspectionCount() > InB.GetItemInspectionCount();
    });

    std::cout << "Level of monkey business: " << monkeys[0].GetItemInspectionCount() * monkeys[1].GetItemInspectionCount() << std::endl;
    return 0;
}
