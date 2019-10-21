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

/**
 * @brief OpticalSafe::SetMirrorInGrid
 * @param pos
 * @param mirror
 */
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

/**
 * @brief OpticalSafe::GetMirrorFromGrid
 * @param pos
 * @return mirrors
 */
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

/**
 * @brief OpticalSafe::GetRowCount
 * @return number of rows
 */
int OpticalSafe::GetRowCount() const
{
    return rowCount;
}

/**
 * @brief OpticalSafe::GetColumnCount
 * @return number of columns
 */
int OpticalSafe::GetColumnCount() const
{
    return colCount;
}

/**
 * @brief OpticalSafe::GetMirrorGrid
 * @return 2d mirror grid
 */
vector<vector<Mirror>> OpticalSafe::GetMirrorGrid() const
{
    return mirrorGrid;
}

/**
 * @brief OpticalSafe::IsEmpty
 * @return bool
 */
bool OpticalSafe::IsEmpty() const
{
    return mirrorGrid.empty();
}
