/*--------------------------------------------------------------------------------------------------
* Lennart Hautzer
* 03/03/2020
* 10 100 5235
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
--------------------------------------------------------------------------------------------------*/

#ifndef _FILE_IO_HEADER_
#define _FILE_IO_HEADER_

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdexcept>
#include "ConditionalIncludes.h"

#define RECURSIVE_SEARCH_TRUE 1
#define RECURSIVE_SEARCH_FALSE 0

/*--------------------------------------------------------------------------------------------------
* This class is a collection of different functions to manage file system
* interaction between both Windows and *nix operating systems.
--------------------------------------------------------------------------------------------------*/

class FileIO
{
public:

    explicit FileIO();
    FileIO(FileIO const & FileIOToCopy) = delete;
    FileIO & operator = (FileIO const & FileIOToCopy) = delete;

    ~FileIO();

    /*--------------------------------------------------------------------------------------------------
    * Overloaded functions to open a filestream on an absolute filepath.
    --------------------------------------------------------------------------------------------------*/

   /* Open a filestream on the filepath located at the pathID provided. 
   * Path must have been set in filePaths already.*/
    std::istream & openInputStream( std::string const & pathIDIn );
    
    /* Overload of openInputStream(std::string const & pathIDIn) */
    std::istream & openInputStream( char const * const pathIDIn );

    /* Open a filestream on the filepath provided, and associate it with the pathID provided. */
    std::istream & openInputStream( std::string const & pathIDIn, std::string const & filePathIn );

    /* Overload of openInputStream(std::string const & pathIDIn, std::string const & filePathIn) */
    std::istream & openInputStream( char const * const pathIDIn, char const * const filePath );

    /*--------------------------------------------------------------------------------------------------
    * Overloaded functions to open a std::ofstream on an absolute filepath.
    --------------------------------------------------------------------------------------------------*/

   /* Open an output filestream on the filepath located at the pathID provided. 
   * Path must have been set in filePaths already.*/
    std::ostream & openOutputStream( std::string const & pathIDIn );
    
    /* Overload of openOutputStream(std::string const & pathIDIn) */
    std::ostream & openOutputStream( char const * pathIDIn );
    
    /* Open an output filestream on the filepath provided, and associate it with the pathID provided. */
    std::ostream & openOutputStream( std::string const & pathIDIn, std::string const & filePath );
    
    /* Overload of openOutputStream(std::string const & pathIDIn, std::string const & filePathIn) */
    std::ostream & openOutputStream( char const * pathIDIn, char const * filePath );

    /*--------------------------------------------------------------------------------------------------
    * Close/Reset input filestreams and close output filestreams.
    --------------------------------------------------------------------------------------------------*/

    /* Retrieves and closes the filestream at the streamID provided. Both must exist. */
    FileIO & closeInputStream( std::string const & streamIDIn );

    /* Retrieves and closes the output filestream at the streamID provided. Both must exist. */
    FileIO & closeOutputStream( std::string const & streamIDIn );

    /* Retrieves and resets the filestream at the streamID provided. Both must exist. */
    FileIO & resetInputStreamToFileStart( std::string const & streamIDIn );

    /*--------------------------------------------------------------------------------------------------
    * Wrapper for reading and writing std::strings
    --------------------------------------------------------------------------------------------------*/

    /* Retrieves and reads one RAW line from the filestream at the streamID provided
    * to the string provided. Both the stream and streamID must exist. */
    FileIO & readLine( std::string const & streamIDIn, std::string & lineToWriteTo );

    /* Retrieves and returns one RAW line from the filestream at the streamID provided. 
    * Both the stream and streamID must exist. */
    std::string const readLine( std::string const & streamIDIn);

    /* Retrieves the std::ofstream at the streamID provided
    * and writes the string provided. Both the stream and streamID must exist. */
    FileIO & writeLine( std::string const & streamIDIn, std::string const & lineToWrite );

    /* Returns a string containing all lines from the filestream at the streamID provided. 
    * Both the stream and streamID must exist. Will reset filestream to file start. */
    std::string const readFileAsString(std::string const & streamIDIn);

    /* Returns a vector containing all lines from the filestream at the streamID provided. 
    * Both the stream and streamID must exist. Will reset filestream to file start. */
    std::vector<std::string> const readFileAsVector(std::string const & streamIDIn, 
        std::string const & delimitingCharacters);

    /*--------------------------------------------------------------------------------------------------
    * Used to associate user directories and files with specific filepaths.
    --------------------------------------------------------------------------------------------------*/

    /* Change the base directory to a specific directory path. */
    FileIO & setBaseDirectory( std::string const & baseDirectory );

    /* Associate a string index with an absolute filepath. */
    FileIO & setFilePath( std::string const & pathIDIn, std::string const & filePathIn );

    /* Associate a string index with an absolute directory path. */
    FileIO & setDirectoryPath( std::string const & pathIDIn, std::string const & pathToSetIn );

    /*--------------------------------------------------------------------------------------------------
    * Retrieve addresses of raw ifstreams/ofstreams.
    --------------------------------------------------------------------------------------------------*/

    std::istream & getInputStream( std::string const & streamIDIn ) const;
    std::ostream & getOutputStream( std::string const & streamIDIn ) const;

    /*--------------------------------------------------------------------------------------------------
    * Retrieve path to working directory or data sub-directory.
    --------------------------------------------------------------------------------------------------*/

    std::string const getBaseDirectory() const;

    /*--------------------------------------------------------------------------------------------------
    * Retrieve a filepath or the address for std::unordered_map of filepaths.
    --------------------------------------------------------------------------------------------------*/

    std::string const getFilePath( std::string const & streamIDIn ) const;
    std::unordered_map<std::string, std::string> const & getAllFilePaths() const;

    /*--------------------------------------------------------------------------------------------------
    * Delete filepath from std::unordered_map of filepaths.
    --------------------------------------------------------------------------------------------------*/

    FileIO & removeFilePath( std::string const & pathIDIn );

    /*--------------------------------------------------------------------------------------------------
    * Retrieve a dirpath or the address for std::unordered_map of dirpaths.
    --------------------------------------------------------------------------------------------------*/

    std::string const getDirectoryPath( std::string const & pathIDIn ) const;
    std::unordered_map<std::string, std::string> const & getAllDirectoryPaths() const;

    /*--------------------------------------------------------------------------------------------------
    * Delete directory path from std::unordered_map of directory paths.
    --------------------------------------------------------------------------------------------------*/

    FileIO & removeDirectoryPath( std::string const & pathIDIn );

    /*--------------------------------------------------------------------------------------------------
    * Find files in a dir, optionally of a certain type,
    * optionally in subdirs. OS-dependent.
    --------------------------------------------------------------------------------------------------*/

    std::vector<std::string> const findAllFiles(std::string const & startingDirectory,
        std::string const & fileExtension = ".*", int const & recursiveSearch = false ) const;

    /*--------------------------------------------------------------------------------------------------
    * Get the filename of a file, given its path on the filesystem.
    --------------------------------------------------------------------------------------------------*/

    std::string const getBaseFileName( std::string const & fileNameIn ) const;

    /*--------------------------------------------------------------------------------------------------
    * Functions to check for errors.
    --------------------------------------------------------------------------------------------------*/

    bool const inputStreamExists(std::string const & streamIDIn) const;
    bool const inputStreamIsValid(std::string const & streamIDIn) const;

    bool const outputStreamExists(std::string const & streamIDIn) const;
    bool const outputStreamIsValid(std::string const & streamIDIn) const;

    bool const filePathExists(std::string const & pathIDIn) const;

    bool const directoryPathExists(std::string const & pathIDIn) const;

private:

    /*--------------------------------------------------------------------------------------------------
    * Internal functions to find working directory.
    --------------------------------------------------------------------------------------------------*/

    std::string const findBaseDirectory() const;

    /*--------------------------------------------------------------------------------------------------
    * Internal functions to give errors.
    --------------------------------------------------------------------------------------------------*/

    void giveStreamAlreadyOpenError(std::string const & streamIDIn) const;
    void giveStreamNotOpenError(std::string const & streamIDIn) const;
    void giveStreamCouldNotOpenError(std::string const & streamIDIn) const;

    void givePathNotFoundError(std::string const & streamIDIn) const;

    std::unordered_map<std::string, std::string> directoryPaths;
    std::unordered_map<std::string, std::string> filePaths;

    std::unordered_map< std::string, std::unique_ptr<std::ifstream> > inputFileStreams;
    std::unordered_map< std::string, std::unique_ptr<std::ofstream> > outputFileStreams;

    std::string baseDirectory;
};

#endif
