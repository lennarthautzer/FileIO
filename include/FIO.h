/*------------------------------------------------------------------------------
 * Lennart Hautzer
 * 27/12/2020
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

#ifndef FILE_IO_H_
#define FILE_IO_H_

#include <algorithm> 
#include <fstream> 
#include <memory> 
#include <sstream> 
#include <stdexcept> 
#include <string> 
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
  #include <unistd.h>
  #include <dirent.h>
  #include <errno.h>
#endif

namespace NSFIO
{
  /*----------------------------------------------------------------------------
   * Convertible wide <-> multi-byte String Wrapper
  ----------------------------------------------------------------------------*/

  /* Wrapper class which converts between strings and wide strings.
   * Can take either as parameter. */
  class FIOString
  {
  public:
    /* In: fs -> an FIO string.
     *
     * Out: An FIOString populated with fs.
     *
     * Construct a populated FIOString. */
    FIOString( FIOString const & fs );

    /* In: fs -> an FIO string.
     *
     * Out: An FIOString populated with fs.
     *
     * Construct a populated FIOString. */
    FIOString & operator=( FIOString const & fs );

    /* In: s -> a multi-byte string.
     *
     * Out: An FIOString populated with s.
     *
     * Construct a populated FIOString. */
    FIOString( std::string const & s );

    /* In: s -> a multi-byte string.
     *
     * Out: An FIOString populated with s.
     *
     * Construct a populated FIOString. */
    FIOString & operator=( std::string const & s );

    /* In: c -> a char array.
     *
     * Out: An FIOString populated with c.
     *
     * Construct a populated FIOString. */
    FIOString( char const * const & c );

    /* In: c -> a char array.
     *
     * Out: An FIOString populated with c.
     *
     * Construct a populated FIOString. */
    FIOString & operator=( char const * const & c );

    /* In: ws -> a wide string.
     *
     * Out: An FIOString populated with ws.
     *
     * Construct a populated FIOString. */
    FIOString( std::wstring const & ws );

    /* In: ws -> a wide string.
     *
     * Out: An FIOString populated with ws.
     *
     * Construct a populated FIOString. */
    FIOString & operator=( std::wstring const & ws );

    /* In: wc -> a wide char array.
     *
     * Out: An FIOString populated with wc.
     *
     * Construct a populated FIOString. */
    FIOString( wchar_t const * const & wc );

    /* In: wc -> a wide char array.
     *
     * Out: An FIOString populated with wc.
     *
     * Construct a populated FIOString. */
    FIOString & operator=( wchar_t const * const & wc );

    /* In: None.
     *
     * Out: A multi-byte string.
     *
     * Construct a multi-byte string with the FIOString's contents. */
    operator std::string() const;

    /* In: None.
     *
     * Out: A wide string.
     *
     * Construct a wide string with the FIOString's contents. */
    operator std::wstring() const;

    /* In: None.
     *
     * Out: A reference to a multi-byte string.
     *
     * Return the FIOString's contents as a string. */
    operator std::string &();

    /* In: None.
     *
     * Out: A reference to a wide string.
     *
     * Return the FIOString's contents as a wide string. */
    operator std::wstring &();

    /* In: None.
     *
     * Out: A multi-byte string.
     *
     * Return the FIOString's contents as a string. */
    std::string str();

    /* In: None.
     *
     * Out: A wide string.
     *
     * Return the FIOString's contents as a wide string. */
    std::wstring wstr();

  private:
    /* In: ws -> the wstring to be converted into a multibyte normal
     *        string.
     *
     * Out: The converted multibyte normal string.
     *
     * Turn a wide string into a multibyte normal string. */
    std::string w2mB( std::wstring const & ws ) const;

    /* In: mb -> the multibyte normal string to be converted
     *        into a wide string.
     *
     * Out: The converted wide string.
     *
     * Turn a multibyte normal string into a wide string. */
    std::wstring mB2w( std::string const & mb ) const;

    /* Keep both wide and multibyte versions of the data to support
     *    reference operators. */
    std::string mbStr;
    std::wstring wStr;
  };

  /*----------------------------------------------------------------------------
   * FIO constexprs.
  ----------------------------------------------------------------------------*/

  /* File search modes. */
  bool const constexpr RecursiveSearchTrue = true;
  bool const constexpr RecursiveSearchFalse = false;

  /* Filepath extension strip modes. */
  bool const constexpr StripExtensionTrue = true;
  bool const constexpr StripExtensionFalse = false;

  /* Output stream write modes. */
  bool const constexpr AppendToFile = true;
  bool const constexpr OpenNewFile = false;

/* The OS path separator. */
#ifdef WINDOWS
  wchar_t const constexpr pathSepW = L'\\';
  char const constexpr pathSep = L'\\';
#else
  wchar_t const constexpr pathSepW = L'/';
  char const constexpr pathSep = L'/';
#endif

  /*----------------------------------------------------------------------------
   * Ancillary Non-Member Functions.
  ----------------------------------------------------------------------------*/

  /*----------------------------------------------------------------------------
   * Filepath manipulation.
  ----------------------------------------------------------------------------*/

  /* In: pathToFile -> an absolute file system path to a file.
   *
   * Out: The path provided, with all leading directories trimmed away.
   *
   * Get the name of a file in its local directory from a full path
   *    to the file. */
  std::wstring baseFileW( FIOString const & pathToFile,
    bool const & stripExtension = StripExtensionTrue );

  /* In: pathToFile -> an absolute file system path to a file.
   *
   * Out: The path provided, with all leading directories trimmed away.
   *
   * Get the name of a file in its local directory from a full path
   *    to the file. */
  std::string baseFile( FIOString const & pathToFile,
    bool const & stripExtension = StripExtensionTrue );

  /*----------------------------------------------------------------------------
   * Get the parent directory of a filepath.
  ----------------------------------------------------------------------------*/

  /* In: path -> a file system path.
   *
   * Out: The parent directory of the path provided.
   *
   * Get the parent directory of the path provided. */
  std::wstring parentDirW( FIOString const & path );

  /* In: path -> a file system path.
   *
   * Out: The parent directory of the path provided.
   *
   * Get the parent directory of the path provided. */
  std::string parentDir( FIOString const & path );

  /*----------------------------------------------------------------------------
   * Split strings. (CSV)
  ----------------------------------------------------------------------------*/

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::wstring > splitStringW(
    FIOString const & source, FIOString const & delim = L"\n\r" );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::string > splitString(
    FIOString const & source, FIOString const & delim = L"\n\r" );

  /*----------------------------------------------------------------------------
   * FIO exception class.
  ----------------------------------------------------------------------------*/
  class FIOExcept : public std::exception
  {
  public:
    /* In: output -> the error message to be presented when throwing an error.
     *
     * Out: The constructed FIOExcept.
     *
     * Construct a new FIO-related exception. */
    FIOExcept( FIOString const & output );

    /* In: None.
     *
     * Out: None.
     *
     * Destroys the FIOExcept. */
    virtual ~FIOExcept() throw();

    /* In: None.
     *
     * Out: The stored error message.
     *
     * Presents the stored error message. */
    virtual char const * what() const throw();

  private:
    std::string output;
  };

  /*----------------------------------------------------------------------------
   * FIO.
  ----------------------------------------------------------------------------*/

  class FIO
  {
  public:
    /*--------------------------------------------------------------------------
     * Construct FIO Singleton.
    --------------------------------------------------------------------------*/

    /* In: ( Optional ) loc -> the locale string to set on construction.
     *                      Defaults to "en_us.UTF-8"
     *
     * Out: A reference to the FIO Singleton.
     *
     * If FIO has not been constructed, it will be constructed. If FIO was
     *    already constructed, a reference to the existing FIO Singleton
     *    will be returned. */
    static FIO & getInstance( FIOString const & loc = L"en_US.UTF-8" );

    /*--------------------------------------------------------------------------
     * Prevent copying of Singleton.
    --------------------------------------------------------------------------*/

    /* In: None.
     *
     * Out: None.
     *
     * FIO is a Singleton. Singletons may not be copied. */
    FIO( FIO const & fio ) = delete;

    /* In: None.
     *
     * Out: None.
     *
     * FIO is a Singleton. Singletons may not be copied. */
    FIO & operator=( FIO const & fio ) = delete;

    /*--------------------------------------------------------------------------
     * Destructor.
    --------------------------------------------------------------------------*/

    /* In: None.
     *
     * Out: None.
     *
     * Safely closes all filestreams, then destroys FIO. */
    ~FIO();

    /*--------------------------------------------------------------------------
     * Return FIO to its initial state.
    --------------------------------------------------------------------------*/

    /* In: None.
     *
     * Out: A reference to the reset FIO Singleton.
     *
     * Will return FIO to its initial state. Safely closes all filestreams,
     *    clears all internal mappings, and resets the FIO root directory
     *    to its starting value. */
    FIO & reset();

    /*--------------------------------------------------------------------------
     * Open an input stream.
    --------------------------------------------------------------------------*/

    /* In: pathOrID -> an absolute filesystem path, or the identifier of a
     *        an absolute filesystem path stored in the FIO pathmap.
     *
     * Out: A file stream opened on the path specified.
     *
     * If pathOrID points to a regular file it will be opened, otherwise
     *    pathOrID will be treated as an identifier for an element
     *    in the pathmap, and the associated path will be opened. */
    std::wistream & openWI( FIOString const & pathOrID );

    /*--------------------------------------------------------------------------
     * Open an output stream.
    --------------------------------------------------------------------------*/

    /* In: pathOrID -> an absolute filesystem path, or the identifier of a
     *        an absolute filesystem path stored in the FIO pathmap.
     *
     *    ( Optional ) appendToFile -> Possible values are
     *        OpenNewFile or AppendToFile.
     *        Defaults to OpenNewFile, which will overwrite any file contents
     *        that may be stored at the path given. AppendToFile will write data
     *        after the existing end of file.
     *
     * Out: A file stream opened on the path specified.
     *
     * If pathOrID points to a regular file it will be opened, otherwise
     *    pathOrID will be treated as an identifier for an element
     *    in the pathmap, and the associated path will be opened. */
    std::wostream & openWO(
      FIOString const & pathOrID, bool const & appendToFile = OpenNewFile );

    /*--------------------------------------------------------------------------
     * Rewind an input stream.
    --------------------------------------------------------------------------*/

    /* In: pathOrID -> an absolute filesystem path, or the identifier of a
     *        an absolute filesystem path stored in the FIO pathmap.
     *
     * Out: The open input file stream stored in the
     *    input stream map at pathOrID, rewound to the filestart.
     *
     * If pathOrID points to a regular file it will be used to locate
     *    the stream, otherwise pathOrID will be treated as an
     *    identifier for an element in the pathmap, and the
     *    associated path will be used. */
    std::wistream & rewindWI( FIOString const & pathOrID );

    /*--------------------------------------------------------------------------
     * Close an input stream.
    --------------------------------------------------------------------------*/

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: A reference to the FIO Singleton.
     *
     * Closes the open input file stream stored in the
     *    input stream map at pathOrID. */
    FIO & closeWI( FIOString const & pathOrID );

    /*--------------------------------------------------------------------------
     * Close an output stream.
    --------------------------------------------------------------------------*/

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     * Out: A reference to the FIO Singleton.
     *
     * Closes the open output file stream stored in the
     *    output stream map at pathOrID. */
    FIO & closeWO( FIOString const & pathOrID );

    /*--------------------------------------------------------------------------
     * Get an open input stream.
    --------------------------------------------------------------------------*/

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: The open input stream.
     *
     * Returns the open input file stream stored in the
     *    input stream map at pathOrID. */
    std::wistream & getWI( FIOString const & pathOrID ) const;

    /*--------------------------------------------------------------------------
     * Get an open output stream.
    --------------------------------------------------------------------------*/

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     * Out: The open output stream.
     *
     * Returns the open output file stream stored in the
     *    output stream map at pathOrID. */
    std::wostream & getWO( FIOString const & pathOrID ) const;

    /*--------------------------------------------------------------------------
     * Get a sorted listing of open input streams.
    --------------------------------------------------------------------------*/

    /* In: None.
     *
     * Out: Collection of open input streams, formatted as wide strings.
     *
     * Returns a lexicographically sorted collection of all the input
     *    streams currently open in the input stream map. The output
     *    format is "stream key : target filepath" */
    std::vector< std::wstring > listOpenWIW() const;

    /* In: None.
     *
     * Out: Collection of open input streams.
     *
     * Returns a lexicographically sorted collection of all the input
     *    streams currently open in the input stream map. The output
     *    format is "stream key : target filepath" */
    std::vector< std::string > listOpenWI() const;

    /*--------------------------------------------------------------------------
     * Get a sorted listing of open output streams.
    --------------------------------------------------------------------------*/

    /* In: None.
     *
     * Out: Collection of open output streams, formatted as wide strings.
     *
     * Returns a lexicographically sorted collection of all the output
     *    streams currently open in the output stream map. The output
     *    format is "stream key : target filepath" */
    std::vector< std::wstring > listOpenWOW() const;

    /* In: None.
     *
     * Out: Collection of open output streams.
     *
     * Returns a lexicographically sorted collection of all the output
     *    streams currently open in the output stream map. The output
     *    format is "stream key : target filepath" */
    std::vector< std::string > listOpenWO() const;

    /*--------------------------------------------------------------------------
     * Read from an input stream.
    --------------------------------------------------------------------------*/

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: The next line in the file stream, unaltered.
     *
     * Returns the next line in the file stream, unaltered. */
    std::wstring readLineW( FIOString const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: The next line in the file stream, unaltered.
     *
     * Returns the next line in the file stream, unaltered. */
    std::string readLine( FIOString const & pathOrID );

    /*--------------------------------------------------------------------------
     * Write to an output stream.
    --------------------------------------------------------------------------*/

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLineW( FIOString const & pathOrID, FIOString const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLine( FIOString const & pathOrID, FIOString const & source );

    /*--------------------------------------------------------------------------
     * Search for files within a directory.
    --------------------------------------------------------------------------*/

    /* In:  ( Optional ) fileExtension -> limits the type of files
     *                      to search for. Defaults to all files.
     *
     *      ( Optional ) pathOrID -> the identifier of an absolute
     *                      filesystem path stored in the FIO pathmap.
     *                      Defaults to the root directory.
     *
     *      ( Optional ) recursiveSearch -> Possible values are
     *                      RecursiveSearchTrue or RecursiveSearchFalse.
     *                      Controls whether search should descend into
     *                      sub directories or not.
     *
     * Out: A collection of absolute filepaths
     *    matching the search parameters.
     *
     * Searches for all files in a directory matching a given pattern.
     */
    std::vector< std::wstring > findFilesW(
      FIOString const & fileExtension = L".*",
      FIOString const & pathOrID = L"root",
      bool const & recursiveSearch = RecursiveSearchTrue ) const;

    /* In:  ( Optional ) fileExtension -> limits the type of files
     *                      to search for. Defaults to all files.
     *
     *      ( Optional ) pathOrID -> the identifier of an absolute
     *                      filesystem path stored in the FIO pathmap.
     *                      Defaults to the root directory.
     *
     *      ( Optional ) recursiveSearch -> Possible values are
     *                      RecursiveSearchTrue or RecursiveSearchFalse.
     *                      Controls whether search should descend into
     *                      sub directories or not.
     *
     * Out: A collection of absolute filepaths
     *    matching the search parameters.
     *
     * Searches for all files in a directory matching a given pattern.
     */
    std::vector< std::string > findFiles(
      FIOString const & fileExtension = L".*",
      FIOString const & pathOrID = L"root",
      bool const & recursiveSearch = RecursiveSearchTrue ) const;

    /*--------------------------------------------------------------------------
     * Read files.
    --------------------------------------------------------------------------*/

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     * Out: the whole contents of a file system path or
     *    stored path identifier.
     *
     * Returns the whole contents of a file system path or
     *    stored path identifier. There doesn't need to
     *    be an open stream on the path. Will reset an open
     *    file stream to its file start before reading */
    std::wstring readFileW( FIOString const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     * Out: the whole contents of a file system path or
     *    stored path identifier.
     *
     * Returns the whole contents of a file system path or
     *    stored path identifier. There doesn't need to
     *    be an open stream on the path. Will reset an open
     *    file stream to its file start before reading */
    std::string readFile( FIOString const & pathOrID );

    /*--------------------------------------------------------------------------
     * Read files into vectors.
    --------------------------------------------------------------------------*/

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *    delim -> the delimiting characters used to split a file
     *        vertically
     *
     * Out: A vector of wide strings split on all delimiters.
     *
     * Returns the contents of a file system path or
     *    stored path identifier, split on all delimiters.
     *    There doesn't need to be an open stream on the path.
     *    Will reset an open file stream to its file start
     *    before reading */
    std::vector< std::wstring > readFileToVectorW(
      FIOString const & pathOrID, FIOString const & delim = L"\n\r" );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *    delim -> the delimiting characters used to split a file
     *        vertically
     *
     * Out: A vector of wide strings split on all delimiters.
     *
     * Returns the contents of a file system path or
     *    stored path identifier, split on all delimiters.
     *    There doesn't need to be an open stream on the path.
     *    Will reset an open file stream to its file start
     *    before reading */
    std::vector< std::string > readFileToVector(
      FIOString const & pathOrID, FIOString const & delim = L"\n\r" );

    /*--------------------------------------------------------------------------
     * Read files into vectors of vectors.
    --------------------------------------------------------------------------*/

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap.
     *
     *     lineDelim -> the delimiting characters used to split a file
     *        horizonally
     *
     *     vertDelim -> the delimiting characters used to split a file
     *        vertically
     *
     * Out: A vector with pathOrID's contents split on all delimiters.
     *
     * Returns the contents of a file system path or
     *    stored path identifier, split on all delimiters.
     *    There doesn't need to be an open stream on the path.
     *    Will reset an open file stream to its file start
     *    before reading */
    std::vector< std::vector< std::wstring > > readFileToMatrixW(
      FIOString const & pathOrID, FIOString const & lineDelim = L",",
      FIOString const & vertDelim = L"\n\r" );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap.
     *
     *     lineDelim -> the delimiting characters used to split a file
     *        horizonally
     *
     *     vertDelim -> the delimiting characters used to split a file
     *        vertically
     *
     * Out: A vector with pathOrID's contents split on all delimiters.
     *
     * Returns the contents of a file system path or
     *    stored path identifier, split on all delimiters.
     *    There doesn't need to be an open stream on the path.
     *    Will reset an open file stream to its file start
     *    before reading */
    std::vector< std::vector< std::string > > readFileToMatrix(
      FIOString const & pathOrID, FIOString const & lineDelim = L",",
      FIOString const & vertDelim = L"\n\r" );

    /*--------------------------------------------------------------------------
     * Change the FIO root directory.
    --------------------------------------------------------------------------*/

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     * Out: A reference to the FIO Singleton.
     *
     * Sets the root directory of FIO. Defaults to the directory of the
     *    executable. */
    FIO & setRoot( FIOString const & pathOrID );

    /*--------------------------------------------------------------------------
     * Store a file path in the FIO path map.
    --------------------------------------------------------------------------*/

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a file.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setFileID( FIOString const & id, FIOString const & path );

    /*--------------------------------------------------------------------------
     * Store a directory path in the FIO path map.
    --------------------------------------------------------------------------*/

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a directory.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setDirID( FIOString const & id, FIOString const & path );

    /*--------------------------------------------------------------------------
     * Retrieve path to the root directory.
    --------------------------------------------------------------------------*/

    /* In: None.
     *
     * Out: The absolute filesystem path to the stored root directory.
     *
     * Returns the absolute filesystem path to the stored root directory. */
    std::wstring getRootW() const;

    /* In: None.
     *
     * Out: The absolute filesystem path to the stored root directory.
     *
     * Returns the absolute filesystem path to the stored root directory. */
    std::string getRoot() const;

    /*--------------------------------------------------------------------------
     * Retrieve a file system path.
    --------------------------------------------------------------------------*/

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap.
     *
     * Out: The absolute filesystem path associated
     *    with the id stored in the FIOpath map. If the id provided is
     *    itself a path, it will be returned unaltered.
     *
     * Returns the absolute filesystem path associated
     *    with the id stored in the FIOpath map. If the id provided is
     *    itself a path, it will be returned unaltered. */
    std::wstring getPathW( FIOString const & pathOrID ) const;

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap.
     *
     * Out: The absolute filesystem path associated
     *    with the id stored in the FIOpath map. If the id provided is
     *    itself a path, it will be returned unaltered.
     *
     * Returns the absolute filesystem path associated
     *    with the id stored in the FIOpath map. If the id provided is
     *    itself a path, it will be returned unaltered. */
    std::string getPath( FIOString const & pathOrID ) const;

    /*--------------------------------------------------------------------------
     * Delete a filepath from filepaths.
    --------------------------------------------------------------------------*/

    /* In: id -> the identifier to be used to find an absolute
     *        filesystem path in the FIO pathmap.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Removes the path located in the FIO path map at the id provided. */
    FIO & removePath( FIOString const & id );

  protected:
    /*--------------------------------------------------------------------------
     * Construct FIO.
    --------------------------------------------------------------------------*/

    /* In: ( Optional ) loc -> the locale string to set on construction.
     *                      Defaults to "en_us.UTF-8"
     *
     * Out: FIO.
     *
     * Constructs FIO. */
    explicit FIO( FIOString const & loc = L"en_US.UTF-8" );

    /* The Singleton instance containing FIO. */
    static std::unique_ptr< FIO > instance;

  private:
    /*--------------------------------------------------------------------------
     * Find root dir.
    --------------------------------------------------------------------------*/

    std::wstring findRoot() const;

    /*--------------------------------------------------------------------------
     * Validate streams.
    --------------------------------------------------------------------------*/

    /* In: id -> the id of a stream in the FIO input stream map.
     *
     * Out: The stream pointed to.
     *
     * Checks that the iterator provided points to a valid stream. */
    std::wistream & validateWIFStream( FIOString const & id ) const;

    /* In: id -> the id of a stream in the FIO output stream map.
     *
     * Out: The stream pointed to.
     *
     * Checks that the iterator provided points to a valid stream. */
    std::wostream & validateWOFStream( FIOString const & id ) const;

    /*--------------------------------------------------------------------------
     * Internal storage for paths.
    --------------------------------------------------------------------------*/

    /* The Path Map in which paths to files and directories are stored */
    std::unordered_map< std::wstring, std::wstring > PM;

    /* The input File  Map in which input File s are stored */
    std::unordered_map< std::wstring, std::unique_ptr< std::wifstream > > iFSM;

    /* The output File  Map in which output File s are stored */
    std::unordered_map< std::wstring, std::unique_ptr< std::wofstream > > oFSM;
  };

  /*----------------------------------------------------------------------------
   * Convertible wide <-> multi-byte String Wrapper
  ----------------------------------------------------------------------------*/

  inline FIOString::FIOString( FIOString const & fs ) : mbStr( fs.mbStr ), wStr( fs.wStr )
  {
  }

  inline FIOString & FIOString::operator=( FIOString const & fs )
  {
    mbStr = fs.mbStr;
    wStr = fs.wStr;
    return *this;
  }

  inline FIOString::FIOString( std::string const & s ) : mbStr( s ), wStr( mB2w( s ) ) {}

  inline FIOString & FIOString::operator=( std::string const & s )
  {
    mbStr = s;
    wStr = mB2w( s );
    return *this;
  }

  inline FIOString::FIOString( char const * const & c ) : FIOString( std::string( c ) )
  {
  }

  inline FIOString & FIOString::operator=( char const * const & c )
  {
    return operator=( std::string( c ) );
  }

  inline FIOString::FIOString( std::wstring const & ws ) :
    mbStr( w2mB( ws ) ), wStr( ws )
  {
  }

  inline FIOString & FIOString::operator=( std::wstring const & ws )
  {
    mbStr = w2mB( ws );
    wStr = ws;
    return *this;
  }

  inline FIOString::FIOString( wchar_t const * const & wc ) :
    FIOString( std::wstring( wc ) )
  {
  }

  inline FIOString & FIOString::operator=( wchar_t const * const & wc )
  {
    return operator=( std::wstring( wc ) );
  }

  inline FIOString::operator std::string() const { return mbStr; }

  inline FIOString::operator std::wstring() const { return wStr; }

  inline FIOString::operator std::string &() { return mbStr; }

  inline FIOString::operator std::wstring &() { return wStr; }

  inline std::string FIOString::str() { return operator std::string(); }

  inline std::wstring FIOString::wstr() { return operator std::wstring(); }

  inline std::string FIOString::w2mB( std::wstring const & ws ) const
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

  inline std::wstring FIOString::mB2w( std::string const & mb ) const
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

  inline std::wstring baseFileW( FIOString const & pathToFile,
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

  inline std::string baseFile( FIOString const & pathToFile,
    bool const & stripExtension /* = StripExtensionTrue */ )
  {
    return FIOString( baseFileW( pathToFile, stripExtension ) );
  }

  /*----------------------------------------------------------------------------
   * Get the parent directory of a filepath.
  ----------------------------------------------------------------------------*/

  inline std::wstring parentDirW( FIOString const & path )
  {
    std::wstring ws = path;
    std::size_t delim = ws.find_last_of( L"/\\" );

    return ws.substr( 0, delim );
  }

  inline std::string parentDir( FIOString const & path )
  {
    return FIOString( parentDirW( path ) );
  }

  /*----------------------------------------------------------------------------
   * Split strings. (CSV)
  ----------------------------------------------------------------------------*/

  inline std::vector< std::wstring > splitStringW(
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

  inline std::vector< std::string > splitString(
    FIOString const & source, FIOString const & del /* = L"\n\r" */ )
  {
    auto v = splitStringW( source, del );
    std::vector< std::string > ret;

    for ( auto & ws : v )
    {
      ret.push_back( FIOString( ws ) );
    }

    return ret;
  }

  /*----------------------------------------------------------------------------
   * FIO exception class.
  ----------------------------------------------------------------------------*/

  inline FIOExcept::FIOExcept( FIOString const & output )
  {
    this->output = output;
  };

  inline FIOExcept::~FIOExcept() throw() {};

  inline char const * FIOExcept::what() const throw() { return output.c_str(); };

  /*----------------------------------------------------------------------------
   * FIO.
  ----------------------------------------------------------------------------*/

  inline std::unique_ptr< FIO > FIO::instance = std::unique_ptr< FIO >( nullptr );

  /*----------------------------------------------------------------------------
   * Construct FIO Singleton.
  ----------------------------------------------------------------------------*/

  inline FIO & FIO::getInstance( FIOString const & loc /* = L"en_US.UTF-8" */ )
  {
    if ( ! instance ) { instance = std::unique_ptr< FIO >( new FIO( loc ) ); }

    return *instance;
  }

  /*----------------------------------------------------------------------------
   * Destructor.
  ----------------------------------------------------------------------------*/

  inline FIO::~FIO() { reset(); }

  /*----------------------------------------------------------------------------
   * Return FIO to its initial state.
  ----------------------------------------------------------------------------*/

  inline FIO & FIO::reset()
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

  inline std::wistream & FIO::openWI( FIOString const & pathOrID )
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

  inline std::wostream & FIO::openWO(
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

  inline std::wistream & FIO::rewindWI( FIOString const & pathOrID )
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

  inline FIO & FIO::closeWI( FIOString const & pathOrID )
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
  inline FIO & FIO::closeWO( FIOString const & pathOrID )
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

  inline std::wistream & FIO::getWI( FIOString const & pathOrID ) const
  {
    std::wstring path = getPathW( pathOrID );

    return validateWIFStream( path );
  }

  /*----------------------------------------------------------------------------
   * Get an open output stream.
  ----------------------------------------------------------------------------*/

  inline std::wostream & FIO::getWO( FIOString const & pathOrID ) const
  {
    std::wstring path = getPathW( pathOrID );

    return validateWOFStream( path );
  }

  /*----------------------------------------------------------------------------
   * Get a sorted listing of open input streams.
  ----------------------------------------------------------------------------*/

  inline std::vector< std::wstring > FIO::listOpenWIW() const
  {
    std::vector< std::wstring > streams;

    for ( auto & stream : iFSM )
    { streams.push_back( stream.first + L" | " + getPathW( stream.first ) ); }

    std::sort( streams.begin(), streams.end() );

    return streams;
  }

  inline std::vector< std::string > FIO::listOpenWI() const
  {
    std::vector< std::wstring > wstreams = listOpenWIW();
    std::vector< std::string > streams;

    for ( auto & stream : wstreams )
    {
      std::string s = FIOString( stream );
      streams.push_back( s );
    }

    return streams;
  }

  /*----------------------------------------------------------------------------
   * Get a sorted listing of open output streams.
  ----------------------------------------------------------------------------*/

  inline std::vector< std::wstring > FIO::listOpenWOW() const
  {
    std::vector< std::wstring > streams;

    for ( auto & stream : oFSM )
    { streams.push_back( stream.first + L" | " + getPathW( stream.first ) ); }

    std::sort( streams.begin(), streams.end() );

    return streams;
  }

  inline std::vector< std::string > FIO::listOpenWO() const
  {
    std::vector< std::wstring > wstreams = listOpenWOW();
    std::vector< std::string > streams;

    for ( auto & stream : wstreams )
    {
      std::string s = FIOString( stream );
      streams.push_back( s );
    }

    return streams;
  }

  /*----------------------------------------------------------------------------
   * Read from an input stream.
  ----------------------------------------------------------------------------*/

  inline std::wstring FIO::readLineW( FIOString const & pathOrID )
  {
    std::wstring path = getPathW( pathOrID );
    std::wstring ws;

    std::getline( validateWIFStream( path ), ws );

    return ws;
  }

  inline std::string FIO::readLine( FIOString const & pathOrID )
  {
    return FIOString( readLineW( pathOrID ) );
  }

  /*----------------------------------------------------------------------------
   * Write to an output stream.
  ----------------------------------------------------------------------------*/

  inline FIO & FIO::writeLineW( FIOString const & pathOrID, FIOString const & source )
  {
    std::wstring path = getPathW( pathOrID );
    std::wstring src = source;

    validateWOFStream( path ) << src;

    return *this;
  }

  inline FIO & FIO::writeLine( FIOString const & pathOrID, FIOString const & source )
  {
    return writeLineW( pathOrID, source );
  }

  /*----------------------------------------------------------------------------
   * Search for files within a directory.
  ----------------------------------------------------------------------------*/

  inline std::vector< std::wstring > FIO::findFilesW(
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

  inline std::vector< std::string > FIO::findFiles(
    FIOString const & fileExtension /* = L".*" */,
    FIOString const & pathOrID /* = L"root" */,
    bool const & recursiveSearch /* = RecursiveSearchTrue */ ) const
  {
    auto foundFiles = findFilesW( fileExtension, pathOrID, recursiveSearch );

    std::vector< std::string > files;

    for ( auto & ws : foundFiles )
    {
      std::string s = FIOString( ws );
      files.push_back( s );
    }

    return files;
  }

  /*----------------------------------------------------------------------------
   * Read files.
  ----------------------------------------------------------------------------*/

  inline std::wstring FIO::readFileW( FIOString const & pathOrID )
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

  inline std::string FIO::readFile( FIOString const & pathOrID )
  {
    return FIOString( readFileW( pathOrID ) );
  }

  /*----------------------------------------------------------------------------
   * Read files into vectors.
  ----------------------------------------------------------------------------*/

  inline std::vector< std::wstring > FIO::readFileToVectorW(
    FIOString const & pathOrID, FIOString const & delim /* = L"\n\r" */ )
  {
    return splitStringW( readFileW( pathOrID ), delim );
  }

  inline std::vector< std::string > FIO::readFileToVector(
    FIOString const & pathOrID, FIOString const & delim /* = L"\n\r" */ )
  {
    return splitString( readFile( pathOrID ), delim );
  }

  /*----------------------------------------------------------------------------
   * Read files into vectors of vectors.
  ----------------------------------------------------------------------------*/

  inline std::vector< std::vector< std::wstring > > FIO::readFileToMatrixW(
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

  inline std::vector< std::vector< std::string > > FIO::readFileToMatrix(
    FIOString const & pathOrID, FIOString const & lineDelim /* = L"," */,
    FIOString const & vertDelim /* = L"\n\r" */ )
  {
    auto wMatrix = readFileToMatrixW( pathOrID, lineDelim, vertDelim );
    std::vector< std::vector< std::string > > ret;

    for ( auto & line : wMatrix )
    {
      std::vector< std::string > vLine;
      for ( auto & ws : line )
      {
        std::string s = FIOString( ws );
        vLine.push_back( s );
      }
      ret.push_back( vLine );
    }

    return ret;
  }

  /*----------------------------------------------------------------------------
   * Change the FIO root directory.
  ----------------------------------------------------------------------------*/

  inline FIO & FIO::setRoot( FIOString const & pathOrID )
  {
    return setDirID( L"root", pathOrID );
  }

  /*----------------------------------------------------------------------------
   * Store a file path in the FIO path map.
  ----------------------------------------------------------------------------*/

  inline FIO & FIO::setFileID( FIOString const & id, FIOString const & path )
  {
    std::wstring idW = id;
    std::wstring pathW = path;
    std::string pathS = path;

    errno = 0;

    struct stat info;

    if ( stat( pathS.c_str(), &info ) == 0 )
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
      perror( pathS.c_str() );
    }

    return *this;
  }

  /*----------------------------------------------------------------------------
   * Store a directory path in the FIO path map.
  ----------------------------------------------------------------------------*/

  inline FIO & FIO::setDirID( FIOString const & id, FIOString const & path )
  {
    std::wstring idW = id;
    std::wstring pathW = path;
    std::string pathS = path;

    errno = 0;

    struct stat info;

    if ( stat( pathS.c_str(), &info ) == 0 )
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
      perror( pathS.c_str() );
    }

    return *this;
  }

  /*----------------------------------------------------------------------------
   * Retrieve path to the root directory.
  ----------------------------------------------------------------------------*/

  inline std::wstring FIO::getRootW() const { return getPathW( L"root" ); }

  inline std::string FIO::getRoot() const { return getPath( L"root" ); }

  /*----------------------------------------------------------------------------
   * Retrieve a file system path.
  ----------------------------------------------------------------------------*/

  inline std::wstring FIO::getPathW( FIOString const & pathOrID ) const
  {
    std::wstring pathOrIDW = pathOrID;
    std::string pathOrIDS = pathOrID;

    errno = 0;

    struct stat info;

    if ( stat( pathOrIDS.c_str(), &info ) == 0
      && ( info.st_mode & S_IFREG 
        || info.st_mode & S_IFDIR ) )
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

  inline std::string FIO::getPath( FIOString const & pathOrID ) const
  {
    return FIOString( getPathW( pathOrID ) );
  }

  /*----------------------------------------------------------------------------
   * Delete a filepath from filepaths.
  ----------------------------------------------------------------------------*/

  inline FIO & FIO::removePath( FIOString const & id )
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

  inline FIO::FIO( FIOString const & loc /* = "en_US.UTF-8" */ )
  {
    std::string locS = loc;

    setlocale( LC_ALL, locS.c_str() );
    setRoot( findRoot() );
  }

  /*----------------------------------------------------------------------------
   * Find root dir.
  ----------------------------------------------------------------------------*/

  inline std::wstring FIO::findRoot() const
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

  inline std::wistream & FIO::validateWIFStream( FIOString const & id ) const
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

  inline std::wostream & FIO::validateWOFStream( FIOString const & id ) const
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

#endif
