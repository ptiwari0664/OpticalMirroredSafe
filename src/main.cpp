#include <iostream>
#include <vector>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <OpticalMirroredSafe/OpticalBeam.h>

#ifdef _MSC_VER
#define SSCANF sscanf_s
#else
#define SSCANF sscanf
#endif

/**
 * @brief UserInputSetupSafes
 * @brief Reacts according to the user input
 * @return vector of OpticalSafe
 */
std::vector<OpticalSafe> UserInputSetupSafes()
{
    std::vector<OpticalSafe> vSafes;
    std::string line;
    int rowCnt, colCnt, mLines, nLines;
    int row, col;

    while (getline(std::cin, line))
    {
        if (line.empty()) { break; }

        if (SSCANF(line.c_str(), "%d %d %d %d", &rowCnt, &colCnt, &mLines, &nLines) != 4)
        {
            std::cout << "\033[1;31mUnknown input Detected\033[0m\n";
            break;
        }
        if (!(rowCnt, colCnt <= 1000000 && rowCnt, colCnt >= 0 &&
              mLines, nLines <= 200000 && mLines, nLines >= 0))
        {
            std::cout << "\033[1;31mInput out of range\033[0m\n";
            break;
        }

        OpticalSafe safe(rowCnt, colCnt);

        for (auto i = 0; i < mLines; i++)
        {
            getline(std::cin, line);
            SSCANF(line.c_str(), "%d %d", &row, &col);
            Position pos(row - 1, col - 1);
            safe.SetMirrorInGrid(pos, ForwardSlantMirror);
            std::cout << "Received Forward Mirror \n";
        }

        for (auto j = 0; j < nLines; j++)
        {
            getline(std::cin, line);
            SSCANF(line.c_str(), "%d %d", &row, &col);
            Position pos(row - 1, col - 1);
            safe.SetMirrorInGrid(pos, BackSlantMirror);
            std::cout << "Received Backward Mirror \n";

        }
        vSafes.push_back(safe);
    }
    return vSafes;
}

/**
 * @brief GetLexSmallestMirror
 * @param mirrorObjects
 * @return mirror positions
 */
Position GetLexSmallestMirror(const std::vector<MirrorObj>& mirrorObjects)
{
    Position smallestPos;

    if (!mirrorObjects.empty())
    {
        smallestPos = mirrorObjects[0].pos;
        for (auto m : mirrorObjects)
        {
            if (m.pos < smallestPos)
            {
                smallestPos = m.pos;
            }
        }
    }
    return smallestPos;
}

/**
 * @brief PrintResults
 * @param missingMirrors
 * @param safeOpenSuccess
 */
void PrintResults(const std::vector<MirrorObj>& missingMirrors, bool safeOpenSuccess)
{
    if (!safeOpenSuccess && missingMirrors.empty())
    {
        std::cout << "Impossible";
    }
    else if (safeOpenSuccess)
    {
        if (missingMirrors.empty())
        {
            std::cout << "0";
        }
        else
        {
            auto mirrorPos = GetLexSmallestMirror(missingMirrors);
            std::cout << missingMirrors.size() << " "
                      << mirrorPos.row + 1 << " " << mirrorPos.col + 1;
        }
    }
    std::cout << std::endl;

}

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[])
{
    // try and catch usage to avoid exceptions
    try
    {
        auto vSafes = UserInputSetupSafes();    //Take input from the file

        std::cout << "Total Safe received = " << vSafes.size() << std::endl;

        auto count = 0;

        for (auto safe : vSafes)
        {
            OpticalBeam optbeam(safe);
            std::vector<MirrorObj> missingMirrorsObj;

            auto safeOpenSuccess = optbeam.BeamOpenSafeMissingMirrorFind(missingMirrorsObj);

            std::cout << "Case " << ++count << " : ";
            PrintResults(missingMirrorsObj, safeOpenSuccess);
        }
    }
    catch (std::exception& exp)
    {
        std::cout << "Error: "<<exp.what() << std::endl;
        return -1;
    }

    return 0;
}
