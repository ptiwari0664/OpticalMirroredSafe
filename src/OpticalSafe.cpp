#include <OpticalMirroredSafe/OpticalSafe.h>
#include <stdexcept>
using namespace std;

/**
 * @brief OpticalSafe::OpticalSafe
 * @param rows
 * @param cols
 * @brief Initializing the optical lasar safe
 */
OpticalSafe::OpticalSafe(const int rows, const int cols):
    rowCount(rows),
    colCount(cols)
{
    std::cout << "<< ----- Received a New OpticalSafe ----- >> \n";
    mirrorGrid.resize(rows, vector<Mirror>(cols, Mirror::None));
}

OpticalSafe::~OpticalSafe() = default;

void OpticalSafe::SetMirrorInGrid(const Position& pos, const Mirror mirror)
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

Mirror OpticalSafe::GetMirrorFromGrid(const Position& pos) const
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

int OpticalSafe::GetRowCount() const
{
    return rowCount;
}

int OpticalSafe::GetColumnCount() const
{
    return colCount;
}

vector<vector<Mirror>> OpticalSafe::GetMirrorGrid() const
{
    return mirrorGrid;
}

bool OpticalSafe::IsEmpty() const
{
    return mirrorGrid.empty();
}
