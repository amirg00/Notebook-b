#include <iostream>
#include <vector>
#include "Direction.hpp"

using namespace std;

#define ROW 100

namespace ariel{
    class Notebook{
    vector<string> _pages;
    public:
        void write(int page, int row, int column, Direction direction, string const& word);
        string read(int page, int row, int column, Direction direction, int length);
        void erase(int page, int row, int column, Direction direction, int length);
        void show(int page);
        void addPagesToVector(unsigned int page);
    };
}
