/*----------------------------------------------------------------------------------------------------------------------
 * Lennart Hautzer
 * 22/04/2020
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
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
----------------------------------------------------------------------------------------------------------------------*/

#ifndef FILE_IO_H_
#define FILE_IO_H_

#include <algorithm>
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

#if defined WIN32 || defined _WIN32 || defined __WIN32 && ! defined __CYGWIN__

  #ifndef WINDOWS

    #define WINDOWS

  #endif
  #ifndef NOMINMAX

    #define NOMINMAX

  #endif

#endif

#include <dirent.h>

#ifdef WINDOWS
  #include <windows.h>
#else
  #include <unistd.h>
#endif

bool const constexpr RecursiveSearchTrue = true;
bool const constexpr RecursiveSearchFalse = false;
bool const constexpr AppendToFile = true;
bool const constexpr OpenNewFile = false;

/*----------------------------------------------------------------------------------------------------------------------
 * This class tracks two basic pieces of data:
 * Streams - used to perform data input and output on files in the filesystem.
 * Path IDs - used to associate an unpronouncable path in the filesystem to one of the following:
 *
 * A filePath is used to track and validate paths to files provided by the user
 *
 * A directoryPath is used to track and validate paths to directories provided by the user
 *
 * Use this class to safely open and close streams while associating them with string pathIDs, instead of long,
 * difficult-to-pronounce filesystem paths.
----------------------------------------------------------------------------------------------------------------------*/

class FileIO
{
public:
  /*--------------------------------------------------------------------------------------------------------------------
   * Construct a Singleton instance of FileIO, and receive a reference to it.
  --------------------------------------------------------------------------------------------------------------------*/
  static FileIO & getInstance();

  /*--------------------------------------------------------------------------------------------------------------------
   * Prevent copying of Singleton.
  --------------------------------------------------------------------------------------------------------------------*/
  FileIO( FileIO const & FileIOToCopy_In ) = delete;
  FileIO & operator=( FileIO const & FileIOToCopy_In ) = delete;

  /*--------------------------------------------------------------------------------------------------------------------
   * Destructor closes and resets all open file streams.
  --------------------------------------------------------------------------------------------------------------------*/

  ~FileIO();

  /*--------------------------------------------------------------------------------------------------------------------
   * Function to close all open file streams, and unset all stored paths and streams.
  --------------------------------------------------------------------------------------------------------------------*/

  FileIO & reset();

  /*--------------------------------------------------------------------------------------------------------------------
   * Overloaded functions to open an input filestream on an absolute filepath.
  --------------------------------------------------------------------------------------------------------------------*/

  /* Open an input filestream on the filepath located at the pathID provided. Path must have been set in input filepaths
   * already.*/
  std::istream & openInputStream( std::string const & pathID_In );

  /* Open an input filestream on the filepath located at the pathID provided. Path must have been set in input filepaths
   * already.*/
  std::istream & openInputStream( char const * const & pathID_In );

  /* Open an input filestream on the filepath provided, and associate it with the pathID provided. */
  std::istream & openInputStream( std::string const & pathID_In, std::string const & filePath_In );

  /* Open an input filestream on the filepath provided, and associate it with the pathID provided. */
  std::istream & openInputStream( std::string const & pathID_In, char const * const & filePath_In );

  /* Open an input filestream on the filepath provided, and associate it with the pathID provided. */
  std::istream & openInputStream( char const * const & pathID_In, std::string const & filePath_In );

  /* Open an input filestream on the filepath provided, and associate it with the pathID provided. */
  std::istream & openInputStream( char const * const & pathID_In, char const * const & filePath_In );

  /*--------------------------------------------------------------------------------------------------------------------
   * Overloaded functions to open an output filestream on an absolute filepath.
  --------------------------------------------------------------------------------------------------------------------*/

  /* Open an output filestream on the filepath located at the pathID provided. Path must have been set in output
   * filepaths already.*/
  std::ostream & openOutputStream( std::string const & pathID_In, bool const & appendToFile_In = OpenNewFile );

  /* Open an output filestream on the filepath located at the pathID provided. Path must have been set in output
   * filepaths already.*/
  std::ostream & openOutputStream( char const * const & pathID_In, bool const & appendToFile_In = OpenNewFile );

  /* Open an output filestream on the filepath provided, and associate it with the pathID provided. */
  std::ostream & openOutputStream(
    std::string const & pathID_In, std::string const & filePath_In, bool const & appendToFile_In = OpenNewFile );

  /* Open an output filestream on the filepath provided, and associate it with the pathID provided. */
  std::ostream & openOutputStream(
    std::string const & pathID_In, char const * const & filePath_In, bool const & appendToFile_In = OpenNewFile );

  /* Open an output filestream on the filepath provided, and associate it with the pathID provided. */
  std::ostream & openOutputStream(
    char const * const & pathID_In, std::string const & filePath_In, bool const & appendToFile_In = OpenNewFile );

  /* Open an output filestream on the filepath provided, and associate it with the pathID provided. */
  std::ostream & openOutputStream(
    char const * const & pathID_In, char const * const & filePath_In, bool const & appendToFile_In = OpenNewFile );

  /*--------------------------------------------------------------------------------------------------------------------
   * Close/Reset input filestreams and close output filestreams.
  --------------------------------------------------------------------------------------------------------------------*/

  /* Retrieves and closes the input filestream at the pathID provided. Both must exist. */
  FileIO & closeInputStream( std::string const & pathID_In );

  /* Retrieves and closes the output filestream at the pathID provided. Both must exist. */
  FileIO & closeOutputStream( std::string const & pathID_In );

  /* Retrieves and resets the input filestream at the pathID provided to its beginning. Both must exist. */
  FileIO & resetInputStreamToFileStart( std::string const & pathID_In );

  /*--------------------------------------------------------------------------------------------------------------------
   * Wrapper for reading and writing strings
  --------------------------------------------------------------------------------------------------------------------*/

  /* Retrieves and reads one line, unaltered, from the input filestream at the pathID provided into the string provided.
   * Both the stream and pathID must exist. */
  FileIO & readLine( std::string const & pathID_In, std::string & lineToWriteTo_In );

  /* Retrieves and returns one line, unaltered, from the output filestream at the pathID provided. Both the stream and
   * pathID must exist. */
  std::string readLine( std::string const & pathID_In );

  /* Retrieves the output filestream at the pathID provided and writes, unaltered, the string provided. Both the stream
   * and pathID must exist. */
  FileIO & writeLine( std::string const & pathID_In, std::string const & lineToWrite_In );

  /* Returns a string containing all lines, unaltered, from the input filestream at the pathID provided. Both the stream
   * and pathID must exist. Will reset filestream to file start before reading. */
  std::string readFileAsString( std::string const & pathID_In );

  /* Returns a vector of strings containing all lines from the input filestream at the pathID provided, separated by the
   * delimiting characters. Both the stream and pathID must exist. Will reset filestream to file start before reading.
   */
  std::vector< std::string > readFileAsVectorOfStrings(
    std::string const & pathID_In, std::string const & delimitingCharacters_In = "\n\r" );

  /* Returns a vector of vectors containing all lines from the input filestream at the pathID provided, separated
   * horizontally by the horizontal delimiting characters, and vertically by the vertical delimiting characters. Both
   * the stream and pathID must exist. Will reset filestream to file start before reading.
   */
  std::vector< std::vector< std::string > > readFileAsVectorOfVectors( std::string const & pathID_In,
    std::string const & horizontalDelimitingCharacters_In = ",",
    std::string const & verticalDelimitingCharacters_In = "\n\r" );

  /*--------------------------------------------------------------------------------------------------------------------
   * Used to associate user directories and files with specific filepaths.
  --------------------------------------------------------------------------------------------------------------------*/

  /* Change the base directory to a specific directory path. */
  FileIO & setBaseDirectory( std::string const & baseDirectory_In );

  /* Associate a string index with an absolute filepath. */
  FileIO & setFilePath( std::string const & pathID_In, std::string const & filePath_In );

  /* Associate a string index with an absolute directory path. */
  FileIO & setDirectoryPath( std::string const & pathID_In, std::string const & pathToSet_In );

  /*--------------------------------------------------------------------------------------------------------------------
   * Get an input filestream or an output filestream.
  --------------------------------------------------------------------------------------------------------------------*/

  std::istream & getInputStream( std::string const & pathID_In ) const;
  std::ostream & getOutputStream( std::string const & pathID_In ) const;

  /*--------------------------------------------------------------------------------------------------------------------
   * Get a sorted list of open filestreams' pathIDs and the absolute filepaths they are tied to.
  --------------------------------------------------------------------------------------------------------------------*/

  /* Retrieves, sorts and returns all open input filestreams' pathIDs and the absolute filepaths they are tied to. The
   * output format is "pathID | absolute filepath" */
  std::vector< std::string > getAllOpenInputStreams() const;

  /* Retrieves, sorts and returns all open output filestreams' pathIDs and the absolute filepaths they are tied to. The
   * output format is "pathID | absolute filepath" */
  std::vector< std::string > getAllOpenOutputStreams() const;

  /*--------------------------------------------------------------------------------------------------------------------
   * Retrieve path to working directory.
  --------------------------------------------------------------------------------------------------------------------*/

  std::string const & getBaseDirectory() const;

  /*--------------------------------------------------------------------------------------------------------------------
   * Retrieve a filepath.
  --------------------------------------------------------------------------------------------------------------------*/

  std::string const & getFilePath( std::string const & pathID_In ) const;

  /*--------------------------------------------------------------------------------------------------------------------
   * Delete a filepath from filepaths.
  --------------------------------------------------------------------------------------------------------------------*/

  FileIO & removeFilePath( std::string const & pathID_In );

  /*--------------------------------------------------------------------------------------------------------------------
   * Retrieve a directorypath.
  --------------------------------------------------------------------------------------------------------------------*/

  std::string const & getDirectoryPath( std::string const & pathID_In ) const;

  /*--------------------------------------------------------------------------------------------------------------------
   * Delete a directory path from directory paths.
  --------------------------------------------------------------------------------------------------------------------*/

  FileIO & removeDirectoryPath( std::string const & pathID_In );

  /*--------------------------------------------------------------------------------------------------------------------
   * Find files, optionally of a certain type, in a directory, and optionally in its sub-directories. OS-dependent.
  --------------------------------------------------------------------------------------------------------------------*/

  std::vector< std::string > findAllFiles( std::string const & startingDirectory_In,
    std::string const & fileExtension_In = ".*", int const & recursiveSearch_In = RecursiveSearchFalse ) const;

  /*--------------------------------------------------------------------------------------------------------------------
   * Get the filename of a file, given its path on the filesystem.
  --------------------------------------------------------------------------------------------------------------------*/

  std::string getBaseFileName( std::string const & fileName_In ) const;

  /*--------------------------------------------------------------------------------------------------------------------
   * Functions to check for errors.
  --------------------------------------------------------------------------------------------------------------------*/

  /* Get the number of open input filestreams */
  std::size_t getNumberOfOpenInputStreams() const;

  /* Get the number of open output filestreams */
  std::size_t getNumberOfOpenOutputStreams() const;

  /* Check if input filestream has been instantiated in FileIO. */
  bool inputStreamExists( std::string const & pathID_In ) const;

  /* Check if input filestream has encountered any errors. Returns false if there are problems. */
  bool inputStreamIsValid( std::string const & pathID_In ) const;

  /* Check if output filestream has been instantiated in FileIO. */
  bool outputStreamExists( std::string const & pathID_In ) const;

  /* Check if output filestream has encountered any errors. Returns false if there are problems. */
  bool outputStreamIsValid( std::string const & pathID_In ) const;

  /* Check if filepath has been instantiated in FileIO. */
  bool filePathExists( std::string const & pathID_In ) const;

  /* Check if filepath is a valid path in the system. */
  bool filePathIsValid( std::string const & pathID_In ) const;

  /* Check if directory path has been instantiated in FileIO. */
  bool directoryPathExists( std::string const & pathID_In ) const;

  /* Check if directory path is a valid path in the system. */
  bool directoryPathIsValid( std::string const & pathID_In ) const;

protected:
  explicit FileIO();

  static std::unique_ptr< FileIO > singletonInstance;

private:
  /*--------------------------------------------------------------------------------------------------------------------
   * Internal functions to check for errors. These handle direct access to data structures for better efficiency than
   * public accessors.
  --------------------------------------------------------------------------------------------------------------------*/

  std::unordered_map< std::string, std::unique_ptr< std::ifstream > >::const_iterator getInputStreamIterator(
    std::string const & pathID_In ) const;

  bool inputStreamExists(
    std::unordered_map< std::string, std::unique_ptr< std::ifstream > >::const_iterator const & streamIterator_In )
    const;

  bool inputStreamIsValid(
    std::unordered_map< std::string, std::unique_ptr< std::ifstream > >::const_iterator const & streamIterator_In )
    const;

  std::unordered_map< std::string, std::unique_ptr< std::ofstream > >::const_iterator getOutputStreamIterator(
    std::string const & pathID_In ) const;

  bool outputStreamExists(
    std::unordered_map< std::string, std::unique_ptr< std::ofstream > >::const_iterator const & streamIterator_In )
    const;

  bool outputStreamIsValid(
    std::unordered_map< std::string, std::unique_ptr< std::ofstream > >::const_iterator const & streamIterator_In )
    const;

  std::unordered_map< std::string, std::string >::const_iterator getFilePathIterator(
    std::string const & pathID_In ) const;

  bool filePathExists( std::unordered_map< std::string, std::string >::const_iterator const & pathIterator_In ) const;

  std::unordered_map< std::string, std::string >::const_iterator getDirectoryPathIterator(
    std::string const & pathID_In ) const;

  bool directoryPathExists(
    std::unordered_map< std::string, std::string >::const_iterator const & pathIterator_In ) const;

  /*--------------------------------------------------------------------------------------------------------------------
   * Internal function to find initial working directory.
  --------------------------------------------------------------------------------------------------------------------*/

  std::string findBaseDirectory() const;

  /*--------------------------------------------------------------------------------------------------------------------
   * Internal functions to give errors.
  --------------------------------------------------------------------------------------------------------------------*/

  void giveStreamAlreadyOpenError( std::string const & throwingFunction_In, std::string const & pathID_In ) const;
  void giveStreamNotOpenError( std::string const & throwingFunction_In, std::string const & pathID_In ) const;
  void giveStreamCouldNotOpenError( std::string const & throwingFunction_In, std::string const & pathID_In ) const;

  void givePathNotFoundError( std::string const & throwingFunction_In, std::string const & pathID_In ) const;
  void givePathNotValidError( std::string const & throwingFunction_In, std::string const & pathID_In ) const;
  void givePathNotReadableError( std::string const & throwingFunction_In, std::string const & pathID_In ) const;

  /*--------------------------------------------------------------------------------------------------------------------
   * Convert between wide strings and normal strings
  --------------------------------------------------------------------------------------------------------------------*/

  std::string wideStringToNormalString( std::wstring const & wideString_In ) const;
  std::wstring normalStringToWideString( std::string const & normalString_In ) const;

  std::unordered_map< std::string, std::string > directoryPaths;
  std::unordered_map< std::string, std::string > filePaths;

  std::unordered_map< std::string, std::unique_ptr< std::ifstream > > inputFileStreams;
  std::unordered_map< std::string, std::unique_ptr< std::ofstream > > outputFileStreams;

  std::string baseDirectory;
};

#endif
