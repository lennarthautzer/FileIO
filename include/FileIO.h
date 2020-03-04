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

#ifndef FileIOHeader
#define FileIOHeader

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
    * Overloaded functions to open a std::ifstream on an absolute filepath.
    --------------------------------------------------------------------------------------------------*/

   /* Open a std::ifstream on the filepath located at the pathID provided. 
   * Path must have been set in filePaths already.*/
    std::istream & openInputStream( std::string const & pathIDIn );
    
    /* Overload of openInputStream(std::string const & pathIDIn) */
    std::istream & openInputStream( char const * const pathIDIn );

    /* Open a std::ifstream on the filepath provided, 
    * and associate it with the pathID provided. */
    std::istream & openInputStream( std::string const & pathIDIn, std::string const & filePathIn );

    /* Overload of openInputStream(std::string const & pathIDIn, std::string const & filePathIn) */
    std::istream & openInputStream( char const * const pathIDIn, char const * const filePath );

    /*--------------------------------------------------------------------------------------------------
    * Overloaded functions to open a std::ofstream on an absolute filepath.
    --------------------------------------------------------------------------------------------------*/

   /* Open a std::ofstream on the filepath located at the pathID provided. 
   * Path must have been set in filePaths already.*/
    std::ostream & openOutputStream( std::string const & pathIDIn );
    
    /* Overload of openOutputStream(std::string const & pathIDIn) */
    std::ostream & openOutputStream( char const * pathIDIn );
    
    /* Open a std::ofstream on the filepath provided, 
    * and associate it with the pathID provided. */
    std::ostream & openOutputStream( std::string const & pathIDIn, std::string const & filePath );
    
    /* Overload of openOutputStream(std::string const & pathIDIn, std::string const & filePathIn) */
    std::ostream & openOutputStream( char const * pathIDIn, char const * filePath );

    /*--------------------------------------------------------------------------------------------------
    * Close/Reset ifstreams and close ofstreams.
    --------------------------------------------------------------------------------------------------*/

    /* Retrieves and closes the std::ifstream at the streamID provided. Both must exist. */
    void closeInputStream( std::string const & streamIDIn );

    /* Retrieves and closes the std::ofstream at the streamID provided. Both must exist. */
    void closeOutputStream( std::string const & streamIDIn );

    /* Retrieves and resets the std::ifstream at the streamID provided. Both must exist. */
    void resetInputStreamToFileStart( std::string const & streamIDIn );

    /*--------------------------------------------------------------------------------------------------
    * Wrapper for reading and writing std::strings
    --------------------------------------------------------------------------------------------------*/

    /* Retrieves and reads one line from the std::ifstream at the streamID provided
    * to the string provided. Both the stream and streamID must exist. */
    void readLine( std::string const & streamIDIn, std::string & lineToWriteTo );

    /* Retrieves and returns one line from the std::ifstream at the streamID provided. 
    * Both the stream and streamID must exist. */
    std::string const readLine( std::string const & streamIDIn);

    /* Retrieves the std::ofstream at the streamID provided
    * and writes the string provided. Both the stream and streamID must exist. */
    void writeLine( std::string const & streamIDIn, std::string const & lineToWrite );

    /*--------------------------------------------------------------------------------------------------
    * Used to associate user directories and files with specific filepaths.
    --------------------------------------------------------------------------------------------------*/

    /* Associate a string index with an absolute filepath. */
    void setFilePath( std::string const & pathIDIn, std::string const & filePathIn );

    /* Associate a string index with a path to a directory, 
    * appended to the working dir. */
    void setRelativeDirectoryPath( std::string const & pathIDIn,
        std::string const & directoryPathIn );

    /* Associate a string index with an absolute path to a directory. */
    void setAbsoluteDirectoryPath( std::string const & pathIDIn,
        std::string const & pathToSetIn );

    /*--------------------------------------------------------------------------------------------------
    * Retrieve addresses of raw ifstreams/ofstreams.
    --------------------------------------------------------------------------------------------------*/

    std::istream & getInputStream( std::string const & streamIDIn );
    std::ostream & getOutputStream( std::string const & streamIDIn );

    /*--------------------------------------------------------------------------------------------------
    * Retrieve path to working directory or data sub-directory.
    --------------------------------------------------------------------------------------------------*/

    std::string const getWorkingDir() const;
    std::string const getDataDir() const;

    /*--------------------------------------------------------------------------------------------------
    * Retrieve a filepath or the address for std::unordered_map of filepaths.
    --------------------------------------------------------------------------------------------------*/

    std::string const getFilePath( std::string const & streamIDIn ) const;
    std::unordered_map<std::string, std::string> const & getAllFilePaths();

    /*--------------------------------------------------------------------------------------------------
    * Delete filepath from std::unordered_map of filepaths.
    --------------------------------------------------------------------------------------------------*/

    void removeFileName( std::string const & pathIDIn );

    /*--------------------------------------------------------------------------------------------------
    * Retrieve a dirpath or the address for std::unordered_map of dirpaths.
    --------------------------------------------------------------------------------------------------*/

    std::string const getDirectoryPath( std::string const & pathIDIn ) const;
    std::unordered_map<std::string, std::string> const & getAllDirectoryPaths();

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

private:

    std::unordered_map<std::string, std::string> directoryPaths;
    std::unordered_map<std::string, std::string> filePaths;

    std::unordered_map< std::string, std::unique_ptr<std::ifstream> > inputFileStreams;
    std::unordered_map< std::string, std::unique_ptr<std::ofstream> > outputFileStreams;

    /*--------------------------------------------------------------------------------------------------
    * Internal functions to find working directory. OS-dependent.
    --------------------------------------------------------------------------------------------------*/

    std::string const findWorkingDir() const;
    void setWorkingDirs( std::string const & workingDir );

    std::string workingDir;
    std::string dataDir;
};

#endif
