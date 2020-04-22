#include "FileIO_UnitTests.h"

std::unique_ptr< FileIO_UnitTests > FileIO_UnitTests::singletonInstance =
  std::unique_ptr< FileIO_UnitTests >( nullptr );

FileIO_UnitTests & FileIO_UnitTests::getInstance()
{
  if ( ! singletonInstance ) { singletonInstance = std::unique_ptr< FileIO_UnitTests >( new FileIO_UnitTests() ); }

  return *singletonInstance;
}

void FileIO_UnitTests::runAllUnitTests()
{
  runResetTest();
  runDirectoriesTest();
  runFileSearchTest();
  runFileReadTest();
  runFileWriteTest();
  runOverloadTest();
  runWholeFileReadTest();
}

void FileIO_UnitTests::runResetTest()
{
  fileIO.setDirectoryPath( "data", fileIO.getBaseDirectory() + "/data" );
  fileIO.setFilePath( "testData", fileIO.getDirectoryPath( "data" ) + "/fileIOTestData.txt" );
  fileIO.setFilePath( "resetTest", fileIO.getDirectoryPath( "data" ) + "/fileIOResetTest.txt" );

  fileIO.openInputStream( "testData" );
  fileIO.openOutputStream( "resetTest" );

  dessert( ( fileIO.getNumberOfOpenInputStreams() == 1 ) ) << ( "ERROR: getNumberOfOpenInputStreams() - EXPECTED: "
    + std::to_string( 1 ) + " RECEIVED: " + std::to_string( fileIO.getNumberOfOpenInputStreams() ) );
  dessert( ( fileIO.getNumberOfOpenOutputStreams() == 1 ) ) << ( "ERROR: getNumberOfOpenOutputStreams() - EXPECTED: "
    + std::to_string( 1 ) + " RECEIVED: " + std::to_string( fileIO.getNumberOfOpenOutputStreams() ) );

  auto inStreams = fileIO.getAllOpenInputStreams();
  auto outStreams = fileIO.getAllOpenOutputStreams();

  dessert( ( inStreams.size() == 1 && inStreams[ 0 ].substr( 0, strlen( "testData |" ) ) == "testData |" ) )
    << ( "ERROR: getAllOpenInputStreams() - EXPECTED: " + std::to_string( 1 ) + ", testData |"
         + " RECEIVED: " + std::to_string( inStreams.size() ) + ", " + inStreams[ 0 ] );

  dessert( ( outStreams.size() == 1 && outStreams[ 0 ].substr( 0, strlen( "resetTest |" ) ) == "resetTest |" ) )
    << ( "ERROR: getAllOpenOutputStreams() - EXPECTED: " + std::to_string( 1 ) + ", resetTest |"
         + " RECEIVED: " + std::to_string( outStreams.size() ) + ", " + outStreams[ 0 ] );

  fileIO.reset();

  dessert( ( fileIO.getNumberOfOpenInputStreams() == 0 ) ) << ( "ERROR: getNumberOfOpenInputStreams() - EXPECTED: "
    + std::to_string( 0 ) + " RECEIVED: " + std::to_string( fileIO.getNumberOfOpenInputStreams() ) );
  dessert( ( fileIO.getNumberOfOpenOutputStreams() == 0 ) ) << ( "ERROR: getNumberOfOpenOutputStreams() - EXPECTED: "
    + std::to_string( 0 ) + " RECEIVED: " + std::to_string( fileIO.getNumberOfOpenOutputStreams() ) );

  inStreams = fileIO.getAllOpenInputStreams();
  outStreams = fileIO.getAllOpenOutputStreams();

  dessert( ( inStreams.size() == 0 ) ) << ( "ERROR: getAllOpenInputStreams() - EXPECTED: " + std::to_string( 0 )
    + " RECEIVED: " + std::to_string( inStreams.size() ) );
  dessert( ( outStreams.size() == 0 ) ) << ( "ERROR: getAllOpenOutputStreams() - EXPECTED: " + std::to_string( 0 )
    + " RECEIVED: " + std::to_string( outStreams.size() ) );

  try
  {
    fileIO.getDirectoryPath( "data" );
  }
  catch ( std::runtime_error const & errorIn )
  {
    dessert(
      ( std::string( errorIn.what() ) == "ERROR: FileIO::getDirectoryPath - Path \"data\" could not be found!" ) )
      << ( "ERROR: getDirectoryPath() - EXPECTED: "
           "ERROR: FileIO::getDirectoryPath - Path \"data\" could not be found! RECEIVED: "
           + std::string( errorIn.what() ) );
  }
  try
  {
    fileIO.getFilePath( "testData" );
  }
  catch ( std::runtime_error const & errorIn )
  {
    dessert( ( std::string( errorIn.what() ) == "ERROR: FileIO::getFilePath - Path \"testData\" could not be found!" ) )
      << ( "ERROR: getFilePath() - EXPECTED: "
           "ERROR: FileIO::getFilePath - Path \"testData\" could not be found! RECEIVED: "
           + std::string( errorIn.what() ) );
  }
  try
  {
    fileIO.getFilePath( "resetTest" );
  }
  catch ( std::runtime_error const & errorIn )
  {
    dessert(
      ( std::string( errorIn.what() ) == "ERROR: FileIO::getFilePath - Path \"resetTest\" could not be found!" ) )
      << ( "ERROR: getFilePath() - EXPECTED: "
           "ERROR: FileIO::getFilePath - Path \"resetTest\" could not be found! RECEIVED: "
           + std::string( errorIn.what() ) );
  }
  try
  {
    fileIO.getInputStream( "testData" );
  }
  catch ( std::runtime_error const & errorIn )
  {
    dessert( ( std::string( errorIn.what() ) == "ERROR: FileIO::getInputStream - Stream \"testData\" is not open!" ) )
      << ( "ERROR: getInputStream() - EXPECTED: "
           "ERROR: FileIO::getInputStream - Stream \"testData\" is not open! RECEIVED: "
           + std::string( errorIn.what() ) );
  }
  try
  {
    fileIO.getOutputStream( "resetTest" );
  }
  catch ( std::runtime_error const & errorIn )
  {
    dessert( ( std::string( errorIn.what() ) == "ERROR: FileIO::getOutputStream - Stream \"resetTest\" is not open!" ) )
      << ( "ERROR: getOutputStream() - EXPECTED: "
           "ERROR: FileIO::getOutputStream - Stream \"resetTest\" is not open! RECEIVED: "
           + std::string( errorIn.what() ) );
  }
}

// Check FileIO can find directories
void FileIO_UnitTests::runDirectoriesTest()
{
  dessert( ( fileIO.getBaseDirectory() == std::filesystem::current_path().string() ) )
    << ( "ERROR: getBaseDirectory() - EXPECTED: " + std::filesystem::current_path().string()
         + " RECEIVED: " + fileIO.getBaseDirectory() );

  fileIO.setDirectoryPath( "data", fileIO.getBaseDirectory() + "/data" );

  dessert( ( fileIO.getDirectoryPath( "data" ) == std::filesystem::current_path().string() + "/data" ) )
    << ( "ERROR: getDirectoryPath() - EXPECTED: " + std::filesystem::current_path().string() + "/data"
         + " RECEIVED: " + fileIO.getDirectoryPath( "data" ) );

  fileIO.reset();
}

// Check FileIO can find files recursively
void FileIO_UnitTests::runFileSearchTest()
{
  fileIO.setDirectoryPath( "data", fileIO.getBaseDirectory() + "/data" );

  auto filePaths = fileIO.findAllFiles( fileIO.getDirectoryPath( "data" ), ".txt", RecursiveSearchTrue );

  dessert(
    ( std::find( filePaths.begin(), filePaths.end(), fileIO.getDirectoryPath( "data" ) + "/" + "fileIOTestData.txt" )
      != filePaths.end() ) )
    << ( "ERROR: findAllFiles() - EXPECTED: " + fileIO.getDirectoryPath( "data" ) + "/" + "fileIOTestData.txt" );

  dessert( ( std::find( filePaths.begin(), filePaths.end(),
               fileIO.getDirectoryPath( "data" ) + "/testSubFolder/fileIOSubTestData.txt" )
    != filePaths.end() ) )
    << ( "ERROR: findAllFiles() - EXPECTED: " + fileIO.getDirectoryPath( "data" )
         + "/testSubFolder/fileIOSubTestData.txt" );

  dessert( ( std::find( filePaths.begin(), filePaths.end(),
               fileIO.getDirectoryPath( "data" ) + "/anotherTestSubFolder/fileIOAnotherSubTestData.txt" )
    != filePaths.end() ) )
    << ( "ERROR: findAllFiles() - EXPECTED: " + fileIO.getDirectoryPath( "data" )
         + "/anotherTestSubFolder/fileIOAnotherSubTestData.txt" );

  dessert(
    ( std::find( filePaths.begin(), filePaths.end(),
        fileIO.getDirectoryPath( "data" ) + "/testSubFolder/yetAnotherTestSubFolder/fileIOYetAnotherSubTestData.txt" )
      != filePaths.end() ) )
    << ( "ERROR: findAllFiles() - EXPECTED: " + fileIO.getDirectoryPath( "data" )
         + "/testSubFolder/yetAnotherTestSubFolder/fileIOYetAnotherSubTestData.txt" );

  fileIO.reset();
}

// Check FileIO can read a file
void FileIO_UnitTests::runFileReadTest()
{
  SyntaxHandler syntax;

  fileIO.setDirectoryPath( "data", fileIO.getBaseDirectory() + "/data" );

  auto filePaths = fileIO.findAllFiles( fileIO.getDirectoryPath( "data" ), ".txt", RecursiveSearchTrue );

  std::string const fileToRead =
    *std::find( filePaths.begin(), filePaths.end(), fileIO.getDirectoryPath( "data" ) + "/" + "fileIOTestData.txt" );

  std::string line;
  std::string line2;
  auto & fileStream = fileIO.openInputStream( "testFile", fileIO.getDirectoryPath( "data" ) + "/fileIOTestData.txt" );

  std::getline( fileStream, line );
  syntax.stripChar( line, '\n' );
  syntax.stripChar( line, '\r' );

  dessert( ( line == "This is a test file." ) )
    << ( "ERROR: openInputStream() - EXPECTED: This is a test file - RECEIVED: " + line );

  fileIO.resetInputStreamToFileStart( "testFile" );

  std::getline( fileStream, line2 );
  syntax.stripChar( line2, '\n' );
  syntax.stripChar( line2, '\r' );

  dessert( ( line == line2 ) ) << ( "ERROR: resetInputStreamToFileStart() - EXPECTED: This is a test file - RECEIVED: "
    + line2 );

  fileIO.resetInputStreamToFileStart( "testFile" );

  fileIO.readLine( "testFile", line2 );
  syntax.stripChar( line2, '\n' );
  syntax.stripChar( line2, '\r' );

  dessert( ( line == line2 ) )
    << ( "ERROR: readLine(streamIDIn, lineToWriteTo) - EXPECTED: This is a test file - RECEIVED: " + line2 );

  fileIO.resetInputStreamToFileStart( "testFile" );

  line2 = fileIO.readLine( "testFile" );
  syntax.stripChar( line2, '\n' );
  syntax.stripChar( line2, '\r' );

  dessert( ( line == line2 ) ) << ( "ERROR: readLine(streamIDIn) - EXPECTED: This is a test file - RECEIVED: "
    + line2 );

  fileIO.reset();
}

// Check FileIO can write a file
void FileIO_UnitTests::runFileWriteTest()
{
  SyntaxHandler syntax;

  fileIO.setDirectoryPath( "data", fileIO.getBaseDirectory() + "/data" );

  for ( int i = 0; i < numberOfFilesToTest / 2; ++i )
  {
    std::string const fileToWriteTo =
      fileIO.getDirectoryPath( "data" ) + "/" + "fileIOTest" + std::to_string( i ) + ".txt";

    auto & fileStream = fileIO.openOutputStream( "testWriteFile" + std::to_string( i ), fileToWriteTo );

    for ( int j = 0; j < numberOfLinesToTest; ++j ) { fileStream << j << std::endl; }
  }

  for ( int i = numberOfFilesToTest / 2; i < numberOfFilesToTest; ++i )
  {
    std::string const fileToWriteTo =
      fileIO.getDirectoryPath( "data" ) + "/" + "fileIOTest" + std::to_string( i ) + ".txt";

    fileIO.openOutputStream( "testWriteFile" + std::to_string( i ), fileToWriteTo );

    for ( int j = 0; j < numberOfLinesToTest; ++j )
    { fileIO.writeLine( "testWriteFile" + std::to_string( i ), std::to_string( j ) + "\n" ); }
  }

  for ( int i = 0; i < numberOfFilesToTest; ++i )
  {
    std::string const fileToRead =
      fileIO.getDirectoryPath( "data" ) + "/" + "fileIOTest" + std::to_string( i ) + ".txt";

    fileIO.openInputStream( "testWriteFile" + std::to_string( i ) );
    std::string line = fileIO.readLine( "testWriteFile" + std::to_string( i ) );

    for ( int j = 0; line != ""; ++j, fileIO.readLine( "testWriteFile" + std::to_string( i ), line ) )
    {
      syntax.stripChar( line, '\n' );
      syntax.stripChar( line, '\r' );

      dessert( ( line == std::to_string( j ) ) )
        << ( "ERROR: openInputStream() - EXPECTED:" + std::to_string( j ) + " - RECEIVED: " + line );
    }
  }

  fileIO.reset();
}

// Check FileIO function overloads
void FileIO_UnitTests::runOverloadTest()
{
  SyntaxHandler syntax;

  fileIO.setDirectoryPath( "data", fileIO.getBaseDirectory() + "/data" );

  char const * const testingFileChar = "testFile";
  char const * const testingFilePathChar = ( fileIO.getDirectoryPath( "data" ) + "/fileIOTestData" ).c_str();
  std::string const testingFileString = "testFile";
  std::string const testingFilePathString = fileIO.getDirectoryPath( "data" ) + "/fileIOTestData";

  auto & fileStreamOut = fileIO.openOutputStream( testingFileChar, testingFilePathChar );

  fileStreamOut << "1" << std::endl;

  fileIO.closeOutputStream( "testFile" );

  fileIO.openInputStream( testingFileChar, testingFilePathChar );
  auto line = fileIO.readLine( "testFile" );

  dessert( ( line == "1" ) ) << ( "ERROR: openInputStream() - EXPECTED: 1 - RECEIVED: " + line );

  fileIO.closeInputStream( "testFile" );

  auto & fileStreamOut2 = fileIO.openOutputStream( testingFileChar, testingFilePathString );

  fileStreamOut2 << "2" << std::endl;

  fileIO.closeOutputStream( "testFile" );

  fileIO.openInputStream( testingFileChar, testingFilePathString );
  line = fileIO.readLine( "testFile" );

  dessert( ( line == "2" ) ) << ( "ERROR: openInputStream() - EXPECTED: 2 - RECEIVED: " + line );

  fileIO.closeInputStream( "testFile" );

  auto & fileStreamOut3 = fileIO.openOutputStream( testingFileString, testingFilePathChar );

  fileStreamOut3 << "3" << std::endl;

  fileIO.closeOutputStream( "testFile" );

  fileIO.openInputStream( testingFileString, testingFilePathChar );
  line = fileIO.readLine( "testFile" );

  dessert( ( line == "3" ) ) << ( "ERROR: openInputStream() - EXPECTED: 3 - RECEIVED: " + line );

  fileIO.closeInputStream( "testFile" );

  auto & fileStreamOut4 = fileIO.openOutputStream( testingFileString, testingFilePathString );

  fileStreamOut4 << "4" << std::endl;

  fileIO.closeOutputStream( "testFile" );

  fileIO.openInputStream( testingFileString, testingFilePathString );
  line = fileIO.readLine( "testFile" );

  dessert( ( line == "4" ) ) << ( "ERROR: openInputStream() - EXPECTED: 4 - RECEIVED: " + line );

  fileIO.closeInputStream( "testFile" );
  fileIO.openInputStream( "testFile", fileIO.getDirectoryPath( "data" ) + "/fileIOTestData.txt" );

  line = fileIO.readFileAsString( "testFile" );
  syntax.stripChar( line, '\n' );
  syntax.stripChar( line, '\r' );
  std::string expectedLine =
    "This is a test file.It will be read into the testing program.The output here should appear."
    "abcdefghijklmnopqrstuvwxyz0123456789";

  dessert( ( line == expectedLine ) ) << ( "ERROR: readFileAsVectorOfStrings() - EXPECTED:\n" + expectedLine
    + "\nRECEIVED:\n" + line );

  fileIO.resetInputStreamToFileStart( "testFile" );

  auto lines = fileIO.readFileAsVectorOfStrings( "testFile" );

  dessert( ( lines[ 0 ] == "This is a test file." ) )
    << ( "ERROR: readFileAsVectorOfStrings() - EXPECTED: This is a test file. - RECEIVED: " + lines[ 0 ] );
  dessert( ( lines[ 1 ] == "It will be read into the testing program." ) )
    << ( "ERROR: readFileAsVectorOfStrings() - EXPECTED: It will be read into the testing program. - RECEIVED: "
         + lines[ 0 ] );
  dessert( ( lines[ 2 ] == "The output here should appear." ) )
    << ( "ERROR: readFileAsVectorOfStrings() - EXPECTED: The output here should appear. - RECEIVED: " + lines[ 0 ] );

  for ( char c = 'a'; c <= 'z'; ++c )
  {
    dessert( ( lines[ c - 'a' + 3 ] == std::string( 1, c ) ) ) << ( "ERROR: readFileAsVectorOfStrings() - EXPECTED: "
      + std::string( 1, c ) + " - RECEIVED: " + lines[ c - 'a' + 3 ] );
  }

  fileIO.resetInputStreamToFileStart( "testFile" );

  auto CSV = fileIO.readFileAsVectorOfVectors( "testFile" );

  dessert( ( CSV[ 0 ][ 0 ] == "This is a test file." ) )
    << ( "ERROR: readFileAsVectorOfVectors() - EXPECTED: This is a test file. - RECEIVED: " + CSV[ 0 ][ 0 ] );
  dessert( ( CSV[ 1 ][ 0 ] == "It will be read into the testing program." ) )
    << ( "ERROR: readFileAsVectorOfVectors() - EXPECTED: It will be read into the testing program. - RECEIVED: "
         + CSV[ 0 ][ 0 ] );
  dessert( ( CSV[ 2 ][ 0 ] == "The output here should appear." ) )
    << ( "ERROR: readFileAsVectorOfVectors() - EXPECTED: The output here should appear. - RECEIVED: " + CSV[ 0 ][ 0 ] );

  for ( char c = 'a'; c <= 'z'; ++c )
  {
    dessert( ( CSV[ c - 'a' + 3 ][ 0 ] == std::string( 1, c ) ) ) << ( "ERROR: readFileAsVectorOfVectors() - EXPECTED: "
      + std::string( 1, c ) + " - RECEIVED: " + CSV[ c - 'a' + 3 ][ 0 ] );
  }

  fileIO.resetInputStreamToFileStart( "testFile" );

  fileIO.reset();
}

// Check FileIO can read a full file in
void FileIO_UnitTests::runWholeFileReadTest()
{
  SyntaxHandler syntax;

  fileIO.setDirectoryPath( "data", fileIO.getBaseDirectory() + "/data" );

  std::string const testingFileString = "testFile";
  std::string const testingFilePathString = fileIO.getDirectoryPath( "data" ) + "/fileIOTestData";

  auto & fileStreamOut = fileIO.openOutputStream( testingFileString, testingFilePathString );

  for ( int i = 0; i < numberOfLinesToTest; ++i ) { fileStreamOut << i << std::endl; }

  fileIO.closeOutputStream( "testFile" );

  auto line = fileIO.readFileAsString( "testFile" );
  syntax.stripChar( line, '\n' );
  syntax.stripChar( line, '\r' );

  fileIO.openInputStream( testingFileString + "1", testingFilePathString );
  auto line2 = fileIO.readFileAsString( "testFile1" );
  fileIO.closeInputStream( testingFileString + "1" );

  syntax.stripChar( line2, '\n' );
  syntax.stripChar( line2, '\r' );

  dessert( ( ( line == line2 ) && ( line2 == "0123456789" ) ) )
    << ( "ERROR: readFileAsString() - EXPECTED: 123456789 - RECEIVED: " + line + " AND: " + line2 );

  fileIO.openOutputStream( testingFileString + "2", testingFilePathString );

  for ( int i = 0; i < numberOfLinesToTest; ++i ) { fileStreamOut << "a,b,c,d,e,f,g,h,i,j" << std::endl; }

  fileIO.closeOutputStream( testingFileString + "2" );

  fileIO.reset();
}

FileIO_UnitTests::FileIO_UnitTests() : fileIO( FileIO::getInstance() ) {}