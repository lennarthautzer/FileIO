/*
 * Lennart Hautzer
 * 10 100 5235
 * All rights reserved.
 */

#include "SyntaxHandler.hxx"

SyntaxHandler::SyntaxHandler()
{
}

SyntaxHandler::~SyntaxHandler()
{
}

void SyntaxHandler::makeUpperCase( std::string & in ) const
{
  std::transform( in.begin(),
    in.end(),
    in.begin(),
    std::function< int( int ) >( ::toupper ) );
}

void SyntaxHandler::makeLowerCase( std::string & in ) const
{
  std::transform( in.begin(),
    in.end(),
    in.begin(),
    std::function< int( int ) >( ::tolower ) );
}

void SyntaxHandler::makeTitleCase( std::string & in ) const
{
  char * charArrayOfIn = ( char * ) in.c_str();
  size_t arrLen = strlen( charArrayOfIn );

  if( charArrayOfIn[ 0 ] == ' ' )
  {
    memmove( charArrayOfIn, charArrayOfIn + 1, arrLen );
    arrLen--;
  }

  charArrayOfIn[ 0 ] = toupper( charArrayOfIn[ 0 ] );

  for( size_t i = 1; i <= arrLen; i++ )
  {
    if( charArrayOfIn[ i - 1 ] == ' ' || charArrayOfIn[ i - 1 ] == '\"' )
    {
      charArrayOfIn[ i ] = toupper( charArrayOfIn[ i ] );

      continue;
    }
    charArrayOfIn[ i ] = tolower( charArrayOfIn[ i ] );
  }

  in = std::string( charArrayOfIn );

  if( in.substr( 0, 4 ).compare( "The " ) == 0 )
  {
    in = in.substr( 4 ) + ", The ";
  }
}

void SyntaxHandler::trimWhiteSpace( std::string & in ) const
{
  std::string tmpStr;

  if( in.at( 0 ) == ' ' )
  {
    in = in.substr( 1 );
  }

  std::unique_copy( in.begin(),
    in.end(),
    std::back_insert_iterator< std::string >( tmpStr ),
    []( char i, char j ) { return std::isspace( i ) && std::isspace( j ); } );

  in = tmpStr;
}

bool SyntaxHandler::startsWith(
  std::string const & in, std::string const & prefix ) const
{
  return in.substr( 0, prefix.length() ) == prefix ? true : false;
}

bool SyntaxHandler::startsWith(
  char const * const & in, char const * const & prefix ) const
{
  return startsWith( std::string( in ), std::string( prefix ) );
}

bool SyntaxHandler::endsWith(
  std::string const & in, std::string const & suffix ) const
{
  if( in.length() < suffix.length() )
  {
    return false;
  }

  return in.substr( in.length() - suffix.length() ) == suffix ? true : false;
}

bool SyntaxHandler::endsWith(
  char const * const & in, char const * const & suffix ) const
{
  return endsWith( std::string( in ), std::string( suffix ) );
}

std::string SyntaxHandler::centerString(
  std::string const & str, int const targetSize ) const
{
  assert( targetSize >= 0 );

  int whiteSpace = targetSize - ( static_cast< int >( str.size() ) );

  if( whiteSpace % 2 != 0 )
  {
    whiteSpace--;
  }

  return whiteSpace > 0 ? ( std::string( whiteSpace / 2, ' ' ) + str
           + std::string( whiteSpace / 2, ' ' ) ) :
                          str;
}

std::vector< std::string > SyntaxHandler::splitString(
  std::string str, char const & charToSplitOn ) const
{
  size_t i = 0;
  std::vector< std::string > ret;

  while( ( i = str.find( charToSplitOn ) ) != std::string::npos )
  {
    std::string const subString = str.substr( 0, i );

    if( subString.find_first_not_of( ' ' ) != std::string::npos )
    {
      ret.push_back( subString );
    }

    str.erase( 0, i + 1 );
  }
  if( str.find_first_not_of( ' ' ) != std::string::npos )
  {
    ret.push_back( str );
  }

  return ret;
}

std::vector< std::string > SyntaxHandler::splitStringsInQuotes(
  std::string str ) const
{
  size_t i = 0;
  std::vector< std::string > ret;
  bool secondQuote = false;

  while( ( i = str.find( "\"" ) ) != std::string::npos )
  {
    if( secondQuote )
    {
      std::string const subString = str.substr( 0, i );
      if( subString.find_first_not_of( ' ' ) != std::string::npos )
      {
        ret.push_back( "\"" + subString + "\"" );
      }
      str.erase( 0, i + 1 );
      secondQuote = false;

      continue;
    }

    std::string const subString = str.substr( 0, i );

    if( subString.find_first_not_of( ' ' ) != std::string::npos )
    {
      ret.push_back( subString );
    }

    str.erase( 0, i + strlen( "\"" ) );
    secondQuote = true;
  }
  if( str.find_first_not_of( ' ' ) != std::string::npos )
  {
    ret.push_back( str );
  }

  return ret;
}

void SyntaxHandler::stripChar(
  std::string & str, char const & charToRemove ) const
{
  str.erase( std::remove( str.begin(), str.end(), charToRemove ), str.end() );
}
