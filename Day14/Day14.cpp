#include <fstream>
#include <iostream>
#include <string>
#include <ranges>
#include <sstream>
#include <vector>

//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";
constexpr int32_t GridSize = 1200;

class Grid
{
public:
    Grid () : GridData(GridSize * GridSize)
    {
        std::memset(GridData.data(), '.', GridSize * GridSize);
    }
    const char* operator[](int x) const
    {
        return &GridData[x * GridSize];
    }
    char* operator[](int x)
    {
        return &GridData[x * GridSize];
    }

    void Print()
    {
        for (int y = 0; y < 10; ++y)
        {
            for (int x = 494; x < 504; ++x)
            {
                std::cout << (*this)[x][y];
            }
            std::cout << std::endl;
        }
    }

    bool RunSand(int InMaxHeight)
    {
        Grid& grid = *this;
        int x = 500, y = 0;
        while (true)
        {
            if (y > GridSize)
                return false;
            bool okNewHeight = (y + 1) < InMaxHeight;
            
            if (okNewHeight && grid[x][y+1] == '.')
            {
                y++;
            }
            else if (okNewHeight && grid[x - 1][y + 1] == '.')
            {
                x--;
                y++;
            }
            else if (okNewHeight && grid[x + 1][y + 1] == '.')
            {
                x++;
                y++;
            }
            else if (y == 0)
                return false;
            else
            {
                grid[x][y] = 'o';
                break;
            }
        }
        return true;
    }

protected:
    std::vector<char> GridData;
};

void DrawLine(Grid& InGrid, int InPrevX, int InPrevY, int InX, int InY)
{
    std::cout << "Draw line from " << InPrevX << "," << InPrevY << " to " << InX << "," << InY << std::endl;
    if (InPrevX != InX)
    {
        for (int x = std::min(InPrevX, InX); x <=std::max(InPrevX, InX); ++x)
            InGrid[x][InY] = '#';
    }
    else
    {
        for (int y = std::min(InPrevY, InY); y <=std::max(InPrevY, InY); ++y)
            InGrid[InX][y] = '#';
    }
}

int main(int /*InArgc*/, char* /*InArgv[]*/)
{
    std::ifstream inputFile;

    Grid grid;
    grid[0][0] = 'p';
    grid[0][1] = 'o';
    
    inputFile.open(Filename);
    std::string inputLine;
    int topY = 0;
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        std::string_view words{inputLine};
        constexpr std::string_view blockDelim{" -> "};

        int prevX = -1, prevY = -1;
        
        for (const auto word : std::views::split(words, blockDelim))
        {
            std::string entry(word.begin(), word.end());
            std::stringstream ss(entry);
            int x = 0, y = 0;
            ss >> x, ss.ignore(1), ss >> y;
            if (y > topY)
                topY = y;
            bool newValue = prevY == -1;
            if (!newValue)
                DrawLine(grid, prevX, prevY, x, y);
            prevX = x;
            prevY = y;
        }
    }
    grid[500][0] = '+';
    int32_t counter = 0;
    while (grid.RunSand(topY + 2))
        counter++;
    
    std::cout << "Num stacked: " << counter + 1 << std::endl;
    return 0;
}
