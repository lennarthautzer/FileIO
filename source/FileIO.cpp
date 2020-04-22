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

#include "FileIO.h"

std::unique_ptr< FileIO > FileIO::singletonInstance = std::unique_ptr< FileIO >( nullptr );

FileIO & FileIO::getInstance()
{
  if ( ! singletonInstance ) { singletonInstance = std::unique_ptr< FileIO >( new FileIO() ); }

  return *singletonInstance;
}

FileIO::~FileIO()
{
  auto inputStreamIterator = inputFileStreams.begin();
  auto outputStreamIterator = outputFileStreams.begin();

  while ( inputStreamIterator != inputFileStreams.end() )
  {
    inputStreamIterator->second->close();
    ++inputStreamIterator;
  }
  while ( outputStreamIterator != outputFileStreams.end() )
  {
    outputStreamIterator->second->close();
    ++outputStreamIterator;
  }
}

FileIO & FileIO::reset()
{
  auto inputStreamIterator = inputFileStreams.begin();
  auto outputStreamIterator = outputFileStreams.begin();

  while ( inputStreamIterator != inputFileStreams.end() )
  {
    inputStreamIterator->second->close();
    ++inputStreamIterator;
  }
  while ( outputStreamIterator != outputFileStreams.end() )
  {
    outputStreamIterator->second->close();
    ++outputStreamIterator;
  }

  directoryPaths.clear();
  filePaths.clear();
  inputFileStreams.clear();
  outputFileStreams.clear();
  setBaseDirectory( findBaseDirectory() );

  return *this;
}

std::istream & FileIO::openInputStream( std::string const & pathID_In )
{
  if ( ! filePathIsValid( pathID_In ) ) { givePathNotValidError( __func__, pathID_In ); }

  auto it = getInputStreamIterator( pathID_In );

  if ( inputStreamExists( it ) ) { giveStreamAlreadyOpenError( __func__, pathID_In ); }

  inputFileStreams[ pathID_In ] =
    std::unique_ptr< std::ifstream >( new std::ifstream( getFilePath( pathID_In ), std::ifstream::in ) );

  it = getInputStreamIterator( pathID_In );

  if ( ! inputStreamExists( it ) || ! inputStreamIsValid( it ) ) { giveStreamCouldNotOpenError( __func__, pathID_In ); }

  return *it->second;
}

std::istream & FileIO::openInputStream( char const * const & pathID_In )
{
  return openInputStream( std::string( pathID_In ) );
}

std::istream & FileIO::openInputStream( std::string const & pathID_In, std::string const & filePath_In )
{
  setFilePath( pathID_In, filePath_In );

  return openInputStream( pathID_In );
}

std::istream & FileIO::openInputStream( std::string const & pathID_In, char const * const & filePath_In )
{
  setFilePath( pathID_In, std::string( filePath_In ) );

  return openInputStream( pathID_In );
}

std::istream & FileIO::openInputStream( char const * const & pathID_In, std::string const & filePath_In )
{
  std::string const _temporaryValue = std::string( pathID_In );

  setFilePath( _temporaryValue, filePath_In );

  return openInputStream( _temporaryValue );
}

std::istream & FileIO::openInputStream( char const * const & pathID_In, char const * const & filePath_In )
{
  std::string const _temporaryValue = std::string( pathID_In );

  setFilePath( _temporaryValue, std::string( filePath_In ) );

  return openInputStream( _temporaryValue );
}

std::ostream & FileIO::openOutputStream( std::string const & pathID_In, bool const & appendToFile_In )
{
  auto it = getOutputStreamIterator( pathID_In );

  if ( outputStreamExists( it ) ) { giveStreamAlreadyOpenError( __func__, pathID_In ); }

  outputFileStreams[ pathID_In ] = std::unique_ptr< std::ofstream >(
    new std::ofstream( getFilePath( pathID_In ), ( appendToFile_In ? std::ofstream::app : std::ofstream::out ) ) );

  it = getOutputStreamIterator( pathID_In );

  if ( ! outputStreamExists( it ) || ! outputStreamIsValid( it ) )
  { giveStreamCouldNotOpenError( __func__, pathID_In ); }

  return *it->second;
}

std::ostream & FileIO::openOutputStream( char const * const & pathID_In, bool const & appendToFile_In )
{
  return openOutputStream( std::string( pathID_In ), appendToFile_In );
}

std::ostream & FileIO::openOutputStream(
  std::string const & pathID_In, std::string const & filePath_In, bool const & appendToFile_In )
{
  setFilePath( pathID_In, filePath_In );

  return openOutputStream( pathID_In, appendToFile_In );
}

std::ostream & FileIO::openOutputStream(
  std::string const & pathID_In, char const * const & filePath_In, bool const & appendToFile_In )
{
  setFilePath( pathID_In, std::string( filePath_In ) );

  return openOutputStream( pathID_In, appendToFile_In );
}

std::ostream & FileIO::openOutputStream(
  char const * const & pathID_In, std::string const & filePath_In, bool const & appendToFile_In )
{
  std::string const _temporaryValue = std::string( pathID_In );

  setFilePath( _temporaryValue, filePath_In );

  return openOutputStream( _temporaryValue, appendToFile_In );
}

std::ostream & FileIO::openOutputStream(
  char const * const & pathID_In, char const * const & filePath_In, bool const & appendToFile_In )
{
  std::string const _temporaryValue = std::string( pathID_In );

  setFilePath( _temporaryValue, std::string( filePath_In ) );

  return openOutputStream( _temporaryValue, appendToFile_In );
}

FileIO & FileIO::closeInputStream( std::string const & pathID_In )
{
  auto it = getInputStreamIterator( pathID_In );

  if ( ! inputStreamExists( it ) ) { giveStreamNotOpenError( __func__, pathID_In ); }

  it->second->close();
  inputFileStreams.erase( pathID_In );

  return *this;
}

FileIO & FileIO::closeOutputStream( std::string const & pathID_In )
{
  auto it = getOutputStreamIterator( pathID_In );

  if ( ! outputStreamExists( it ) ) { giveStreamNotOpenError( __func__, pathID_In ); }

  it->second->close();
  outputFileStreams.erase( pathID_In );

  return *this;
}

FileIO & FileIO::resetInputStreamToFileStart( std::string const & pathID_In )
{
  auto it = getInputStreamIterator( pathID_In );

  if ( ! inputStreamExists( it ) || ! inputStreamIsValid( it ) ) { giveStreamNotOpenError( __func__, pathID_In ); }

  it->second->clear();
  it->second->seekg( 0, std::ios::beg );

  return *this;
}

FileIO & FileIO::readLine( std::string const & pathID_In, std::string & lineToWriteTo )
{
  auto it = getInputStreamIterator( pathID_In );

  if ( ! inputStreamExists( it ) || ! inputStreamIsValid( it ) ) { giveStreamNotOpenError( __func__, pathID_In ); }

  getline( *( it->second ), lineToWriteTo );

  return *this;
}

std::string FileIO::readLine( std::string const & pathID_In )
{
  std::string lineToReturn;

  readLine( pathID_In, lineToReturn );

  return lineToReturn;
}

FileIO & FileIO::writeLine( std::string const & pathID_In, std::string const & lineToWrite_In )
{
  auto it = getOutputStreamIterator( pathID_In );

  if ( ! outputStreamExists( it ) || ! outputStreamIsValid( it ) ) { giveStreamNotOpenError( __func__, pathID_In ); }

  *it->second << lineToWrite_In;

  return *this;
}

std::string FileIO::readFileAsString( std::string const & pathID_In )
{
  auto it = getInputStreamIterator( pathID_In );
  bool hasOpenedNewStream = false;

  if ( ! inputStreamExists( it ) )
  {
    openInputStream( pathID_In );
    it = getInputStreamIterator( pathID_In );
    hasOpenedNewStream = true;
  }
  else
  {
    resetInputStreamToFileStart( pathID_In );
  }

  if ( ! inputStreamExists( it ) || ! inputStreamIsValid( it ) ) { giveStreamNotOpenError( __func__, pathID_In ); }

  std::stringstream _temporaryValue;

  _temporaryValue << it->second->rdbuf();

  if ( hasOpenedNewStream ) { closeInputStream( pathID_In ); }

  return _temporaryValue.str();
}

std::vector< std::string > FileIO::readFileAsVectorOfStrings(
  std::string const & pathID_In, std::string const & delimitingCharacters_In /* = "\n\r" */ )
{
  auto it = getInputStreamIterator( pathID_In );
  bool hasOpenedNewStream = false;

  if ( ! inputStreamExists( it ) )
  {
    openInputStream( pathID_In );
    it = getInputStreamIterator( pathID_In );
    hasOpenedNewStream = true;
  }
  else
  {
    resetInputStreamToFileStart( pathID_In );
  }

  if ( ! inputStreamExists( it ) || ! inputStreamIsValid( it ) ) { giveStreamNotOpenError( __func__, pathID_In ); }

  char currentChar;
  bool jumpToNextChar;
  std::vector< char > stringToAdd;
  std::vector< std::string > returnValues;

  while ( it->second->good() )
  {
    jumpToNextChar = false;

    it->second->get( currentChar );

    for ( char const delimiter : delimitingCharacters_In )
    {
      if ( ! jumpToNextChar && currentChar == delimiter )
      {
        std::string const _temporaryValue( stringToAdd.begin(), stringToAdd.end() );

        if ( _temporaryValue != "" ) { returnValues.push_back( _temporaryValue ); }

        stringToAdd.clear();
        jumpToNextChar = true;
      }
    }

    if ( ! jumpToNextChar ) { stringToAdd.push_back( currentChar ); }
  }

  std::string const _temporaryValue( stringToAdd.begin(), stringToAdd.end() - 1 );

  if ( _temporaryValue != "" ) { returnValues.push_back( _temporaryValue ); }

  if ( ! it->second->eof() && it->second->fail() ) { givePathNotReadableError( __func__, pathID_In ); }

  if ( hasOpenedNewStream ) { closeInputStream( pathID_In ); }

  return returnValues;
}

std::vector< std::vector< std::string > > FileIO::readFileAsVectorOfVectors( std::string const & pathID_In,
  std::string const & horizontalDelimitingCharacters_In /* = "," */,
  std::string const & verticalDelimitingCharacters_In /* = "\n\r" */ )
{
  auto it = getInputStreamIterator( pathID_In );
  bool hasOpenedNewStream = false;

  if ( ! inputStreamExists( it ) )
  {
    openInputStream( pathID_In );
    it = getInputStreamIterator( pathID_In );
    hasOpenedNewStream = true;
  }
  else
  {
    resetInputStreamToFileStart( pathID_In );
  }

  if ( ! inputStreamExists( it ) || ! inputStreamIsValid( it ) ) { giveStreamNotOpenError( __func__, pathID_In ); }

  char currentChar;
  bool jumpToNextChar;
  std::vector< char > stringToAdd;
  std::vector< std::string > rowValues;
  std::vector< std::vector< std::string > > returnValues;

  while ( it->second->good() )
  {
    jumpToNextChar = false;

    it->second->get( currentChar );

    for ( char const delimiter : horizontalDelimitingCharacters_In )
    {
      if ( ! jumpToNextChar && currentChar == delimiter )
      {
        std::string const _temporaryValue( stringToAdd.begin(), stringToAdd.end() );

        if ( _temporaryValue != "" ) { rowValues.push_back( _temporaryValue ); }

        stringToAdd.clear();
        jumpToNextChar = true;
      }
    }

    for ( char const delimiter : verticalDelimitingCharacters_In )
    {
      if ( ! jumpToNextChar && currentChar == delimiter )
      {
        std::string const _temporaryValue( stringToAdd.begin(), stringToAdd.end() );

        if ( _temporaryValue != "" ) { rowValues.push_back( _temporaryValue ); }

        if ( rowValues.size() > 0 ) { returnValues.push_back( rowValues ); }

        stringToAdd.clear();
        rowValues.clear();
        jumpToNextChar = true;
      }
    }

    if ( ! jumpToNextChar ) { stringToAdd.push_back( currentChar ); }
  }

  std::string const _temporaryValue( stringToAdd.begin(), stringToAdd.end() );

  if ( _temporaryValue != "" ) { rowValues.push_back( _temporaryValue ); }

  if ( rowValues.size() > 0 ) { returnValues.push_back( rowValues ); }

  if ( ! it->second->eof() && it->second->fail() ) { givePathNotReadableError( __func__, pathID_In ); }

  if ( hasOpenedNewStream ) { closeInputStream( pathID_In ); }

  return returnValues;
}

FileIO & FileIO::setBaseDirectory( std::string const & baseDirectory_In )
{
  struct stat directoryCheck;

  if ( stat( baseDirectory_In.c_str(), &directoryCheck ) == 0 && directoryCheck.st_mode & S_IFDIR )
  { baseDirectory = baseDirectory_In; }
  else
  {
    perror( "ERROR: " );
  }

  return *this;
}

FileIO & FileIO::setFilePath( std::string const & pathID_In, std::string const & filePath_In )
{
  filePaths[ pathID_In ] = filePath_In;

  return *this;
}

FileIO & FileIO::setDirectoryPath( std::string const & pathID_In, std::string const & pathToSet_In )
{
  directoryPaths[ pathID_In ] = pathToSet_In;

  return *this;
}

std::istream & FileIO::getInputStream( std::string const & pathID_In ) const
{
  auto it = getInputStreamIterator( pathID_In );

  if ( ! inputStreamExists( it ) || ! inputStreamIsValid( it ) ) { giveStreamNotOpenError( __func__, pathID_In ); }

  return *( static_cast< std::istream * >( &( *it->second ) ) );
}

std::ostream & FileIO::getOutputStream( std::string const & pathID_In ) const
{
  auto it = getOutputStreamIterator( pathID_In );

  if ( ! outputStreamExists( it ) || ! outputStreamIsValid( it ) ) { giveStreamNotOpenError( __func__, pathID_In ); }

  return *( static_cast< std::ostream * >( &( *it->second ) ) );
}

std::vector< std::string > FileIO::getAllOpenInputStreams() const
{
  std::vector< std::string > _returnValues;

  for ( auto & stream : inputFileStreams )
  { _returnValues.push_back( stream.first + " | " + getFilePath( stream.first ) ); }

  std::sort( _returnValues.begin(), _returnValues.end() );

  return _returnValues;
}
std::vector< std::string > FileIO::getAllOpenOutputStreams() const
{
  std::vector< std::string > _returnValues;

  for ( auto & stream : outputFileStreams )
  { _returnValues.push_back( stream.first + " | " + getFilePath( stream.first ) ); }

  std::sort( _returnValues.begin(), _returnValues.end() );

  return std::move( _returnValues );
}

std::string const & FileIO::getBaseDirectory() const { return baseDirectory; }

std::string const & FileIO::getFilePath( std::string const & pathID_In ) const
{
  auto it = getFilePathIterator( pathID_In );

  if ( ! filePathExists( it ) ) { givePathNotFoundError( __func__, pathID_In ); }

  return it->second;
}

FileIO & FileIO::removeFilePath( std::string const & pathID_In )
{
  auto it = getFilePathIterator( pathID_In );

  if ( ! filePathExists( it ) ) { givePathNotFoundError( __func__, pathID_In ); }

  filePaths.erase( it );

  return *this;
}

std::string const & FileIO::getDirectoryPath( std::string const & pathID_In ) const
{
  auto it = getDirectoryPathIterator( pathID_In );

  if ( ! directoryPathExists( it ) ) { givePathNotFoundError( __func__, pathID_In ); }

  return it->second;
}

FileIO & FileIO::removeDirectoryPath( std::string const & pathID_In )
{
  auto it = getDirectoryPathIterator( pathID_In );

  if ( ! directoryPathExists( it ) ) { givePathNotFoundError( __func__, pathID_In ); }

  directoryPaths.erase( it );

  return *this;
}

std::vector< std::string > FileIO::findAllFiles( std::string const & startingDirectory_In,
  std::string const & fileExtension_In /* = ".*" */, int const & recursiveSearch_In /* = RecursiveSearchFalse */ ) const
{
  std::vector< std::string > directoriesOfFiles;

#ifdef WINDOWS

  std::string const pathToCheck = startingDirectory_In + "\\*.*";

  WIN32_FIND_DATA windowsFindData;

  HANDLE fileHandle_InDirectory = ::FindFirstFile( CA2W( pathToCheck.c_str() ), &windowsFindData );

  if ( fileHandle_InDirectory != INVALID_HANDLE_VALUE )
  {
    while ( ::FindNextFile( fileHandle_InDirectory, &windowsFindData ) )
    {
      if ( ! ( windowsFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
      {
        std::string const filePath = CW2A( windowsFindData.cFileName );

        if ( fileExtension_In != ".*" && filePath.length() > fileExtension_In.length()
          && filePath.substr( filePath.length() - fileExtension_In.length() ) == fileExtension_In )
        { directoriesOfFiles.push_back( startingDirectory_In + "\\" + filePath ); }

        else if ( fileExtension_In == ".*" )
        {
          directoriesOfFiles.push_back( startingDirectory_In + "\\" + filePath );
        }
      }
      else if ( ( recursiveSearch_In == RecursiveSearchTrue )
        && ( windowsFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        && ( CW2A( windowsFindData.cFileName ) != ".." ) && ( CW2A( windowsFindData.cFileName ) != "." ) )
      {
        std::vector< std::string > subDirectoryFiles = findAllFiles(
          ( startingDirectory_In + "\\" + std::string( CW2A( windowsFindData.cFileName ) ) ), fileExtension_In, true );

        for ( auto & file : subDirectoryFiles ) { directoriesOfFiles.push_back( file ); }
      }
    }
    ::FindClose( fileHandle_InDirectory );
  }

  return directoriesOfFiles;

#else

  struct dirent * file_InDirectory;

  DIR * currentDirectory = static_cast< DIR * >( opendir( startingDirectory_In.c_str() ) );

  if ( ! currentDirectory ) { return std::vector< std::string >(); }

  while ( ( file_InDirectory = readdir( currentDirectory ) ) != NULL )
  {
    struct stat directoryCheck;

    if ( std::string( file_InDirectory->d_name ) == "." || std::string( file_InDirectory->d_name ) == ".." )
    { continue; }

    if ( stat( ( startingDirectory_In + "/" + file_InDirectory->d_name ).c_str(), &directoryCheck ) < 0 )
    { throw std::runtime_error( "ERROR: _Invalid File." ); }

    else if ( S_ISREG( directoryCheck.st_mode ) )
    {
      std::string const filePath = std::string( file_InDirectory->d_name );

      if ( fileExtension_In != ".*" && filePath.length() > fileExtension_In.length()
        && filePath.substr( filePath.length() - fileExtension_In.length() ) == fileExtension_In )
      { directoriesOfFiles.push_back( startingDirectory_In + "/" + filePath ); }

      else if ( fileExtension_In == ".*" )
      {
        directoriesOfFiles.push_back( startingDirectory_In + "/" + filePath );
      }
    }
    else if ( recursiveSearch_In == true && S_ISDIR( directoryCheck.st_mode ) )
    {
      std::vector< std::string > subDirectoryFiles = findAllFiles(
        ( startingDirectory_In + "/" + std::string( file_InDirectory->d_name ) ), fileExtension_In, true );

      for ( auto & file : subDirectoryFiles ) { directoriesOfFiles.push_back( file ); }
    }
  }

  closedir( currentDirectory );

  return directoriesOfFiles;

#endif
}

std::string FileIO::getBaseFileName( std::string const & fileName_In ) const
{
  std::size_t const windowsStyleDelimiterPosition = fileName_In.find_last_of( "\\" );
  std::size_t const nixStyleDelimiterPosition = fileName_In.find_last_of( "/" );
  bool const windowsStyleDelimiterFound = ( windowsStyleDelimiterPosition != std::string::npos );
  bool const nixStyleDelimiterFound = ( windowsStyleDelimiterPosition != std::string::npos );

  std::string returnString;
  std::size_t lastDelimiter = ( windowsStyleDelimiterFound && nixStyleDelimiterFound ?
      std::max( windowsStyleDelimiterPosition, nixStyleDelimiterPosition ) :
      ( windowsStyleDelimiterFound ? windowsStyleDelimiterPosition : nixStyleDelimiterPosition ) );

  returnString = fileName_In.substr( lastDelimiter + 1 );

  std::size_t const extensionDelimiter = returnString.find_last_of( "." );
  bool const extensionDelimiterFound = ( extensionDelimiter != std::string::npos );
  std::size_t const returnStringLength = returnString.length();

  returnString = ( extensionDelimiterFound ?
      returnString.substr( 0, ( returnStringLength - ( returnStringLength - extensionDelimiter ) ) ) :
      returnString );

  return returnString;
}

std::size_t FileIO::getNumberOfOpenInputStreams() const { return inputFileStreams.size(); }

std::size_t FileIO::getNumberOfOpenOutputStreams() const { return outputFileStreams.size(); }

bool FileIO::inputStreamExists( std::string const & pathID_In ) const
{
  return inputFileStreams.find( pathID_In ) != inputFileStreams.end();
}

bool FileIO::inputStreamIsValid( std::string const & pathID_In ) const
{
  auto it = getInputStreamIterator( pathID_In );

  return inputStreamExists( it ) & it->second->good() & it->second->is_open();
}

bool FileIO::outputStreamExists( std::string const & pathID_In ) const
{
  return outputFileStreams.find( pathID_In ) != outputFileStreams.end();
}

bool FileIO::outputStreamIsValid( std::string const & pathID_In ) const
{
  auto it = getOutputStreamIterator( pathID_In );

  return outputStreamExists( it ) & it->second->good() & it->second->is_open();
}

bool FileIO::filePathExists( std::string const & pathID_In ) const
{
  return filePaths.find( pathID_In ) != filePaths.end();
}

bool FileIO::filePathIsValid( std::string const & pathID_In ) const
{
  struct stat fileCheck;

  if ( filePathExists( pathID_In ) && stat( getFilePath( pathID_In ).c_str(), &fileCheck ) == 0
    && fileCheck.st_mode & S_IFREG )
  { return true; }

  return false;
}

bool FileIO::directoryPathExists( std::string const & pathID_In ) const
{
  return directoryPaths.find( pathID_In ) != directoryPaths.end();
}

bool FileIO::directoryPathIsValid( std::string const & pathID_In ) const
{
  struct stat directoryCheck;

  if ( directoryPathExists( pathID_In ) && stat( getDirectoryPath( pathID_In ).c_str(), &directoryCheck ) == 0
    && directoryCheck.st_mode & S_IFDIR )
  { return true; }

  return false;
}

FileIO::FileIO() { setBaseDirectory( findBaseDirectory() ); }

std::unordered_map< std::string, std::unique_ptr< std::ifstream > >::const_iterator FileIO::getInputStreamIterator(
  std::string const & pathID_In ) const
{
  return inputFileStreams.find( pathID_In );
}

bool FileIO::inputStreamExists(
  std::unordered_map< std::string, std::unique_ptr< std::ifstream > >::const_iterator const & streamIterator_In ) const
{
  return streamIterator_In != inputFileStreams.end();
}

bool FileIO::inputStreamIsValid(
  std::unordered_map< std::string, std::unique_ptr< std::ifstream > >::const_iterator const & streamIterator_In ) const
{
  return streamIterator_In->second->good() & streamIterator_In->second->is_open();
}

std::unordered_map< std::string, std::unique_ptr< std::ofstream > >::const_iterator FileIO::getOutputStreamIterator(
  std::string const & pathID_In ) const
{
  return outputFileStreams.find( pathID_In );
}

bool FileIO::outputStreamExists(
  std::unordered_map< std::string, std::unique_ptr< std::ofstream > >::const_iterator const & streamIterator_In ) const
{
  return streamIterator_In != outputFileStreams.end();
}

bool FileIO::outputStreamIsValid(
  std::unordered_map< std::string, std::unique_ptr< std::ofstream > >::const_iterator const & streamIterator_In ) const
{
  return streamIterator_In->second->good() & streamIterator_In->second->is_open();
}

std::unordered_map< std::string, std::string >::const_iterator FileIO::getFilePathIterator(
  std::string const & pathID_In ) const
{
  return filePaths.find( pathID_In );
}

bool FileIO::filePathExists(
  std::unordered_map< std::string, std::string >::const_iterator const & pathIterator_In ) const
{
  return pathIterator_In != filePaths.end();
}

std::unordered_map< std::string, std::string >::const_iterator FileIO::getDirectoryPathIterator(
  std::string const & pathID_In ) const
{
  return directoryPaths.find( pathID_In );
}

bool FileIO::directoryPathExists(
  std::unordered_map< std::string, std::string >::const_iterator const & pathIterator_In ) const
{
  return pathIterator_In != directoryPaths.end();
}

std::string FileIO::findBaseDirectory() const
{
  char _temporaryValueBuffer[ FILENAME_MAX ];

  getCurrentDirectory( _temporaryValueBuffer, FILENAME_MAX );

  return std::string( _temporaryValueBuffer );
}

// File IO error checking
void FileIO::giveStreamAlreadyOpenError( std::string const & throwingFunction_In, std::string const & pathID_In ) const
{
  throw std::runtime_error(
    "ERROR: FileIO::" + throwingFunction_In + " - Stream \"" + pathID_In + "\" is already open!" );
}

void FileIO::giveStreamNotOpenError( std::string const & throwingFunction_In, std::string const & pathID_In ) const
{
  throw std::runtime_error( "ERROR: FileIO::" + throwingFunction_In + " - Stream \"" + pathID_In + "\" is not open!" );
}

void FileIO::giveStreamCouldNotOpenError( std::string const & throwingFunction_In, std::string const & pathID_In ) const
{
  throw std::runtime_error(
    "ERROR: FileIO::" + throwingFunction_In + " - Stream \"" + pathID_In + "\" could not be opened!" );
}

void FileIO::givePathNotFoundError( std::string const & throwingFunction_In, std::string const & pathID_In ) const
{
  throw std::runtime_error(
    "ERROR: FileIO::" + throwingFunction_In + " - Path \"" + pathID_In + "\" could not be found!" );
}

void FileIO::givePathNotValidError( std::string const & throwingFunction_In, std::string const & pathID_In ) const
{
  throw std::runtime_error( "ERROR: FileIO::" + throwingFunction_In + " - Path \"" + pathID_In + "\" ("
    + getFilePath( pathID_In ) + ") could be found but is an invalid format!" );
}

void FileIO::givePathNotReadableError( std::string const & throwingFunction_In, std::string const & pathID_In ) const
{
  throw std::runtime_error( "ERROR: FileIO::" + throwingFunction_In + " - Path \"" + pathID_In + "\" ("
    + getFilePath( pathID_In ) + ") could be found but not read!" );
}