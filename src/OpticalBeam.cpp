#include <OpticalMirroredSafe/OpticalBeam.h>
#include <stdexcept>
#include <cmath>
using namespace std;

/**
 * @brief OpticalBeam::OpticalBeam
 * @param safe
 */
OpticalBeam::OpticalBeam(OpticalSafe safe) : optSafeObj_(safe) {}
OpticalBeam::~OpticalBeam() = default;

/**
 * @brief OpticalBeam::LeftTravellingBeam
 * @param mirror
 * @return Beam Next Direction
 * @abstract While travelling towards left side these cases will occur
 */
Direction OpticalBeam::LeftTravellingBeam(const Mirror mirror) const
{
    switch (mirror)
    {
    case None:
        return Left;
    case BackSlantMirror:
        return Up;
    case ForwardSlantMirror:
        return Down;
    default:
        throw runtime_error("Unknown Mirror type");
    }
}

/**
 * @brief OpticalBeam::RightTravellingBeam
 * @param mirror
 * @return Beam Next Direction
 * @abstract While travelling towards right side these cases will occur
 */
Direction OpticalBeam::RightTravellingBeam(const Mirror mirror) const
{
    switch (mirror)
    {
    case None:
        return Right;
    case BackSlantMirror:
        return Down;
    case ForwardSlantMirror:
        return Up;
    default:
        throw runtime_error("Unknown Mirror type");
    }
}

/**
 * @brief OpticalBeam::UpTravellingBeam
 * @param mirror
 * @return Beam Next Direction
 * @abstract While travelling towards upward these cases will occur
 */
Direction OpticalBeam::UpTravellingBeam(const Mirror mirror) const
{
    switch (mirror)
    {
    case None:
        return Up;
    case BackSlantMirror:
        return Left;
    case ForwardSlantMirror:
        return Right;
    default:
        throw runtime_error("Unknown Mirror type");
    }
}

/**
 * @brief OpticalBeam::DownTravellingBeam
 * @param mirror
 * @return Beam Next Direction
 * @abstract While travelling towards downward these cases will occur
 */
Direction OpticalBeam::DownTravellingBeam(const Mirror mirror) const
{
    switch (mirror)
    {
    case None:
        return Down;
    case BackSlantMirror:
        return Right;
    case ForwardSlantMirror:
        return Left;
    default:
        throw runtime_error("Unknown Mirror type");
    }
}

/**
 * @brief OpticalBeam::DetermineNextDirection
 * @param curDirection
 * @param mirror
 * @return Optical beam direction based on mirror and direction
 */
Direction OpticalBeam::DetermineNextDirection(const Direction currentDirection, const Mirror mirror) const
{
    switch (currentDirection)
    {
    case Left:
        return LeftTravellingBeam(mirror);
    case Right:
        return RightTravellingBeam(mirror);
    case Up:
        return UpTravellingBeam(mirror);
    case Down:
        return DownTravellingBeam(mirror);
    default:
        throw runtime_error("Received Unknown Direction");
    }
}

/**
 * @brief OpticalBeam::DetermineNextPos
 * @param Direction
 * @param curPos
 * @return beam position based on travel
 */
Position OpticalBeam::DetermineNextPosition(const Direction Direction, const Position& currentPosition) const
{
    Position newPos = currentPosition;
    switch (Direction)
    {
    case Left:
        newPos.col--;
        break;
    case Right:
        newPos.col++;
        break;
    case Up:
        newPos.row--;
        break;
    case Down:
        newPos.row++;
        break;
    default:
        throw runtime_error("Unknown Direction type");
    }
    return newPos;
}

/**
 * @brief OpticalBeam::BeamWithinSafe
 * @param curPos
 * @return true / false based on beam existance within the safe
 */
bool OpticalBeam::BeamWithinSafe(const Position& curPos) const
{
    const auto rowCount = optSafeObj_.GetRowCount();
    const auto colCount = optSafeObj_.GetColumnCount();
    return (curPos.col < colCount && curPos.col >= 0 && curPos.row < rowCount && curPos.row >= 0);
}

/**
 * @brief OpticalBeam::BeamOpensSafe
 * @param curPos
 * @param curDir
 * @return bool
 */
bool OpticalBeam::BeamOpensSafe(const Position& curPos, const Direction curDir) const
{
    const auto rowCount = optSafeObj_.GetRowCount();
    const auto colCount = optSafeObj_.GetColumnCount();
    return (curDir == Direction::Right && curPos.row == (rowCount - 1) && curPos.col >= colCount);
}

/**
 * @brief OpticalBeam::BeamOpenSafeMissingMirrorFind
 * @param missingMirrors
 * @return bool
 */
bool OpticalBeam::BeamOpenSafeMissingMirrorFind(vector<MirrorObj>& missingMirrors) const
{
    if (optSafeObj_.IsEmpty())
    {
        return false;
    }

    //Define the initial beam pose to be 0,0 which will be travelling towards right
    Position curPos(0, 0);
    auto curDir = Direction::Right;
    auto curMirror = optSafeObj_.GetMirrorFromGrid(curPos);
    auto isNewDir = false;

    auto beamOpenSafeSuccess = false;
    auto beamFailCount = 0;
    auto beamLastKnownPos = Position(0, 0);
    auto beamFirstFailPos = Position(0, 0);
    Direction beamFirstFailDir;
    vector<MirrorObj> possibleMissingMirrors;
    vector<MirrorObj>::iterator it;

    while (true)
    {
        while (BeamWithinSafe(curPos))
        {
            auto nextDirection = DetermineNextDirection(curDir, curMirror);
            auto nextPosition = DetermineNextPosition(nextDirection, curPos);
            if (nextDirection != curDir && beamFailCount == 0)
            {
                beamLastKnownPos = nextPosition; //Possible missing mirrors start here
            }
            curDir = nextDirection;
            curPos = nextPosition;
            curMirror = optSafeObj_.GetMirrorFromGrid(curPos);
        }

        if (BeamOpensSafe(curPos, curDir))
        {
            beamOpenSafeSuccess = true;
            if (beamFailCount == 0) { break; }
            else { missingMirrors.push_back(*it); }
        }

        beamFailCount++;
        if (beamFailCount == 1)
        {
            beamFirstFailPos = curPos;
            beamFirstFailDir = curDir;
            possibleMissingMirrors = GetPossibleMissingMirrors(beamLastKnownPos, beamFirstFailPos, beamFirstFailDir);
            if (possibleMissingMirrors.empty()) { break; }
            it = possibleMissingMirrors.begin();
        }
        else
        {
            it++;
        }

        if (it != possibleMissingMirrors.end())
        {
            auto mirrorObj = *it;
            curPos = mirrorObj.pos;
            curMirror = mirrorObj.mirror;
            curDir = beamFirstFailDir;
        }
        else
        {
            break;
        }

    }

    return beamOpenSafeSuccess;
}

/**
 * @brief OpticalBeam::GetPossibleMissingMirrors
 * @param beamLastKnownPos
 * @param beamFirstFailPos
 * @param beamFirstFailDir
 * @return mirror postions
 */
vector<MirrorObj> OpticalBeam::GetPossibleMissingMirrors(const Position& beamLastKnownPos, const Position& beamFirstFailPos, const Direction beamFirstFailDir) const
{
    vector<MirrorObj> missingMirrorObj;

    if (beamFirstFailDir == Left || beamFirstFailDir == Right)
    {
        //missing in multiple columns of a row
        const auto row = beamLastKnownPos.row;
        auto col = beamLastKnownPos.col;
        auto nPossiblePositions = abs(beamFirstFailPos.col - beamLastKnownPos.col);

        for (auto i = 0; i < nPossiblePositions; i++)
        {
            MirrorObj mo1(row, col + i, BackSlantMirror);
            MirrorObj mo2(row, col + i, ForwardSlantMirror);
            missingMirrorObj.push_back(mo1);
            missingMirrorObj.push_back(mo2);
        }
    }
    else if (beamFirstFailDir == Up || beamFirstFailDir == Down)
    {
        //missing in multiple rows of a column
        auto row = beamLastKnownPos.row;
        const auto col = beamLastKnownPos.col;
        auto nPossiblePositions = abs(beamFirstFailPos.row - beamLastKnownPos.row);

        for (auto i = 0; i < nPossiblePositions; i++)
        {
            MirrorObj mo1(row + i, col, BackSlantMirror);
            MirrorObj mo2(row + i, col, ForwardSlantMirror);
            missingMirrorObj.push_back(mo1);
            missingMirrorObj.push_back(mo2);
        }
    }
    return missingMirrorObj;
}
