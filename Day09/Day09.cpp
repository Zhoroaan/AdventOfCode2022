#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

class TrackPosition
{
public:
    void MoveDirection(char InDirection)
    {
        if (InDirection == 'R')
            PosX++;
        if (InDirection == 'L')
            PosX--;
        if (InDirection == 'U')
            PosY++;
        if (InDirection == 'D')
            PosY--;
    }

    int16_t DistanceTo(const TrackPosition& InOther)
    {
        return std::max(std::abs(PosX - InOther.PosX), std::abs(PosY - InOther.PosY));
    }
    void MoveTowards(const TrackPosition& InOther)
    {
        if (DistanceTo(InOther) <= 1)
            return;

        if (std::abs(PosX - InOther.PosX) >= 1)
        {
            PosX += (PosX - InOther.PosX) < 0 ? 1 : -1;
        }
        if (std::abs(PosY - InOther.PosY) >= 1)
        {
            PosY += (PosY - InOther.PosY) < 0 ? 1 : -1;
        }
    }

    int16_t GetPosX() const { return PosX; }
    int16_t GetPosY() const { return PosY; }
    int32_t GetHash() const
    {
        const int hash = PosY;
        return (hash << 16) + PosX;
    }
protected:
    int16_t PosX = 0;
    int16_t PosY = 0;
};

std::ostream &operator<<(std::ostream &InOs, TrackPosition const &InCurrentPosition) { 
    return InOs << "PosX: " << InCurrentPosition.GetPosX() << " PosY: " << InCurrentPosition.GetPosY();
}

int main(int /*InArgc*/, char* /*InArgv[]*/)
{
    std::ifstream inputFile;

    inputFile.open(Filename);
    std::string inputLine;
    TrackPosition head, tail;
    std::set<int32_t> uniquePositions;
    while (!inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        std::stringstream l(inputLine);
        char direction = ' ';
        int16_t stepCount = std::numeric_limits<int16_t>::min();
        l >> direction >>stepCount;
        //std::cout << direction << ": " << stepCount << std::endl;
        for (int a = 0; a < stepCount; ++a)
        {
            head.MoveDirection(direction);
            tail.MoveTowards(head);
            //std::cout << "Head: " << head << std::endl;
            //std::cout << "Tail: " << tail << std::endl;
            //std::cout << "Tail hash: " << tail.GetHash() << std::endl;
            uniquePositions.emplace(tail.GetHash());
        }
    }
    
    std::cout << "Number of unique positions: " << uniquePositions.size() << std::endl;
    
    return 0;
}
