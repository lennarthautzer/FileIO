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
    setBaseDirectory( findBaseDirectory() );
}

FileIO::~FileIO()
{
    auto inputStreamIterator = inputFileStreams.begin();
    auto outputStreamIterator = outputFileStreams.begin();

    while( true )
    {
        if( inputStreamIterator != inputFileStreams.end() )
        {
            inputStreamIterator->second->close();
            ++inputStreamIterator;
        }
        if( outputStreamIterator != outputFileStreams.end() )
        {
            outputStreamIterator->second->close();
            ++outputStreamIterator;
        }
        if( inputStreamIterator == inputFileStreams.end()
            && outputStreamIterator == outputFileStreams.end() )
        { break; }
    }
}

std::istream & FileIO::openInputStream( std::string const & pathIDIn )
{
    auto it = getInputStreamIterator(pathIDIn);

    if( inputStreamExists(it) )
    { giveStreamAlreadyOpenError(pathIDIn); }

    inputFileStreams[ pathIDIn ] = std::unique_ptr<std::ifstream>
        ( new std::ifstream( getFilePath( pathIDIn ), std::ifstream::in ) );

    it = getInputStreamIterator(pathIDIn);

    if( !inputStreamExists(it) || !inputStreamIsValid(it) )
    { giveStreamCouldNotOpenError(pathIDIn); }
    
    return *it->second;
}

std::istream & FileIO::openInputStream( char const * const pathIDIn )
{ return openInputStream( std::string( pathIDIn ) ); }

std::istream & FileIO::openInputStream( std::string const & pathIDIn, std::string const & filePathIn )
{
    setFilePath( pathIDIn, filePathIn );
    return openInputStream( pathIDIn );
}

std::istream & FileIO::openInputStream( char const * const streamIDIn, char const * const filePathIn )
{
    std::string const tmp = std::string( streamIDIn );

    setFilePath( tmp, std::string( filePathIn ) );
    return openInputStream( tmp );
}

std::ostream & FileIO::openOutputStream( std::string const & pathIDIn )
{
    auto it = getOutputStreamIterator(pathIDIn);

    if( outputStreamExists(it) )
    { giveStreamAlreadyOpenError(pathIDIn); }

    outputFileStreams[ pathIDIn ] = std::unique_ptr<std::ofstream>
        ( new std::ofstream( getFilePath( pathIDIn ), std::ofstream::out ) );
    
    it = getOutputStreamIterator(pathIDIn);

    if( !outputStreamExists(it) || !outputStreamIsValid(it) )
    { giveStreamCouldNotOpenError(pathIDIn); }

    return *it->second;
}

std::ostream & FileIO::openOutputStream( char const * streamIDIn )
{ return openOutputStream( std::string( streamIDIn ) ); }

std::ostream & FileIO::openOutputStream( std::string const & streamIDIn, std::string const & filePathIn )
{
    setFilePath( streamIDIn, filePathIn );
    return openOutputStream( streamIDIn );
}

std::ostream & FileIO::openOutputStream( char const * & streamIDIn, char const * filePathIn )
{
    std::string const tmp = std::string( streamIDIn );

    setFilePath( tmp, std::string( filePathIn ) );
    return openOutputStream( tmp );
}

FileIO & FileIO::closeInputStream( std::string const & streamIDIn )
{
    auto it = getInputStreamIterator(streamIDIn);

    if( !inputStreamExists(it) )
    { giveStreamNotOpenError(streamIDIn); }

    it->second->close();
    inputFileStreams.erase( streamIDIn );

    return *this;
}

FileIO & FileIO::closeOutputStream( std::string const & streamIDIn )
{
    auto it = getOutputStreamIterator(streamIDIn);

    if( !outputStreamExists(it) )
    { giveStreamNotOpenError(streamIDIn); }

    it->second->close();
    outputFileStreams.erase( streamIDIn );

    return *this;
}

FileIO & FileIO::resetInputStreamToFileStart( std::string const & streamIDIn )
{
     auto it = getInputStreamIterator(streamIDIn);

    if( !inputStreamExists(it) || !inputStreamIsValid(it))
    { giveStreamNotOpenError(streamIDIn); }

    it->second->clear();
    it->second->seekg( 0, std::ios::beg );

    return *this;
}

FileIO & FileIO::readLine( std::string const & streamIDIn, std::string & lineToWriteTo )
{
    auto it = getInputStreamIterator(streamIDIn);

    if( !inputStreamExists(it) || !inputStreamIsValid(it))
    { giveStreamNotOpenError(streamIDIn); }

    getline( *(it->second ), lineToWriteTo );

    return *this;
}

std::string const FileIO::readLine( std::string const & streamIDIn)
{
    std::string lineToReturn;

    readLine(streamIDIn, lineToReturn);

    return lineToReturn;
}

FileIO & FileIO::writeLine( std::string const & streamIDIn, std::string const & lineToWrite )
{
    auto it = getOutputStreamIterator(streamIDIn);

    if( !outputStreamExists(it) || !outputStreamIsValid(it))
    { giveStreamNotOpenError(streamIDIn); }

    *it->second << lineToWrite;

    return *this;
}

FileIO & FileIO::setBaseDirectory( std::string const & baseDirectoryIn )
{
    struct stat directoryCheck;

    if( stat( baseDirectoryIn.c_str(), &directoryCheck ) == 0 && directoryCheck.st_mode & S_IFDIR )
    { baseDirectory = baseDirectoryIn; }
    else
    { perror( "ERROR: " ); }

    return *this;
}

FileIO & FileIO::setFilePath( std::string const & pathIDIn, std::string const & filePathIn )
{
    filePaths[ pathIDIn ] = filePathIn;

    return *this;
}

FileIO & FileIO::setDirectoryPath( std::string const & pathIDIn, std::string const & pathToSetIn )
{
    directoryPaths[ pathIDIn ] = pathToSetIn;

    return *this;
}

std::istream & FileIO::getInputStream( std::string const & streamIDIn ) const
{
    auto it = getInputStreamIterator(streamIDIn);

    if( !inputStreamExists(it) || !inputStreamIsValid(it))
    { giveStreamNotOpenError(streamIDIn); }

    return *( static_cast<std::istream *>( &(*it->second) ) );
}

std::ostream & FileIO::getOutputStream( std::string const & streamIDIn ) const
{
    auto it = getOutputStreamIterator(streamIDIn);

    if( !outputStreamExists(it) || !outputStreamIsValid(it))
    { giveStreamNotOpenError(streamIDIn); }

    return *( static_cast<std::ostream *>( &(*it->second) ) );
}

std::string const FileIO::getBaseDirectory() const { return baseDirectory; }

std::string const FileIO::getFilePath( std::string const & pathIDIn ) const
{
    auto it = getFilePathIterator(pathIDIn);

    if( !filePathExists(it) )
    { givePathNotFoundError(pathIDIn); }

    return it->second;
}

std::unordered_map<std::string, std::string> const & FileIO::getAllFilePaths() const
{ return filePaths; }

FileIO & FileIO::removeFilePath( std::string const & pathIDIn )
{
    auto it = getFilePathIterator(pathIDIn);

    if( !filePathExists(it) )
    { givePathNotFoundError(pathIDIn); }

    filePaths.erase( it );

    return *this;
}

std::string const FileIO::getDirectoryPath( std::string const & pathIDIn ) const
{
    auto it = getDirectoryPathIterator(pathIDIn);

    if( !directoryPathExists(it) )
    { givePathNotFoundError(pathIDIn); }
    
    return it->second;
}

std::unordered_map<std::string, std::string> const & FileIO::getAllDirectoryPaths() const
{ return directoryPaths; }

FileIO & FileIO::removeDirectoryPath( std::string const & pathIDIn )
{
    auto it = getDirectoryPathIterator(pathIDIn);

    if( !directoryPathExists(it) )
    { givePathNotFoundError(pathIDIn); }

    directoryPaths.erase( it );

    return *this;
}

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
                    && filePath.substr( filePath.length() - fileExtension.length() ) == fileExtension )
                { directoriesOfFiles.push_back( startingDirectory + "\\" + filePath ); }

                else if( fileExtension == ".*" )
                { directoriesOfFiles.push_back( startingDirectory + "\\" + filePath ); }
            }
            else if( ( recursiveSearch == recursiveSearchTrue )
                && ( windowsFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
                && ( std::string( windowsFindData.cFileName ) != ".." )
                && ( std::string( windowsFindData.cFileName ) != "." ) )
            {
                std::vector<std::string> subDirectoryFiles =
                    findAllFiles( ( startingDirectory + "\\" + std::string( windowsFindData.cFileName ) ),
                        fileExtension, true );

                for( auto & file : subDirectoryFiles )
                {
                    directoriesOfFiles.push_back( file );
                }
            }
        }
        ::FindClose( fileHandleInDirectory );
    }

    return directoriesOfFiles;

#else

    struct dirent * fileInDirectory;

    DIR * currentDirectory = static_cast< DIR * > ( opendir( startingDirectory.c_str() ) );

    if( !currentDirectory )
    { return std::vector<std::string>(); }

    while( ( fileInDirectory = readdir( currentDirectory ) ) != NULL )
    {
        struct stat directoryCheck;

        if( std::string( fileInDirectory->d_name ) == "." || std::string( fileInDirectory->d_name ) == ".." )
        { continue; }

        if( stat( ( startingDirectory + "/" + fileInDirectory->d_name ).c_str(), &directoryCheck ) < 0 )
        { throw std::runtime_error("ERROR: Invalid File."); }

        else if( S_ISREG( directoryCheck.st_mode ) )
        {
            std::string const filePath = std::string( fileInDirectory->d_name );

            if( fileExtension != ".*"
                && filePath.length() > fileExtension.length()
                && filePath.substr( filePath.length() - fileExtension.length() )
                == fileExtension )
            { directoriesOfFiles.push_back( startingDirectory + "/" + filePath ); }

            else if( fileExtension == ".*" )
            { directoriesOfFiles.push_back( startingDirectory + "/" + filePath ); }
        }
        else if( recursiveSearch == true
            && S_ISDIR( directoryCheck.st_mode ) )
        {
            std::vector<std::string> subDirectoryFiles = findAllFiles( ( startingDirectory + "/" + 
                std::string( fileInDirectory->d_name ) ), fileExtension, true );

            for( auto & file : subDirectoryFiles )
            { directoriesOfFiles.push_back( file ); }
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

bool const FileIO::inputStreamExists(std::string const & streamIDIn) const
{ return inputFileStreams.find( streamIDIn ) != inputFileStreams.end(); }

bool const FileIO::inputStreamIsValid(std::string const & streamIDIn) const
{ return inputStreamExists(streamIDIn) & inputFileStreams.find( streamIDIn )->second->good(); }

bool const FileIO::outputStreamExists(std::string const & streamIDIn) const
{ return outputFileStreams.find( streamIDIn ) != outputFileStreams.end(); }

bool const FileIO::outputStreamIsValid(std::string const & streamIDIn) const
{ return outputStreamExists(streamIDIn) & outputFileStreams.find( streamIDIn )->second->good(); }

bool const FileIO::filePathExists(std::string const & pathIDIn) const
{ return filePaths.find( pathIDIn ) != filePaths.end(); }

bool const FileIO::directoryPathExists(std::string const & pathIDIn) const
{ return directoryPaths.find( pathIDIn ) != directoryPaths.end(); }

std::unordered_map< std::string, std::unique_ptr<std::ifstream> >::const_iterator 
    FileIO::getInputStreamIterator(std::string const & streamIDIn) const
{ return inputFileStreams.find( streamIDIn ); }

bool const FileIO::inputStreamExists(std::unordered_map< std::string, 
    std::unique_ptr<std::ifstream> >::const_iterator & streamIteratorIn) const
{ return streamIteratorIn != inputFileStreams.end(); }

bool const FileIO::inputStreamIsValid(std::unordered_map< std::string, 
    std::unique_ptr<std::ifstream> >::const_iterator & streamIteratorIn) const
{ return streamIteratorIn->second->good(); }

std::unordered_map< std::string, std::unique_ptr<std::ofstream> >::const_iterator 
    FileIO::getOutputStreamIterator(std::string const & streamIDIn) const
{ return outputFileStreams.find( streamIDIn ); }

bool const FileIO::outputStreamExists(std::unordered_map< std::string, 
    std::unique_ptr<std::ofstream> >::const_iterator & streamIteratorIn) const
{ return streamIteratorIn != outputFileStreams.end(); }

bool const FileIO::outputStreamIsValid(std::unordered_map< std::string, 
    std::unique_ptr<std::ofstream> >::const_iterator & streamIteratorIn) const
{ return streamIteratorIn->second->good(); }

std::unordered_map<std::string, std::string>::const_iterator 
        FileIO::getFilePathIterator(std::string const & pathIDIn) const
{ return filePaths.find(pathIDIn); }
    
bool const FileIO::filePathExists(std::unordered_map< std::string, 
    std::string>::const_iterator & pathIteratorIn) const
{ return pathIteratorIn != filePaths.end(); }

std::unordered_map<std::string, std::string>::const_iterator 
    FileIO::getDirectoryPathIterator(std::string const & pathIDIn) const
{ return directoryPaths.find(pathIDIn); }

bool const FileIO::directoryPathExists(std::unordered_map< std::string, 
    std::string>::const_iterator & pathIteratorIn) const
    { return pathIteratorIn != directoryPaths.end(); }

std::string const FileIO::findBaseDirectory() const
{
    char tmpBuffer[ FILENAME_MAX ];

    getCurrentDirectory( tmpBuffer, FILENAME_MAX );

    return std::string( tmpBuffer );
}

void FileIO::giveStreamAlreadyOpenError(std::string const & streamIDIn) const
{ throw std::runtime_error("ERROR: Stream \"" + streamIDIn + "\" is already open!"); }

void FileIO::giveStreamNotOpenError(std::string const & streamIDIn) const
{ throw std::runtime_error("ERROR: Stream \"" + streamIDIn + "\" is not open!"); }

void FileIO::giveStreamCouldNotOpenError(std::string const & streamIDIn) const
{ throw std::runtime_error("ERROR: Stream \"" + streamIDIn + "\" could not be opened!"); }

void FileIO::givePathNotFoundError(std::string const & pathIDIn) const
{ throw std::runtime_error("ERROR: Path \"" + pathIDIn + "\" could not be found!");}