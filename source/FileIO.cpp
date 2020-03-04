/*--------------------------------------------------------------------
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
--------------------------------------------------------------------*/

#include "FileIO.h"

FileIO::FileIO()
{
    setWorkingDirs( findWorkingDir() );
}

FileIO::~FileIO()
{
    auto inputStreamIterator = inputFileStreams.begin();
    auto outputStreamIterator = outputFileStreams.begin();
    bool done = false;

    while( !done )
    {
        if( inputStreamIterator != inputFileStreams.end() )
        {
            inputStreamIterator->second->close();
            inputStreamIterator++;
        }
        if( outputStreamIterator != outputFileStreams.end() )
        {
            outputStreamIterator->second->close();
            outputStreamIterator++;
        }
        if( inputStreamIterator == inputFileStreams.end()
            && outputStreamIterator == outputFileStreams.end() )
        {
            done = !done;
        }
    }
}

std::istream & FileIO::openInputStream( std::string const & pathIDIn )
{
    std::string const filePath = getFilePath( pathIDIn );

    if( inputFileStreams.find( pathIDIn ) != inputFileStreams.end() )
    {
        throw std::runtime_error("ERROR: InputStream" + pathIDIn + "still open for \"" +
            filePath + "\" in InputStreams.");
    }

    inputFileStreams[ pathIDIn ] = std::unique_ptr<std::ifstream>
        ( new std::ifstream( filePath, std::ifstream::in ) );

    auto inputFileStreamIterator = inputFileStreams.find( pathIDIn );

    if( inputFileStreamIterator == inputFileStreams.end()
        || ( !inputFileStreamIterator->second->good() ) )
    {
        throw std::runtime_error("ERROR: Could not open InputStream on file \"" +
            filePath + "\".");
    }

    return *inputFileStreamIterator->second;
}

std::istream & FileIO::openInputStream( char const * const pathIDIn )
{
    return openInputStream( std::string( pathIDIn ) );
}

std::istream &
FileIO::openInputStream( std::string const & pathIDIn, std::string const & filePathIn )
{
    setFilePath( pathIDIn, filePathIn );
    return openInputStream( pathIDIn );
}

std::istream &
FileIO::openInputStream( char const * const streamIDIn, char const * const filePathIn )
{
    std::string const tmp = std::string( streamIDIn );

    setFilePath( tmp, std::string( filePathIn ) );
    return openInputStream( tmp );
}

std::ostream & FileIO::openOutputStream( std::string const & pathIDIn )
{
    std::string const filePath = getFilePath( pathIDIn );

    if( outputFileStreams.find( pathIDIn ) != outputFileStreams.end() )
    {
        throw std::runtime_error("ERROR: OutputStream" + pathIDIn + "still open for \"" +
            filePath + "\" in OutputStreams.");
    }

    outputFileStreams[ filePath ] = std::unique_ptr<std::ofstream>
        ( new std::ofstream( filePath, std::ofstream::out ) );

    auto outputFileStreamIterator = outputFileStreams.find( filePath );

    if( outputFileStreamIterator == outputFileStreams.end()
        || ( !outputFileStreamIterator->second->good() ) )
    {
        throw std::runtime_error("ERROR: Could not open OutputStream on file \"" +
            filePath + "\".");
    }

    return *outputFileStreamIterator->second;
}

std::ostream & FileIO::openOutputStream( char const * streamIDIn )
{
    return openOutputStream( std::string( streamIDIn ) );
}

std::ostream & FileIO::openOutputStream( std::string const & streamIDIn, std::string const & filePathIn )
{
    setFilePath( streamIDIn, filePathIn );
    return openOutputStream( streamIDIn );
}

std::ostream & FileIO::openOutputStream( char const * streamIDIn, char const * filePathIn )
{
    std::string const tmp = std::string( streamIDIn );

    setFilePath( tmp, std::string( filePathIn ) );
    return openOutputStream( tmp );
}

void FileIO::closeInputStream( std::string const & streamIDIn )
{
    std::string const filePath = getFilePath( streamIDIn );

    auto inputFileStreamIterator = inputFileStreams.find( filePath );

    if( inputFileStreamIterator == inputFileStreams.end()
        || ( !inputFileStreamIterator->second->good() ) )
    {
        throw std::runtime_error("ERROR: Could not find InputStream to close: " + streamIDIn);
    }

    inputFileStreamIterator->second->close();
    inputFileStreams.erase( filePath );
}

void FileIO::closeOutputStream( std::string const & streamIDIn )
{
    std::string const filePath = getFilePath( streamIDIn );

    auto outputFileStreamIterator = outputFileStreams.find( filePath );

    if( outputFileStreamIterator == outputFileStreams.end()
        || ( !outputFileStreamIterator->second->good() ) )
    {
        throw std::runtime_error("ERROR: Could not find OutputStream to close: " + streamIDIn);
    }

    outputFileStreamIterator->second->close();
    outputFileStreams.erase( filePath );
}

void FileIO::resetInputStreamToFileStart( std::string const & streamIDIn )
{
    std::string const filePath = getFilePath( streamIDIn );

     auto inputFileStreamIterator = inputFileStreams.find( filePath );

    if( inputFileStreamIterator == inputFileStreams.end()
        || ( !inputFileStreamIterator->second->good() ) )
    {
        throw std::runtime_error("ERROR: Could not find InputStream to reset: " + streamIDIn);
    }

    inputFileStreamIterator->second->clear();
    inputFileStreamIterator->second->seekg( 0, std::ios::beg );
}

void FileIO::readLine( std::string const & streamIDIn, std::string & lineToWriteTo )
{
    std::string const filePath = getFilePath( streamIDIn );

     auto inputFileStreamIterator = inputFileStreams.find( filePath );

    if( inputFileStreamIterator == inputFileStreams.end()
        || ( !inputFileStreamIterator->second->good() ) )
    {
        throw std::runtime_error("ERROR: Could not find InputStream to read from: " + streamIDIn);
    }

    getline( *(inputFileStreamIterator->second), lineToWriteTo );
}

std::string const FileIO::readLine( std::string const & streamIDIn)
{
    std::string lineToReturn;

    readLine(streamIDIn, lineToReturn);

    return lineToReturn;
}

void FileIO::writeLine( std::string const & streamIDIn, std::string const & lineToWrite )
{
    std::string const filePath = getFilePath( streamIDIn );

    auto outputFileStreamIterator = outputFileStreams.find( filePath );

    if( outputFileStreamIterator == outputFileStreams.end()
        || ( !outputFileStreamIterator->second->good() ) )
    {
        throw std::runtime_error("ERROR: Could not find OutputStream to close: " + streamIDIn);
    }

    *( outputFileStreamIterator->second ) << lineToWrite;
}

std::string const FileIO::findWorkingDir() const
{
    char tmpBuffer[ FILENAME_MAX ];

    getCurrentDir( tmpBuffer, FILENAME_MAX );
    
    std::string const ret = std::string( tmpBuffer );

    return ret;
}

void FileIO::setWorkingDirs( std::string const & workingDirIn )
{
    struct stat relativeDirectoryCheck;

    std::string parentDir;
    std::string pathToCheck;

#ifdef WINDOWS

    pathToCheck = workingDirIn;

    if( stat( pathToCheck.c_str(), &relativeDirectoryCheck ) == 0
        && relativeDirectoryCheck.st_mode & S_IFDIR )
    {
        workingDir = workingDirIn;
    }
    else
    {
        perror( "ERROR: " );
    }

    pathToCheck = workingDirIn + "\\data";

    if( stat( pathToCheck.c_str(), &relativeDirectoryCheck ) == 0
        && relativeDirectoryCheck.st_mode & S_IFDIR )
    {
        dataDir = workingDirIn + "\\data";
    }
    else
    {
        parentDir = workingDirIn.substr( 0, workingDirIn.find_last_of( "\\" ) );
        pathToCheck = parentDir + "\\data";

        if( stat( pathToCheck.c_str(), &relativeDirectoryCheck ) == 0
            && relativeDirectoryCheck.st_mode & S_IFDIR )
        {
            dataDir = parentDir + "\\data";
            return;
        }
        else
        {
            perror( "ERROR: " );
        }
    }

#else

    pathToCheck = workingDirIn;
    stat( pathToCheck.c_str(), &relativeDirectoryCheck );

    if( relativeDirectoryCheck.st_mode & S_IFDIR )
    {
        workingDir = workingDirIn;
    }
    else
    {
        perror( "ERROR: " );
    }

    pathToCheck = workingDirIn + "/data";
    stat( pathToCheck.c_str(), &relativeDirectoryCheck );

    if( relativeDirectoryCheck.st_mode & S_IFDIR )
    {
        dataDir = workingDirIn + "/data";
    }
    else
    {
        parentDir = workingDirIn.substr( 0, workingDirIn.find_last_of( "/" ) );
        pathToCheck = parentDir + "/data";
        stat( pathToCheck.c_str(), &relativeDirectoryCheck );

        if( relativeDirectoryCheck.st_mode & S_IFDIR )
        {
            dataDir = parentDir + "/data";
        }
        else
        {
            perror( "ERROR: " );
        }
    }

#endif
}

void FileIO::setFilePath( std::string const & pathIDIn, std::string const & filePathIn )
{
    if( filePaths.find( pathIDIn ) != filePaths.end() )
    {
        throw std::runtime_error("ERROR: Filepath already set for \"" + pathIDIn +
        "\" in Filepaths.");
    }

    filePaths[ pathIDIn ] = filePathIn;
}

void FileIO::setRelativeDirectoryPath( std::string const & pathIDIn, std::string const & directoryPathIn )
{
    if( directoryPaths.find( pathIDIn ) != directoryPaths.end() )
    {
        throw std::runtime_error("ERROR: Directory Path already set for \"" + pathIDIn +
        "\" in Directory Paths.");
    }

    directoryPaths[ pathIDIn ] = getWorkingDir() + '/' + directoryPathIn;
}

void FileIO::setAbsoluteDirectoryPath( std::string const & pathIDIn, std::string const & pathToSetIn )
{
    if( directoryPaths.find( pathIDIn ) == directoryPaths.end() )
    {
        throw std::runtime_error("ERROR: Directory Path already set for \"" + pathIDIn + 
            "\" in Directory Paths.");
    }

    directoryPaths[ pathIDIn ] = pathToSetIn;
}

std::istream & FileIO::getInputStream( std::string const & streamIDIn )
{
    auto inputFileStreamIterator = inputFileStreams.find( streamIDIn );

    if( inputFileStreamIterator == inputFileStreams.end() )
    {
        throw std::runtime_error("ERROR: No InputStream set for \"" + streamIDIn + 
        "\" in InputStreams.");
    }

    return *inputFileStreamIterator->second;
}

std::ostream & FileIO::getOutputStream( std::string const & streamIDIn )
{
    auto outputFileStreamIterator = outputFileStreams.find( streamIDIn );

    if( outputFileStreamIterator == outputFileStreams.end() )
    {
        throw std::runtime_error("ERROR: No OutputStream set for \"" + streamIDIn + 
        "\" in OutputStreams.");
    }

    return *outputFileStreamIterator->second;
}

std::string const FileIO::getWorkingDir() const { return workingDir; }

std::string const FileIO::getDataDir() const { return dataDir; }

std::string const FileIO::getFilePath( std::string const & pathIDIn ) const
{
    auto filePathIterator = filePaths.find( pathIDIn );

    if( filePathIterator == filePaths.end() )
    {
        throw std::runtime_error("ERROR: No Filepath set for \"" + pathIDIn +
        "\" in Filepaths.");
    }

    return filePathIterator->second;
}

std::unordered_map<std::string, std::string> const & FileIO::getAllFilePaths() 
{ return filePaths; }

void FileIO::removeFileName( std::string const & pathIDIn )
{
    auto filePathIterator = filePaths.find( pathIDIn );

    if( filePathIterator == filePaths.end() )
    {
        throw std::runtime_error("ERROR: No Filepath set for \"" + pathIDIn +
        "\" in Filepaths.");
    }

    filePaths.erase(filePathIterator);
}

std::string const FileIO::getDirectoryPath( std::string const & pathIDIn ) const
{
    auto directoryPath = directoryPaths.find( pathIDIn );

    if( directoryPath != directoryPaths.end() )
    {
        return directoryPath->second;
    }
    std::cout << "ERROR: No Directory Path set for \"" + pathIDIn +
        "\" in Directory Paths." << std::endl;

    return nullptr;
}

std::unordered_map<std::string, std::string> const & FileIO::getAllDirectoryPaths() 
{ return directoryPaths; }

std::vector<std::string> const FileIO::findAllFiles( std::string const & startingDirectory,
    std::string const & fileExtension /* = ".*" */,
    int const & recursiveSearch /*= false*/ ) const
{
    std::vector<std::string> directoriesOfFiles;

#ifdef WINDOWS

    std::string const pathToCheck = startingDirectory + "\\*.*";

    WIN32_FIND_DATA windowsFindData;

    HANDLE fileHandleInDirectory = ::FindFirstFile( pathToCheck.c_str(),
        &windowsFindData );

    if( fileHandleInDirectory != INVALID_HANDLE_VALUE )
    {
        while( ::FindNextFile( fileHandleInDirectory, &windowsFindData ) )
        {
            if( !( windowsFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
            {
                std::string const filePath = std::string( windowsFindData.cFileName );

                if( fileExtension != ".*"
                    && filePath.length() > fileExtension.length()
                    && filePath.substr( filePath.length() - fileExtension.length() )
                    == fileExtension )
                {
                    directoriesOfFiles.push_back( startingDirectory + "\\" +
                        filePath );
                }
                else if( fileExtension == ".*" )
                {
                    directoriesOfFiles.push_back( startingDirectory + "\\" +
                        filePath );
                }
            }
            else if( ( recursiveSearch == RECURSIVE_SEARCH_TRUE )
                && ( windowsFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
                && ( std::string( windowsFindData.cFileName ) != ".." )
                && ( std::string( windowsFindData.cFileName ) != "." ) )
            {
                std::vector<std::string> subDirectoryFiles =
                    findAllFiles( ( startingDirectory + "\\" + std::string( windowsFindData.cFileName ) ),
                        fileExtension, true );

                for( auto & s : subDirectoryFiles )
                {
                    directoriesOfFiles.push_back( s );
                }
            }
        }
        ::FindClose( fileHandleInDirectory );
    }

    return directoriesOfFiles;

#else
    struct dirent * fileInDirectory;

    DIR * currentDirectory =
        static_cast< DIR * > ( opendir( startingDirectory.c_str() ) );

    if( !currentDirectory )
    {
        return std::vector<std::string>();
    }
    while( ( fileInDirectory = readdir( currentDirectory ) ) != NULL )
    {
        struct stat directoryCheck;

        if( std::string( fileInDirectory->d_name ) == "." || std::string( fileInDirectory->d_name ) == ".." )
        {
            continue;
        }
        if( stat( ( startingDirectory + "/" + fileInDirectory->d_name ).c_str(),
            &directoryCheck ) < 0 )
        {
            std::cout <<
                ( startingDirectory + "/" + fileInDirectory->d_name ).c_str() <<
                std::endl;
            std::cout << "ERROR: Invalid File." << std::endl;
        }
        else if( S_ISREG( directoryCheck.st_mode ) )
        {
            std::string const filePath = std::string( fileInDirectory->d_name );

            if( fileExtension != ".*"
                && filePath.length() > fileExtension.length()
                && filePath.substr( filePath.length() - fileExtension.length() )
                == fileExtension )
            {
                directoriesOfFiles.push_back( startingDirectory + "/" +
                    filePath );
            }
            else if( fileExtension == ".*" )
            {
                directoriesOfFiles.push_back( startingDirectory + "/" +
                    filePath );
            }
        }
        else if( recursiveSearch == true
            && S_ISDIR( directoryCheck.st_mode ) )
        {
            std::vector<std::string> subDirectoryFiles =
                findAllFiles( ( startingDirectory + "/" + std::string( fileInDirectory->d_name ) ),
                    fileExtension, true );

            for( auto & s : subDirectoryFiles )
            {
                directoriesOfFiles.push_back( s );
            }
        }
    }

    closedir( currentDirectory );

    return directoriesOfFiles;

#endif
}

std::string const FileIO::getBaseFileName( std::string const & fileNameIn ) const
{
#ifdef WINDOWS

    auto tmp = fileNameIn.substr( fileNameIn.find_last_of( "\\" ) + 1 );
    tmp = tmp.substr( 0, tmp.length() - ( tmp.length() - tmp.find_last_of( "." ) ) );

    return tmp;

#else

    auto tmp = fileNameIn.substr( fileNameIn.find_last_of( "/" ) + 1 );
    tmp = tmp.substr( 0, tmp.length() - ( tmp.length() - tmp.find_last_of( "." ) ) );

    return tmp;

#endif
}
