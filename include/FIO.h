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
   * Convert between normal strings and wide strings.
  ----------------------------------------------------------------------------*/

  /* In: wideString -> the wstring to be converted into a multibyte normal
   *        string.
   *
   * Out: The converted multibyte normal string.
   *
   * Turn a wide string into a multibyte normal string. */
  std::string wideToMultiByte( std::wstring const & wideString );

  /* In: normalString -> the multibyte normal string to be converted
   * into a wide string.
   *
   * Out: The converted wide string.
   *
   * Turn a multibyte normal string into a wide string. */
  std::wstring multiByteToWide( std::string const & normalString );

  /*----------------------------------------------------------------------------
   * Filepath manipulation.
  ----------------------------------------------------------------------------*/

  /* In: pathToFile -> an absolute file system path to a file.
   *
   * Out: The path provided, with all leading directories trimmed away.
   *
   * Get the name of a file in its local directory from a full path
   *    to the file. */
  std::wstring baseFileW( std::wstring const & pathToFile,
    bool const & stripExtension = StripExtensionTrue );

  /* In: pathToFile -> an absolute file system path to a file.
   *
   * Out: The path provided, with all leading directories trimmed away.
   *
   * Get the name of a file in its local directory from a full path
   *    to the file. */
  std::wstring baseFileW( std::string const & pathToFile,
    bool const & stripExtension = StripExtensionTrue );

  /* In: pathToFile -> an absolute file system path to a file.
   *
   * Out: The path provided, with all leading directories trimmed away.
   *
   * Get the name of a file in its local directory from a full path
   *    to the file. */
  std::wstring baseFileW( wchar_t const * const & pathToFile,
    bool const & stripExtension = StripExtensionTrue );

  /* In: pathToFile -> an absolute file system path to a file.
   *
   * Out: The path provided, with all leading directories trimmed away.
   *
   * Get the name of a file in its local directory from a full path
   *    to the file. */
  std::wstring baseFileW( char const * const & pathToFile,
    bool const & stripExtension = StripExtensionTrue );

  /* In: pathToFile -> an absolute file system path to a file.
   *
   * Out: The path provided, with all leading directories trimmed away.
   *
   * Get the name of a file in its local directory from a full path
   *    to the file. */
  std::string baseFile( std::wstring const & pathToFile,
    bool const & stripExtension = StripExtensionTrue );

  /* In: pathToFile -> an absolute file system path to a file.
   *
   * Out: The path provided, with all leading directories trimmed away.
   *
   * Get the name of a file in its local directory from a full path
   *    to the file. */
  std::string baseFile( std::string const & pathToFile,
    bool const & stripExtension = StripExtensionTrue );

  /* In: pathToFile -> an absolute file system path to a file.
   *
   * Out: The path provided, with all leading directories trimmed away.
   *
   * Get the name of a file in its local directory from a full path
   *    to the file. */
  std::string baseFile( wchar_t const * const & pathToFile,
    bool const & stripExtension = StripExtensionTrue );

  /* In: pathToFile -> an absolute file system path to a file.
   *
   * Out: The path provided, with all leading directories trimmed away.
   *
   * Get the name of a file in its local directory from a full path
   *    to the file. */
  std::string baseFile( char const * const & pathToFile,
    bool const & stripExtension = StripExtensionTrue );

  /*----------------------------------------------------------------------------
   * Get the parent directory of a filepath.
  ----------------------------------------------------------------------------*/

  /* In: path -> a file system path.
   *
   * Out: The parent directory of the path provided.
   *
   * Get the parent directory of the path provided. */
  std::wstring parentDirW( std::wstring const & path );

  /* In: path -> a file system path.
   *
   * Out: The parent directory of the path provided.
   *
   * Get the parent directory of the path provided. */
  std::wstring parentDirW( std::string const & path );

  /* In: path -> a file system path.
   *
   * Out: The parent directory of the path provided.
   *
   * Get the parent directory of the path provided. */
  std::wstring parentDirW( wchar_t const * const & path );

  /* In: path -> a file system path.
   *
   * Out: The parent directory of the path provided.
   *
   * Get the parent directory of the path provided. */
  std::wstring parentDirW( char const * const & path );

  /* In: path -> a file system path.
   *
   * Out: The parent directory of the path provided.
   *
   * Get the parent directory of the path provided. */
  std::string parentDir( std::wstring const & path );

  /* In: path -> a file system path.
   *
   * Out: The parent directory of the path provided.
   *
   * Get the parent directory of the path provided. */
  std::string parentDir( std::string const & path );

  /* In: path -> a file system path.
   *
   * Out: The parent directory of the path provided.
   *
   * Get the parent directory of the path provided. */
  std::string parentDir( wchar_t const * const & path );

  /* In: path -> a file system path.
   *
   * Out: The parent directory of the path provided.
   *
   * Get the parent directory of the path provided. */
  std::string parentDir( char const * const & path );

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
    std::wstring const & source, std::wstring const & delim = L"\n\r" );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::wstring > splitStringW(
    std::wstring const & source, std::string const & delim /* = "\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::wstring > splitStringW( std::wstring const & source,
    wchar_t const * const & delim /* = L"\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::wstring > splitStringW(
    std::wstring const & source, char const * const & delim /* = "\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::wstring > splitStringW(
    std::string const & source, std::wstring const & delim = L"\n\r" );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::wstring > splitStringW(
    std::string const & source, std::string const & delim /* = "\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::wstring > splitStringW(
    std::string const & source, wchar_t const * const & delim /* = L"\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::wstring > splitStringW(
    std::string const & source, char const * const & delim /* = "\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::wstring > splitStringW(
    wchar_t const * const & source, std::wstring const & delim = L"\n\r" );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::wstring > splitStringW(
    wchar_t const * const & source, std::string const & delim /* = "\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::wstring > splitStringW( wchar_t const * const & source,
    wchar_t const * const & delim /* = L"\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::wstring > splitStringW(
    wchar_t const * const & source, char const * const & delim /* = "\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::wstring > splitStringW(
    char const * const & source, std::wstring const & delim = L"\n\r" );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::wstring > splitStringW(
    char const * const & source, std::string const & delim /* = "\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::wstring > splitStringW( char const * const & source,
    wchar_t const * const & delim /* = L"\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::wstring > splitStringW(
    char const * const & source, char const * const & delim /* = "\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::string > splitString(
    std::wstring const & source, std::wstring const & delim = L"\n\r" );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::string > splitString(
    std::wstring const & source, std::string const & delim /* = "\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::string > splitString( std::wstring const & source,
    wchar_t const * const & delim /* = L"\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::string > splitString(
    std::wstring const & source, char const * const & delim /* = "\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::string > splitString(
    std::string const & source, std::wstring const & delim = L"\n\r" );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::string > splitString(
    std::string const & source, std::string const & delim /* = "\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::string > splitString(
    std::string const & source, wchar_t const * const & delim /* = L"\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::string > splitString(
    std::string const & source, char const * const & delim /* = "\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::string > splitString(
    wchar_t const * const & source, std::wstring const & delim = L"\n\r" );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::string > splitString(
    wchar_t const * const & source, std::string const & delim /* = "\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::string > splitString( wchar_t const * const & source,
    wchar_t const * const & delim /* = L"\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::string > splitString(
    wchar_t const * const & source, char const * const & delim /* = "\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::string > splitString(
    char const * const & source, std::wstring const & delim = L"\n\r" );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::string > splitString(
    char const * const & source, std::string const & delim /* = "\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::string > splitString( char const * const & source,
    wchar_t const * const & delim /* = L"\n\r" */ );

  /* In: source -> a string to split.
   *
   *     ( Optional ) delim -> a string of character delimiters.
   *          Ex. "\n\r" will split on both '\n' and '\r'.
   *
   * Out: The source split on a set of delimiters.
   *
   * Split a string on one or more delimiters. */
  std::vector< std::string > splitString(
    char const * const & source, char const * const & delim /* = "\n\r" */ );

  /*----------------------------------------------------------------------------
   * FIOExcept.
  ----------------------------------------------------------------------------*/
  class FIOExcept : public std::exception
  {
  public:
    /* In: output -> the error message to be presented when throwing an error.
     *
     * Out: The constructed FIOExcept.
     *
     * Construct a new FIO-related exception. */
    FIOExcept( std::wstring output );

    /* In: output -> the error message to be presented when throwing an error.
     *
     * Out: The constructed FIOExcept.
     *
     * Construct a new FIO-related exception. */
    FIOExcept( std::string output );

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
    static FIO & getInstance( std::wstring const & loc = L"en_US.UTF-8" );

    /* In: ( Optional ) loc -> the locale string to set on construction.
     *                      Defaults to "en_us.UTF-8"
     *
     * Out: A reference to the FIO Singleton.
     *
     * If FIO has not been constructed, it will be constructed. If FIO was
     *    already constructed, a reference to the existing FIO Singleton
     *    will be returned. */
    static FIO & getInstance( std::string const & loc );

    /* In: ( Optional ) loc -> the locale string to set on construction.
     *                      Defaults to "en_us.UTF-8"
     *
     * Out: A reference to the FIO Singleton.
     *
     * If FIO has not been constructed, it will be constructed. If FIO was
     *    already constructed, a reference to the existing FIO Singleton
     *    will be returned. */
    static FIO & getInstance( wchar_t const * const & loc );

    /* In: ( Optional ) loc -> the locale string to set on construction.
     *                      Defaults to "en_us.UTF-8"
     *
     * Out: A reference to the FIO Singleton.
     *
     * If FIO has not been constructed, it will be constructed. If FIO was
     *    already constructed, a reference to the existing FIO Singleton
     *    will be returned. */
    static FIO & getInstance( char const * const & loc );

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
    std::wistream & openWI( std::wstring const & pathOrID );

    /* In: pathOrID -> an absolute filesystem path, or the identifier of a
     *        an absolute filesystem path stored in the FIO pathmap.
     *
     * Out: A file stream opened on the path specified.
     *
     * If pathOrID points to a regular file it will be opened, otherwise
     *    pathOrID will be treated as an identifier for an element
     *    in the pathmap, and the associated path will be opened. */
    std::wistream & openWI( std::string const & pathOrID );

    /* In: pathOrID -> an absolute filesystem path, or the identifier of a
     *        an absolute filesystem path stored in the FIO pathmap.
     *
     * Out: A file stream opened on the path specified.
     *
     * If pathOrID points to a regular file it will be opened, otherwise
     *    pathOrID will be treated as an identifier for an element
     *    in the pathmap, and the associated path will be opened. */
    std::wistream & openWI( wchar_t const * const & pathOrID );

    /* In: pathOrID -> an absolute filesystem path, or the identifier of a
     *        an absolute filesystem path stored in the FIO pathmap.
     *
     * Out: A file stream opened on the path specified.
     *
     * If pathOrID points to a regular file it will be opened, otherwise
     *    pathOrID will be treated as an identifier for an element
     *    in the pathmap, and the associated path will be opened. */
    std::wistream & openWI( char const * const & pathOrID );

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
      std::wstring const & pathOrID, bool const & appendToFile = OpenNewFile );

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
      std::string const & pathOrID, bool const & appendToFile = OpenNewFile );

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
    std::wostream & openWO( wchar_t const * const & pathOrID,
      bool const & appendToFile = OpenNewFile );

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
      char const * const & pathOrID, bool const & appendToFile = OpenNewFile );

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
    std::wistream & rewindWI( std::wstring const & pathOrID );

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
    std::wistream & rewindWI( std::string const & pathOrID );

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
    std::wistream & rewindWI( wchar_t const * const & pathOrID );

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
    std::wistream & rewindWI( char const * const & pathOrID );

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
    FIO & closeWI( std::wstring const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: A reference to the FIO Singleton.
     *
     * Closes the open input file stream stored in the
     *    input stream map at pathOrID. */
    FIO & closeWI( std::string const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: A reference to the FIO Singleton.
     *
     * Closes the open input file stream stored in the
     *    input stream map at pathOrID. */
    FIO & closeWI( wchar_t const * const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: A reference to the FIO Singleton.
     *
     * Closes the open input file stream stored in the
     *    input stream map at pathOrID. */
    FIO & closeWI( char const * const & pathOrID );

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
    FIO & closeWO( std::wstring const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     * Out: A reference to the FIO Singleton.
     *
     * Closes the open output file stream stored in the
     *    output stream map at pathOrID. */
    FIO & closeWO( std::string const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     * Out: A reference to the FIO Singleton.
     *
     * Closes the open output file stream stored in the
     *    output stream map at pathOrID. */
    FIO & closeWO( wchar_t const * const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     * Out: A reference to the FIO Singleton.
     *
     * Closes the open output file stream stored in the
     *    output stream map at pathOrID. */
    FIO & closeWO( char const * const & pathOrID );

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
    std::wistream & getWI( std::wstring const & pathOrID ) const;

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: The open input stream.
     *
     * Returns the open input file stream stored in the
     *    input stream map at pathOrID. */
    std::wistream & getWI( std::string const & pathOrID ) const;

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: The open input stream.
     *
     * Returns the open input file stream stored in the
     *    input stream map at pathOrID. */
    std::wistream & getWI( wchar_t const * const & pathOrID ) const;

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: The open input stream.
     *
     * Returns the open input file stream stored in the
     *    input stream map at pathOrID. */
    std::wistream & getWI( char const * const & pathOrID ) const;

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
    std::wostream & getWO( std::wstring const & pathOrID ) const;

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     * Out: The open output stream.
     *
     * Returns the open output file stream stored in the
     *    output stream map at pathOrID. */
    std::wostream & getWO( std::string const & pathOrID ) const;

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     * Out: The open output stream.
     *
     * Returns the open output file stream stored in the
     *    output stream map at pathOrID. */
    std::wostream & getWO( wchar_t const * const & pathOrID ) const;

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     * Out: The open output stream.
     *
     * Returns the open output file stream stored in the
     *    output stream map at pathOrID. */
    std::wostream & getWO( char const * const & pathOrID ) const;

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
     *     dest -> the string to which the unaltered line should be written.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes the next line in the file stream, unaltered, to
     *    the destination string. */
    FIO & readLineW( std::wstring const & pathOrID, std::wstring & dest );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     *     dest -> the string to which the unaltered line should be written.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes the next line in the file stream, unaltered, to
     *    the destination string. */
    FIO & readLineW( std::string const & pathOrID, std::wstring & dest );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     *     dest -> the string to which the unaltered line should be written.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes the next line in the file stream, unaltered, to
     *    the destination string. */
    FIO & readLineW( wchar_t const * const & pathOrID, std::wstring & dest );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     *     dest -> the string to which the unaltered line should be written.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes the next line in the file stream, unaltered, to
     *    the destination string. */
    FIO & readLineW( char const * const & pathOrID, std::wstring & dest );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     *     dest -> the string to which the unaltered line should be written.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes the next line in the file stream, unaltered, to
     *    the destination string. */
    FIO & readLine( std::wstring const & pathOrID, std::string & dest );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     *     dest -> the string to which the unaltered line should be written.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes the next line in the file stream, unaltered, to
     *    the destination string. */
    FIO & readLine( std::string const & pathOrID, std::string & dest );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     *     dest -> the string to which the unaltered line should be written.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes the next line in the file stream, unaltered, to
     *    the destination string. */
    FIO & readLine( wchar_t const * const & pathOrID, std::string & dest );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     *     dest -> the string to which the unaltered line should be written.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes the next line in the file stream, unaltered, to
     *    the destination string. */
    FIO & readLine( char const * const & pathOrID, std::string & dest );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: The next line in the file stream, unaltered.
     *
     * Returns the next line in the file stream, unaltered. */
    std::wstring readLineW( std::wstring const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: The next line in the file stream, unaltered.
     *
     * Returns the next line in the file stream, unaltered. */
    std::wstring readLineW( std::string const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: The next line in the file stream, unaltered.
     *
     * Returns the next line in the file stream, unaltered. */
    std::wstring readLineW( wchar_t const * const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: The next line in the file stream, unaltered.
     *
     * Returns the next line in the file stream, unaltered. */
    std::wstring readLineW( char const * const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: The next line in the file stream, unaltered.
     *
     * Returns the next line in the file stream, unaltered. */
    std::string readLine( std::wstring const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: The next line in the file stream, unaltered.
     *
     * Returns the next line in the file stream, unaltered. */
    std::string readLine( std::string const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: The next line in the file stream, unaltered.
     *
     * Returns the next line in the file stream, unaltered. */
    std::string readLine( wchar_t const * const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWI().
     *
     * Out: The next line in the file stream, unaltered.
     *
     * Returns the next line in the file stream, unaltered. */
    std::string readLine( char const * const & pathOrID );

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
    FIO & writeLineW(
      std::wstring const & pathOrID, std::wstring const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLineW(
      std::wstring const & pathOrID, std::string const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLineW(
      std::wstring const & pathOrID, wchar_t const * const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLineW(
      std::wstring const & pathOrID, char const * const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLineW(
      std::string const & pathOrID, std::wstring const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLineW(
      std::string const & pathOrID, std::string const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLineW(
      std::string const & pathOrID, wchar_t const * const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLineW(
      std::string const & pathOrID, char const * const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLineW(
      wchar_t const * const & pathOrID, std::wstring const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLineW(
      wchar_t const * const & pathOrID, std::string const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLineW(
      wchar_t const * const & pathOrID, wchar_t const * const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLineW(
      wchar_t const * const & pathOrID, char const * const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLineW(
      char const * const & pathOrID, std::wstring const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLineW(
      char const * const & pathOrID, std::string const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLineW(
      char const * const & pathOrID, wchar_t const * const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLineW(
      char const * const & pathOrID, char const * const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLine(
      std::wstring const & pathOrID, std::wstring const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLine(
      std::wstring const & pathOrID, std::string const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLine(
      std::wstring const & pathOrID, wchar_t const * const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLine(
      std::wstring const & pathOrID, char const * const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLine(
      std::string const & pathOrID, std::wstring const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLine( std::string const & pathOrID, std::string const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLine(
      std::string const & pathOrID, wchar_t const * const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLine(
      std::string const & pathOrID, char const * const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLine(
      wchar_t const * const & pathOrID, std::wstring const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLine(
      wchar_t const * const & pathOrID, std::string const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLine(
      wchar_t const * const & pathOrID, wchar_t const * const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLine(
      wchar_t const * const & pathOrID, char const * const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLine(
      char const * const & pathOrID, std::wstring const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLine(
      char const * const & pathOrID, std::string const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLine(
      char const * const & pathOrID, wchar_t const * const & source );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     *     source -> the string to which should be written, unaltered.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Writes source as an unaltered line to the output stream. */
    FIO & writeLine(
      char const * const & pathOrID, char const * const & source );

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
      std::wstring const & fileExtension = L".*",
      std::wstring const & pathOrID = L"root",
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
    std::vector< std::wstring > findFilesW(
      std::wstring const & fileExtension /* = L".*" */,
      std::string const & pathOrID /* = "root" */,
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
    std::vector< std::wstring > findFilesW(
      std::wstring const & fileExtension /* = L".*" */,
      wchar_t const * const & pathOrID /* = L"root" */,
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
    std::vector< std::wstring > findFilesW(
      std::wstring const & fileExtension /* = L".*" */,
      char const * const & pathOrID /* = "root" */,
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
    std::vector< std::wstring > findFilesW(
      std::string const & fileExtension /* = ".*" */,
      std::wstring const & pathOrID = L"root",
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
    std::vector< std::wstring > findFilesW(
      std::string const & fileExtension /* = ".*" */,
      std::string const & pathOrID /* = "root" */,
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
    std::vector< std::wstring > findFilesW(
      std::string const & fileExtension /* = ".*" */,
      wchar_t const * const & pathOrID /* = L"root" */,
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
    std::vector< std::wstring > findFilesW(
      std::string const & fileExtension /* = ".*" */,
      char const * const & pathOrID /* = "root" */,
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
    std::vector< std::wstring > findFilesW(
      wchar_t const * const & fileExtension /* = L".*" */,
      std::wstring const & pathOrID = L"root",
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
    std::vector< std::wstring > findFilesW(
      wchar_t const * const & fileExtension /* = L".*" */,
      std::string const & pathOrID /* = "root" */,
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
    std::vector< std::wstring > findFilesW(
      wchar_t const * const & fileExtension /* = L".*" */,
      wchar_t const * const & pathOrID /* = L"root" */,
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
    std::vector< std::wstring > findFilesW(
      wchar_t const * const & fileExtension /* = L".*" */,
      char const * const & pathOrID /* = "root" */,
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
    std::vector< std::wstring > findFilesW(
      char const * const & fileExtension /* = ".*" */,
      std::wstring const & pathOrID = L"root",
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
    std::vector< std::wstring > findFilesW(
      char const * const & fileExtension /* = ".*" */,
      std::string const & pathOrID /* = "root" */,
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
    std::vector< std::wstring > findFilesW(
      char const * const & fileExtension /* = ".*" */,
      wchar_t const * const & pathOrID /* = L"root" */,
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
    std::vector< std::wstring > findFilesW(
      char const * const & fileExtension /* = ".*" */,
      char const * const & pathOrID /* = "root" */,
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
      std::wstring const & fileExtension = L".*",
      std::wstring const & pathOrID = L"root",
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
      std::wstring const & fileExtension /* = L".*" */,
      std::string const & pathOrID /* = "root" */,
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
      std::wstring const & fileExtension /* = L".*" */,
      wchar_t const * const & pathOrID /* = L"root" */,
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
      std::wstring const & fileExtension /* = L".*" */,
      char const * const & pathOrID /* = "root" */,
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
      std::string const & fileExtension /* = ".*" */,
      std::wstring const & pathOrID = L"root",
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
      std::string const & fileExtension /* = ".*" */,
      std::string const & pathOrID /* = "root" */,
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
      std::string const & fileExtension /* = ".*" */,
      wchar_t const * const & pathOrID /* = L"root" */,
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
      std::string const & fileExtension /* = ".*" */,
      char const * const & pathOrID /* = "root" */,
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
      wchar_t const * const & fileExtension /* = L".*" */,
      std::wstring const & pathOrID = L"root",
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
      wchar_t const * const & fileExtension /* = L".*" */,
      std::string const & pathOrID /* = "root" */,
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
      wchar_t const * const & fileExtension /* = L".*" */,
      wchar_t const * const & pathOrID /* = L"root" */,
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
      wchar_t const * const & fileExtension /* = L".*" */,
      char const * const & pathOrID /* = "root" */,
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
      char const * const & fileExtension /* = ".*" */,
      std::wstring const & pathOrID = L"root",
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
      char const * const & fileExtension /* = ".*" */,
      std::string const & pathOrID /* = "root" */,
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
      char const * const & fileExtension /* = ".*" */,
      wchar_t const * const & pathOrID /* = L"root" */,
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
      char const * const & fileExtension /* = ".*" */,
      char const * const & pathOrID /* = "root" */,
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
    std::wstring readFileW( std::wstring const & pathOrID );

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
    std::wstring readFileW( std::string const & pathOrID );

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
    std::wstring readFileW( wchar_t const * const & pathOrID );

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
    std::wstring readFileW( char const * const & pathOrID );

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
    std::string readFile( std::wstring const & pathOrID );

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
    std::string readFile( std::string const & pathOrID );

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
    std::string readFile( wchar_t const * const & pathOrID );

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
    std::string readFile( char const * const & pathOrID );

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
      std::wstring const & pathOrID, std::wstring const & delim = L"\n\r" );

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
      std::wstring const & pathOrID, std::string const & delim /* = "\n\r" */ );

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
      std::wstring const & pathOrID,
      wchar_t const * const & delim /* = L"\n\r" */ );

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
      std::wstring const & pathOrID,
      char const * const & delim /* = "\n\r" */ );

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
      std::string const & pathOrID, std::wstring const & delim = L"\n\r" );

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
      std::string const & pathOrID, std::string const & delim /* = "\n\r" */ );

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
    std::vector< std::wstring > readFileToVectorW( std::string const & pathOrID,
      wchar_t const * const & delim /* = L"\n\r" */ );

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
      std::string const & pathOrID, char const * const & delim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID, std::wstring const & delim = L"\n\r" );

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
      wchar_t const * const & pathOrID,
      std::string const & delim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID,
      wchar_t const * const & delim /* = L"\n\r" */ );

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
      wchar_t const * const & pathOrID,
      char const * const & delim /* = "\n\r" */ );

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
      char const * const & pathOrID, std::wstring const & delim = L"\n\r" );

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
      char const * const & pathOrID, std::string const & delim /* = "\n\r" */ );

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
      char const * const & pathOrID,
      wchar_t const * const & delim /* = L"\n\r" */ );

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
      char const * const & pathOrID,
      char const * const & delim /* = "\n\r" */ );

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
      std::wstring const & pathOrID, std::wstring const & delim = L"\n\r" );

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
      std::wstring const & pathOrID, std::string const & delim /* = "\n\r" */ );

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
    std::vector< std::string > readFileToVector( std::wstring const & pathOrID,
      wchar_t const * const & delim /* = L"\n\r" */ );

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
    std::vector< std::string > readFileToVector( std::wstring const & pathOrID,
      char const * const & delim /* = "\n\r" */ );

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
      std::string const & pathOrID, std::wstring const & delim = L"\n\r" );

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
      std::string const & pathOrID, std::string const & delim /* = "\n\r" */ );

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
    std::vector< std::string > readFileToVector( std::string const & pathOrID,
      wchar_t const * const & delim /* = L"\n\r" */ );

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
      std::string const & pathOrID, char const * const & delim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID, std::wstring const & delim = L"\n\r" );

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
      wchar_t const * const & pathOrID,
      std::string const & delim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID,
      wchar_t const * const & delim /* = L"\n\r" */ );

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
      wchar_t const * const & pathOrID,
      char const * const & delim /* = "\n\r" */ );

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
      char const * const & pathOrID, std::wstring const & delim = L"\n\r" );

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
      char const * const & pathOrID, std::string const & delim /* = "\n\r" */ );

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
    std::vector< std::string > readFileToVector( char const * const & pathOrID,
      wchar_t const * const & delim /* = L"\n\r" */ );

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
    std::vector< std::string > readFileToVector( char const * const & pathOrID,
      char const * const & delim /* = "\n\r" */ );

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
      std::wstring const & pathOrID, std::wstring const & lineDelim = L",",
      std::wstring const & vertDelim = L"\n\r" );

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
      std::wstring const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      std::wstring const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      std::wstring const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      std::wstring const & pathOrID, std::string const & lineDelim /*= ","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      std::wstring const & pathOrID, std::string const & lineDelim /*= ","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      std::wstring const & pathOrID, std::string const & lineDelim /*= ","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      std::wstring const & pathOrID, std::string const & lineDelim /*= ","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      std::wstring const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      std::wstring const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      std::wstring const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      std::wstring const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      std::wstring const & pathOrID, char const * const & lineDelim /*= ","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      std::wstring const & pathOrID, char const * const & lineDelim /*= ","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      std::wstring const & pathOrID, char const * const & lineDelim /*= ","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      std::wstring const & pathOrID, char const * const & lineDelim /*= ","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      std::string const & pathOrID, std::wstring const & lineDelim = L",",
      std::wstring const & vertDelim = L"\n\r" );

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
      std::string const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      std::string const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      std::string const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      std::string const & pathOrID, std::string const & lineDelim /*= ","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      std::string const & pathOrID, std::string const & lineDelim /*= ","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      std::string const & pathOrID, std::string const & lineDelim /*= ","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      std::string const & pathOrID, std::string const & lineDelim /*= ","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      std::string const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      std::string const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      std::string const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      std::string const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      std::string const & pathOrID, char const * const & lineDelim /*= ","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      std::string const & pathOrID, char const * const & lineDelim /*= ","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      std::string const & pathOrID, char const * const & lineDelim /*= ","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      std::string const & pathOrID, char const * const & lineDelim /*= ","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID, std::wstring const & lineDelim = L",",
      std::wstring const & vertDelim = L"\n\r" );

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
      wchar_t const * const & pathOrID,
      std::wstring const & lineDelim /*= L","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID,
      std::wstring const & lineDelim /*= L","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      wchar_t const * const & pathOrID,
      std::wstring const & lineDelim /*= L","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID, std::string const & lineDelim /*= ","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      wchar_t const * const & pathOrID, std::string const & lineDelim /*= ","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID, std::string const & lineDelim /*= ","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      wchar_t const * const & pathOrID, std::string const & lineDelim /*= ","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      wchar_t const * const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      wchar_t const * const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID,
      char const * const & lineDelim /*= ","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      wchar_t const * const & pathOrID,
      char const * const & lineDelim /*= ","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID,
      char const * const & lineDelim /*= ","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      wchar_t const * const & pathOrID,
      char const * const & lineDelim /*= ","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      char const * const & pathOrID, std::wstring const & lineDelim = L",",
      std::wstring const & vertDelim = L"\n\r" );

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
      char const * const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      char const * const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      char const * const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      char const * const & pathOrID, std::string const & lineDelim /*= ","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      char const * const & pathOrID, std::string const & lineDelim /*= ","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      char const * const & pathOrID, std::string const & lineDelim /*= ","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      char const * const & pathOrID, std::string const & lineDelim /*= ","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      char const * const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      char const * const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      char const * const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      char const * const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      char const * const & pathOrID, char const * const & lineDelim /*= ","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      char const * const & pathOrID, char const * const & lineDelim /*= ","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      char const * const & pathOrID, char const * const & lineDelim /*= ","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      char const * const & pathOrID, char const * const & lineDelim /*= ","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      std::wstring const & pathOrID, std::wstring const & lineDelim = L",",
      std::wstring const & vertDelim = L"\n\r" );

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
      std::wstring const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      std::wstring const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      std::wstring const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      std::wstring const & pathOrID, std::string const & lineDelim /*= ","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      std::wstring const & pathOrID, std::string const & lineDelim /*= ","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      std::wstring const & pathOrID, std::string const & lineDelim /*= ","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      std::wstring const & pathOrID, std::string const & lineDelim /*= ","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      std::wstring const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      std::wstring const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      std::wstring const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      std::wstring const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      std::wstring const & pathOrID, char const * const & lineDelim /*= ","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      std::wstring const & pathOrID, char const * const & lineDelim /*= ","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      std::wstring const & pathOrID, char const * const & lineDelim /*= ","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      std::wstring const & pathOrID, char const * const & lineDelim /*= ","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      std::string const & pathOrID, std::wstring const & lineDelim = L",",
      std::wstring const & vertDelim = L"\n\r" );

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
      std::string const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      std::string const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      std::string const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      std::string const & pathOrID, std::string const & lineDelim /*= ","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      std::string const & pathOrID, std::string const & lineDelim /*= ","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      std::string const & pathOrID, std::string const & lineDelim /*= ","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      std::string const & pathOrID, std::string const & lineDelim /*= ","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      std::string const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      std::string const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      std::string const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      std::string const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      std::string const & pathOrID, char const * const & lineDelim /*= ","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      std::string const & pathOrID, char const * const & lineDelim /*= ","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      std::string const & pathOrID, char const * const & lineDelim /*= ","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      std::string const & pathOrID, char const * const & lineDelim /*= ","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID, std::wstring const & lineDelim = L",",
      std::wstring const & vertDelim = L"\n\r" );

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
      wchar_t const * const & pathOrID,
      std::wstring const & lineDelim /*= L","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID,
      std::wstring const & lineDelim /*= L","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      wchar_t const * const & pathOrID,
      std::wstring const & lineDelim /*= L","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID, std::string const & lineDelim /*= ","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      wchar_t const * const & pathOrID, std::string const & lineDelim /*= ","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID, std::string const & lineDelim /*= ","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      wchar_t const * const & pathOrID, std::string const & lineDelim /*= ","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      wchar_t const * const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      wchar_t const * const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID,
      char const * const & lineDelim /*= ","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      wchar_t const * const & pathOrID,
      char const * const & lineDelim /*= ","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      wchar_t const * const & pathOrID,
      char const * const & lineDelim /*= ","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      wchar_t const * const & pathOrID,
      char const * const & lineDelim /*= ","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      char const * const & pathOrID, std::wstring const & lineDelim = L",",
      std::wstring const & vertDelim = L"\n\r" );

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
      char const * const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      char const * const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      char const * const & pathOrID, std::wstring const & lineDelim /*= L","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      char const * const & pathOrID, std::string const & lineDelim /*= ","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      char const * const & pathOrID, std::string const & lineDelim /*= ","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      char const * const & pathOrID, std::string const & lineDelim /*= ","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      char const * const & pathOrID, std::string const & lineDelim /*= ","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      char const * const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      char const * const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      char const * const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      char const * const & pathOrID,
      wchar_t const * const & lineDelim /*= L","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
      char const * const & pathOrID, char const * const & lineDelim /*= ","*/,
      std::wstring const & vertDelim = L"\n\r" );

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
      char const * const & pathOrID, char const * const & lineDelim /*= ","*/,
      std::string const & vertDelim /* = "\n\r" */ );

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
      char const * const & pathOrID, char const * const & lineDelim /*= ","*/,
      wchar_t const * const & vertDelim /* = L"\n\r" */ );

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
      char const * const & pathOrID, char const * const & lineDelim /*= ","*/,
      char const * const & vertDelim /* = "\n\r" */ );

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
    FIO & setRoot( std::wstring const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     * Out: A reference to the FIO Singleton.
     *
     * Sets the root directory of FIO. Defaults to the directory of the
     *    executable. */
    FIO & setRoot( std::string const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     * Out: A reference to the FIO Singleton.
     *
     * Sets the root directory of FIO. Defaults to the directory of the
     *    executable. */
    FIO & setRoot( wchar_t const * const & pathOrID );

    /* In: pathOrID -> the identifier of an absolute filesystem path stored
     *        in the FIO pathmap. This value ought to be whatever was
     *        passed in the initial call to openWO().
     *
     * Out: A reference to the FIO Singleton.
     *
     * Sets the root directory of FIO. Defaults to the directory of the
     *    executable. */
    FIO & setRoot( char const * const & pathOrID );

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
    FIO & setFileID( std::wstring const & id, std::wstring const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a file.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setFileID( std::wstring const & id, std::string const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a file.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setFileID( std::wstring const & id, wchar_t const * const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a file.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setFileID( std::wstring const & id, char const * const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a file.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setFileID( std::string const & id, std::wstring const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a file.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setFileID( std::string const & id, std::string const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a file.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setFileID( std::string const & id, wchar_t const * const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a file.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setFileID( std::string const & id, char const * const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a file.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setFileID( wchar_t const * const & id, std::wstring const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a file.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setFileID( wchar_t const * const & id, std::string const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a file.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setFileID( wchar_t const * const & id, wchar_t const * const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a file.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setFileID( wchar_t const * const & id, char const * const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a file.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setFileID( char const * const & id, std::wstring const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a file.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setFileID( char const * const & id, std::string const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a file.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setFileID( char const * const & id, wchar_t const * const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a file.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setFileID( char const * const & id, char const * const & path );

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
    FIO & setDirID( std::wstring const & id, std::wstring const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a directory.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setDirID( std::wstring const & id, std::string const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a directory.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setDirID( std::wstring const & id, wchar_t const * const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a directory.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setDirID( std::wstring const & id, char const * const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a directory.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setDirID( std::string const & id, std::wstring const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a directory.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setDirID( std::string const & id, std::string const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a directory.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setDirID( std::string const & id, wchar_t const * const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a directory.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setDirID( std::string const & id, char const * const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a directory.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setDirID( wchar_t const * const & id, std::wstring const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a directory.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setDirID( wchar_t const * const & id, std::string const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a directory.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setDirID( wchar_t const * const & id, wchar_t const * const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a directory.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setDirID( wchar_t const * const & id, char const * const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a directory.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setDirID( char const * const & id, std::wstring const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a directory.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setDirID( char const * const & id, std::string const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a directory.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setDirID( char const * const & id, wchar_t const * const & path );

    /* In: id -> the identifier to be used to store an absolute
     *        filesystem path in the FIO pathmap.
     *
     *    path -> an absolute filesystem path pointing to a directory.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Stores the path provided in the FIO path map at the id provided.
     * The path can then be accessed using the id for convenience. */
    FIO & setDirID( char const * const & id, char const * const & path );

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
    std::wstring getPathW( std::wstring const & pathOrID ) const;

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
    std::wstring getPathW( std::string const & pathOrID ) const;

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
    std::wstring getPathW( wchar_t const * const & pathOrID ) const;

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
    std::wstring getPathW( char const * const & pathOrID ) const;

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
    std::string getPath( std::wstring const & pathOrID ) const;

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
    std::string getPath( std::string const & pathOrID ) const;

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
    std::string getPath( wchar_t const * const & pathOrID ) const;

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
    std::string getPath( char const * const & pathOrID ) const;

    /*--------------------------------------------------------------------------
     * Delete a filepath from filepaths.
    --------------------------------------------------------------------------*/

    /* In: id -> the identifier to be used to find an absolute
     *        filesystem path in the FIO pathmap.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Removes the path located in the FIO path map at the id provided. */
    FIO & removePath( std::wstring const & id );

    /* In: id -> the identifier to be used to find an absolute
     *        filesystem path in the FIO pathmap.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Removes the path located in the FIO path map at the id provided. */
    FIO & removePath( std::string const & id );

    /* In: id -> the identifier to be used to find an absolute
     *        filesystem path in the FIO pathmap.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Removes the path located in the FIO path map at the id provided. */
    FIO & removePath( wchar_t const * const & id );

    /* In: id -> the identifier to be used to find an absolute
     *        filesystem path in the FIO pathmap.
     *
     * Out: A reference to the FIO Singleton.
     *
     * Removes the path located in the FIO path map at the id provided. */
    FIO & removePath( char const * const & id );

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
    explicit FIO( std::wstring const & loc = L"en_US.UTF-8" );

    /* In: ( Optional ) loc -> the locale string to set on construction.
     *                      Defaults to "en_us.UTF-8"
     *
     * Out: FIO.
     *
     * Constructs FIO. */
    explicit FIO( std::string const & loc );

    /* In: ( Optional ) loc -> the locale string to set on construction.
     *                      Defaults to "en_us.UTF-8"
     *
     * Out: FIO.
     *
     * Constructs FIO. */
    explicit FIO( wchar_t const * const & loc );

    /* In: ( Optional ) loc -> the locale string to set on construction.
     *                      Defaults to "en_us.UTF-8"
     *
     * Out: FIO.
     *
     * Constructs FIO. */
    explicit FIO( char const * const & loc );

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
    std::wistream & validateWIFStream( std::wstring const & id ) const;

    /* In: id -> the id of a stream in the FIO output stream map.
     *
     * Out: The stream pointed to.
     *
     * Checks that the iterator provided points to a valid stream. */
    std::wostream & validateWOFStream( std::wstring const & id ) const;

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
