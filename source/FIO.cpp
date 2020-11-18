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
   * Wide String and Narrow String conversion wrapper.
  ----------------------------------------------------------------------------*/

  FIOString::FIOString( FIOString const & fs ) : str( fs.str ), strW( fs.strW )
  {
  }

  FIOString & FIOString::operator=( FIOString const & fs )
  {
    str = fs.str;
    strW = fs.strW;
    return *this;
  }

  FIOString::FIOString( std::string const & s ) : str( s ), strW( mB2w( s ) ) {}

  FIOString & FIOString::operator=( std::string const & s )
  {
    str = s;
    strW = mB2w( s );
    return *this;
  }

  FIOString::FIOString( char const * const & c ) : FIOString( std::string( c ) )
  {
  }

  FIOString & FIOString::operator=( char const * const & c )
  {
    return operator=( std::string( c ) );
  }

  FIOString::FIOString( std::wstring const & ws ) :
    str( w2mB( ws ) ), strW( ws )
  {
  }

  FIOString & FIOString::operator=( std::wstring const & ws )
  {
    str = w2mB( ws );
    strW = ws;
    return *this;
  }

  FIOString::FIOString( wchar_t const * const & wc ) :
    FIOString( std::wstring( wc ) )
  {
  }

  FIOString & FIOString::operator=( wchar_t const * const & wc )
  {
    return operator=( std::wstring( wc ) );
  }

  FIOString::operator std::string() const { return str; }

  FIOString::operator std::wstring() const { return strW; }

  FIOString::operator std::string &() { return str; }

  FIOString::operator std::wstring &() { return strW; }

  std::string FIOString::w2mB( std::wstring const & ws ) const
  {
    std::string mb;
    std::string buff( MB_CUR_MAX, '\0' );

    for ( wchar_t const & wc : ws )
    {
      int mbCharLen = std::wctomb( &buff[ 0 ], wc );

      if ( mbCharLen < 1 ) { break; }

      for ( int i = 0; i < mbCharLen; ++i ) { mb += buff[ i ]; }
    }

    return mb;
  }

  std::wstring FIOString::mB2w( std::string const & mb ) const
  {
    std::wstring ws( mb.size(), L' ' );
    ws.resize( std::mbstowcs( &ws[ 0 ], mb.c_str(), mb.size() ) );

    return ws;
  }

  /*----------------------------------------------------------------------------
   * Ancillary Static Functions.
  ----------------------------------------------------------------------------*/

  /*----------------------------------------------------------------------------
   * Filepath manipulation.
  ----------------------------------------------------------------------------*/

  std::wstring baseFileW( FIOString const & pathToFile,
    bool const & stripExtension /* = StripExtensionTrue */ )
  {
    std::wstring pathToFileW = pathToFile;
    std::wstring ws = pathToFileW.substr( parentDirW( pathToFile ).size() );

    if ( stripExtension == StripExtensionTrue )
    {
      std::size_t const delim = ws.find_last_of( L"." );

      ws = delim != std::string::npos ? ws.substr( 0, delim ) : ws;
    }

    return ws;
  }

  std::string baseFile( FIOString const & pathToFile,
    bool const & stripExtension /* = StripExtensionTrue */ )
  {
    return FIOString( baseFileW( pathToFile, stripExtension ) );
  }

  /*----------------------------------------------------------------------------
   * Get the parent directory of a filepath.
  ----------------------------------------------------------------------------*/

  std::wstring parentDirW( FIOString const & path )
  {
    std::wstring ws = path;
    std::size_t delim = ws.find_last_of( L"/\\" );

    return ws.substr( 0, ++delim );
  }

  std::string parentDir( FIOString const & path )
  {
    return FIOString( parentDirW( path ) );
  }

  /*----------------------------------------------------------------------------
   * Split strings. (CSV)
  ----------------------------------------------------------------------------*/

  std::vector< std::wstring > splitStringW(
    FIOString const & source, FIOString const & delim /* = L"\n\r" */ )
  {
    std::size_t i = 0;
    std::wstring str = source;
    std::wstring del = delim;
    std::vector< std::wstring > ret;

    while ( ( i = str.find_first_of( del ) ) != std::string::npos )
    {
      std::wstring substr = str.substr( 0, i );

      if ( substr.find_first_not_of( del ) != std::string::npos )
      { ret.push_back( substr ); }

      str.erase( 0, i + 1 );
    }

    if ( str.find_first_not_of( del ) != std::string::npos )
    { ret.push_back( str ); }

    return ret;
  }

  std::vector< std::string > splitString(
    FIOString const & source, FIOString const & del /* = L"\n\r" */ )
  {
    auto v = splitStringW( source, del );
    std::vector< std::string > ret;

    for ( auto & ws : v ) { ret.push_back( FIOString( ws ) ); }

    return ret;
  }

  /*----------------------------------------------------------------------------
   * FIO exception class.
  ----------------------------------------------------------------------------*/

  FIOExcept::FIOExcept( FIOString const & output )
  {
    std::string s = output;
    _output = s;
  };

  FIOExcept::~FIOExcept() throw() {};

  char const * FIOExcept::what() const throw() { return _output.c_str(); };

  /*----------------------------------------------------------------------------
   * FIO.
  ----------------------------------------------------------------------------*/

  std::unique_ptr< FIO > FIO::instance = std::unique_ptr< FIO >( nullptr );

  /*----------------------------------------------------------------------------
   * Construct FIO Singleton.
  ----------------------------------------------------------------------------*/

  FIO & FIO::getInstance( FIOString const & loc /* = L"en_US.UTF-8" */ )
  {
    if ( ! instance ) { instance = std::unique_ptr< FIO >( new FIO( loc ) ); }

    return *instance;
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

  std::wistream & FIO::openWI( FIOString const & pathOrID )
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

  /*----------------------------------------------------------------------------
   * Open an output stream.
  ----------------------------------------------------------------------------*/

  std::wostream & FIO::openWO(
    FIOString const & pathOrID, bool const & appendToFile /* = OpenNewFile */ )
  {
    std::wstring pathOrIDW = pathOrID;
    std::wstring path = pathOrID;

    if ( PM.find( pathOrIDW ) != PM.end() ) { path = getPathW( pathOrID ); }

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

  /*----------------------------------------------------------------------------
   * Rewind an input stream.
  ----------------------------------------------------------------------------*/

  std::wistream & FIO::rewindWI( FIOString const & pathOrID )
  {
    std::wstring path = getPathW( pathOrID );

    auto & stream = validateWIFStream( path );

    stream.clear();
    stream.seekg( 0, std::ios::beg );

    return stream;
  }

  /*----------------------------------------------------------------------------
   * Close an input stream.
  ----------------------------------------------------------------------------*/

  FIO & FIO::closeWI( FIOString const & pathOrID )
  {
    std::wstring path = getPathW( pathOrID );

    auto it = iFSM.find( path );
    validateWIFStream( path );

    it->second->close();
    iFSM.erase( path );

    return *this;
  }

  /*----------------------------------------------------------------------------
   * Close an output stream.
  ----------------------------------------------------------------------------*/
  FIO & FIO::closeWO( FIOString const & pathOrID )
  {
    std::wstring path = getPathW( pathOrID );

    auto it = oFSM.find( path );
    validateWOFStream( path );

    it->second->close();
    oFSM.erase( path );

    return *this;
  }

  /*----------------------------------------------------------------------------
   * Get an open input stream.
  ----------------------------------------------------------------------------*/

  std::wistream & FIO::getWI( FIOString const & pathOrID ) const
  {
    std::wstring path = getPathW( pathOrID );

    return validateWIFStream( path );
  }

  /*----------------------------------------------------------------------------
   * Get an open output stream.
  ----------------------------------------------------------------------------*/

  std::wostream & FIO::getWO( FIOString const & pathOrID ) const
  {
    std::wstring path = getPathW( pathOrID );

    return validateWOFStream( path );
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
    { streams.push_back( FIOString( stream ) ); }

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
    { streams.push_back( FIOString( stream ) ); }

    return streams;
  }

  /*----------------------------------------------------------------------------
   * Read from an input stream.
  ----------------------------------------------------------------------------*/

  std::wstring FIO::readLineW( FIOString const & pathOrID )
  {
    std::wstring path = getPathW( pathOrID );
    std::wstring ws;

    std::getline( validateWIFStream( path ), ws );

    return ws;
  }

  std::string FIO::readLine( FIOString const & pathOrID )
  {
    return FIOString( readLineW( pathOrID ) );
  }

  /*----------------------------------------------------------------------------
   * Write to an output stream.
  ----------------------------------------------------------------------------*/

  FIO & FIO::writeLineW( FIOString const & pathOrID, FIOString const & source )
  {
    std::wstring path = getPathW( pathOrID );
    std::wstring src = source;

    validateWOFStream( path ) << src;

    return *this;
  }

  FIO & FIO::writeLine( FIOString const & pathOrID, FIOString const & source )
  {
    return writeLineW( pathOrID, source );
  }

  /*----------------------------------------------------------------------------
   * Search for files within a directory.
  ----------------------------------------------------------------------------*/

  std::vector< std::wstring > FIO::findFilesW(
    FIOString const & fileExtension /* = L".*" */,
    FIOString const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    std::vector< std::wstring > dirs;
    std::wstring const rootDir = getPathW( pathOrID );
    std::wstring fExt = fileExtension;

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
          if ( fExt != L".*" && fileName.length() > fExt.length()
            && fileName.substr( fileName.length() - fExt.length() ) == fExt )
          { dirs.push_back( rootDir + pathSepW + fileName ); }

          else if ( fExt == L".*" )
          {
            dirs.push_back( rootDir + pathSepW + fileName );
          }
        }
        else if ( ( recursiveSearch == RecursiveSearchTrue )
          && ( info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
          && fileName != L".." && fileName != L"." )
        {
          std::vector< std::wstring > subDirs =
            findFilesW( fExt, rootDir + pathSepW + fileName );

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

    std::string rootDirS = FIOString( rootDir );

    DIR * dirhandle = static_cast< DIR * >( opendir( rootDirS.c_str() ) );

    if ( ! dirhandle ) { return std::vector< std::wstring >(); }

    while ( ( dirInfo = readdir( dirhandle ) ) != NULL )
    {
      struct stat info;

      std::wstring fileName = FIOString( std::string( dirInfo->d_name ) );

      if ( fileName == L"." || fileName == L".." ) { continue; }

      std::string fileToCheck = FIOString( rootDir + pathSepW + fileName );
      errno = 0;
      if ( stat( fileToCheck.c_str(), &info ) < 0 )
      { perror( strerror( errno ) ); }
      else if ( S_ISREG( info.st_mode ) )
      {
        if ( fExt != L".*" && fileName.length() > fExt.length()
          && fileName.substr( fileName.length() - fExt.length() ) == fExt )
        { dirs.push_back( rootDir + pathSepW + fileName ); }

        else if ( fExt == L".*" )
        {
          dirs.push_back( rootDir + pathSepW + fileName );
        }
      }
      else if ( recursiveSearch == RecursiveSearchTrue
        && S_ISDIR( info.st_mode ) )
      {
        std::vector< std::wstring > subDirContents =
          findFilesW( fExt, rootDir + pathSepW + fileName );

        for ( auto & file : subDirContents ) { dirs.push_back( file ); }
      }
    }

    closedir( dirhandle );

    return dirs;

#endif
  }

  std::vector< std::string > FIO::findFiles(
    FIOString const & fileExtension /* = L".*" */,
    FIOString const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    auto foundFiles = findFilesW( fileExtension, pathOrID, recursiveSearch );

    std::vector< std::string > files;

    for ( auto & ws : foundFiles ) { files.push_back( FIOString( ws ) ); }

    return files;
  }

  /*----------------------------------------------------------------------------
   * Read files.
  ----------------------------------------------------------------------------*/

  std::wstring FIO::readFileW( FIOString const & pathOrID )
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

  std::string FIO::readFile( FIOString const & pathOrID )
  {
    return FIOString( readFileW( pathOrID ) );
  }

  /*----------------------------------------------------------------------------
   * Read files into vectors.
  ----------------------------------------------------------------------------*/

  std::vector< std::wstring > FIO::readFileToVectorW(
    FIOString const & pathOrID, FIOString const & delim /* = L"\n\r" */ )
  {
    return splitStringW( readFileW( pathOrID ), delim );
  }

  std::vector< std::string > FIO::readFileToVector(
    FIOString const & pathOrID, FIOString const & delim /* = L"\n\r" */ )
  {
    return splitString( readFile( pathOrID ), delim );
  }

  /*----------------------------------------------------------------------------
   * Read files into vectors of vectors.
  ----------------------------------------------------------------------------*/

  std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
    FIOString const & pathOrID, FIOString const & lineDelim /* = L"," */,
    FIOString const & vertDelim /* = L"\n\r" */ )
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

  std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    FIOString const & pathOrID, FIOString const & lineDelim /* = L"," */,
    FIOString const & vertDelim /* = L"\n\r" */ )
  {
    auto wMatrix = readFileToMatrixW( pathOrID, lineDelim, vertDelim );
    std::vector< std::vector< std::string > > ret;

    for ( auto & line : wMatrix )
    {
      std::vector< std::string > vLine;
      for ( auto & ws : line ) { vLine.push_back( FIOString( ws ) ); }
      ret.push_back( vLine );
    }

    return ret;
  }

  /*----------------------------------------------------------------------------
   * Change the FIO root directory.
  ----------------------------------------------------------------------------*/

  FIO & FIO::setRoot( FIOString const & pathOrID )
  {
    return setDirID( L"root", pathOrID );
  }

  /*----------------------------------------------------------------------------
   * Store a file path in the FIO path map.
  ----------------------------------------------------------------------------*/

  FIO & FIO::setFileID( FIOString const & id, FIOString const & path )
  {
    std::wstring idW = id;
    std::wstring pathW = path;

    errno = 0;

#ifdef WINDOWS

    struct _stat info;

    if ( _wstat( pathW.c_str(), &info ) == 0 )

#else

    std::string pathS = path;

    struct stat info;

    if ( stat( pathS.c_str(), &info ) == 0 )

#endif

    {
      if ( info.st_mode & S_IFREG ) { PM[ idW ] = pathW; }
      else
      {
        throw( FIOExcept( L"Cannot set id " + idW + L", as " + pathW
          + L" does not point to a regular file." ) );
      }
    }
    else
    {
      perror( strerror( errno ) );
    }

    return *this;
  }

  /*----------------------------------------------------------------------------
   * Store a directory path in the FIO path map.
  ----------------------------------------------------------------------------*/

  FIO & FIO::setDirID( FIOString const & id, FIOString const & path )
  {
    std::wstring idW = id;
    std::wstring pathW = path;

    errno = 0;

#ifdef WINDOWS

    struct _stat info;

    if ( _wstat( pathW.c_str(), &info ) == 0 )

#else

    std::string pathS = path;

    struct stat info;

    if ( stat( pathS.c_str(), &info ) == 0 )

#endif

    {
      if ( info.st_mode & S_IFDIR ) { PM[ idW ] = pathW; }
      else
      {
        throw( FIOExcept( L"Cannot set id " + idW + L", as " + pathW
          + L" does not point to a directory." ) );
      }
    }
    else
    {
      perror( strerror( errno ) );
    }

    return *this;
  }

  /*----------------------------------------------------------------------------
   * Retrieve path to the root directory.
  ----------------------------------------------------------------------------*/

  std::wstring FIO::getRootW() const { return getPathW( L"root" ); }

  std::string FIO::getRoot() const { return getPath( L"root" ); }

  /*----------------------------------------------------------------------------
   * Retrieve a file system path.
  ----------------------------------------------------------------------------*/

  std::wstring FIO::getPathW( FIOString const & pathOrID ) const
  {
    std::wstring pathOrIDW = pathOrID;

#ifdef WINDOWS

    struct _stat info;

    if ( _wstat( pathOrIDW.c_str(), &info ) == 0

#else

    std::string pathOrIDS = pathOrID;

    struct stat info;

    if ( stat( pathOrIDS.c_str(), &info ) == 0

#endif

      && ( info.st_mode & S_IFREG || info.st_mode & S_IFDIR ) )
    {
      return pathOrID;
    }

    auto it = PM.find( pathOrIDW );

    if ( it == PM.end() )
    {
      throw( FIOExcept( L"Path cannot be retrieved as " + pathOrIDW
        + L" is neither a path to a file or directory, "
        + L"nor points to a path stored in the FIO path map." ) );
    }

    return it->second;
  }

  std::string FIO::getPath( FIOString const & pathOrID ) const
  {
    return FIOString( getPathW( pathOrID ) );
  }

  /*----------------------------------------------------------------------------
   * Delete a filepath from filepaths.
  ----------------------------------------------------------------------------*/

  FIO & FIO::removePath( FIOString const & id )
  {
    std::wstring idW = id;
    if ( PM.find( idW ) != PM.end() ) { PM.erase( idW ); }
    else
    {
      throw( FIOExcept( L"Path cannot be removed as " + idW
        + L" does not point to a path stored in the FIO pathmap." ) );
    }

    return *this;
  }

  /*----------------------------------------------------------------------------
   * Construct FIO.
  ----------------------------------------------------------------------------*/

  FIO::FIO( FIOString const & loc /* = "en_US.UTF-8" */ )
  {
    std::string locS = loc;

    setlocale( LC_ALL, locS.c_str() );
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

    return FIOString( buffer );
#endif
  }

  std::wistream & FIO::validateWIFStream( FIOString const & id ) const
  {
    std::wstring idW = id;

    auto it = iFSM.find( idW );

    if ( it == iFSM.end() )
    { throw( FIOExcept( L"No output stream exists at " + idW ) ); }

    if ( it->second->good() & it->second->is_open() )
    { return *( static_cast< std::wistream * >( &( *it->second ) ) ); }
    else
    {
      throw( FIOExcept(
        L"Output stream exists at " + idW + L", but cannot be read!" ) );
    }
  }

  std::wostream & FIO::validateWOFStream( FIOString const & id ) const
  {
    std::wstring idW = id;

    auto it = oFSM.find( idW );

    if ( it == oFSM.end() )
    { throw( FIOExcept( L"No output stream exists at " + idW ) ); }

    if ( it->second->good() & it->second->is_open() )
    { return *( static_cast< std::wostream * >( &( *it->second ) ) ); }
    else
    {
      throw( FIOExcept(
        L"Output stream exists at " + idW + L", but cannot be read!" ) );
    }
  }

} // namespace NSFIO