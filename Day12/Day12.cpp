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

struct Position
{
    Position (int InX = 0, int InY = 0)
        : X(InX), Y(InY)
    {}

    int X = 0;
    int Y = 0;
    bool operator==(const Position& InVector) const = default;

    Position operator+(const Position& InOther) const
    {
        return Position{X + InOther.X, Y + InOther.Y};
    }
    
    Position operator<(const Position& InOther) const
    {
        return GetHash() < InOther.GetHash();
    }

    [[nodiscard]] int32_t GetHash() const
    {
        const int hash = Y;
        return (hash << 16) + X;
    }
};

struct Leaf
{
    Leaf(int32_t InCost = -1, const Position& InPosition = Position(), const Position& InWalkedFrom = Position())
        : Cost(InCost), CurrentPosition(InPosition), WalkedFrom(InWalkedFrom)
    {}
    int32_t Cost;
    Position CurrentPosition;
    Position WalkedFrom;
};

std::vector<Position> GetValidMoves(const Position& InPos, const std::vector<std::string>& InHeightData, const std::vector<Leaf>& InClosed, const std::vector<Leaf>& InOpen)
{
    std::vector<Position> returnVector;

    const std::array dirExplore = {Position{-1, 0}, Position{0, 1}, Position{1, 0}, Position{0, -1}};

    for (const Position& direction : dirExplore)
    {
        const auto pos = InPos + direction;
        if (pos.X < 0 || pos.Y < 0 || pos.X >= InHeightData[0].length() || pos.Y >= InHeightData.size())
            continue;

        bool found = false;
        for (const auto& leaf : InClosed)
        {
            if (leaf.CurrentPosition == pos)
                found = true;
        }
        for (const auto& leaf : InOpen)
        {
            if (leaf.CurrentPosition == pos)
                found = true;
        }
        if (found)
            continue;

        const auto currentHeight = InHeightData[InPos.Y][InPos.X];
        const auto newHeight = InHeightData[pos.Y][pos.X];
        if (newHeight <= currentHeight || newHeight - currentHeight <= 1)
            returnVector.push_back(pos);
    }
    
    return returnVector;
}

int32_t CountMoveSteps(const Position& InStart, const Position& InEnd, const std::vector<std::string>& InHeightData)
{
    std::vector<Leaf> open;
    std::vector<Leaf> closed;

    open.emplace_back(0, InStart);

    bool foundEnd = false;
    int32_t endCost = 0;
    while (!open.empty() && !foundEnd)
    {
        const auto currentPos = open[0].CurrentPosition;
        const auto currentCost = open[0].Cost;

        std::vector<Position> directionsToExplore = GetValidMoves(currentPos, InHeightData, closed, open);
        closed.emplace_back(open[0]);
        open.erase(open.begin());
        
        for (const auto& newPosition : directionsToExplore)
        {
            if (newPosition == InEnd)
            {
                foundEnd = true;
                closed.emplace_back(currentCost + 1, newPosition, currentPos);
                endCost = currentCost + 1;
            }
            else
                open.emplace_back(currentCost + 1, newPosition, currentPos);
        }

        
        std::ranges::sort(open, [] (const auto& InA, const auto& InB) { return InA.Cost < InB.Cost;});
    }
    return endCost;
}

int main(int /*InArgc*/, char* /*InArgv[]*/)
{
    std::ifstream inputFile;

    inputFile.open(Filename);
    std::string inputLine;
    std::vector<std::string> data;
    Position end;
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        for (int a = 0; a < inputLine.length(); ++a)
        {
            char& height = inputLine[a];
            if (height == 'S')
                height = 'a';
            if (height == 'E')
            {
                end = {a, static_cast<int>(data.size())};
                height = 'z';
            }
        }
        data.emplace_back(inputLine);
    }


    std::vector<Position> startPositions;
    for(size_t y = 0; y < data.size(); ++y)
    {
        for(size_t x = 0; x < data[y].length(); ++x)
        {
            if (data[y][x] == 'a')
                startPositions.emplace_back(static_cast<int>(x), static_cast<int>(y));
        }
    }
    int32_t minCost = std::numeric_limits<int32_t>::max();
    for (auto& startPosition : startPositions)
    {
        const int32_t steps = CountMoveSteps(startPosition, end, data);
        if (steps != 0)
            minCost = std::min(minCost, CountMoveSteps(startPosition, end, data));
    }
    
    std::cout << "Number of steps: " << minCost << std::endl;
    return 0;
}
