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

#ifndef FILE_IO_H_
#define FILE_IO_H_

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unordered_map>
#include <vector>
#include <cwchar>

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
   * Wide String and Narrow String conversion wrapper.
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

    /* In: s -> a narrow string.
     *
     * Out: An FIOString populated with s.
     *
     * Construct a populated FIOString. */
    FIOString( std::string const & s );

    /* In: s -> a narrow string.
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
     * Out: A multi-byte narrow string.
     *
     * Construct a narrow string with the FIOString's contents. */
    operator std::string() const;

    /* In: None.
     *
     * Out: A wide string.
     *
     * Construct a wide string with the FIOString's contents. */
    operator std::wstring() const;

    /* In: None.
     *
     * Out: A wide string.
     *
     * Return the FIOString's contents as a string. */
    operator std::string &();

    /* In: None.
     *
     * Out: A wide string.
     *
     * Return the FIOString's contents as a wide string. */
    operator std::wstring &();

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
    std::string str;
    std::wstring strW;
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
    std::string _output;
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
} // namespace NSFIO

#endif
