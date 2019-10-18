#pragma once
#include <OpticalMirroredSafe/OpticalSafe.h>

class OpticalBeam
{
private:
    OpticalSafe safe;

    Direction LeftReflect(const Mirror mirror) const;
    Direction RightReflect(const Mirror mirror) const;
    Direction UpReflect(const Mirror mirror) const;
    Direction DownReflect(const Mirror mirror) const;

    Direction DetermineNextDirection(const Direction curDirection, const Mirror mirror)const;
    Position DetermineNextPos(const Direction curDir, const Position& curPos) const;

    bool BeamWithinSafe(const Position& curPos) const;
    bool BeamOpensSafe(const Position& curPos, const Direction curDir) const;

    std::vector<MirrorObj> GetPossibleMissingMirrors(const Position& beamLastKnownPos,
            const Position& beamFirstFailPos, const Direction beamFirstFailDir) const;

public:
    OpticalBeam(OpticalSafe safe);
    ~OpticalBeam();

    bool BeamOpenSafeMissingMirrorFind(std::vector<MirrorObj>& missingMirrors) const;
};
