#pragma once
#include<vector>
#include <OpticalMirroredSafe/Pose.h>
#include <iostream>

enum Mirror
{
    None,
    BackSlantMirror,
    ForwardSlantMirror,
};

enum Direction
{
    Up,
    Right,
    Down,
    Left,
};

class MirrorObj
{
public:
    Position pos;
    Mirror mirror;
    MirrorObj() = default;
    MirrorObj(int row, int col, Mirror mir) : pos(row, col), mirror(mir) {}
};

class Safe
{
private:
    int rowCount;
    int colCount;
    std::vector<std::vector<Mirror>> mirrorGrid;

public:
    Safe(const int rows, const int cols);
    ~Safe();

    void SetMirrorInGrid(const Position& pos, const Mirror mirror);
    Mirror GetMirrorFromGrid(const Position& pos) const;
    std::vector<std::vector<Mirror>> GetMirrorGrid() const;
    int GetColumnCount() const;
    int GetRowCount() const;
    bool IsEmpty() const;
};
