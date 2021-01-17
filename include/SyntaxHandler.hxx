/*
* Lennart Hautzer
* 10 100 5235
* All rights reserved.
*/

#ifndef SyntaxHandlerHeader
#define SyntaxHandlerHeader

#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstring>
#include <cctype>
#include <cassert>

class SyntaxHandler
{
public:

    SyntaxHandler();

    ~SyntaxHandler();

    /*
    * String manipulation functions
    */

    void makeUpperCase( std::string & in ) const;
    void makeLowerCase( std::string & in ) const;
    void makeTitleCase( std::string & in ) const;
    void trimWhiteSpace( std::string & in ) const;
    void stripChar( std::string & str, char const & charToRemove ) const;
    std::string centerString( std::string const & str,
        int const targetSize ) const;

    std::vector<std::string> splitString( std::string str,
        char const & charToSplitOn ) const;
    std::vector<std::string> splitStringsInQuotes( std::string str ) const;

    /*
    * String analysis functions
    */

    bool startsWith( std::string const & in, std::string const & prefix ) const;
    bool startsWith( char const * const & in,
        char const * const & prefix ) const;

    bool endsWith( std::string const & in, std::string const & suffix ) const;
    bool endsWith( char const * const & in,
        char const * const & suffix ) const;
};

#endif
