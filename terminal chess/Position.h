#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <cassert>
#include <cctype>
// Internal representation: file is lowercase a-h, rank is 1-8

struct Position
{
    using uchar = unsigned char;
    char file{};
    int rank{};

    bool operator==(const Position& p) const
    {
        return (this->file == p.file) && (this->rank == p.rank);
    }
    bool validPosition() const
    {
        return (file >= 'a' && file <= 'h') && (rank >= 1 && rank <= 8);
    }
    friend std::ostream& operator<<(std::ostream& out, const Position& pos)
    {
        out << pos.file << pos.rank;
        return out;
    }
    friend std::istream& operator>>(std::istream& in, Position& pos)
    {
        std::string input{};
        in >> input;
 
        if(input.length() != 2)
        {
            in.setstate(std::ios_base::failbit);
            return in;
        }
        
        auto fileInRange {
            [](char c) -> bool
            {
                return (c >= 'a' && c <= 'h') || (c >= 'A' && c <= 'H');
            }
        };
        auto rankInRange {
            [](char c) -> bool
            {
                return (c >= '1' && c <= '8');
            }
        };

        if(fileInRange(input[0]) && rankInRange(input[1]))
        {
            pos.file = static_cast<char>(std::tolower(static_cast<uchar>(input[0])));
            pos.rank = input[1] - '0';
        }
        else
        {
            in.setstate(std::ios_base::failbit);
        }
        return in;
    }
    Position operator+(const std::pair<int,int>& fileRankIncrement) const
    {
        return Position{ 
            static_cast<char>(file + fileRankIncrement.first) 
            , rank + fileRankIncrement.second 
        };
    }
    Position operator-(const std::pair<int,int>& fileRankIncrement) const
    {
        return Position{ 
            static_cast<char>(file - fileRankIncrement.first) 
            , rank - fileRankIncrement.second 
        };
    }
    bool isLastRank() const
    {
        assert( validPosition() && "Check last rank on invalid position.\n");
        
        return rank == 1 || rank == 8;
    }
};



#endif