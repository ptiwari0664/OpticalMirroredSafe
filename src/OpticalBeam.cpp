#include <OpticalMirroredSafe/OpticalBeam.h>
#include <stdexcept>
#include <cmath>
using namespace std;

BeamDriver::BeamDriver(Safe safe) : safe(safe) {}
BeamDriver::~BeamDriver() = default;

Direction BeamDriver::LeftReflect(const Mirror mirror) const
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

Direction BeamDriver::RightReflect(const Mirror mirror) const
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

Direction BeamDriver::UpReflect(const Mirror mirror) const
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

Direction BeamDriver::DownReflect(const Mirror mirror) const
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

Direction BeamDriver::DetermineNextDirection(const Direction curDirection, const Mirror mirror) const
{
    switch (curDirection)
    {
    case Left:
        return LeftReflect(mirror);
    case Right:
        return RightReflect(mirror);
    case Up:
        return UpReflect(mirror);
    case Down:
        return DownReflect(mirror);
    default:
        throw runtime_error("Unknown Direction type");
    }
}

Position BeamDriver::DetermineNextPos(const Direction Direction, const Position& curPos) const
{
    Position newPos = curPos;
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

bool BeamDriver::BeamWithinSafe(const Position& curPos) const
{
    const auto rowCount = safe.GetRowCount();
    const auto colCount = safe.GetColumnCount();
    return (curPos.col < colCount && curPos.col >= 0 && curPos.row < rowCount && curPos.row >= 0);
}

bool BeamDriver::BeamOpensSafe(const Position& curPos, const Direction curDir) const
{
    const auto rowCount = safe.GetRowCount();
    const auto colCount = safe.GetColumnCount();
    return (curDir == Direction::Right && curPos.row == (rowCount - 1) && curPos.col >= colCount);
}

bool BeamDriver::BeamOpenSafeMissingMirrorFind(vector<MirrorObj>& missingMirrors) const
{
    if (safe.IsEmpty())
    {
        return false;
    }
    Position curPos(0, 0);
    auto curDir = Direction::Right;
    auto curMirror = safe.GetMirrorFromGrid(curPos);
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
            auto nextDir = DetermineNextDirection(curDir, curMirror);
            auto nextPos = DetermineNextPos(nextDir, curPos);
            if (nextDir != curDir && beamFailCount == 0)
            {
                beamLastKnownPos = nextPos; //Possible missing mirrors start here
            }
            curDir = nextDir;
            curPos = nextPos;
            curMirror = safe.GetMirrorFromGrid(curPos);
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

vector<MirrorObj> BeamDriver::GetPossibleMissingMirrors(const Position& beamLastKnownPos, const Position& beamFirstFailPos, const Direction beamFirstFailDir) const
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
