#pragma once
#include <OpticalMirroredSafe/OpticalSafe.h>

class OpticalBeam
{
private:
    OpticalSafe optSafeObj_;

    Direction LeftTravellingBeam(const Mirror mirror) const;
    Direction RightTravellingBeam(const Mirror mirror) const;
    Direction UpTravellingBeam(const Mirror mirror) const;
    Direction DownTravellingBeam(const Mirror mirror) const;

    Direction DetermineNextDirection(const Direction curDirection, const Mirror mirror)const;
    Position DetermineNextPosition(const Direction curDir, const Position& curPos) const;

    bool BeamWithinSafe(const Position& curPos) const;
    bool BeamOpensSafe(const Position& curPos, const Direction curDir) const;

    std::vector<MirrorObj> GetPossibleMissingMirrors(const Position& beamLastKnownPos,
            const Position& beamFirstFailPos, const Direction beamFirstFailDir) const;

public:
    OpticalBeam(OpticalSafe optSafeObj_);
    ~OpticalBeam();

    bool BeamOpenSafeMissingMirrorFind(std::vector<MirrorObj>& missingMirrors) const;
};
