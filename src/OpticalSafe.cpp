#include <OpticalMirroredSafe/OpticalSafe.h>
#include <stdexcept>
using namespace std;

Safe::Safe(const int rows, const int cols):
    rowCount(rows),
    colCount(cols)
{
    std::cout << "<< ----- Received a New case ----- >> \n";
    mirrorGrid.resize(rows, vector<Mirror>(cols, Mirror::None));
}

Safe::~Safe() = default;

void Safe::SetMirrorInGrid(const Position& pos, const Mirror mirror)
{
    if (pos.row < rowCount && pos.col < colCount && pos.row >=0 && pos.col >= 0)
    {
        mirrorGrid[pos.row][pos.col] = mirror;
    }
    else
    {
        throw runtime_error("Position out of range.");
    }
}

Mirror Safe::GetMirrorFromGrid(const Position& pos) const
{
    if (pos.row < rowCount && pos.col < colCount && pos.row >= 0 && pos.col >= 0)
    {
        return mirrorGrid[pos.row][pos.col];
    }
    else
    {
        return None;
    }

}

int Safe::GetRowCount() const
{
    return rowCount;
}

int Safe::GetColumnCount() const
{
    return colCount;
}

vector<vector<Mirror>> Safe::GetMirrorGrid() const
{
    return mirrorGrid;
}

bool Safe::IsEmpty() const
{
    return mirrorGrid.empty();
}
