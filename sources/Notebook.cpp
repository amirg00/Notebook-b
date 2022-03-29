/**
 * @author Amir Gillette
 * @date 2022-03-18
 */

#include "Notebook.hpp"
#include "Direction.hpp"
#include <iostream>
#include <stdexcept>
#include <valarray>

using ariel::Direction;
using namespace std;

string createNewLine(string page);
string addLines(string str, unsigned int amount);
void addPagesToVector(vector<string> _pages, unsigned int page);

/**
 * The method writes a given word in a given direction (horizontal/vertical) and position (row, col)
 * @param page a given page index.
 * @param row row location.
 * @param column column location.
 * @param direction the direction in which the word will be read.
 * @param word word to write.
 * @note Exception is thrown whenever the writing position is already written or deleted (everything except '_').
 */
void ariel::Notebook::write(int page, int row, int column, Direction direction, string const& word) {
    if (page < 0 || row < 0 || column < 0 || column >= ROW){
        throw invalid_argument("Page, row and column cannot be negative, column cannot exceed 100!");
    }

    // checking for invalid chars in the given word
    for (size_t i = 0; i < word.length(); ++i) {
        if (word[i] < ' ' || word[i] >= '~'){
            throw invalid_argument("Char isn't printable!");
        }
    }

    unsigned int unsigned_page = unsigned(page);
    unsigned int unsigned_row = unsigned(row);
    unsigned int unsigned_column = unsigned(column);


    string currPage;
    try {
        currPage =_pages.at(unsigned_page);
    }
    catch (exception) { // Page doesn't exist
        addPagesToVector(unsigned_page);
        string newPage = createNewLine("");
        _pages.insert(_pages.begin() + unsigned_page, newPage);
        currPage = _pages.at(unsigned_page);
    }

    unsigned int pos = unsigned_row > 0 ? (ROW + 1) * (unsigned_row) + unsigned_column : unsigned_column;

    if (direction == Direction::Horizontal){
        if (unsigned_column + word.length() - 1 > ROW - 1){
            throw invalid_argument("Out of bounds!");
        }

        unsigned int MaxLineIndex = currPage.length() / ((ROW + 1) + 1);
        if (unsigned_row > MaxLineIndex){
            unsigned int linesToCreate = unsigned_row - MaxLineIndex;
            currPage = addLines(currPage, linesToCreate);
        }
        for (size_t i = pos, k = 0; i < pos + word.length(); ++i, k++) {
            if (currPage[i] != '_'){
                throw invalid_argument("Char has written/deleted already in this position.");
            }
            currPage[i] = word[k];
        }
    }
    else{  // direction is Vertical

        unsigned int MaxLineIndex = currPage.length() / ((ROW + 1) + 1);
        if (unsigned_row + word.length() > MaxLineIndex){
            unsigned int linesToCreate = (unsigned_row + word.length()) - MaxLineIndex;
            currPage = addLines(currPage, linesToCreate);
        }

        for (size_t i = 0; i < word.length(); ++i) {
            if (currPage[pos + (ROW + 1) * i] != '_'){
                throw invalid_argument("Char has written/deleted already in this position.");
            }
            currPage[pos + (ROW + 1) * i] = word[i];
        }
    }
    _pages.at(unsigned_page) = currPage;
}

/**
 * Method erases a word in a given length, direction (horizontal/vertical), and starting position (row, column)/
 * @param page a given page index.
 * @param row row location.
 * @param column column location.
 * @param direction the direction in which the word will be read.
 * @param length the length of the word to be read.
 */
void ariel::Notebook::erase(int page, int row, int column, Direction direction, int length) {
    if (page < 0 || row < 0 || column < 0 || length < 0 || column >= ROW){
        throw invalid_argument("Page, row, column and length cannot be negative!");
    }

    unsigned int unsigned_page = unsigned(page);
    string currPage;

    try {
        currPage =_pages.at(unsigned_page); // throws exception if page doesn't exist.
    }
    catch (exception) { // Page doesn't exist
        addPagesToVector(unsigned_page);
        string newPage = createNewLine("");
        _pages.insert(_pages.begin() + unsigned_page, newPage);
        currPage = _pages.at(unsigned_page);
    }

    unsigned int unsigned_row = unsigned(row);
    unsigned int unsigned_column = unsigned(column);
    unsigned int unsigned_length = unsigned(length);

    unsigned int MaxLineIndex = currPage.length() / ((ROW + 1) + 1);
    unsigned int pos = unsigned_row > 0 ? (ROW + 1) * (unsigned_row) + unsigned_column : unsigned_column;

    if (direction == Direction::Horizontal){
        if (unsigned_column + unsigned_length > ROW - 1){
            throw errc::result_out_of_range;
        }
        if (unsigned_row > MaxLineIndex){
            unsigned int linesToCreate = unsigned_row - MaxLineIndex;
            currPage = addLines(currPage, linesToCreate);
        }
        for (size_t i = pos; i < pos + unsigned_length; ++i) {
            currPage[i] = '~';
        }
    }
    else{  // direction is Vertical

        if (unsigned_row + unsigned_length > MaxLineIndex){
            unsigned int linesToCreate = (unsigned_row + unsigned_length) - MaxLineIndex;
            currPage = addLines(currPage, linesToCreate);
        }

        for (size_t i = 0; i < length; ++i) {
            currPage[pos + (ROW + 1) * i] = '~';
        }
    }
    _pages.at(unsigned_page) = currPage;

}

/**
 * The method reads the word at the position (row, col), a word which its length is given.
 *
 * @param page a given page index.
 * @param row row location.
 * @param column column location.
 * @param direction the direction in which the word will be read.
 * @param length the length of the word to be read.
 * @return the read word.
 * @note Exception is thrown whenever the word to read exceed row length, which is an integer constant (ROW = 100).
 */
string ariel::Notebook::read(int page, int row, int column, Direction direction, int length) {
    if (page < 0 || row < 0 || column < 0 || length < 0 || column >= ROW){
        throw invalid_argument("Page, row, column and length cannot be negative!");
    }

    unsigned int unsigned_page = unsigned(page);
    string currPage;

    try {
        currPage =_pages.at(unsigned_page); // throws exception if page doesn't exist.
    }
    catch (exception) { // Page doesn't exist
        addPagesToVector(unsigned_page);
        string newPage = createNewLine("");
        _pages.insert(_pages.begin() + unsigned_page, newPage);
        currPage = _pages.at(unsigned_page);
    }

    unsigned int unsigned_row = unsigned(row);
    unsigned int unsigned_column = unsigned(column);
    unsigned int unsigned_length = unsigned(length);

    unsigned int MaxLineIndex = currPage.length() / ((ROW + 1) + 1);
    unsigned int pos = unsigned_row > 0 ? (ROW + 1) * (unsigned_row) + unsigned_column : unsigned_column;
    string readStr;

    if (direction == Direction::Horizontal){
        if (unsigned_column + unsigned_length - 1 > ROW - 1){
            throw errc::result_out_of_range;
        }

        if (unsigned_row > MaxLineIndex){
           return string(unsigned_length, '_');
        }

        for (size_t i = pos; i < pos + unsigned_length; ++i) {
            readStr += currPage[i];
        }
    }
    else{  // direction is Vertical

        if (unsigned_row + unsigned_length > MaxLineIndex){
            unsigned int linesToCreate = (unsigned_row + unsigned_length) - MaxLineIndex;
            currPage = addLines(currPage, linesToCreate);
        }

        for (size_t i = 0; i < unsigned_length; ++i) {
            readStr += currPage[pos + (ROW + 1) * i];
        }
    }
    return readStr;
}

/**
 * Method shows the page by a given index.
 * The representation of the page determined according to the example representation in Demo.cpp file.
   E.G.:
                    1:   _________
		      2:   ___~_____
		      3:   __a~cd___
		      4:   ___~_____
		      .
		      .
		      .
		      100: _________

 * @param page a page index to the page string.
 */
void ariel::Notebook::show(int page) {
    if (page < 0){
        throw invalid_argument("Page cannot be negative!");
    }
    unsigned int unsigned_page = unsigned(page);
    string currPage;

    try {
        currPage =_pages.at(unsigned_page); // throws exception if page doesn't exist.
    }
    catch (exception) { // Page doesn't exist
        addPagesToVector(unsigned_page);
    }

    unsigned int MaxLineIndex = currPage.length() / ((ROW + 1) + 1);
    unsigned int MaxLineLen = ((unsigned int)(log10(MaxLineIndex))) + 1;

    for (size_t i = 0, line = 1; i < currPage.length(); ++i) {
        unsigned int lineNumLen = ((unsigned int)(log10(line)));
        string space = string(MaxLineLen - lineNumLen, ' ');
        if (i % (ROW + 1) == 0){
            cout << line << ':' << space << currPage[i];
            line++;
        }
        else{
            cout << currPage[i];
        }
    }
}

/**
 * Create pages before page index if they haven't created yet.
 * @param pages pages vector
 * @param page page index
 */
void ariel::Notebook::addPagesToVector(unsigned int page)
{
    // Create pages before page index if they haven't created yet.
    if(_pages.size() < page){
        for (int i = _pages.size(); i < page; ++i) {
            string newPage = createNewLine("");
            _pages.insert(_pages.begin() + i, newPage);
        }
    }
}
/**
 * Function creates new line to the given page.
 * @param page a given page
 * @return the page after adding new line to it.
 */
string createNewLine(string page){
    for (int i = 0; i < ROW; ++i) {
        page += '_';
    }
    page += '\n';
    return page;
}

/**
 * Function add lines to the page string by a given amount.
 * @param str a given string page.
 * @param amount the amount of lines to add.
 * @return the new page string.
 */
string addLines(string str, unsigned int amount){
    for (int i = 0; i < amount; ++i) {
        str = createNewLine(str);
    }
    return str;
}