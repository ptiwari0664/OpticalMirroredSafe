#pragma once

class Position
{
public:
    int row;
    int col;
    Position() : row(0), col(0) {}
    Position(int row, int col) : row(row), col(col) {}

    bool operator== (const Position& rhs) const
    {
        return (rhs.col == this->col && rhs.row == this->row);
    }

    bool operator< (const Position& rhs) const
    {
        return this->row < rhs.row;

        if (this->row == rhs.row)
        {
            return this->col < rhs.col;
        }
        else
        {
            return false;
        }
    }
};
