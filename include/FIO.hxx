/*------------------------------------------------------------------------------
 * Lennart Hautzer
 * 11/02/2021
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

#ifndef HYBRID_STRING_H_
#define HYBRID_STRING_H_

#include <locale>
#include <ostream>
#include <string>

namespace hst
{
  // Turn a wide string into a multi-byte string.
  inline static std::string wideToMultiByte( std::wstring const & str )
  {
    std::string multiByteString;
    std::string multiByteCharacterBuffer( MB_CUR_MAX, '\0' );

    for ( wchar_t const & wideCharacter : str )
    {
      int multiByteCharacterLength =
        std::wctomb( &multiByteCharacterBuffer[ 0 ], wideCharacter );

      if ( multiByteCharacterLength < 1 ) { break; }
      multiByteString +=
        multiByteCharacterBuffer.substr( 0, multiByteCharacterLength );
    }

    return multiByteString;
  }

  // Turn a multi-byte string into a wide string.
  inline static std::wstring multiByteToWide( std::string const & str )
  {
    std::wstring wideString( str.size(), L' ' );

    wideString.resize(
      std::mbstowcs( &wideString[ 0 ], str.c_str(), str.size() ) );

    return wideString;
  }

  // A string wrapper class for conversion between multi-byte and wide strings.
  class hstring
  {
    public:
    hstring() : multiByteString( "" ), wideString( L"" ) {}

    ~hstring() = default;

    hstring( hstring const & hstr ) :
      multiByteString( hstr.multiByteString ), wideString( hstr.wideString )
    {
    }

    hstring( hstring && hstr ) noexcept :
      multiByteString( std::move( hstr.multiByteString ) ),
      wideString( std::move( hstr.wideString ) )
    {
    }

    hstring( std::wstring const & wstr ) :
      multiByteString( wideToMultiByte( wstr ) ), wideString( wstr )
    {
    }

    hstring( wchar_t const * const & wstr ) : hstring( std::wstring( wstr ) ) {}

    hstring( wchar_t const & wc ) : hstring( std::wstring( 1, wc ) ) {}

    hstring( std::string const & mbstr ) :
      multiByteString( mbstr ), wideString( multiByteToWide( mbstr ) )
    {
    }

    hstring( char const * const & mbstr ) : hstring( std::string( mbstr ) ) {}

    hstring( char const & mbc ) : hstring( std::string( 1, mbc ) ) {}

    hstring & operator=( hstring const & hstr )
    {
      multiByteString = hstr.multiByteString;
      wideString = hstr.wideString;
      return *this;
    }

    hstring & operator=( hstring && hstr ) noexcept
    {
      if ( this != &hstr )
      {
        multiByteString = std::move( hstr.multiByteString );
        wideString = std::move( hstr.wideString );
      }
      return *this;
    }

    hstring & operator+=( hstring const & hstr )
    {
      wideString += hstr.wideString;
      multiByteString += hstr.multiByteString;
      return *this;
    }

    friend hstring operator+( hstring lhs, hstring const & rhs )
    {
      return lhs += rhs;
    }

    friend bool operator==( hstring const & lhs, hstring const & rhs )
    {
      return lhs.multiByteString == rhs.multiByteString &&
        lhs.wideString == rhs.wideString;
    }

    friend bool operator!=( hstring const & lhs, hstring const & rhs )
    {
      return ! ( lhs == rhs );
    }

    operator std::wstring() const { return wideString; }

    std::wstring wstr() const { return operator std::wstring(); }

    wchar_t const * wc_str() const { return wideString.c_str(); }

    operator std::string() const { return multiByteString; }

    std::string str() const { return operator std::string(); }

    char const * mb_str() const { return multiByteString.c_str(); }

    private:
    std::string multiByteString;
    std::wstring wideString;
  };

  inline static std::ostream & operator<<( std::ostream & stream,
                                           hstring const & str )
  {
    return stream << str.str();
  }

  inline static std::wostream & operator<<( std::wostream & stream,
                                            hstring const & str )
  {
    return stream << str.wstr();
  }
} // namespace hst

#endif

#ifndef FILE_INPUT_OUTPUT_H_
#define FILE_INPUT_OUTPUT_H_

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <sys/stat.h>
#include <unordered_map>
#include <vector>

#if defined WIN32 || defined _WIN32 || defined __WIN32 && ! defined __CYGWIN__

  #ifndef WINDOWS
    #define WINDOWS
  #endif

  #ifndef NOMINMAX
    #define NOMINMAX
  #endif

#endif

#ifdef WINDOWS
  #include <windows.h>
#else
  #include <dirent.h>
  #include <errno.h>
  #include <unistd.h>
#endif

namespace FileIO
{
  /*
    Given a complete filepath, retrieve the path to the directory immediately
    above the location it points to. Passing a malformed filepath results in
    undefined behaviour.
    Ex: parentDir( "/dir/file.txt" ) -> "/dir"
    Ex: parentDir( "/dir" ) -> "/dir"
    Ex: parentDir( "notADir" ) -> "notADir"
  */
  static hst::hstring parentDir( hst::hstring const & path );

  // Remove filepath extensions
  bool const constexpr StripExtensionTrue = true;
  // Keep filepath extensions
  bool const constexpr StripExtensionFalse = false;

  /*
    Given a complete filepath, retrieve just the name of the file it points
    to. Optionally, the extension of the file may be stripped as well.
    Passing in a malformed filepath results in undefined behaviour.
    Ex: baseFile( /dir/file.txt, StripExtensionFalse ) -> "file.txt"
    Ex: baseFile( /dir/file.txt, StripExtensionTrue ) -> "file"
    Ex: baseFile( file.txt, StripExtensionTrue ) -> "file"
    Ex: baseFile( /file.txt, StripExtensionTrue ) -> "file"
  */
  static hst::hstring baseFile(
    hst::hstring const & pathToFile,
    bool const & stripExtension = StripExtensionTrue );

  /*
    Split a string on one or more delimiters.
    Ex: splitString( "The,Quick,Brown|Fox", ",|" ) ->
      [ "The", "Quick", "Brown", "Fox" ]
    Ex: splitString( "The,Quick,Brown|Fox", "," ) ->
      [ "The", "Quick", "Brown|Fox" ]
  */
  static std::vector< hst::hstring > splitString(
    hst::hstring const & source, hst::hstring const & delim = L"\n\r" );

  /*
    Given a complete filepath to a file, permanently deletes it.
    Ex: deleteFile( "/dir/img/i.jpg" ) -> "/dir/img/i.jpg" is permanently
    deleted.
  */
  static void deleteFile( hst::hstring const & pathToFile );

  // Output Stream append mode.
  bool const constexpr AppendToFile = true;
  // Output Stream overwrite mode.
  bool const constexpr OpenNewFile = false;

  // File search sub-directory recursion mode.
  bool const constexpr RecursiveSearchTrue = true;
  // File search sub-directory exclusion mode.
  bool const constexpr RecursiveSearchFalse = false;

  // Simplifies filesystem interaction for applications.
  class FIO
  {
    public:
    /*
      Construct an FIO object using a given locale for string conversions. The
      default locale is the user environment's locale.
    */
    FIO( hst::hstring const & loc = "" );
    FIO( FIO const & ) = delete;
    FIO & operator=( FIO const & ) = delete;
    ~FIO() = default;

    /*
      Clear FIO to its initial state. Safely closes all filestreams, and clears
      all internal mappings.
    */
    FIO & clear();

    /*
      Open a wide input stream on the filepath pointed to by pathOrID. If
      pathOrID is a stored ID, the ID's target will be used as the stream
      source, otherwise pathOrID will be used as the stream source.
      Ex: Path Map contains [ { "data", "/dir/data/file.txt" } ]
        openInputStream( "data" ) -> reads from filepath "/dir/data/file.txt"
        openInputStream( "/dir/data/file.txt" ) ->
          reads from filepath "/dir/data/file.txt"
      openInputStream( "/dir/data/desc/spawnpoints.txt" ->
        reads from filepath "/dir/data/desc/spawnpoints.txt" )
    */
    std::wistream & openInputStream( hst::hstring const & pathOrID );

    /*
      Open a wide output stream on the filepath pointed to by pathOrID. If
      pathOrID is a stored ID, the ID's target will be used as the stream
      target, otherwise pathOrID will be used as the stream target.
      Optionally, the new wide output stream can be opened in append mode.
      Ex: Path Map contains [ { "data", "/dir/data/file.txt" } ]
        openOutputStream( "data" ) -> writes to filepath "/dir/data/file.txt"
        openOutputStream( "/dir/data/file.txt" ) ->
          writes to filepath "/dir/data/file.txt"
        openOutputStream( "/dir/data/desc/spawnpoints.txt" ->
          writes to filepath "/dir/data/desc/spawnpoints.txt" )
    */
    std::wostream & openOutputStream( hst::hstring const & pathOrID,
                                      bool const & appendToFile = OpenNewFile );

    /*
      Rewind a previously opened input stream pointed to by pathOrID. If
      pathOrID is a stored ID, the ID's target will be used as the rewind
      target, otherwise pathOrID will be used as the rewind target.
      Ex: Path Map contains [ { "data", "/dir/data/file.txt" } ]
        rewindInputStream( "data" ) -> rewinds the input stream stored
          under"/dir/data/file.txt"
    */
    std::wistream & rewindInputStream( hst::hstring const & streamID );

    /*
      Close a previously opened input stream pointed to by pathOrID. If
      pathOrID is a stored ID, the ID's target will be used to close the
      target stream, otherwise pathOrID will be used to close the target
      stream.
      Ex: Path Map contains [ { "data", "/dir/data/file.txt" } ]
        closeInputStream( "data" ) -> closes the input stream stored
          under"/dir/data/file.txt"
    */
    FIO & closeInputStream( hst::hstring const & pathOrID );

    /*
      Close a previously opened output stream pointed to by pathOrID. If
      pathOrID is a stored ID, the ID's target will be used to close the
      target stream, otherwise pathOrID will be used to close the target
      stream.
      Ex: Path Map contains [ { "data", "/dir/data/file.txt" } ]
        closeOutputStream( "data" ) -> closes the output stream stored
          under"/dir/data/file.txt"
    */
    FIO & closeOutputStream( hst::hstring const & pathOrID );

    /*
      Check if an input stream is currently opened on the source pointed to by
      pathOrID. If pathOrID is a stored ID, the ID's target will be used to
      check for the target stream, otherwise pathOrID will be used to check for
      the target stream.
      Ex: Path Map contains [ { "data", "/dir/data/file.txt" } ]
        hasInputStream( "data" ) -> checks if there is an input stream stored
        under"/dir/data/file.txt"
    */
    bool hasInputStream( hst::hstring const & pathOrID );

    /*
      Check if an output stream is currently opened on the source pointed to by
      pathOrID. If pathOrID is a stored ID, the ID's target will be used to
      check for the target stream, otherwise pathOrID will be used to check for
      the target stream.
      Ex: Path Map contains [ { "data", "/dir/data/file.txt" } ]
        hasOutputStream( "data" ) -> checks if there is an output stream stored
          under"/dir/data/file.txt"
    */
    bool hasOutputStream( hst::hstring const & pathOrID );

    /*
      Retrieve a previously opened input stream pointed to by pathOrID. If
      pathOrID is a stored ID, the ID's target will be used to retrieve the
      target stream, otherwise pathOrID will be used to retrieve the target
      stream.
      Ex: Path Map contains [ { "data", "/dir/data/file.txt" } ]
        getInputStream( "data" ) -> retrieves the input stream stored
          under"/dir/data/file.txt"
    */
    std::wistream & getInputStream( hst::hstring const & pathOrID ) const;

    /*
      Retrieve a previously opened output stream pointed to by pathOrID. If
      pathOrID is a stored ID, the ID's target will be used to retrieve the
      target stream, otherwise pathOrID will be used to retrieve the target
      stream.
      Ex: Path Map contains [ { "data", "/dir/data/file.txt" } ]
        getOutputStream( "data" ) -> retrieves the output stream stored
          under"/dir/data/file.txt"
    */
    std::wostream & getOutputStream( hst::hstring const & pathOrID ) const;

    /*
      Read one unaltered line from a previously opened input stream pointed
      to by pathOrID. If pathOrID is a stored ID, the ID's target will be used
      to read from the target stream, otherwise pathOrID will be used to
      read from the target stream.
      Ex: Path Map contains [ { "data", "/dir/data/file.txt" } ]
        readLine( "data" ) -> reads one line from the input stream stored
          under"/dir/data/file.txt"
    */
    hst::hstring readLine( hst::hstring const & pathOrID );

    /*
      Write one unaltered line to a previously opened output stream pointed
      to by pathOrID. If pathOrID is a stored ID, the ID's target will be used
      to write to the target stream, otherwise pathOrID will be used to
      write to the target stream.
      Ex: Path Map contains [ { "data", "/dir/data/file.txt" } ]
        writeLine( "data", "foo\n" ) -> writes "foo\n" to the output stream
          stored under"/dir/data/file.txt"
    */
    FIO & writeLine( hst::hstring const & pathOrID,
                     hst::hstring const & source );

    /*
      Finds all regular files matching a given file extension starting at a
      directory pointed to by pathOrID. If pathOrID is a stored ID, the ID's
      target will be used to find the files, otherwise pathOrID
      will be used to write to find the files. Optionally recursively
      searches through sub-directories.
      Ex: findFiles( ".txt", "/dir", RecursiveSearchTrue ) -> a vector of .txt
        filenames in or below /dir.
      Ex: findFiles( ".png", "/dir", RecursiveSearchFalse ) -> a vector of .png
        filenames only within /dir.
    */
    std::vector< hst::hstring > findFiles(
      hst::hstring const & fileExtension = L".*",
      hst::hstring const & pathOrID = L"__root",
      bool const & recursiveSearch = RecursiveSearchTrue ) const;

    /*
      Read the unaltered contents of a file pointed
      to by pathOrID. If pathOrID is a stored ID, the ID's target will be used
      as the target filepath, otherwise pathOrID will be used as the target
      filepath. There does not need to be an open stream on the filepath. Will
      reset an open filestream to its filestart before reading.
      Ex: Path Map contains [ { "data", "/dir/data/file.txt" } ]
        readFile( "data" ) -> reads the contents of "/dir/data/file.txt"
    */
    hst::hstring readFile( hst::hstring const & pathOrID );

    /*
      Read the contents of a file pointed to by pathOrID as a vector split on
      all provided delimiting characters. If pathOrID is a stored ID, the ID's
      target will be used as the target filepath, otherwise pathOrID will be
      used as the target filepath. There does not need to be an open stream on
      the filepath. Will reset an open filestream to its filestart before
      reading.
      Ex: Path Map contains [ { "data", "/dir/data/file.txt" } ]
        readFileToVector( "data", "\n\r" ) -> reads the contents of
        "/dir/data/file.txt", split on the '\n' and '\r' characters.
    */
    std::vector< hst::hstring > readFileToVector(
      hst::hstring const & pathOrID, hst::hstring const & delim = L"\n\r" );

    /*
      Read the contents of a file pointed to by pathOrID as a vector of
      vectors, split on all provided delimiting characters. If pathOrID is a
      stored ID, the ID's target will be used as the target filepath, otherwise
      pathOrID will be used as the target filepath. There does not need to be
      an open stream on the filepath. Will reset an open filestream to its
      filestart before reading.
      Ex: Path Map contains [ { "data", "/dir/data/file.txt" } ]
        readFileToMatrix( "data", ",", "\n\r" ) -> reads the contents of
          "/dir/data/file.txt", split horizontally on the ',' character, and
          split vertically on the '\n' and '\r' characters.
    */
    std::vector< std::vector< hst::hstring > > readFileToMatrix(
      hst::hstring const & pathOrID,
      hst::hstring const & lineDelim = L",",
      hst::hstring const & vertDelim = L"\n\r" );

    // Finds the application's root directory (Where the executable is located).
    hst::hstring findRootDir() const;

    /*
      Sets the root directory of FIO to the filepath pointed to by pathOrID. If
      pathOrID is a stored ID, the ID's target will be used as the root
      filepath, otherwise pathOrID will be used as the root filepath.
      Ex: Path Map contains [ { "data", "/dir/data/file.txt" } ]
        setRootDir( "data") -> sets the root dir to "/dir/data/file.txt"
        setRootDir( "/dir/data/pictures" ) -> sets the root dir to
          "/dir/data/pictures"
    */
    FIO & setRootDir( hst::hstring const & pathOrID );

    // Returns the root directory's currently stored filepath.
    hst::hstring getRootDir() const;

    /*
      Stores the path provided in the Path Map at the ID provided. The path
      can then be accessed using the ID for convenience. The ID "__root" is
      reserved by FIO, and may only be changed using the FIO::setRootDir()
      member function.
    */
    FIO & storePathAtID( hst::hstring const & ID, hst::hstring const & path );

    /*
      Returns the absolute filesystem path associated with the ID stored in the
      FIO path map. If the ID provided is not contained in the path map, it
      will be returned unaltered.
      Ex: Path map contains [ { "data", "/dir/data" },
          { "users", "/dir/users" } ]
        getPath( "data" ) -> "/dir/data"
        getPath( "users" ) -> "/dir/users"
        getPath( "/dir/images" ) -> "/dir/images"
        getPath( "invalid" ) -> "invalid"
    */
    hst::hstring getPath( hst::hstring const & pathOrID ) const;

    // Removes the path located in the FIO path map at the ID provided.
    FIO & removePathAtID( hst::hstring const & ID );

    private:
    // Checks that the ID points to a valid input stream.
    std::wistream & validateInputStream( hst::hstring const & ID ) const;
    // Checks that the ID points to a valid output stream.
    std::wostream & validateOutputStream( hst::hstring const & ID ) const;

    // The Path Map in which path shorthands to files and directories are
    // stored.
    std::unordered_map< std::wstring, std::wstring > pathIDM;
    // The input file stream map in which open input streams are stored.
    std::unordered_map< std::wstring, std::unique_ptr< std::wifstream > >
      inputFSM;
    // The output file stream map in which open output streams are stored.
    std::unordered_map< std::wstring, std::unique_ptr< std::wofstream > >
      outputFSM;
  };

  inline hst::hstring parentDir( hst::hstring const & path )
  {
    auto str = path.wstr();
    auto const delim = str.find_last_of( L"/\\" );

    if ( delim != std::wstring::npos ) { str = str.substr( 0, delim ); }
    return str;
  }

  inline hst::hstring baseFile(
    hst::hstring const & pathToFile,
    bool const & stripExtension /* = StripExtensionTrue */ )
  {
    auto const directoryPath = parentDir( pathToFile ).wstr();
    auto filePath = pathToFile.wstr();

    if ( directoryPath.size() < filePath.size() )
    {
      filePath = filePath.substr( directoryPath.size() + 1 );
    }
    if ( stripExtension == StripExtensionTrue )
    {
      auto const delim = filePath.find_last_of( L"." );

      if ( delim != std::wstring::npos )
      {
        filePath = filePath.substr( 0, delim );
      }
    }
    return filePath;
  }

  inline std::vector< hst::hstring > splitString(
    hst::hstring const & source, hst::hstring const & delim /* = L"\n\r" */ )
  {
    auto str = source.wstr();
    auto const del = delim.wstr();
    std::vector< hst::hstring > splitStr;
    std::size_t i;

    while ( ( i = str.find_first_of( del ) ) != std::wstring::npos )
    {
      auto const substr = str.substr( 0, i );

      if ( substr.find_first_not_of( del ) != std::wstring::npos )
      {
        splitStr.push_back( substr );
      }
      str.erase( 0, i + 1 );
    }
    if ( str.find_first_not_of( del ) != std::wstring::npos )
    {
      splitStr.push_back( str );
    }

    return splitStr;
  }

  inline void deleteFile( hst::hstring const & pathToFile )
  {
    try
    {
      std::remove( pathToFile.mb_str() );
    }
    catch ( std::exception & e )
    {
      throw std::runtime_error( ( "Could not remove file: \"" + pathToFile +
                                  "\"." + "Error Message: \"" + e.what() +
                                  "\"" )
                                  .mb_str() );
    }
  }

  inline FIO::FIO( hst::hstring const & loc /* = "" */ )
  {
    if ( ! setlocale( LC_ALL, loc.mb_str() ) )
    {
      throw std::runtime_error(
        ( "Could not set locale to: \"" + loc + "\"!" ).mb_str() );
    }
    setRootDir( findRootDir() );
  }

  inline FIO & FIO::clear()
  {
    auto const currentRootDir = getRootDir();

    pathIDM.clear();
    inputFSM.clear();
    outputFSM.clear();

    setRootDir( currentRootDir );

    return *this;
  }

  inline std::wistream & FIO::openInputStream( hst::hstring const & pathOrID )
  {
    auto const path = getPath( pathOrID );

    if ( inputFSM.find( path ) == inputFSM.end() )
    {
      inputFSM[ path ] = std::make_unique< std::wifstream >(
        getPath( path ).str(), std::wifstream::in );
    }
    return validateInputStream( path );
  }

  inline std::wostream & FIO::openOutputStream(
    hst::hstring const & pathOrID,
    bool const & appendToFile /* = OpenNewFile */ )
  {
    auto const path = getPath( pathOrID );

    if ( outputFSM.find( path ) == outputFSM.end() )
    {
      outputFSM[ path ] = std::make_unique< std::wofstream >(
        getPath( path ).str(),
        ( appendToFile ? std::wofstream::app : std::wofstream::out ) );
    }
    return validateOutputStream( path );
  }

  inline std::wistream & FIO::rewindInputStream( hst::hstring const & pathOrID )
  {
    auto & stream = validateInputStream( getPath( pathOrID ) );
    stream.clear();
    return stream.seekg( 0, std::ios::beg );
  }

  inline FIO & FIO::closeInputStream( hst::hstring const & pathOrID )
  {
    auto const it = inputFSM.find( getPath( pathOrID ) );

    if ( it != inputFSM.end() ) { inputFSM.erase( it ); }
    return *this;
  }

  inline FIO & FIO::closeOutputStream( hst::hstring const & pathOrID )
  {
    auto const it = outputFSM.find( getPath( pathOrID ) );

    if ( it != outputFSM.end() ) { outputFSM.erase( it ); }
    return *this;
  }

  inline bool FIO::hasInputStream( hst::hstring const & pathOrID )
  {
    return inputFSM.find( getPath( pathOrID ) ) != inputFSM.end();
  }

  inline bool FIO::hasOutputStream( hst::hstring const & pathOrID )
  {
    return outputFSM.find( getPath( pathOrID ) ) != outputFSM.end();
  }

  inline std::wistream & FIO::getInputStream(
    hst::hstring const & pathOrID ) const
  {
    return validateInputStream( getPath( pathOrID ) );
  }

  inline std::wostream & FIO::getOutputStream(
    hst::hstring const & pathOrID ) const
  {
    return validateOutputStream( getPath( pathOrID ) );
  }

  inline hst::hstring FIO::readLine( hst::hstring const & pathOrID )
  {
    std::wstring line;
    std::getline( validateInputStream( getPath( pathOrID ) ), line );
    return line;
  }

  inline FIO & FIO::writeLine( hst::hstring const & pathOrID,
                               hst::hstring const & source )
  {
    validateOutputStream( getPath( pathOrID ) ) << source;
    return *this;
  }

#ifdef WINDOWS
  #ifndef PATH_SEP
    // Windows-style path separator
    #define PATH_SEP hst::hstring( L"\\" )
  #endif
#else
  #ifndef PATH_SEP
    // Unix-style path separator
    #define PATH_SEP hst::hstring( L"/" )
  #endif
#endif

  inline std::vector< hst::hstring > FIO::findFiles(
    hst::hstring const & fileExtension /* = L".*" */,
    hst::hstring const & pathOrID /* = L"__root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    std::vector< hst::hstring > foundFiles;
    auto const rootDir = getPath( pathOrID );

#ifdef WINDOWS
    auto const searchPath = rootDir + PATH_SEP + hst::hstring( "*.*" );

    WIN32_FIND_DATAW info;
    HANDLE dirHandle = ::FindFirstFileW( searchPath.wc_str(), &info );

    if ( dirHandle != INVALID_HANDLE_VALUE )
    {
      while ( ::FindNextFileW( dirHandle, &info ) )
      {
        auto const fileName = hst::hstring( info.cFileName );

        if ( fileName == L"." || fileName == L".." ) { continue; }

        auto const filePath = rootDir + PATH_SEP + fileName;

        if ( ! ( info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
        {
          if ( fileExtension == L".*" ||
               ( fileName.wstr().length() > fileExtension.wstr().length() &&
                 fileName.wstr().substr( fileName.wstr().length() -
                                         fileExtension.wstr().length() ) ==
                   fileExtension ) )
          {
            foundFiles.push_back( filePath );
          }
        }
        else if ( ( recursiveSearch == RecursiveSearchTrue ) &&
                  ( info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
        {
          auto const subDirFiles = findFiles( fileExtension, filePath );

          for ( auto const & it : subDirFiles ) { foundFiles.push_back( it ); }
        }
      }
      ::FindClose( dirHandle );
    }
#else
    struct dirent * dirInfo;

    DIR * dirhandle = static_cast< DIR * >( opendir( rootDir.mb_str() ) );

    if ( dirhandle )
    {
      while ( ( dirInfo = readdir( dirhandle ) ) != NULL )
      {
        auto const fileName = hst::hstring( dirInfo->d_name );

        if ( fileName == "." || fileName == ".." ) { continue; }

        auto const filePath = rootDir + PATH_SEP + fileName;

        struct stat info;
        errno = 0;

        if ( stat( filePath.mb_str(), &info ) < 0 )
        {
          perror( "Invalid File encountered." );
        }
        else if ( S_ISREG( info.st_mode ) )
        {
          if ( fileExtension == ".*" ||
               ( fileName.str().length() > fileExtension.str().length() &&
                 fileName.str().substr( fileName.str().length() -
                                        fileExtension.str().length() ) ==
                   fileExtension ) )
          {
            foundFiles.push_back( filePath );
          }
        }
        else if ( ( recursiveSearch == RecursiveSearchTrue ) &&
                  S_ISDIR( info.st_mode ) )
        {
          auto const subDirFiles = findFiles( fileExtension, filePath );

          for ( auto const & it : subDirFiles ) { foundFiles.push_back( it ); }
        }
      }
      closedir( dirhandle );
    }
#endif

    return foundFiles;
  }

  inline hst::hstring FIO::readFile( hst::hstring const & pathOrID )
  {
    auto const path = getPath( pathOrID );
    std::wstringstream wss;

    if ( hasInputStream( path ) ) { wss << rewindInputStream( path ).rdbuf(); }
    else
    {
      wss << openInputStream( path ).rdbuf();
      closeInputStream( path );
    }
    return wss.str();
  }

  inline std::vector< hst::hstring > FIO::readFileToVector(
    hst::hstring const & pathOrID, hst::hstring const & delim /* = L"\n\r" */ )
  {
    return splitString( readFile( pathOrID ), delim );
  }

  inline std::vector< std::vector< hst::hstring > > FIO::readFileToMatrix(
    hst::hstring const & pathOrID,
    hst::hstring const & lineDelim /* = L"," */,
    hst::hstring const & vertDelim /* = L"\n\r" */ )
  {
    auto const lines = readFileToVector( pathOrID, vertDelim );
    std::vector< std::vector< hst::hstring > > matrix;

    for ( auto const & it : lines )
    {
      auto const splitLine = splitString( it, lineDelim );

      if ( splitLine.size() > 0 ) { matrix.push_back( splitLine ); }
    }
    return matrix;
  }

  inline hst::hstring FIO::findRootDir() const
  {
#ifdef WINDOWS
    wchar_t buffer[ FILENAME_MAX ];
    GetModuleFileNameW( NULL, buffer, FILENAME_MAX );
    return parentDir( buffer );
#else
    char buffer[ FILENAME_MAX ];
    if ( getcwd( buffer, FILENAME_MAX ) == NULL )
    {
      throw std::runtime_error(
        "Could not determine the application's root directory." );
    }
    return buffer;
#endif
  }

  inline FIO & FIO::setRootDir( hst::hstring const & pathOrID )
  {
    pathIDM[ L"__root" ] = getPath( pathOrID );
    return *this;
  }

  inline hst::hstring FIO::getRootDir() const { return getPath( L"__root" ); }

  inline FIO & FIO::storePathAtID( hst::hstring const & ID,
                                   hst::hstring const & path )
  {
    if ( ID != L"__root" ) { pathIDM[ ID ] = path; }
    return *this;
  }

  inline hst::hstring FIO::getPath( hst::hstring const & pathOrID ) const
  {
    auto const it = pathIDM.find( pathOrID );
    if ( it != pathIDM.end() ) { return it->second; }
    return pathOrID;
  }

  inline FIO & FIO::removePathAtID( hst::hstring const & ID )
  {
    auto const it = pathIDM.find( ID );
    if ( it != pathIDM.end() ) { pathIDM.erase( it ); }
    return *this;
  }

  inline std::wistream & FIO::validateInputStream(
    hst::hstring const & ID ) const
  {
    auto const it = inputFSM.find( ID );

    if ( it == inputFSM.end() )
    {
      throw std::runtime_error(
        ( L"Could not validate input stream \"" + ID +
          L"\". No such input stream exists. " +
          L"Hint: Did you provide the correct ID?" +
          L" Did you mean to check for an output stream?" )
          .mb_str() );
    }
    if ( it->second->good() && it->second->is_open() )
    {
      return static_cast< std::wistream & >( *it->second.get() );
    }
    else
    {
      throw std::system_error(
        errno,
        std::system_category(),
        ( L"Input stream \"" + ID +
          L"\" was found, but it could not be read. System Error Message" )
          .mb_str() );
    }
  }

  inline std::wostream & FIO::validateOutputStream(
    hst::hstring const & ID ) const
  {
    auto const it = outputFSM.find( ID );

    if ( it == outputFSM.end() )
    {
      throw std::runtime_error(
        ( L"Could not validate output stream \"" + ID +
          L"\". No such output stream exists. " +
          L"Hint: Did you provide the correct ID?" +
          L" Did you mean to check for an input stream?" )
          .mb_str() );
    }

    if ( it->second->good() && it->second->is_open() )
    {
      return static_cast< std::wostream & >( *it->second.get() );
    }
    else
    {
      throw std::system_error(
        errno,
        std::system_category(),
        ( L"Output stream \"" + ID +
          L"\" was found, but it could not be written to. System Error Message" )
          .mb_str() );
    }
  }
} // namespace FileIO
#endif