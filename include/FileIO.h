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

#ifndef FILE_IO_H_
#define FILE_IO_H_

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdexcept>

#if defined WIN32 || defined _WIN32 || defined __WIN32 && !defined __CYGWIN__

    #ifndef WINDOWS

        #define WINDOWS

    #endif

#endif

#ifdef WINDOWS

    #include <windows.h>
    #include <direct.h>

    #define getCurrentDirectory _getcwd

#else

    #include <unistd.h>
    #include <dirent.h>

    #define getCurrentDirectory getcwd

#endif

bool constexpr recursiveSearchTrue = 1;
bool constexpr recursiveSearchFalse = 0;

/*--------------------------------------------------------------------------------------------------
* This class tracks two basic pieces of data:
* Streams - used to perform data input and output on files in the filesystem.
* Path IDs - used to associate an unpronouncable path in the filesystem to one of the following:

**  A filePath is used to track and validate paths to files provided by the user
**  A directoryPath is used to track and validate paths to directories provided by the user 

* Use this class to safely open and close streams while associating them with string pathIDs, 
* instead of long, difficult-to-pronounce filesystem paths.
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
    std::istream & openInputStream( char const * const & pathIDIn );

    /* Open a filestream on the filepath provided, and associate it with the pathID provided. */
    std::istream & openInputStream( std::string const & pathIDIn, std::string const & filePathIn );

    /* Overload of openInputStream(std::string const & pathIDIn, std::string const & filePathIn) */
    std::istream & openInputStream( std::string const & pathIDIn, char const * const & filePathIn );

    /* Overload of openInputStream(std::string const & pathIDIn, std::string const & filePathIn) */
    std::istream & openInputStream( char const * const & pathIDIn, std::string const & filePathIn );

    /* Overload of openInputStream(std::string const & pathIDIn, std::string const & filePathIn) */
    std::istream & openInputStream( char const * const & pathIDIn, char const * const & filePathIn );

    /*--------------------------------------------------------------------------------------------------
    * Overloaded functions to open a std::ofstream on an absolute filepath.
    --------------------------------------------------------------------------------------------------*/

   /* Open an output filestream on the filepath located at the pathID provided. 
   * Path must have been set in filePaths already.*/
    std::ostream & openOutputStream( std::string const & pathIDIn );
    
    /* Overload of openOutputStream(std::string const & pathIDIn) */
    std::ostream & openOutputStream( char const * const & pathIDIn );
    
    /* Open an output filestream on the filepath provided, and associate it with the pathID provided. */
    std::ostream & openOutputStream( std::string const & pathIDIn, std::string const & filePathIn );
    
    /* Overload of openOutputStream(std::string const & pathIDIn, std::string const & filePathIn) */
    std::ostream & openOutputStream( std::string const & pathIDIn, char const * const & filePathIn );

    /* Overload of openOutputStream(std::string const & pathIDIn, std::string const & filePathIn) */
    std::ostream & openOutputStream( char const * const & pathIDIn, std::string const & filePathIn );

    /* Overload of openOutputStream(std::string const & pathIDIn, std::string const & filePathIn) */
    std::ostream & openOutputStream( char const * const & pathIDIn, char const * const & filePathIn );

    /*--------------------------------------------------------------------------------------------------
    * Close/Reset input filestreams and close output filestreams.
    --------------------------------------------------------------------------------------------------*/

    /* Retrieves and closes the filestream at the pathID provided. Both must exist. */
    FileIO & closeInputStream( std::string const & pathIDIn );

    /* Retrieves and closes the output filestream at the pathID provided. Both must exist. */
    FileIO & closeOutputStream( std::string const & pathIDIn );

    /* Retrieves and resets the filestream at the pathID provided. Both must exist. */
    FileIO & resetInputStreamToFileStart( std::string const & pathIDIn );

    /*--------------------------------------------------------------------------------------------------
    * Wrapper for reading and writing strings
    --------------------------------------------------------------------------------------------------*/

    /* Retrieves and reads one RAW line from the filestream at the pathID provided
    * to the string provided. Both the stream and pathID must exist. */
    FileIO & readLine( std::string const & pathIDIn, std::string & lineToWriteTo );

    /* Retrieves and returns one RAW line from the filestream at the pathID provided. 
    * Both the stream and pathID must exist. */
    std::string const readLine( std::string const & pathIDIn);

    /* Retrieves the output filestream at the pathID provided
    * and writes the RAW string provided. Both the stream and pathID must exist. */
    FileIO & writeLine( std::string const & pathIDIn, std::string const & lineToWrite );

    /* Returns a string containing all lines from the filestream at the pathID provided. 
    * Both the stream and pathID must exist. Will reset filestream to file start. */
    std::string const readFileAsString(std::string const & pathIDIn);

    /* Returns a vector containing all lines from the filestream at the pathID provided. 
    * Both the stream and pathID must exist. Will reset filestream to file start. */
    std::vector<std::string> const readFileAsVector(std::string const & pathIDIn, 
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

    std::istream & getInputStream( std::string const & pathIDIn ) const;
    std::ostream & getOutputStream( std::string const & pathIDIn ) const;

    /*--------------------------------------------------------------------------------------------------
    * Retrieve path to working directory or data sub-directory.
    --------------------------------------------------------------------------------------------------*/

    std::string const getBaseDirectory() const;

    /*--------------------------------------------------------------------------------------------------
    * Retrieve a filepath or the address for std::unordered_map of filepaths.
    --------------------------------------------------------------------------------------------------*/

    std::string const getFilePath( std::string const & pathIDIn ) const;
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

    /* Check if inputStream has been instantiated in FileIO. */
    bool const inputStreamExists(std::string const & pathIDIn) const;

    /* Check if inputStream has encountered any errors. Returns false if there are problems. */
    bool const inputStreamIsValid(std::string const & pathIDIn) const;

    /* Check if outputStream has been instantiated in FileIO. */
    bool const outputStreamExists(std::string const & pathIDIn) const;

    /* Check if outputStream has encountered any errors. Returns false if there are problems. */
    bool const outputStreamIsValid(std::string const & pathIDIn) const;

    /* Check if filepath has been instantiated in FileIO. */
    bool const filePathExists(std::string const & pathIDIn) const;

    /* Check if filepath is a valid path in the system. */
    bool const filePathIsValid(std::string const & pathIDIn) const;
    
    /* Check if directory path has been instantiated in FileIO. */
    bool const directoryPathExists(std::string const & pathIDIn) const;

    /* Check if directory path is a valid path in the system. */
    bool const directoryPathIsValid(std::string const & pathIDIn) const;

private:

    /*--------------------------------------------------------------------------------------------------
    * Internal functions to check for errors. These handle direct access to data structures for better
    * efficiency than public accessors.
    --------------------------------------------------------------------------------------------------*/

    std::unordered_map< std::string, std::unique_ptr<std::ifstream> >::const_iterator 
        getInputStreamIterator(std::string const & pathIDIn) const;
    
    bool const inputStreamExists(std::unordered_map< std::string, 
        std::unique_ptr<std::ifstream> >::const_iterator & streamIteratorIn) const;
    
    bool const inputStreamIsValid(std::unordered_map< std::string, 
        std::unique_ptr<std::ifstream> >::const_iterator & streamIteratorIn) const;

    std::unordered_map< std::string, std::unique_ptr<std::ofstream> >::const_iterator 
        getOutputStreamIterator(std::string const & pathIDIn) const;
    
    bool const outputStreamExists(std::unordered_map< std::string, 
        std::unique_ptr<std::ofstream> >::const_iterator & streamIteratorIn) const;

    bool const outputStreamIsValid(std::unordered_map< std::string, 
        std::unique_ptr<std::ofstream> >::const_iterator & streamIteratorIn) const;
    
    std::unordered_map<std::string, std::string>::const_iterator 
        getFilePathIterator(std::string const & pathIDIn) const;
    
    bool const filePathExists(std::unordered_map< std::string, 
        std::string>::const_iterator & pathIteratorIn) const;
    
    std::unordered_map<std::string, std::string>::const_iterator 
        getDirectoryPathIterator(std::string const & pathIDIn) const;
    
    bool const directoryPathExists(std::unordered_map< std::string, 
        std::string>::const_iterator & pathIteratorIn) const;

    /*--------------------------------------------------------------------------------------------------
    * Internal function to find initial working directory.
    --------------------------------------------------------------------------------------------------*/

    std::string const findBaseDirectory() const;

    /*--------------------------------------------------------------------------------------------------
    * Internal functions to give errors.
    --------------------------------------------------------------------------------------------------*/

    void giveStreamAlreadyOpenError(std::string const & pathIDIn) const;
    void giveStreamNotOpenError(std::string const & pathIDIn) const;
    void giveStreamCouldNotOpenError(std::string const & pathIDIn) const;

    void givePathNotFoundError(std::string const & pathIDIn) const;
    void givePathNotValidError(std::string const & pathIDIn) const;

    std::unordered_map<std::string, std::string> directoryPaths;
    std::unordered_map<std::string, std::string> filePaths;

    std::unordered_map< std::string, std::unique_ptr<std::ifstream> > inputFileStreams;
    std::unordered_map< std::string, std::unique_ptr<std::ofstream> > outputFileStreams;

    std::string baseDirectory;
};

#endif
