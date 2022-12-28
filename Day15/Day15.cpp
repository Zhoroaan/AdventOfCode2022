#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

struct Vector2
{
    int64_t X = 0;
	int64_t Y = 0;

    [[nodiscard]] int64_t ManDistance(Vector2 InOther) const
    {
        return std::abs(X - InOther.X) + std::abs(Y - InOther.Y);
    }

    bool operator==(const Vector2& InOther) const = default;
};

struct TargetPositon
{
    TargetPositon(Vector2 InPos, Vector2 InClosest)
        : Position(InPos), BeaconPosition(InClosest), ClosestDistance(InPos.ManDistance(InClosest))
    {}
    
    Vector2 Position;
    Vector2 BeaconPosition;
    int64_t ClosestDistance;
};



int main(int /*InArgc*/, char* /*InArgv[]*/)
{
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;
    std::vector<TargetPositon> sensors;
    int64_t minX(std::numeric_limits<int64_t>::max());
    int64_t maxX(std::numeric_limits<int64_t>::min());
    int64_t maxDistance(std::numeric_limits<int64_t>::min());
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        std::stringstream ss(inputLine);
        Vector2 sensor, beacon;
        // Sensor at x=2899860, y=3122031
        ss.ignore(12), ss >> sensor.X, ss.ignore(4),  ss >> sensor.Y;
        // : closest beacon is at x=2701269, y=3542780
        ss.ignore(25), ss >> beacon.X, ss.ignore(4),  ss >> beacon.Y;
        minX= std::min(sensor.X, std::min(beacon.X, minX));
        maxX = std::max(sensor.X, std::max(beacon.X, maxX));
        TargetPositon targetPosition(sensor, beacon);
        minX= std::min(sensor.X, std::min(beacon.X, minX));
        maxX = std::max(sensor.X, std::max(beacon.X, maxX));
        maxDistance = std::max(targetPosition.ClosestDistance, maxDistance);
        sensors.emplace_back(targetPosition);
    }
    constexpr int checkLine = 10;

    int64_t nonBeaconPosition = 0;
    for (int64_t x = minX -maxDistance; x <= maxX + maxDistance; ++x)
    {
        Vector2 pos(x, checkLine);
        bool found = true;
        for (const auto& sensor : sensors)
        {
            if (sensor.BeaconPosition == pos)
            {
                found = true;
                break;
            }
            // ..####B######################..
            if (pos.ManDistance(sensor.Position) <= sensor.ClosestDistance)
                found = false;
        }
        if (!found)
        {
            nonBeaconPosition++;
            //std::cout << "#";
        }
        else
        {
            //std::cout << ".";
        }
    }
    std::cout << std::endl << "Non-beacon locations: " << nonBeaconPosition << std::endl;
    return 0;
}
