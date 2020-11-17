/*------------------------------------------------------------------------------
 * Lennart Hautzer
 * 16/11/2020
 *
 * Copyright (c) 2020 Lennart Hautzer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
------------------------------------------------------------------------------*/

#include "FIO.h"

namespace NSFIO
{
  /*----------------------------------------------------------------------------
   * Ancillary Static Functions.
  ----------------------------------------------------------------------------*/
  /*----------------------------------------------------------------------------
   * Convert between normal strings and wide strings.
  ----------------------------------------------------------------------------*/

  std::string wideToMultiByte( std::wstring const & wideString )
  {
    std::string ret;
    std::string buff( MB_CUR_MAX, '\0' );

    for ( wchar_t const & wc : wideString )
    {
      int mbCharLen = std::wctomb( &buff[ 0 ], wc );

      if ( mbCharLen < 1 ) { break; }

      for ( int i = 0; i < mbCharLen; ++i ) { ret += buff[ i ]; }
    }

    return ret;
  }

  std::wstring multiByteToWide( std::string const & multiByteString )
  {
    std::wstring ws( multiByteString.size(), L' ' );
    ws.resize( std::mbstowcs(
      &ws[ 0 ], multiByteString.c_str(), multiByteString.size() ) );

    return ws;
  }

  /*----------------------------------------------------------------------------
   * Filepath manipulation.
  ----------------------------------------------------------------------------*/

  std::wstring baseFileW( std::wstring const & pathToFile,
    bool const & stripExtension /* = StripExtensionTrue */ )
  {
    std::wstring ws = pathToFile.substr( parentDirW( pathToFile ).size() );

    if ( stripExtension == StripExtensionTrue )
    {
      std::size_t const delim = ws.find_last_of( L"." );

      ws = delim != std::string::npos ? ws.substr( 0, delim ) : ws;
    }

    return ws;
  }

  std::wstring baseFileW( std::string const & pathToFile,
    bool const & stripExtension /* = StripExtensionTrue */ )
  {
    return baseFileW( multiByteToWide( pathToFile ), stripExtension );
  }

  std::wstring baseFileW( wchar_t const * const & pathToFile,
    bool const & stripExtension /* = StripExtensionTrue */ )
  {
    return baseFileW( std::wstring( pathToFile ), stripExtension );
  }

  std::wstring baseFileW( char const * const & pathToFile,
    bool const & stripExtension /* = StripExtensionTrue */ )
  {
    return baseFileW( std::string( pathToFile ), stripExtension );
  }

  std::string baseFile( std::wstring const & pathToFile,
    bool const & stripExtension /* = StripExtensionTrue */ )
  {
    return wideToMultiByte( baseFileW( pathToFile, stripExtension ) );
  }

  std::string baseFile( std::string const & pathToFile,
    bool const & stripExtension /* = StripExtensionTrue */ )
  {
    return baseFile( multiByteToWide( pathToFile ), stripExtension );
  }

  std::string baseFile( wchar_t const * const & pathToFile,
    bool const & stripExtension /* = StripExtensionTrue */ )
  {
    return baseFile( std::wstring( pathToFile ), stripExtension );
  }

  std::string baseFile( char const * const & pathToFile,
    bool const & stripExtension /* = StripExtensionTrue */ )
  {
    return baseFile( std::string( pathToFile ), stripExtension );
  }

  /*----------------------------------------------------------------------------
   * Get the parent directory of a filepath.
  ----------------------------------------------------------------------------*/

  std::wstring parentDirW( std::wstring const & path )
  {
    std::wstring ws;
    std::size_t delim = path.find_last_of( L"/\\" );

    ws = path.substr( 0, ++delim );

    return ws;
  }

  std::wstring parentDirW( std::string const & path )
  {
    return parentDirW( multiByteToWide( path ) );
  }

  std::wstring parentDirW( wchar_t const * const & path )
  {
    return parentDirW( std::wstring( path ) );
  }

  std::wstring parentDirW( char const * const & path )
  {
    return parentDirW( std::string( path ) );
  }

  std::string parentDir( std::wstring const & path )
  {
    return wideToMultiByte( parentDirW( path ) );
  }

  std::string parentDir( std::string const & path )
  {
    return parentDir( multiByteToWide( path ) );
  }

  std::string parentDir( wchar_t const * const & path )
  {
    return parentDir( std::wstring( path ) );
  }

  std::string parentDir( char const * const & path )
  {
    return parentDir( std::string( path ) );
  }

  /*----------------------------------------------------------------------------
   * Split strings. (CSV)
  ----------------------------------------------------------------------------*/

  std::vector< std::wstring > splitStringW(
    std::wstring const & source, std::wstring const & delim /* = L"\n\r" */ )
  {
    std::size_t i = 0;
    std::wstring str = source;
    std::vector< std::wstring > ret;

    while ( ( i = str.find_first_of( delim ) ) != std::string::npos )
    {
      std::wstring substr = str.substr( 0, i );

      if ( substr.find_first_not_of( delim ) != std::string::npos )
      { ret.push_back( substr ); }

      str.erase( 0, i + 1 );
    }

    if ( str.find_first_not_of( delim ) != std::string::npos )
    { ret.push_back( str ); }

    return ret;
  }

  std::vector< std::wstring > splitStringW(
    std::wstring const & source, std::string const & delim /* = "\n\r" */ )
  {
    return splitStringW( source, multiByteToWide( delim ) );
  }

  std::vector< std::wstring > splitStringW(
    std::wstring const & source, wchar_t const * const & delim /* = L"\n\r" */ )
  {
    return splitStringW( source, std::wstring( delim ) );
  }

  std::vector< std::wstring > splitStringW(
    std::wstring const & source, char const * const & delim /* = "\n\r" */ )
  {
    return splitStringW( source, std::string( delim ) );
  }

  std::vector< std::wstring > splitStringW(
    std::string const & source, std::wstring const & delim /* = L"\n\r" */ )
  {
    return splitStringW( multiByteToWide( source ), delim );
  }

  std::vector< std::wstring > splitStringW(
    std::string const & source, std::string const & delim /* = "\n\r" */ )
  {
    return splitStringW( multiByteToWide( source ), delim );
  }

  std::vector< std::wstring > splitStringW(
    std::string const & source, wchar_t const * const & delim /* = L"\n\r" */ )
  {
    return splitStringW( multiByteToWide( source ), delim );
  }

  std::vector< std::wstring > splitStringW(
    std::string const & source, char const * const & delim /* = "\n\r" */ )
  {
    return splitStringW( multiByteToWide( source ), delim );
  }

  std::vector< std::wstring > splitStringW(
    wchar_t const * const & source, std::wstring const & delim /* = L"\n\r" */ )
  {
    return splitStringW( std::wstring( source ), delim );
  }

  std::vector< std::wstring > splitStringW(
    wchar_t const * const & source, std::string const & delim /* = "\n\r" */ )
  {
    return splitStringW( std::wstring( source ), delim );
  }

  std::vector< std::wstring > splitStringW( wchar_t const * const & source,
    wchar_t const * const & delim /* = L"\n\r" */ )
  {
    return splitStringW( std::wstring( source ), delim );
  }

  std::vector< std::wstring > splitStringW(
    wchar_t const * const & source, char const * const & delim /* = "\n\r" */ )
  {
    return splitStringW( std::wstring( source ), delim );
  }

  std::vector< std::wstring > splitStringW(
    char const * const & source, std::wstring const & delim /* = L"\n\r" */ )
  {
    return splitStringW( std::string( source ), delim );
  }

  std::vector< std::wstring > splitStringW(
    char const * const & source, std::string const & delim /* = "\n\r" */ )
  {
    return splitStringW( std::string( source ), delim );
  }

  std::vector< std::wstring > splitStringW(
    char const * const & source, wchar_t const * const & delim /* = L"\n\r" */ )
  {
    return splitStringW( std::string( source ), delim );
  }

  std::vector< std::wstring > splitStringW(
    char const * const & source, char const * const & delim /* = "\n\r" */ )
  {
    return splitStringW( std::string( source ), delim );
  }

  std::vector< std::string > splitString(
    std::wstring const & source, std::wstring const & delim /* = L"\n\r" */ )
  {
    auto v = splitStringW( source, delim );
    std::vector< std::string > ret;

    for ( auto & ws : v ) { ret.push_back( wideToMultiByte( ws ) ); }

    return ret;
  }

  std::vector< std::string > splitString(
    std::wstring const & source, std::string const & delim /* = "\n\r" */ )
  {
    return splitString( source, multiByteToWide( delim ) );
  }

  std::vector< std::string > splitString(
    std::wstring const & source, wchar_t const * const & delim /* = L"\n\r" */ )
  {
    return splitString( source, std::wstring( delim ) );
  }

  std::vector< std::string > splitString(
    std::wstring const & source, char const * const & delim /* = "\n\r" */ )
  {
    return splitString( source, std::string( delim ) );
  }

  std::vector< std::string > splitString(
    std::string const & source, std::wstring const & delim /* = L"\n\r" */ )
  {
    return splitString( multiByteToWide( source ), delim );
  }

  std::vector< std::string > splitString(
    std::string const & source, std::string const & delim /* = "\n\r" */ )
  {
    return splitString( multiByteToWide( source ), delim );
  }

  std::vector< std::string > splitString(
    std::string const & source, wchar_t const * const & delim /* = L"\n\r" */ )
  {
    return splitString( multiByteToWide( source ), delim );
  }

  std::vector< std::string > splitString(
    std::string const & source, char const * const & delim /* = "\n\r" */ )
  {
    return splitString( multiByteToWide( source ), delim );
  }

  std::vector< std::string > splitString(
    wchar_t const * const & source, std::wstring const & delim /* = L"\n\r" */ )
  {
    return splitString( std::wstring( source ), delim );
  }

  std::vector< std::string > splitString(
    wchar_t const * const & source, std::string const & delim /* = "\n\r" */ )
  {
    return splitString( std::wstring( source ), delim );
  }

  std::vector< std::string > splitString( wchar_t const * const & source,
    wchar_t const * const & delim /* = L"\n\r" */ )
  {
    return splitString( std::wstring( source ), delim );
  }

  std::vector< std::string > splitString(
    wchar_t const * const & source, char const * const & delim /* = "\n\r" */ )
  {
    return splitString( std::wstring( source ), delim );
  }

  std::vector< std::string > splitString(
    char const * const & source, std::wstring const & delim /* = L"\n\r" */ )
  {
    return splitString( std::string( source ), delim );
  }

  std::vector< std::string > splitString(
    char const * const & source, std::string const & delim /* = "\n\r" */ )
  {
    return splitString( std::string( source ), delim );
  }

  std::vector< std::string > splitString(
    char const * const & source, wchar_t const * const & delim /* = L"\n\r" */ )
  {
    return splitString( std::string( source ), delim );
  }

  std::vector< std::string > splitString(
    char const * const & source, char const * const & delim /* = "\n\r" */ )
  {
    return splitString( std::string( source ), delim );
  }

  /*----------------------------------------------------------------------------
   * FIOExcept.
  ----------------------------------------------------------------------------*/

  FIOExcept::FIOExcept( std::wstring output ) :
    _output( wideToMultiByte( output ) ) {};
  FIOExcept::FIOExcept( std::string output ) : _output( output ) {};

  FIOExcept::~FIOExcept() throw() {};

  char const * FIOExcept::what() const throw() { return _output.c_str(); };

  /*----------------------------------------------------------------------------
   * FIO.
  ----------------------------------------------------------------------------*/

  std::unique_ptr< FIO > FIO::instance = std::unique_ptr< FIO >( nullptr );

  /*----------------------------------------------------------------------------
   * Construct FIO Singleton.
  ----------------------------------------------------------------------------*/

  FIO & FIO::getInstance( std::wstring const & loc /* = L"en_US.UTF-8" */ )
  {
    if ( ! instance ) { instance = std::unique_ptr< FIO >( new FIO( loc ) ); }

    return *instance;
  }

  FIO & FIO::getInstance( std::string const & loc /* = "en_US.UTF-8" */ )
  {
    return getInstance( multiByteToWide( loc ) );
  }

  FIO & FIO::getInstance( wchar_t const * const & loc /* = L"en_US.UTF-8" */ )
  {
    return getInstance( std::wstring( loc ) );
  }

  FIO & FIO::getInstance( char const * const & loc /* = "en_US.UTF-8" */ )
  {
    return getInstance( std::string( loc ) );
  }

  /*----------------------------------------------------------------------------
   * Destructor.
  ----------------------------------------------------------------------------*/

  FIO::~FIO() { reset(); }

  /*----------------------------------------------------------------------------
   * Return FIO to its initial state.
  ----------------------------------------------------------------------------*/

  FIO & FIO::reset()
  {
    for ( auto it = iFSM.begin(); it != iFSM.end(); ++it )
    { it->second->close(); }
    for ( auto it = oFSM.begin(); it != oFSM.end(); ++it )
    { it->second->close(); }

    PM.clear();
    iFSM.clear();
    oFSM.clear();

    setRoot( findRoot() );

    return *this;
  }

  /*----------------------------------------------------------------------------
   * Open an input stream.
  ----------------------------------------------------------------------------*/

  std::wistream & FIO::openWI( std::wstring const & pathOrID )
  {
    std::wstring path = getPathW( pathOrID );

    if ( iFSM.find( path ) != iFSM.end() )
    {
      throw( FIOExcept( L"Cannot open stream on " + path
        + L" as there is already a stream stored on that id." ) );
    }

    iFSM[ path ] = std::unique_ptr< std::wifstream >(
      new std::wifstream( getPath( path ), std::wifstream::in ) );

    return validateWIFStream( path );
  }

  std::wistream & FIO::openWI( std::string const & pathOrID )
  {
    return openWI( multiByteToWide( pathOrID ) );
  }

  std::wistream & FIO::openWI( wchar_t const * const & pathOrID )
  {
    return openWI( std::wstring( pathOrID ) );
  }

  std::wistream & FIO::openWI( char const * const & pathOrID )
  {
    return openWI( std::string( pathOrID ) );
  }

  /*----------------------------------------------------------------------------
   * Open an output stream.
  ----------------------------------------------------------------------------*/

  std::wostream & FIO::openWO( std::wstring const & pathOrID,
    bool const & appendToFile /* = OpenNewFile */ )
  {
    std::wstring path = pathOrID;

    if ( PM.find( pathOrID ) != PM.end() ) { path = getPathW( pathOrID ); }

    if ( oFSM.find( path ) != oFSM.end() )
    {
      throw( FIOExcept( L"Cannot open stream on " + path
        + L" as there is already a stream stored on that id." ) );
    }

    oFSM[ path ] =
      std::unique_ptr< std::wofstream >( new std::wofstream( getPath( path ),
        ( appendToFile ? std::wofstream::app : std::wofstream::out ) ) );

    return validateWOFStream( path );
  }

  std::wostream & FIO::openWO( std::string const & pathOrID,
    bool const & appendToFile /* = OpenNewFile */ )
  {
    return openWO( multiByteToWide( pathOrID ), appendToFile );
  }

  std::wostream & FIO::openWO( wchar_t const * const & pathOrID,
    bool const & appendToFile /* = OpenNewFile */ )
  {
    return openWO( std::wstring( pathOrID ), appendToFile );
  }

  std::wostream & FIO::openWO( char const * const & pathOrID,
    bool const & appendToFile /* = OpenNewFile */ )
  {
    return openWO( std::string( pathOrID ), appendToFile );
  }

  /*----------------------------------------------------------------------------
   * Rewind an input stream.
  ----------------------------------------------------------------------------*/

  std::wistream & FIO::rewindWI( std::wstring const & pathOrID )
  {
    std::wstring path = getPathW( pathOrID );

    auto & stream = validateWIFStream( path );

    stream.clear();
    stream.seekg( 0, std::ios::beg );

    return stream;
  }

  std::wistream & FIO::rewindWI( std::string const & pathOrID )
  {
    return rewindWI( multiByteToWide( pathOrID ) );
  }

  std::wistream & FIO::rewindWI( wchar_t const * const & pathOrID )
  {
    return rewindWI( std::wstring( pathOrID ) );
  }

  std::wistream & FIO::rewindWI( char const * const & pathOrID )
  {
    return rewindWI( std::string( pathOrID ) );
  }

  /*----------------------------------------------------------------------------
   * Close an input stream.
  ----------------------------------------------------------------------------*/

  FIO & FIO::closeWI( std::wstring const & pathOrID )
  {
    std::wstring path = getPathW( pathOrID );

    auto it = iFSM.find( path );
    validateWIFStream( path );

    it->second->close();
    iFSM.erase( path );

    return *this;
  }

  FIO & FIO::closeWI( std::string const & pathOrID )
  {
    return closeWI( multiByteToWide( pathOrID ) );
  }

  FIO & FIO::closeWI( wchar_t const * const & pathOrID )
  {
    return closeWI( std::wstring( pathOrID ) );
  }

  FIO & FIO::closeWI( char const * const & pathOrID )
  {
    return closeWI( std::string( pathOrID ) );
  }

  /*----------------------------------------------------------------------------
   * Close an output stream.
  ----------------------------------------------------------------------------*/
  FIO & FIO::closeWO( std::wstring const & pathOrID )
  {
    std::wstring path = getPathW( pathOrID );

    auto it = oFSM.find( path );
    validateWOFStream( path );

    it->second->close();
    oFSM.erase( path );

    return *this;
  }

  FIO & FIO::closeWO( std::string const & pathOrID )
  {
    return closeWO( multiByteToWide( pathOrID ) );
  }

  FIO & FIO::closeWO( wchar_t const * const & pathOrID )
  {
    return closeWO( std::wstring( pathOrID ) );
  }

  FIO & FIO::closeWO( char const * const & pathOrID )
  {
    return closeWO( std::string( pathOrID ) );
  }

  /*----------------------------------------------------------------------------
   * Get an open input stream.
  ----------------------------------------------------------------------------*/

  std::wistream & FIO::getWI( std::wstring const & pathOrID ) const
  {
    std::wstring path = getPathW( pathOrID );

    return validateWIFStream( path );
  }

  std::wistream & FIO::getWI( std::string const & pathOrID ) const
  {
    return getWI( multiByteToWide( pathOrID ) );
  }

  std::wistream & FIO::getWI( wchar_t const * const & pathOrID ) const
  {
    return getWI( std::wstring( pathOrID ) );
  }

  std::wistream & FIO::getWI( char const * const & pathOrID ) const
  {
    return getWI( std::string( pathOrID ) );
  }

  /*----------------------------------------------------------------------------
   * Get an open output stream.
  ----------------------------------------------------------------------------*/

  std::wostream & FIO::getWO( std::wstring const & pathOrID ) const
  {
    std::wstring path = getPathW( pathOrID );

    return validateWOFStream( path );
  }

  std::wostream & FIO::getWO( std::string const & pathOrID ) const
  {
    return getWO( multiByteToWide( pathOrID ) );
  }

  std::wostream & FIO::getWO( wchar_t const * const & pathOrID ) const
  {
    return getWO( std::wstring( pathOrID ) );
  }

  std::wostream & FIO::getWO( char const * const & pathOrID ) const
  {
    return getWO( std::string( pathOrID ) );
  }

  /*----------------------------------------------------------------------------
   * Get a sorted listing of open input streams.
  ----------------------------------------------------------------------------*/

  std::vector< std::wstring > FIO::listOpenWIW() const
  {
    std::vector< std::wstring > streams;

    for ( auto & stream : iFSM )
    { streams.push_back( stream.first + L" | " + getPathW( stream.first ) ); }

    std::sort( streams.begin(), streams.end() );

    return streams;
  }

  std::vector< std::string > FIO::listOpenWI() const
  {
    std::vector< std::wstring > wstreams = listOpenWIW();
    std::vector< std::string > streams;

    for ( auto & stream : wstreams )
    { streams.push_back( wideToMultiByte( stream ) ); }

    return streams;
  }

  /*----------------------------------------------------------------------------
   * Get a sorted listing of open output streams.
  ----------------------------------------------------------------------------*/

  std::vector< std::wstring > FIO::listOpenWOW() const
  {
    std::vector< std::wstring > streams;

    for ( auto & stream : oFSM )
    { streams.push_back( stream.first + L" | " + getPathW( stream.first ) ); }

    std::sort( streams.begin(), streams.end() );

    return streams;
  }

  std::vector< std::string > FIO::listOpenWO() const
  {
    std::vector< std::wstring > wstreams = listOpenWOW();
    std::vector< std::string > streams;

    for ( auto & stream : wstreams )
    { streams.push_back( wideToMultiByte( stream ) ); }

    return streams;
  }

  /*----------------------------------------------------------------------------
   * Read from an input stream.
  ----------------------------------------------------------------------------*/

  FIO & FIO::readLineW( std::wstring const & pathOrID, std::wstring & dest )
  {
    std::wstring path = getPathW( pathOrID );

    std::getline( validateWIFStream( path ), dest );

    return *this;
  }

  FIO & FIO::readLineW( std::string const & pathOrID, std::wstring & dest )
  {
    return readLineW( multiByteToWide( pathOrID ), dest );
  }

  FIO & FIO::readLineW( wchar_t const * const & pathOrID, std::wstring & dest )
  {
    return readLineW( std::wstring( pathOrID ), dest );
  }

  FIO & FIO::readLineW( char const * const & pathOrID, std::wstring & dest )
  {
    return readLineW( std::string( pathOrID ), dest );
  }

  FIO & FIO::readLine( std::wstring const & pathOrID, std::string & dest )
  {
    std::wstring wLine;
    readLineW( pathOrID, wLine );

    dest = wideToMultiByte( wLine );

    return *this;
  }

  FIO & FIO::readLine( std::string const & pathOrID, std::string & dest )
  {
    return readLine( multiByteToWide( pathOrID ), dest );
  }

  FIO & FIO::readLine( wchar_t const * const & pathOrID, std::string & dest )
  {
    return readLine( std::wstring( pathOrID ), dest );
  }

  FIO & FIO::readLine( char const * const & pathOrID, std::string & dest )
  {
    return readLine( std::string( pathOrID ), dest );
  }

  std::wstring FIO::readLineW( std::wstring const & pathOrID )
  {
    std::wstring wLine;
    readLineW( pathOrID, wLine );

    return wLine;
  }

  std::wstring FIO::readLineW( std::string const & pathOrID )
  {
    return readLineW( multiByteToWide( pathOrID ) );
  }

  std::wstring FIO::readLineW( wchar_t const * const & pathOrID )
  {
    return readLineW( std::wstring( pathOrID ) );
  }

  std::wstring FIO::readLineW( char const * const & pathOrID )
  {
    return readLineW( std::string( pathOrID ) );
  }

  std::string FIO::readLine( std::wstring const & pathOrID )
  {
    return wideToMultiByte( readLineW( pathOrID ) );
  }

  std::string FIO::readLine( std::string const & pathOrID )
  {
    return wideToMultiByte( readLineW( pathOrID ) );
  }

  std::string FIO::readLine( wchar_t const * const & pathOrID )
  {
    return wideToMultiByte( readLineW( pathOrID ) );
  }

  std::string FIO::readLine( char const * const & pathOrID )
  {
    return wideToMultiByte( readLineW( pathOrID ) );
  }

  /*----------------------------------------------------------------------------
   * Write to an output stream.
  ----------------------------------------------------------------------------*/

  FIO & FIO::writeLineW(
    std::wstring const & pathOrID, std::wstring const & source )
  {
    std::wstring path = getPathW( pathOrID );

    validateWOFStream( path ) << source;

    return *this;
  }

  FIO & FIO::writeLineW(
    std::wstring const & pathOrID, std::string const & source )
  {
    return writeLineW( pathOrID, multiByteToWide( source ) );
  }

  FIO & FIO::writeLineW(
    std::wstring const & pathOrID, wchar_t const * const & source )
  {
    return writeLineW( pathOrID, std::wstring( source ) );
  }

  FIO & FIO::writeLineW(
    std::wstring const & pathOrID, char const * const & source )
  {
    return writeLineW( pathOrID, std::string( source ) );
  }

  FIO & FIO::writeLineW(
    std::string const & pathOrID, std::wstring const & source )
  {
    return writeLineW( multiByteToWide( pathOrID ), source );
  }

  FIO & FIO::writeLineW(
    std::string const & pathOrID, std::string const & source )
  {
    return writeLineW( multiByteToWide( pathOrID ), source );
  }

  FIO & FIO::writeLineW(
    std::string const & pathOrID, wchar_t const * const & source )
  {
    return writeLineW( multiByteToWide( pathOrID ), source );
  }

  FIO & FIO::writeLineW(
    std::string const & pathOrID, char const * const & source )
  {
    return writeLineW( multiByteToWide( pathOrID ), source );
  }

  FIO & FIO::writeLineW(
    wchar_t const * const & pathOrID, std::wstring const & source )
  {
    return writeLineW( std::wstring( pathOrID ), source );
  }

  FIO & FIO::writeLineW(
    wchar_t const * const & pathOrID, std::string const & source )
  {
    return writeLineW( std::wstring( pathOrID ), source );
  }

  FIO & FIO::writeLineW(
    wchar_t const * const & pathOrID, wchar_t const * const & source )
  {
    return writeLineW( std::wstring( pathOrID ), source );
  }

  FIO & FIO::writeLineW(
    wchar_t const * const & pathOrID, char const * const & source )
  {
    return writeLineW( std::wstring( pathOrID ), source );
  }

  FIO & FIO::writeLineW(
    char const * const & pathOrID, std::wstring const & source )
  {
    return writeLineW( std::string( pathOrID ), source );
  }

  FIO & FIO::writeLineW(
    char const * const & pathOrID, std::string const & source )
  {
    return writeLineW( std::string( pathOrID ), source );
  }

  FIO & FIO::writeLineW(
    char const * const & pathOrID, wchar_t const * const & source )
  {
    return writeLineW( std::string( pathOrID ), source );
  }

  FIO & FIO::writeLineW(
    char const * const & pathOrID, char const * const & source )
  {
    return writeLineW( std::string( pathOrID ), source );
  }

  FIO & FIO::writeLine(
    std::wstring const & pathOrID, std::wstring const & source )
  {
    return writeLineW( pathOrID, source );
  }

  FIO & FIO::writeLine(
    std::wstring const & pathOrID, std::string const & source )
  {
    return writeLineW( pathOrID, source );
  }

  FIO & FIO::writeLine(
    std::wstring const & pathOrID, wchar_t const * const & source )
  {
    return writeLineW( pathOrID, source );
  }

  FIO & FIO::writeLine(
    std::wstring const & pathOrID, char const * const & source )
  {
    return writeLineW( pathOrID, source );
  }

  FIO & FIO::writeLine(
    std::string const & pathOrID, std::wstring const & source )
  {
    return writeLineW( pathOrID, source );
  }

  FIO & FIO::writeLine(
    std::string const & pathOrID, std::string const & source )
  {
    return writeLineW( pathOrID, source );
  }

  FIO & FIO::writeLine(
    std::string const & pathOrID, wchar_t const * const & source )
  {
    return writeLineW( pathOrID, source );
  }

  FIO & FIO::writeLine(
    std::string const & pathOrID, char const * const & source )
  {
    return writeLineW( pathOrID, source );
  }

  FIO & FIO::writeLine(
    wchar_t const * const & pathOrID, std::wstring const & source )
  {
    return writeLineW( pathOrID, source );
  }

  FIO & FIO::writeLine(
    wchar_t const * const & pathOrID, std::string const & source )
  {
    return writeLineW( pathOrID, source );
  }

  FIO & FIO::writeLine(
    wchar_t const * const & pathOrID, wchar_t const * const & source )
  {
    return writeLineW( pathOrID, source );
  }

  FIO & FIO::writeLine(
    wchar_t const * const & pathOrID, char const * const & source )
  {
    return writeLineW( pathOrID, source );
  }

  FIO & FIO::writeLine(
    char const * const & pathOrID, std::wstring const & source )
  {
    return writeLineW( pathOrID, source );
  }

  FIO & FIO::writeLine(
    char const * const & pathOrID, std::string const & source )
  {
    return writeLineW( pathOrID, source );
  }

  FIO & FIO::writeLine(
    char const * const & pathOrID, wchar_t const * const & source )
  {
    return writeLineW( pathOrID, source );
  }

  FIO & FIO::writeLine(
    char const * const & pathOrID, char const * const & source )
  {
    return writeLineW( pathOrID, source );
  }

  /*----------------------------------------------------------------------------
   * Search for files within a directory.
  ----------------------------------------------------------------------------*/

  std::vector< std::wstring > FIO::findFilesW(
    std::wstring const & fileExtension /* = L".*" */,
    std::wstring const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    std::vector< std::wstring > dirs;
    std::wstring const rootDir = getPathW( pathOrID );

#ifdef WINDOWS

    std::wstring const path = rootDir + pathSepW + L"*.*";

    WIN32_FIND_DATAW info;

    HANDLE dirhandle = ::FindFirstFileW( path.c_str(), &info );

    if ( dirhandle != INVALID_HANDLE_VALUE )
    {
      while ( ::FindNextFileW( dirhandle, &info ) )
      {
        std::wstring const fileName = std::wstring( info.cFileName );

        if ( ! ( info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
        {
          if ( fileExtension != L".*"
            && fileName.length() > fileExtension.length()
            && fileName.substr( fileName.length() - fileExtension.length() )
              == fileExtension )
          { dirs.push_back( rootDir + pathSepW + fileName ); }

          else if ( fileExtension == L".*" )
          {
            dirs.push_back( rootDir + pathSepW + fileName );
          }
        }
        else if ( ( recursiveSearch == RecursiveSearchTrue )
          && ( info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
          && fileName != L".." && fileName != L"." )
        {
          std::vector< std::wstring > subDirs =
            findFilesW( fileExtension, rootDir + pathSepW + fileName );

          for ( auto & file : subDirs ) { dirs.push_back( file ); }
        }
      }
      ::FindClose( dirhandle );
    }
    else
    {
      throw FIOExcept( "Invalid directory handle encountered." );
    }

    return dirs;

#else
    struct dirent * dirInfo;

    DIR * dirhandle =
      static_cast< DIR * >( opendir( wideToMultiByte( rootDir ).c_str() ) );

    if ( ! dirhandle ) { return std::vector< std::wstring >(); }

    while ( ( dirInfo = readdir( dirhandle ) ) != NULL )
    {
      struct stat info;

      std::wstring fileName = multiByteToWide( std::string( dirInfo->d_name ) );

      if ( fileName == L"." || fileName == L".." ) { continue; }

      errno = 0;
      if ( stat(
             wideToMultiByte( rootDir + pathSepW + fileName ).c_str(), &info )
        < 0 )
      { perror( strerror( errno ) ); }
      else if ( S_ISREG( info.st_mode ) )
      {
        if ( fileExtension != L".*"
          && fileName.length() > fileExtension.length()
          && fileName.substr( fileName.length() - fileExtension.length() )
            == fileExtension )
        { dirs.push_back( rootDir + pathSepW + fileName ); }

        else if ( fileExtension == L".*" )
        {
          dirs.push_back( rootDir + pathSepW + fileName );
        }
      }
      else if ( recursiveSearch == RecursiveSearchTrue
        && S_ISDIR( info.st_mode ) )
      {
        std::vector< std::wstring > subDirContents =
          findFilesW( fileExtension, rootDir + pathSepW + fileName );

        for ( auto & file : subDirContents ) { dirs.push_back( file ); }
      }
    }

    closedir( dirhandle );

    return dirs;

#endif
  }

  std::vector< std::wstring > FIO::findFilesW(
    std::wstring const & fileExtension /* = L".*" */,
    std::string const & pathOrID /* = "root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFilesW(
      fileExtension, multiByteToWide( pathOrID ), recursiveSearch );
  }

  std::vector< std::wstring > FIO::findFilesW(
    std::wstring const & fileExtension /* = L".*" */,
    wchar_t const * const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFilesW(
      fileExtension, std::wstring( pathOrID ), recursiveSearch );
  }

  std::vector< std::wstring > FIO::findFilesW(
    std::wstring const & fileExtension /* = L".*" */,
    char const * const & pathOrID /* = "root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFilesW(
      fileExtension, std::string( pathOrID ), recursiveSearch );
  }

  std::vector< std::wstring > FIO::findFilesW(
    std::string const & fileExtension /* = ".*" */,
    std::wstring const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFilesW(
      multiByteToWide( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::wstring > FIO::findFilesW(
    std::string const & fileExtension /* = ".*" */,
    std::string const & pathOrID /* = "root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFilesW(
      multiByteToWide( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::wstring > FIO::findFilesW(
    std::string const & fileExtension /* = ".*" */,
    wchar_t const * const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFilesW(
      multiByteToWide( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::wstring > FIO::findFilesW(
    std::string const & fileExtension /* = ".*" */,
    char const * const & pathOrID /* = "root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFilesW(
      multiByteToWide( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::wstring > FIO::findFilesW(
    wchar_t const * const & fileExtension /* = L".*" */,
    std::wstring const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFilesW(
      std::wstring( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::wstring > FIO::findFilesW(
    wchar_t const * const & fileExtension /* = L".*" */,
    std::string const & pathOrID /* = "root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFilesW(
      std::wstring( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::wstring > FIO::findFilesW(
    wchar_t const * const & fileExtension /* = L".*" */,
    wchar_t const * const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFilesW(
      std::wstring( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::wstring > FIO::findFilesW(
    wchar_t const * const & fileExtension /* = L".*" */,
    char const * const & pathOrID /* = "root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFilesW(
      std::wstring( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::wstring > FIO::findFilesW(
    char const * const & fileExtension /* = ".*" */,
    std::wstring const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFilesW(
      std::string( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::wstring > FIO::findFilesW(
    char const * const & fileExtension /* = ".*" */,
    std::string const & pathOrID /* = "root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFilesW(
      std::string( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::wstring > FIO::findFilesW(
    char const * const & fileExtension /* = ".*" */,
    wchar_t const * const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFilesW(
      std::string( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::wstring > FIO::findFilesW(
    char const * const & fileExtension /* = ".*" */,
    char const * const & pathOrID /* = "root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFilesW(
      std::string( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::string > FIO::findFiles(
    std::wstring const & fileExtension /* = L".*" */,
    std::wstring const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    std::vector< std::wstring > foundFiles =
      findFilesW( fileExtension, pathOrID, recursiveSearch );

    std::vector< std::string > files;

    for ( auto & ws : foundFiles ) { files.push_back( wideToMultiByte( ws ) ); }

    return files;
  }

  std::vector< std::string > FIO::findFiles(
    std::wstring const & fileExtension /* = L".*" */,
    std::string const & pathOrID /* = "root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFiles(
      fileExtension, multiByteToWide( pathOrID ), recursiveSearch );
  }

  std::vector< std::string > FIO::findFiles(
    std::wstring const & fileExtension /* = L".*" */,
    wchar_t const * const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFiles(
      fileExtension, std::wstring( pathOrID ), recursiveSearch );
  }

  std::vector< std::string > FIO::findFiles(
    std::wstring const & fileExtension /* = L".*" */,
    char const * const & pathOrID /* = "root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFiles( fileExtension, std::string( pathOrID ), recursiveSearch );
  }

  std::vector< std::string > FIO::findFiles(
    std::string const & fileExtension /* = ".*" */,
    std::wstring const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFiles(
      multiByteToWide( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::string > FIO::findFiles(
    std::string const & fileExtension /* = ".*" */,
    std::string const & pathOrID /* = "root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFiles(
      multiByteToWide( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::string > FIO::findFiles(
    std::string const & fileExtension /* = ".*" */,
    wchar_t const * const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFiles(
      multiByteToWide( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::string > FIO::findFiles(
    std::string const & fileExtension /* = ".*" */,
    char const * const & pathOrID /* = "root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFiles(
      multiByteToWide( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::string > FIO::findFiles(
    wchar_t const * const & fileExtension /* = L".*" */,
    std::wstring const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFiles(
      std::wstring( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::string > FIO::findFiles(
    wchar_t const * const & fileExtension /* = L".*" */,
    std::string const & pathOrID /* = "root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFiles(
      std::wstring( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::string > FIO::findFiles(
    wchar_t const * const & fileExtension /* = L".*" */,
    wchar_t const * const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFiles(
      std::wstring( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::string > FIO::findFiles(
    wchar_t const * const & fileExtension /* = L".*" */,
    char const * const & pathOrID /* = "root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFiles(
      std::wstring( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::string > FIO::findFiles(
    char const * const & fileExtension /* = ".*" */,
    std::wstring const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFiles( std::string( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::string > FIO::findFiles(
    char const * const & fileExtension /* = ".*" */,
    std::string const & pathOrID /* = "root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFiles( std::string( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::string > FIO::findFiles(
    char const * const & fileExtension /* = ".*" */,
    wchar_t const * const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFiles( std::string( fileExtension ), pathOrID, recursiveSearch );
  }

  std::vector< std::string > FIO::findFiles(
    char const * const & fileExtension /* = ".*" */,
    char const * const & pathOrID /* = "root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    return findFiles( std::string( fileExtension ), pathOrID, recursiveSearch );
  }

  /*----------------------------------------------------------------------------
   * Read files.
  ----------------------------------------------------------------------------*/

  std::wstring FIO::readFileW( std::wstring const & pathOrID )
  {
    bool openedNew = false;
    std::wstring path = getPathW( pathOrID );

    std::wstringstream wss;

    try
    {
      validateWIFStream( path );
      wss << rewindWI( path ).rdbuf();
    }
    catch ( FIOExcept const & e )
    {
      openWI( path );
      openedNew = true;
      wss << validateWIFStream( path ).rdbuf();
    }

    if ( openedNew ) { closeWI( pathOrID ); }

    return wss.str();
  }

  std::wstring FIO::readFileW( std::string const & pathOrID )
  {
    return readFileW( multiByteToWide( pathOrID ) );
  }

  std::wstring FIO::readFileW( wchar_t const * const & pathOrID )
  {
    return readFileW( std::wstring( pathOrID ) );
  }

  std::wstring FIO::readFileW( char const * const & pathOrID )
  {
    return readFileW( std::string( pathOrID ) );
  }

  std::string FIO::readFile( std::wstring const & pathOrID )
  {
    return wideToMultiByte( readFileW( pathOrID ) );
  }

  std::string FIO::readFile( std::string const & pathOrID )
  {
    return wideToMultiByte( readFileW( pathOrID ) );
  }

  std::string FIO::readFile( wchar_t const * const & pathOrID )
  {
    return wideToMultiByte( readFileW( pathOrID ) );
  }

  std::string FIO::readFile( char const * const & pathOrID )
  {
    return wideToMultiByte( readFileW( pathOrID ) );
  }

  /*----------------------------------------------------------------------------
   * Read files into vectors.
  ----------------------------------------------------------------------------*/

  std::vector< std::wstring > FIO::readFileToVectorW(
    std::wstring const & pathOrID, std::wstring const & delim /* = L"\n\r" */ )
  {
    return splitStringW( readFileW( pathOrID ), delim );
  }

  std::vector< std::wstring > FIO::readFileToVectorW(
    std::wstring const & pathOrID, std::string const & delim /* = "\n\r" */ )
  {
    return readFileToVectorW( pathOrID, multiByteToWide( delim ) );
  }

  std::vector< std::wstring > FIO::readFileToVectorW(
    std::wstring const & pathOrID,
    wchar_t const * const & delim /* = L"\n\r" */ )
  {
    return readFileToVectorW( pathOrID, std::wstring( delim ) );
  }

  std::vector< std::wstring > FIO::readFileToVectorW(
    std::wstring const & pathOrID, char const * const & delim /* = "\n\r" */ )
  {
    return readFileToVectorW( pathOrID, std::string( delim ) );
  }

  std::vector< std::wstring > FIO::readFileToVectorW(
    std::string const & pathOrID, std::wstring const & delim /* = L"\n\r" */ )
  {
    return readFileToVectorW( multiByteToWide( pathOrID ), delim );
  }

  std::vector< std::wstring > FIO::readFileToVectorW(
    std::string const & pathOrID, std::string const & delim /* = "\n\r" */ )
  {
    return readFileToVectorW( multiByteToWide( pathOrID ), delim );
  }

  std::vector< std::wstring > FIO::readFileToVectorW(
    std::string const & pathOrID,
    wchar_t const * const & delim /* = L"\n\r" */ )
  {
    return readFileToVectorW( multiByteToWide( pathOrID ), delim );
  }

  std::vector< std::wstring > FIO::readFileToVectorW(
    std::string const & pathOrID, char const * const & delim /* = "\n\r" */ )
  {
    return readFileToVectorW( multiByteToWide( pathOrID ), delim );
  }

  std::vector< std::wstring > FIO::readFileToVectorW(
    wchar_t const * const & pathOrID,
    std::wstring const & delim /* = L"\n\r" */ )
  {
    return readFileToVectorW( std::wstring( pathOrID ), delim );
  }

  std::vector< std::wstring > FIO::readFileToVectorW(
    wchar_t const * const & pathOrID, std::string const & delim /* = "\n\r" */ )
  {
    return readFileToVectorW( std::wstring( pathOrID ), delim );
  }

  std::vector< std::wstring > FIO::readFileToVectorW(
    wchar_t const * const & pathOrID,
    wchar_t const * const & delim /* = L"\n\r" */ )
  {
    return readFileToVectorW( std::wstring( pathOrID ), delim );
  }

  std::vector< std::wstring > FIO::readFileToVectorW(
    wchar_t const * const & pathOrID,
    char const * const & delim /* = "\n\r" */ )
  {
    return readFileToVectorW( std::wstring( pathOrID ), delim );
  }

  std::vector< std::wstring > FIO::readFileToVectorW(
    char const * const & pathOrID, std::wstring const & delim /* = L"\n\r" */ )
  {
    return readFileToVectorW( std::string( pathOrID ), delim );
  }

  std::vector< std::wstring > FIO::readFileToVectorW(
    char const * const & pathOrID, std::string const & delim /* = "\n\r" */ )
  {
    return readFileToVectorW( std::string( pathOrID ), delim );
  }

  std::vector< std::wstring > FIO::readFileToVectorW(
    char const * const & pathOrID,
    wchar_t const * const & delim /* = L"\n\r" */ )
  {
    return readFileToVectorW( std::string( pathOrID ), delim );
  }

  std::vector< std::wstring > FIO::readFileToVectorW(
    char const * const & pathOrID, char const * const & delim /* = "\n\r" */ )
  {
    return readFileToVectorW( std::string( pathOrID ), delim );
  }

  std::vector< std::string > FIO::readFileToVector(
    std::wstring const & pathOrID, std::wstring const & delim /* = L"\n\r" */ )
  {
    std::string contents = readFile( pathOrID );

    return splitString( contents, delim );
  }

  std::vector< std::string > FIO::readFileToVector(
    std::wstring const & pathOrID, std::string const & delim /* = "\n\r" */ )
  {
    return readFileToVector( pathOrID, multiByteToWide( delim ) );
  }

  std::vector< std::string > FIO::readFileToVector(
    std::wstring const & pathOrID,
    wchar_t const * const & delim /* = L"\n\r" */ )
  {
    return readFileToVector( pathOrID, std::wstring( delim ) );
  }

  std::vector< std::string > FIO::readFileToVector(
    std::wstring const & pathOrID, char const * const & delim /* = "\n\r" */ )
  {
    return readFileToVector( pathOrID, std::string( delim ) );
  }

  std::vector< std::string > FIO::readFileToVector(
    std::string const & pathOrID, std::wstring const & delim /* = L"\n\r" */ )
  {
    return readFileToVector( multiByteToWide( pathOrID ), delim );
  }

  std::vector< std::string > FIO::readFileToVector(
    std::string const & pathOrID, std::string const & delim /* = "\n\r" */ )
  {
    return readFileToVector( multiByteToWide( pathOrID ), delim );
  }

  std::vector< std::string > FIO::readFileToVector(
    std::string const & pathOrID,
    wchar_t const * const & delim /* = L"\n\r" */ )
  {
    return readFileToVector( multiByteToWide( pathOrID ), delim );
  }

  std::vector< std::string > FIO::readFileToVector(
    std::string const & pathOrID, char const * const & delim /* = "\n\r" */ )
  {
    return readFileToVector( multiByteToWide( pathOrID ), delim );
  }

  std::vector< std::string > FIO::readFileToVector(
    wchar_t const * const & pathOrID,
    std::wstring const & delim /* = L"\n\r" */ )
  {
    return readFileToVector( std::wstring( pathOrID ), delim );
  }

  std::vector< std::string > FIO::readFileToVector(
    wchar_t const * const & pathOrID, std::string const & delim /* = "\n\r" */ )
  {
    return readFileToVector( std::wstring( pathOrID ), delim );
  }

  std::vector< std::string > FIO::readFileToVector(
    wchar_t const * const & pathOrID,
    wchar_t const * const & delim /* = L"\n\r" */ )
  {
    return readFileToVector( std::wstring( pathOrID ), delim );
  }

  std::vector< std::string > FIO::readFileToVector(
    wchar_t const * const & pathOrID,
    char const * const & delim /* = "\n\r" */ )
  {
    return readFileToVector( std::wstring( pathOrID ), delim );
  }

  std::vector< std::string > FIO::readFileToVector(
    char const * const & pathOrID, std::wstring const & delim /* = L"\n\r" */ )
  {
    return readFileToVector( std::string( pathOrID ), delim );
  }

  std::vector< std::string > FIO::readFileToVector(
    char const * const & pathOrID, std::string const & delim /* = "\n\r" */ )
  {
    return readFileToVector( std::string( pathOrID ), delim );
  }

  std::vector< std::string > FIO::readFileToVector(
    char const * const & pathOrID,
    wchar_t const * const & delim /* = L"\n\r" */ )
  {
    return readFileToVector( std::string( pathOrID ), delim );
  }

  std::vector< std::string > FIO::readFileToVector(
    char const * const & pathOrID, char const * const & delim /* = "\n\r" */ )
  {
    return readFileToVector( std::string( pathOrID ), delim );
  }

  /*----------------------------------------------------------------------------
   * Read files into vectors of vectors.
  ----------------------------------------------------------------------------*/

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::wstring const & pathOrID, std::wstring const & lineDelim /* = L"," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    std::vector< std::vector< std::wstring > > ret;
    std::vector< std::wstring > lines =
      readFileToVectorW( pathOrID, vertDelim );

    for ( auto & line : lines )
    {
      std::vector< std::wstring > vLine = splitStringW( line, lineDelim );

      if ( vLine.size() > 0 ) { ret.push_back( vLine ); }
    }

    return ret;
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::wstring const & pathOrID, std::wstring const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrixW(
      pathOrID, lineDelim, multiByteToWide( vertDelim ) );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::wstring const & pathOrID, std::wstring const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( pathOrID, lineDelim, std::wstring( vertDelim ) );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::wstring const & pathOrID, std::wstring const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( pathOrID, lineDelim, std::string( vertDelim ) );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::wstring const & pathOrID, std::string const & lineDelim /* = "," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW(
      pathOrID, multiByteToWide( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::wstring const & pathOrID, std::string const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrixW(
      pathOrID, multiByteToWide( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::wstring const & pathOrID, std::string const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW(
      pathOrID, multiByteToWide( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::wstring const & pathOrID, std::string const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW(
      pathOrID, multiByteToWide( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::wstring const & pathOrID,
    wchar_t const * const & lineDelim /* = L"," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( pathOrID, std::wstring( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::wstring const & pathOrID,
    wchar_t const * const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrixW( pathOrID, std::wstring( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::wstring const & pathOrID,
    wchar_t const * const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( pathOrID, std::wstring( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::wstring const & pathOrID,
    wchar_t const * const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( pathOrID, std::wstring( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::wstring const & pathOrID, char const * const & lineDelim /* = "," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( pathOrID, std::string( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::wstring const & pathOrID, char const * const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrixW( pathOrID, std::string( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::wstring const & pathOrID, char const * const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( pathOrID, std::string( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::wstring const & pathOrID, char const * const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( pathOrID, std::string( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::string const & pathOrID, std::wstring const & lineDelim /* = L"," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::string const & pathOrID, std::wstring const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrixW(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::string const & pathOrID, std::wstring const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::string const & pathOrID, std::wstring const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::string const & pathOrID, std::string const & lineDelim /* = "," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::string const & pathOrID, std::string const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrixW(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::string const & pathOrID, std::string const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::string const & pathOrID, std::string const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::string const & pathOrID,
    wchar_t const * const & lineDelim /* = L"," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::string const & pathOrID, wchar_t const * const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrixW(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::string const & pathOrID, wchar_t const * const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::string const & pathOrID,
    wchar_t const * const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::string const & pathOrID, char const * const & lineDelim /* = "," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::string const & pathOrID, char const * const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrixW(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::string const & pathOrID, char const * const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    std::string const & pathOrID, char const * const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    wchar_t const * const & pathOrID,
    std::wstring const & lineDelim /* = L"," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    wchar_t const * const & pathOrID,
    std::wstring const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrixW( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    wchar_t const * const & pathOrID,
    std::wstring const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    wchar_t const * const & pathOrID,
    std::wstring const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    wchar_t const * const & pathOrID, std::string const & lineDelim /* = "," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    wchar_t const * const & pathOrID, std::string const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrixW( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    wchar_t const * const & pathOrID, std::string const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    wchar_t const * const & pathOrID,
    std::string const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    wchar_t const * const & pathOrID,
    wchar_t const * const & lineDelim /* = L"," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    wchar_t const * const & pathOrID,
    wchar_t const * const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrixW( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    wchar_t const * const & pathOrID,
    wchar_t const * const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    wchar_t const * const & pathOrID,
    wchar_t const * const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    wchar_t const * const & pathOrID,
    char const * const & lineDelim /* = "," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    wchar_t const * const & pathOrID,
    char const * const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrixW( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    wchar_t const * const & pathOrID,
    char const * const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    wchar_t const * const & pathOrID,
    char const * const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    char const * const & pathOrID, std::wstring const & lineDelim /* = L"," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    char const * const & pathOrID, std::wstring const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrixW( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    char const * const & pathOrID, std::wstring const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    char const * const & pathOrID, std::wstring const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    char const * const & pathOrID, std::string const & lineDelim /* = "," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    char const * const & pathOrID, std::string const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrixW( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    char const * const & pathOrID, std::string const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    char const * const & pathOrID, std::string const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    char const * const & pathOrID,
    wchar_t const * const & lineDelim /* = L"," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    char const * const & pathOrID,
    wchar_t const * const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrixW( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    char const * const & pathOrID,
    wchar_t const * const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    char const * const & pathOrID,
    wchar_t const * const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    char const * const & pathOrID, char const * const & lineDelim /* = "," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    char const * const & pathOrID, char const * const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrixW( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    char const * const & pathOrID, char const * const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    char const * const & pathOrID, char const * const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrixW( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::wstring const & pathOrID, std::wstring const & lineDelim /* = L"," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    auto wMatrix = readFileToMatrixW( pathOrID, lineDelim, vertDelim );
    std::vector< std::vector< std::string > > ret;

    for ( auto & line : wMatrix )
    {
      std::vector< std::string > vLine;
      for ( auto & ws : line ) { vLine.push_back( wideToMultiByte( ws ) ); }
      ret.push_back( vLine );
    }

    return ret;
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::wstring const & pathOrID, std::wstring const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrix(
      pathOrID, lineDelim, multiByteToWide( vertDelim ) );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::wstring const & pathOrID, std::wstring const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( pathOrID, lineDelim, std::wstring( vertDelim ) );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::wstring const & pathOrID, std::wstring const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( pathOrID, lineDelim, std::string( vertDelim ) );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::wstring const & pathOrID, std::string const & lineDelim /* = "," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix(
      pathOrID, multiByteToWide( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::wstring const & pathOrID, std::string const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrix(
      pathOrID, multiByteToWide( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::wstring const & pathOrID, std::string const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix(
      pathOrID, multiByteToWide( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::wstring const & pathOrID, std::string const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix(
      pathOrID, multiByteToWide( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::wstring const & pathOrID,
    wchar_t const * const & lineDelim /* = L"," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( pathOrID, std::wstring( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::wstring const & pathOrID,
    wchar_t const * const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrix( pathOrID, std::wstring( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::wstring const & pathOrID,
    wchar_t const * const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( pathOrID, std::wstring( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::wstring const & pathOrID,
    wchar_t const * const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( pathOrID, std::wstring( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::wstring const & pathOrID, char const * const & lineDelim /* = "," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( pathOrID, std::string( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::wstring const & pathOrID, char const * const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrix( pathOrID, std::string( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::wstring const & pathOrID, char const * const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( pathOrID, std::string( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::wstring const & pathOrID, char const * const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( pathOrID, std::string( lineDelim ), vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::string const & pathOrID, std::wstring const & lineDelim /* = L"," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::string const & pathOrID, std::wstring const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrix(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::string const & pathOrID, std::wstring const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::string const & pathOrID, std::wstring const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::string const & pathOrID, std::string const & lineDelim /* = "," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::string const & pathOrID, std::string const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrix(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::string const & pathOrID, std::string const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::string const & pathOrID, std::string const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::string const & pathOrID,
    wchar_t const * const & lineDelim /* = L"," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::string const & pathOrID, wchar_t const * const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrix(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::string const & pathOrID, wchar_t const * const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::string const & pathOrID,
    wchar_t const * const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::string const & pathOrID, char const * const & lineDelim /* = "," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::string const & pathOrID, char const * const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrix(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::string const & pathOrID, char const * const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    std::string const & pathOrID, char const * const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix(
      multiByteToWide( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    wchar_t const * const & pathOrID,
    std::wstring const & lineDelim /* = L"," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    wchar_t const * const & pathOrID,
    std::wstring const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrix( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    wchar_t const * const & pathOrID,
    std::wstring const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    wchar_t const * const & pathOrID,
    std::wstring const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    wchar_t const * const & pathOrID, std::string const & lineDelim /* = "," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    wchar_t const * const & pathOrID, std::string const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrix( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    wchar_t const * const & pathOrID, std::string const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    wchar_t const * const & pathOrID,
    std::string const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    wchar_t const * const & pathOrID,
    wchar_t const * const & lineDelim /* = L"," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    wchar_t const * const & pathOrID,
    wchar_t const * const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrix( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    wchar_t const * const & pathOrID,
    wchar_t const * const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    wchar_t const * const & pathOrID,
    wchar_t const * const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    wchar_t const * const & pathOrID,
    char const * const & lineDelim /* = "," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    wchar_t const * const & pathOrID,
    char const * const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrix( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    wchar_t const * const & pathOrID,
    char const * const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    wchar_t const * const & pathOrID,
    char const * const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::wstring( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    char const * const & pathOrID, std::wstring const & lineDelim /* = L"," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    char const * const & pathOrID, std::wstring const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrix( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    char const * const & pathOrID, std::wstring const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    char const * const & pathOrID, std::wstring const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    char const * const & pathOrID, std::string const & lineDelim /* = "," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    char const * const & pathOrID, std::string const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrix( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    char const * const & pathOrID, std::string const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    char const * const & pathOrID, std::string const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    char const * const & pathOrID,
    wchar_t const * const & lineDelim /* = L"," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    char const * const & pathOrID,
    wchar_t const * const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrix( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    char const * const & pathOrID,
    wchar_t const * const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    char const * const & pathOrID,
    wchar_t const * const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    char const * const & pathOrID, char const * const & lineDelim /* = "," */,
    std::wstring const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    char const * const & pathOrID, char const * const & lineDelim /* = L","*/,
    std::string const & vertDelim /* = "\n\r" */ )
  {
    return readFileToMatrix( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    char const * const & pathOrID, char const * const & lineDelim /* = L","*/,
    wchar_t const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::string( pathOrID ), lineDelim, vertDelim );
  }

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    char const * const & pathOrID, char const * const & lineDelim /* = L"," */,
    char const * const & vertDelim /* = L"\n\r" */ )
  {
    return readFileToMatrix( std::string( pathOrID ), lineDelim, vertDelim );
  }

  /*----------------------------------------------------------------------------
   * Change the FIO root directory.
  ----------------------------------------------------------------------------*/

  FIO & FIO::setRoot( std::wstring const & pathOrID )
  {
    return setDirID( L"root", pathOrID );
  }

  FIO & FIO::setRoot( std::string const & pathOrID )
  {
    return setRoot( multiByteToWide( pathOrID ) );
  }

  FIO & FIO::setRoot( wchar_t const * const & pathOrID )
  {
    return setRoot( std::wstring( pathOrID ) );
  }

  FIO & FIO::setRoot( char const * const & pathOrID )
  {
    return setRoot( std::string( pathOrID ) );
  }

  /*----------------------------------------------------------------------------
   * Store a file path in the FIO path map.
  ----------------------------------------------------------------------------*/

  FIO & FIO::setFileID( std::wstring const & id, std::wstring const & path )
  {
    errno = 0;

#ifdef WINDOWS
    struct _stat info;

    if ( _wstat( path.c_str(), &info ) == 0 )

#else
    struct stat info;

    if ( stat( wideToMultiByte( path ).c_str(), &info ) == 0 )

#endif

    {
      if ( info.st_mode & S_IFREG ) { PM[ id ] = path; }
      else
      {
        throw( FIOExcept( L"Cannot set id " + id + L", as " + path
          + L" does not point to a regular file." ) );
      }
    }
    else
    {
      perror( strerror( errno ) );
    }

    return *this;
  }

  FIO & FIO::setFileID( std::wstring const & id, std::string const & path )
  {
    return setFileID( id, multiByteToWide( path ) );
  }

  FIO & FIO::setFileID( std::wstring const & id, wchar_t const * const & path )
  {
    return setFileID( id, std::wstring( path ) );
  }

  FIO & FIO::setFileID( std::wstring const & id, char const * const & path )
  {
    return setFileID( id, std::string( path ) );
  }

  FIO & FIO::setFileID( std::string const & id, std::wstring const & path )
  {
    return setFileID( multiByteToWide( id ), path );
  }

  FIO & FIO::setFileID( std::string const & id, std::string const & path )
  {
    return setFileID( multiByteToWide( id ), path );
  }

  FIO & FIO::setFileID( std::string const & id, wchar_t const * const & path )
  {
    return setFileID( multiByteToWide( id ), path );
  }

  FIO & FIO::setFileID( std::string const & id, char const * const & path )
  {
    return setFileID( multiByteToWide( id ), path );
  }

  FIO & FIO::setFileID( wchar_t const * const & id, std::wstring const & path )
  {
    return setFileID( std::wstring( id ), path );
  }

  FIO & FIO::setFileID( wchar_t const * const & id, std::string const & path )
  {
    return setFileID( std::wstring( id ), path );
  }

  FIO & FIO::setFileID(
    wchar_t const * const & id, wchar_t const * const & path )
  {
    return setFileID( std::wstring( id ), path );
  }

  FIO & FIO::setFileID( wchar_t const * const & id, char const * const & path )
  {
    return setFileID( std::wstring( id ), path );
  }

  FIO & FIO::setFileID( char const * const & id, std::wstring const & path )
  {
    return setFileID( std::string( id ), path );
  }

  FIO & FIO::setFileID( char const * const & id, std::string const & path )
  {
    return setFileID( std::string( id ), path );
  }

  FIO & FIO::setFileID( char const * const & id, wchar_t const * const & path )
  {
    return setFileID( std::string( id ), path );
  }

  FIO & FIO::setFileID( char const * const & id, char const * const & path )
  {
    return setFileID( std::string( id ), path );
  }

  /*----------------------------------------------------------------------------
   * Store a directory path in the FIO path map.
  ----------------------------------------------------------------------------*/

  FIO & FIO::setDirID( std::wstring const & id, std::wstring const & path )
  {
    errno = 0;

#ifdef WINDOWS

    struct _stat info;

    if ( _wstat( path.c_str(), &info ) == 0 )

#else
    struct stat info;

    if ( stat( wideToMultiByte( path ).c_str(), &info ) == 0 )

#endif

    {
      if ( info.st_mode & S_IFDIR ) { PM[ id ] = path; }
      else
      {
        throw( FIOExcept( L"Cannot set id " + id + L", as " + path
          + L" does not point to a directory." ) );
      }
    }
    else
    {
      perror( strerror( errno ) );
    }

    return *this;
  }

  FIO & FIO::setDirID( std::wstring const & id, std::string const & path )
  {
    return setDirID( id, multiByteToWide( path ) );
  }

  FIO & FIO::setDirID( std::wstring const & id, wchar_t const * const & path )
  {
    return setDirID( id, std::wstring( path ) );
  }

  FIO & FIO::setDirID( std::wstring const & id, char const * const & path )
  {
    return setDirID( id, std::string( path ) );
  }

  FIO & FIO::setDirID( std::string const & id, std::wstring const & path )
  {
    return setDirID( multiByteToWide( id ), path );
  }

  FIO & FIO::setDirID( std::string const & id, std::string const & path )
  {
    return setDirID( multiByteToWide( id ), path );
  }

  FIO & FIO::setDirID( std::string const & id, wchar_t const * const & path )
  {
    return setDirID( multiByteToWide( id ), path );
  }

  FIO & FIO::setDirID( std::string const & id, char const * const & path )
  {
    return setDirID( multiByteToWide( id ), path );
  }

  FIO & FIO::setDirID( wchar_t const * const & id, std::wstring const & path )
  {
    return setDirID( std::wstring( id ), path );
  }

  FIO & FIO::setDirID( wchar_t const * const & id, std::string const & path )
  {
    return setDirID( std::wstring( id ), path );
  }

  FIO & FIO::setDirID(
    wchar_t const * const & id, wchar_t const * const & path )
  {
    return setDirID( std::wstring( id ), path );
  }

  FIO & FIO::setDirID( wchar_t const * const & id, char const * const & path )
  {
    return setDirID( std::wstring( id ), path );
  }

  FIO & FIO::setDirID( char const * const & id, std::wstring const & path )
  {
    return setDirID( std::string( id ), path );
  }

  FIO & FIO::setDirID( char const * const & id, std::string const & path )
  {
    return setDirID( std::string( id ), path );
  }

  FIO & FIO::setDirID( char const * const & id, wchar_t const * const & path )
  {
    return setDirID( std::string( id ), path );
  }

  FIO & FIO::setDirID( char const * const & id, char const * const & path )
  {
    return setDirID( std::string( id ), path );
  }

  /*----------------------------------------------------------------------------
   * Retrieve path to the root directory.
  ----------------------------------------------------------------------------*/

  std::wstring FIO::getRootW() const { return getPathW( L"root" ); }

  std::string FIO::getRoot() const { return getPath( L"root" ); }

  /*----------------------------------------------------------------------------
   * Retrieve a file system path.
  ----------------------------------------------------------------------------*/

  std::wstring FIO::getPathW( std::wstring const & pathOrID ) const
  {
#ifdef WINDOWS

    struct _stat info;

    if ( _wstat( pathOrID.c_str(), &info ) == 0

#else
    struct stat info;

    if ( stat( wideToMultiByte( pathOrID ).c_str(), &info ) == 0

#endif

      && ( info.st_mode & S_IFREG || info.st_mode & S_IFDIR ) )
    {
      return pathOrID;
    }

    auto it = PM.find( pathOrID );

    if ( it == PM.end() )
    {
      throw( FIOExcept( L"Path cannot be retrieved as " + pathOrID
        + L" is neither a path to a file or directory, "
        + L"nor points to a path stored in the FIO path map." ) );
    }

    return it->second;
  }

  std::wstring FIO::getPathW( std::string const & pathOrID ) const
  {
    return getPathW( multiByteToWide( pathOrID ) );
  }

  std::wstring FIO::getPathW( wchar_t const * const & pathOrID ) const
  {
    return getPathW( std::wstring( pathOrID ) );
  }

  std::wstring FIO::getPathW( char const * const & pathOrID ) const
  {
    return getPathW( std::string( pathOrID ) );
  }

  std::string FIO::getPath( std::wstring const & pathOrID ) const
  {
    return wideToMultiByte( getPathW( pathOrID ) );
  }

  std::string FIO::getPath( std::string const & pathOrID ) const
  {
    return getPath( multiByteToWide( pathOrID ) );
  }

  std::string FIO::getPath( wchar_t const * const & pathOrID ) const
  {
    return getPath( std::wstring( pathOrID ) );
  }

  std::string FIO::getPath( char const * const & pathOrID ) const
  {
    return getPath( std::string( pathOrID ) );
  }

  /*----------------------------------------------------------------------------
   * Delete a filepath from filepaths.
  ----------------------------------------------------------------------------*/

  FIO & FIO::removePath( std::wstring const & id )
  {
    if ( PM.find( id ) != PM.end() ) { PM.erase( id ); }
    else
    {
      throw( FIOExcept( L"Path cannot be removed as " + id
        + +L" does not point to a path stored in the FIO pathmap." ) );
    }

    return *this;
  }

  FIO & FIO::removePath( std::string const & id )
  {
    return removePath( multiByteToWide( id ) );
  }

  FIO & FIO::removePath( wchar_t const * const & id )
  {
    return removePath( std::wstring( id ) );
  }

  FIO & FIO::removePath( char const * const & id )
  {
    return removePath( std::string( id ) );
  }

  /*----------------------------------------------------------------------------
   * Construct FIO.
  ----------------------------------------------------------------------------*/

  FIO::FIO( std::wstring const & loc /* = L"en_US.UTF-8" */ ) :
    FIO( wideToMultiByte( loc ) )
  {
  }

  FIO::FIO( std::string const & loc /* = "en_US.UTF-8" */ ) : FIO( loc.c_str() )
  {
  }

  FIO::FIO( wchar_t const * const & loc /* = L"en_US.UTF-8" */ ) :
    FIO( std::wstring( loc ) )
  {
  }

  FIO::FIO( char const * const & loc /* = "en_US.UTF-8" */ )
  {
    setlocale( LC_ALL, loc );
    setRoot( findRoot() );
  }

  /*----------------------------------------------------------------------------
   * Find root dir.
  ----------------------------------------------------------------------------*/

  std::wstring FIO::findRoot() const
  {
#ifdef WINDOWS

    wchar_t buffer[ FILENAME_MAX ];

    GetModuleFileNameW( NULL, buffer, FILENAME_MAX );

    return parentDirW( buffer );
#else

    char buffer[ FILENAME_MAX ];

    getcwd( buffer, FILENAME_MAX );

    return multiByteToWide( std::string( buffer ) );
#endif
  }

  std::wistream & FIO::validateWIFStream( std::wstring const & id ) const
  {
    auto it = iFSM.find( id );

    if ( it == iFSM.end() )
    { throw( FIOExcept( L"No output stream exists at " + id ) ); }

    if ( it->second->good() & it->second->is_open() )
    { return *( static_cast< std::wistream * >( &( *it->second ) ) ); }
    else
    {
      throw( FIOExcept(
        L"Output stream exists at " + id + L", but cannot be read!" ) );
    }
  }

  std::wostream & FIO::validateWOFStream( std::wstring const & id ) const
  {
    auto it = oFSM.find( id );

    if ( it == oFSM.end() )
    { throw( FIOExcept( L"No output stream exists at " + id ) ); }

    if ( it->second->good() & it->second->is_open() )
    { return *( static_cast< std::wostream * >( &( *it->second ) ) ); }
    else
    {
      throw( FIOExcept(
        L"Output stream exists at " + id + L", but cannot be read!" ) );
    }
  }

} // namespace NSFIO