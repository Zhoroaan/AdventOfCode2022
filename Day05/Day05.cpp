#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

int main(int argc, char* argv[])
{
    std::ifstream inputFile;

    inputFile.open(Filename);
    std::string inputLine;
    std::vector<std::vector<char>> buckets;
    bool readingStart = true;
    while (!inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        if (buckets.empty())
        {
            const int32_t bucketCount = static_cast<int32_t>(inputLine.length() + 1) / 4;
            buckets.reserve(bucketCount);
            for (int a = 0; a < bucketCount; ++a)
                buckets.emplace_back();
        }
        if (inputLine.length() == 0)
        {
            readingStart = false;
            for (auto& bucket : buckets)
                std::reverse(bucket.begin(), bucket.end());
            continue;
        }

        if (readingStart)
        {
            for (int bucketStart = 0; bucketStart < inputLine.length(); bucketStart += 4)
            {
                if (inputLine[bucketStart] != '[')
                    continue;
                buckets[bucketStart / 4].push_back(inputLine[bucketStart + 1]);
            }
        }
        else
        {
            int32_t count, sourceBucket, targetBucket;
            const auto found =sscanf_s(inputLine.c_str(), "move %d from %d to %d", &count, &sourceBucket, &targetBucket);
            assert(found == 3);
            for (int pickupBox = 0; pickupBox < count; ++pickupBox)
            {
                const auto offset = - count + pickupBox;
                auto element = buckets[sourceBucket - 1].end() + offset;
                buckets[targetBucket - 1].push_back(*element);
            }
            for (int removeIt = 0; removeIt < count; ++removeIt)
            {
                buckets[sourceBucket - 1].pop_back();
            }
        }
        
    }

    std::cout << "Top crates: ";
    for (auto& bucket : buckets)
    {
        std::cout << bucket.back();
    }

    std::cout << std::endl;
    
    return 0;
}
