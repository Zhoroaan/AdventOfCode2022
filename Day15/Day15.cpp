#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>

std::string Filename = "TestInput.txt";
constexpr int64_t CheckExtent = 20;
//std::string Filename = "Input.txt";
//constexpr int64_t CheckExtent = 4000000;


constexpr int ThreadCount = 23;

class Timer
{
public:
    Timer(const char* InName)
        : Name(InName)
    {
        StartTime = std::chrono::high_resolution_clock::now();
    }
    ~Timer()
    {
        auto span  = std::chrono::high_resolution_clock::now() - StartTime;
        std::cout << Name << " - Duration: " << std::chrono::duration<double>(span) << std::endl;
    }
protected:
    std::chrono::high_resolution_clock::time_point StartTime;
    const char* Name;
};

struct Vector2
{
    int64_t X = 0;
	int64_t Y = 0;

    [[nodiscard]] int64_t ManDistance(const Vector2& InOther) const noexcept
    {
        auto distX = std::abs(X - InOther.X);
        auto distY = std::abs(Y - InOther.Y);
        return distX + distY; //std::abs(X - InOther.X) + std::abs(Y - InOther.Y);
    }

    [[nodiscard]] bool ManDistanceWithin(const Vector2& InOther, int64_t InDist) const noexcept
    {
        const auto distX = std::abs(X - InOther.X);
        if (distX > InDist)
            return false;
        const auto distY = std::abs(Y - InOther.Y);
        return  (distX + distY) <= InDist; //std::abs(X - InOther.X) + std::abs(Y - InOther.Y);
    }

    bool operator==(const Vector2& InOther) const = default;
};

struct TargetPosition
{
    TargetPosition(Vector2 InPos, Vector2 InClosest)  noexcept
        : Position(InPos), BeaconPosition(InClosest),
          ClosestDistance(InPos.ManDistance(InClosest))
    {
        MinBounds = {
            std::min(Position.X, BeaconPosition.X) - ClosestDistance,
                    std::min(Position.Y, BeaconPosition.Y) - ClosestDistance
        };

        MaxBounds = {
            std::max(Position.X, BeaconPosition.X) + ClosestDistance,
                    std::max(Position.Y, BeaconPosition.Y) + ClosestDistance
        };
    }

    [[nodiscard]] bool IsWithinBounds(const Vector2& InPosition) const noexcept
    {
        return InPosition.X >= MinBounds.X && InPosition.X <= MaxBounds.X
            && InPosition.Y >= MinBounds.Y && InPosition.Y <= MaxBounds.Y;
    }

    bool IsWithXBounds(int64_t InX) const noexcept
    {
        return InX >= MinBounds.X && InX <= MaxBounds.X;
    }

    Vector2 MinBounds;
    Vector2 MaxBounds;
    
    Vector2 Position;
    Vector2 BeaconPosition;
    int64_t ClosestDistance;
};


bool IsValidBeaconPosition(const std::vector<TargetPosition>& InSensors, const Vector2& InPos) noexcept
{
    bool found = true;
    for (const auto& sensor : InSensors)
    {
        if (!sensor.IsWithinBounds(InPos))
            continue;
        if (InPos.ManDistanceWithin(sensor.Position, sensor.ClosestDistance))
        {
            found = false;
            break;
        }
    }
    return found;
}

void TestFrequencies(const std::vector<TargetPosition>& InSensors, int64_t InStart, int64_t InEnd)
{
    std::vector<TargetPosition> selectedSensors;
    selectedSensors.reserve(InSensors.size());
    for (int64_t x = InStart; x <= InEnd; ++x)
    {
        for (const auto& sensor : InSensors)
        {
            if (sensor.IsWithXBounds(x))
                selectedSensors.push_back(sensor);
        }
        
        for (int64_t y = 0; y <= CheckExtent; ++y)
        {
            Vector2 pos(x, y);
            if (IsValidBeaconPosition(selectedSensors, pos))
                std::cout << std::endl << "Frequency: " << 4000000 * x + y << std::endl;
        }
        if (InStart == 0 && x % 400 == 0)
        {
            std::cout << "Progress: " <<  (x * 100.f) / InEnd << "%" << std::endl;
        }
        selectedSensors.clear();
    }
}

int main(int /*InArgc*/, char* /*InArgv[]*/)
{
    std::ifstream inputFile;
    inputFile.open(Filename);
    std::string inputLine;
    std::vector<TargetPosition> sensors;
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
        TargetPosition targetPosition(sensor, beacon);
        minX= std::min(sensor.X, std::min(beacon.X, minX));
        maxX = std::max(sensor.X, std::max(beacon.X, maxX));
        maxDistance = std::max(targetPosition.ClosestDistance, maxDistance);
        sensors.emplace_back(targetPosition);
    }
    
    {
        Timer t("Test time");
        std::vector<std::jthread> threads;
        int64_t sliceSize = CheckExtent / ThreadCount;
        for (int64_t a = 0; a < ThreadCount; ++a)
        {
            threads.emplace_back([a, sliceSize, &sensors]
            {
               TestFrequencies(sensors, sliceSize * a, std::min(sliceSize*(a + 1), CheckExtent)); 
            });
        }
        for (auto& thread : threads)
        {
            thread.join();
        }
    }
    return 0;
}
