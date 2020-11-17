#include "FIO_UnitTests.h"

namespace FIOTests
{
  std::unique_ptr< FIO_UnitTests > FIO_UnitTests::instance =
    std::unique_ptr< FIO_UnitTests >( nullptr );

  FIO_UnitTests & FIO_UnitTests::getInstance()
  {
    if ( ! instance )
    { instance = std::unique_ptr< FIO_UnitTests >( new FIO_UnitTests() ); }

    return *instance;
  }

  void FIO_UnitTests::runAllUnitTests()
  {
    runStringConversionTest();
    runPathTest();
    runStreamTest();
    runResetTest();
    runFileSearchTest();
    runFileReadTest();
    runFileWriteTest();
    runOverloadTest();
    runWholeFileReadTest();
    runCSVTest();
  }

  void FIO_UnitTests::runStringConversionTest()
  {
    std::string s = "A normal string.";
    std::wstring ws = L"おはよう";

    std::string s2WS = wideToMultiByte( multiByteToWide( s ) );
    dessert( ( s2WS == s ) )
      << ( "ERROR - " + std::string( "NSFIO::multiByteToWide()" )
           + " \r\nEXPECTED: " + s + " ACTUAL: " + s2WS );

    std::string inter = wideToMultiByte( ws );
    std::wstring ws2S = multiByteToWide( wideToMultiByte( ws ) );

    dessert( ( ws2S == ws ) ) << wideToMultiByte( L"ERROR - "
      + std::wstring( L"NSFIO::wideToMultiByte()" ) + L" \r\nEXPECTED: " + ws
      + L" ACTUAL: " + ws2S );
  }

  void FIO_UnitTests::runPathTest()
  {
    fio.setDirID( "data", fio.getRoot() + pathSep + "data" );
    fio.setFileID(
      "testData", fio.getPath( "data" ) + pathSep + "fileIOTestData.txt" );

    // Try setting non-existent path
    try
    {
      fio.setDirID( "data", fio.getRoot() + pathSep + "BiBimBap" );
    }
    catch ( FIOExcept const & errorIn )
    {
      dessert( ( std::string( errorIn.what() )
        == "Cannot set id data, as "
           "/cygdrive/e/Work/OngoingProjects/FileIO/BiBimBap "
           "does not point to a directory." ) )
        << ( "\r\n\r\nERROR: setDirID() - \r\nEXPECTED: "
             "Cannot set id data, as "
             "/cygdrive/e/Work/OngoingProjects/FileIO/BiBimBap "
             "does not point to a directory."
             "\r\nRECEIVED: "
             + std::string( errorIn.what() ) );
    }

    // Try setting non-existent path
    try
    {
      fio.setDirID( L"data", fio.getRootW() + L"/BiBimBap" );
    }
    catch ( FIOExcept const & errorIn )
    {
      dessert( ( std::string( errorIn.what() )
        == "Cannot set id data, as "
           "/cygdrive/e/Work/OngoingProjects/FileIO/BiBimBap "
           "does not point to a directory." ) )
        << ( "\r\n\r\nERROR: setDirID() - \r\nEXPECTED: "
             "Cannot set id data, as "
             "/cygdrive/e/Work/OngoingProjects/FileIO/BiBimBap "
             "does not point to a directory."
             "\r\nRECEIVED: "
             + std::string( errorIn.what() ) );
    }

    // Try setting non-existent path
    try
    {
      fio.setFileID( "testData", fio.getRoot() + pathSep + "BiBimBap" );
    }
    catch ( FIOExcept const & errorIn )
    {
      dessert( ( std::string( errorIn.what() )
        == "Cannot set id testData, as "
           "/cygdrive/e/Work/OngoingProjects/FileIO/BiBimBap "
           "does not point to a regular file." ) )
        << ( "\r\n\r\nERROR: setFileID() - \r\nEXPECTED: "
             "Cannot set id testData, as "
             "/cygdrive/e/Work/OngoingProjects/FileIO/BiBimBap "
             "does not point to a regular file."
             "\r\nRECEIVED: "
             + std::string( errorIn.what() ) );
    }

    // Try setting non-existent path
    try
    {
      fio.setFileID( L"testData", fio.getRootW() + L"/BiBimBap" );
    }
    catch ( FIOExcept const & errorIn )
    {
      dessert( ( std::string( errorIn.what() )
        == "Cannot set id testData, as "
           "/cygdrive/e/Work/OngoingProjects/FileIO/BiBimBap "
           "does not point to a regular file." ) )
        << ( "\r\n\r\nERROR: setFileID() - \r\nEXPECTED: "
             "Cannot set id testData, as "
             "/cygdrive/e/Work/OngoingProjects/FileIO/BiBimBap "
             "does not point to a regular file."
             "\r\nRECEIVED: "
             + std::string( errorIn.what() ) );
    }

    // Try getting non-existent id
    try
    {
      fio.getPath( "BiBimBap" );
    }
    catch ( FIOExcept const & errorIn )
    {
      dessert( ( std::string( errorIn.what() )
        == "Path cannot be retrieved as BiBimBap is neither a "
           "path to a file or directory, nor points to a path "
           "stored in the FIO path map." ) )
        << ( "\r\n\r\nERROR: getPath() - \r\nEXPECTED: "
             "Path cannot be retrieved as BiBimBap is neither a "
             "path to a file or directory, nor points to a path "
             "stored in the FIO path map."
             "\r\nRECEIVED: "
             + std::string( errorIn.what() ) );
    }

    // Try getting non-existent id
    try
    {
      fio.getPathW( L"BiBimBap" );
    }
    catch ( FIOExcept const & errorIn )
    {
      dessert( ( std::string( errorIn.what() )
        == "Path cannot be retrieved as BiBimBap is neither a "
           "path to a file or directory, nor points to a path "
           "stored in the FIO path map." ) )
        << ( "\r\n\r\nERROR: getPath() - \r\nEXPECTED: "
             "Path cannot be retrieved as BiBimBap is neither a "
             "path to a file or directory, nor points to a path "
             "stored in the FIO path map."
             "\r\nRECEIVED: "
             + std::string( errorIn.what() ) );
    }
  }

  void FIO_UnitTests::runStreamTest()
  {
    fio.setDirID( "data", fio.getRoot() + pathSep + "data" );
    fio.setFileID(
      "testData", fio.getPath( "data" ) + pathSep + "fileIOTestData.txt" );
    fio.setFileID(
      "resetTest", fio.getPath( "data" ) + pathSep + "fileIOResetTest.txt" );

    fio.openWI( "testData" );
    fio.openWO( "resetTest" );

    // Try opening duplicate in stream. should fail.
    try
    {
      fio.openWI( "testData" );
    }
    catch ( FIOExcept const & errorIn )
    {
      dessert( ( std::string( errorIn.what() )
        == "Cannot open stream on data/fileIOTestData.txt as "
           "there is already a stream stored on that id." ) )
        << ( "\r\n\r\nERROR: openWI() - \r\nEXPECTED: "
             "Cannot open stream on data/fileIOTestData.txt as "
             "there is already a stream stored on that id."
             "\r\nRECEIVED: "
             + std::string( errorIn.what() ) );
    }

    // Try opening duplicate in stream. should fail.
    try
    {
      fio.openWI( L"testData" );
    }
    catch ( FIOExcept const & errorIn )
    {
      dessert( ( std::string( errorIn.what() )
        == "Cannot open stream on data/fileIOTestData.txt as "
           "there is already a stream stored on that id." ) )
        << ( "\r\n\r\nERROR: openWI() - \r\nEXPECTED: "
             "Cannot open stream on data/fileIOTestData.txt as "
             "there is already a stream stored on that id."
             "\r\nRECEIVED: "
             + std::string( errorIn.what() ) );
    }

    // Try opening duplicate out stream. should fail.
    try
    {
      fio.openWO( "resetTest" );
    }
    catch ( FIOExcept const & errorIn )
    {
      dessert( ( std::string( errorIn.what() )
        == "Cannot open stream on data/fileIOResetTest.txt as "
           "there is already a stream stored on that id." ) )
        << ( "\r\n\r\nERROR: openWI() - \r\nEXPECTED: "
             "Cannot open stream on data/fileIOTestData.txt as "
             "there is already a stream stored on that id."
             "\r\nRECEIVED: "
             + std::string( errorIn.what() ) );
    }

    // Try opening duplicate out stream. should fail.
    try
    {
      fio.openWO( L"resetTest" );
    }
    catch ( FIOExcept const & errorIn )
    {
      dessert( ( std::string( errorIn.what() )
        == "Cannot open stream on data/fileIOResetTest.txt as "
           "there is already a stream stored on that id." ) )
        << ( "\r\n\r\nERROR: openWI() - \r\nEXPECTED: "
             "Cannot open stream on data/fileIOTestData.txt as "
             "there is already a stream stored on that id."
             "\r\nRECEIVED: "
             + std::string( errorIn.what() ) );
    }

    // Try closing a closed out stream. should fail.
    try
    {
      fio.closeWO( L"resetTest" );
      fio.closeWO( L"resetTest" );
    }
    catch ( FIOExcept const & errorIn )
    {
      dessert( ( std::string( errorIn.what() )
        == "No output stream exists at data/fileIOResetTest.txt" ) )
        << ( "\r\n\r\nERROR: closeWO() - \r\nEXPECTED: "
             "No output stream exists at data/fileIOResetTest.txt"
             "\r\nRECEIVED: "
             + std::string( errorIn.what() ) );
    }

    // Try closing a closed in stream. should fail.
    try
    {
      fio.closeWI( L"testData" );
      fio.closeWI( L"testData" );
    }
    catch ( FIOExcept const & errorIn )
    {
      dessert( ( std::string( errorIn.what() )
        == "No output stream exists at data/fileIOTestData.txt" ) )
        << ( "\r\n\r\nERROR: closeWI() - \r\nEXPECTED: "
             "No output stream exists at data/fileIOTestData.txt"
             "\r\nRECEIVED: "
             + std::string( errorIn.what() ) );
    }
  }

  void FIO_UnitTests::runResetTest()
  {
    std::string rt = fio.getRoot();
    fio.setRoot( rt + pathSep + "data" );

    dessert( ( fio.getRoot() == rt + pathSep + "data" ) )
      << ( "\r\n\r\nERROR: setRoot() - \r\nEXPECTED: " + rt + pathSep + "data"
           + "\r\nRECEIVED: " + fio.getRoot() );

    fio.reset();

    dessert( ( fio.getRoot() == rt ) )
      << ( "\r\n\r\nERROR: setRoot() - \r\nEXPECTED: " + rt
           + "\r\nRECEIVED: " + fio.getRoot() );
  }

  // Check FIO can find files recursively
  void FIO_UnitTests::runFileSearchTest()
  {
    fio.setDirID( "data", fio.getRoot() + pathSep + "data" );

    auto filePaths = fio.findFiles( ".txt", fio.getPath( "data" ) );

    dessert( ( std::find( filePaths.begin(), filePaths.end(),
                 fio.getPath( "data" ) + pathSep + "fileIOTestData.txt" )
      != filePaths.end() ) )
      << ( "\r\n\r\nERROR: findFiles() - \r\nEXPECTED: " + fio.getPath( "data" )
           + pathSep + "fileIOTestData.txt" );

    dessert( ( std::find( filePaths.begin(), filePaths.end(),
                 fio.getPath( "data" ) + pathSep
                   + "testSubFolder/fileIOSubTestData.txt" )
      != filePaths.end() ) )
      << ( "\r\n\r\nERROR: findFiles() - \r\nEXPECTED: " + fio.getPath( "data" )
           + pathSep + "testSubFolder/fileIOSubTestData.txt" );

    dessert( ( std::find( filePaths.begin(), filePaths.end(),
                 fio.getPath( "data" ) + pathSep + ""
                   + "anotherTestSubFolder/fileIOAnotherSubTestData.txt" )
      != filePaths.end() ) )
      << ( "\r\n\r\nERROR: findFiles() - \r\nEXPECTED: " + fio.getPath( "data" )
           + pathSep + "anotherTestSubFolder/fileIOAnotherSubTestData.txt" );

    dessert( (
      std::find( filePaths.begin(), filePaths.end(),
        fio.getPath( "data" ) + pathSep + ""
          + "testSubFolder/yetAnotherTestSubFolder/fileIOYetAnotherSubTestData.txt" )
      != filePaths.end() ) )
      << ( "\r\n\r\nERROR: findFiles() - \r\nEXPECTED: " + fio.getPath( "data" )
           + pathSep + ""
           + "testSubFolder/yetAnotherTestSubFolder/fileIOYetAnotherSubTestData.txt" );

    fio.reset();
  }

  // Check FIO can read a file
  void FIO_UnitTests::runFileReadTest()
  {
    SyntaxHandler syntax;

    fio.setDirID( "data", fio.getRoot() + pathSep + "data" );

    auto filePaths = fio.findFiles( ".txt", fio.getPath( "data" ) );

    std::string const fileToRead = *std::find( filePaths.begin(),
      filePaths.end(), fio.getPath( "data" ) + pathSep + "fileIOTestData.txt" );

    std::string line;
    std::string line2;
    std::ifstream fs( fio.getPath( "data" ) + pathSep + "fileIOTestData.txt" );
    fio.openWI( fio.getPath( "data" ) + pathSep + "fileIOTestData.txt" );

    fio.setFileID(
      "tmp", fio.getPath( "data" ) + pathSep + "fileIOTestData.txt" );

    std::getline( fs, line );
    line2 = fio.readLine( "tmp" );

    syntax.stripChar( line, '\n' );
    syntax.stripChar( line, '\r' );
    syntax.stripChar( line2, '\n' );
    syntax.stripChar( line2, '\r' );

    dessert( ( line == line2 ) ) << ( "\r\n\r\nERROR: openWI() - \r\nEXPECTED:"
      + line + "- \r\nRECEIVED: " + line2 );

    fio.rewindWI( "tmp" );

    line2 = fio.readLine( "tmp" );
    syntax.stripChar( line2, '\n' );
    syntax.stripChar( line2, '\r' );

    dessert( ( line == line2 ) ) << ( "\r\n\r\nERROR: openWI() - \r\nEXPECTED:"
      + line + "- \r\nRECEIVED: " + line2 );

    fio.reset();
  }

  // Check FIO can write a file
  void FIO_UnitTests::runFileWriteTest()
  {
    SyntaxHandler syntax;

    fio.setDirID( "data", fio.getRoot() + pathSep + "data" );

    for ( int i = 0; i < numberOfFilesToTest / 2; ++i )
    {
      std::string const fileToWriteTo = fio.getPath( "data" ) + pathSep
        + "fileIOTest" + std::to_string( i ) + ".txt";

      fio.setFileID( "testWriteFile" + std::to_string( i ), fileToWriteTo );

      auto & file = fio.openWO( fileToWriteTo );

      for ( int j = 0; j < numberOfLinesToTest; ++j )
      { file << j << std::endl; }
    }

    for ( int i = numberOfFilesToTest / 2; i < numberOfFilesToTest; ++i )
    {
      std::string const fileToWriteTo = fio.getPath( "data" ) + pathSep
        + "fileIOTest" + std::to_string( i ) + ".txt";

      fio.openWO( fileToWriteTo );

      for ( int j = 0; j < numberOfLinesToTest; ++j )
      { fio.writeLine( fileToWriteTo, std::to_string( j ) + "\n\r" ); }
    }

    for ( int i = 0; i < numberOfFilesToTest; ++i )
    {
      std::string const fileToReadFrom = fio.getPath( "data" ) + pathSep
        + "fileIOTest" + std::to_string( i ) + ".txt";

      fio.openWI( fileToReadFrom );
      std::string line = fio.readLine( fileToReadFrom );

      for ( int j = 0; line != ""; ++j, fio.readLine( fileToReadFrom, line ) )
      {
        syntax.stripChar( line, '\n' );
        syntax.stripChar( line, '\r' );

        dessert( ( line == std::to_string( j ) ) )
          << ( "\r\n\r\nERROR: openWI() - \r\nEXPECTED:" + std::to_string( j )
               + " - \r\nRECEIVED: " + line );
      }
    }

    fio.reset();
  }

  // Check FIO function overloads
  void FIO_UnitTests::runOverloadTest()
  { /*
     SyntaxHandler syntax;

     fio.setDirectoryPath( "data", fio.getBaseDirectory() + pathSep + "data" );

     char const * const testingFileChar = "testFile";
     char const * const testingFilePathChar =
       ( fio.getDirectoryPath( "data" ) + pathSep + "ioTestData" ).c_str();
     std::string const testingFileString = "testFile";
     std::string const testingFilePathString =
       fio.getDirectoryPath( "data" ) + pathSep + "ioTestData";

     auto & fileOut =
       fio.openOutput( testingFileChar, testingFilePathChar );

     fileOut << "1" << std::endl;

     fio.closeOutput( "testFile" );

     fio.openInput( testingFileChar, testingFilePathChar );
     auto line = fio.readLine( "testFile" );

     dessert( ( line == "1" ) )
       << ( "\r\n\r\nERROR: openInput() - \r\nEXPECTED: 1 -
     \r\nRECEIVED
            : "
              + line );

     fio.closeInput( "testFile" );

     auto & fileOut2 =
       fio.openOutput( testingFileChar, testingFilePathString );

     fileOut2 << "2" << std::endl;

     fio.closeOutput( "testFile" );

     fio.openInput( testingFileChar, testingFilePathString );
     line = fio.readLine( "testFile" );

     dessert( ( line == "2" ) )
       << ( "\r\n\r\nERROR: openInput() - \r\nEXPECTED: 2 -
     \r\nRECEIVED
            : "
              + line );

     fio.closeInput( "testFile" );

     auto & fileOut3 =
       fio.openOutput( testingFileString, testingFilePathChar );

     fileOut3 << "3" << std::endl;

     fio.closeOutput( "testFile" );

     fio.openInput( testingFileString, testingFilePathChar );
     line = fio.readLine( "testFile" );

     dessert( ( line == "3" ) )
       << ( "\r\n\r\nERROR: openInput() - \r\nEXPECTED: 3 -
     \r\nRECEIVED
            : "
              + line );

     fio.closeInput( "testFile" );

     auto & fileOut4 =
       fio.openOutput( testingFileString, testingFilePathString );

     fileOut4 << "4" << std::endl;

     fio.closeOutput( "testFile" );

     fio.openInput( testingFileString, testingFilePathString );
     line = fio.readLine( "testFile" );

     dessert( ( line == "4" ) )
       << ( "\r\n\r\nERROR: openInput() - \r\nEXPECTED: 4 -
     \r\nRECEIVED
            : "
              + line );

     fio.closeInput( "testFile" );
     fio.openInput(
       "testFile", fio.getDirectoryPath( "data" ) + pathSep + "ioTestData.txt"
     );

     line = fio.readFile( "testFile" );
     syntax.stripChar( line, '\n' );
     syntax.stripChar( line, '\r' );
     std::string \r\nEXPECTEDLine =
       "This is a test file.It will be read into the testing program.The
       output here should appear." " abcdefghijklmnopqrstuvwxyz0123456789 ";

       dessert( ( line == \r\nEXPECTEDLine ) )
       << ( "\r\n\r\nERROR: readFileAsVectorOfStrings() - \r\nEXPECTED:\n" +
     \r\nEXPECTEDLine + "\n\r\nRECEIVED:\n" + line );

     fio.resetInputToFileStart( "testFile" );

     auto lines = fio.readFileAsVectorOfStrings( "testFile" );

     dessert( ( lines[ 0 ] == "This is a test file." ) )
       << ( "\r\n\r\nERROR: readFileAsVectorOfStrings() - \r\nEXPECTED: This
              is a test file.
              - \r\nRECEIVED
            : "
              + lines[ 0 ] );
     dessert( ( lines[ 1 ] == "It will be read into the testing program." ) )
       << ( "\r\n\r\nERROR: readFileAsVectorOfStrings() - \r\nEXPECTED: It
              will be read into the testing program.
              - \r\nRECEIVED
            : "
              + lines[ 0 ] );
     dessert( ( lines[ 2 ] == "The output here should appear." ) )
       << ( "\r\n\r\nERROR: readFileAsVectorOfStrings() - \r\nEXPECTED: The
              output here should appear.
              - \r\nRECEIVED
            : "
              + lines[ 0 ] );

     for ( char c = 'a'; c <= 'z'; ++c )
     {
       dessert( ( lines[ c - 'a' + 3 ] == std::string( 1, c ) ) )
         << ( "\r\n\r\nERROR: readFileAsVectorOfStrings() - \r\nEXPECTED: "
              + std::string( 1, c )
              + " - \r\nRECEIVED: " + lines[ c - 'a' + 3 ] );
     }

     fio.resetInputToFileStart( "testFile" );

     auto CSV = fio.readFileAsVectorOfVectors( "testFile" );

     dessert( ( CSV[ 0 ][ 0 ] == "This is a test file." ) )
       << ( "\r\n\r\nERROR: readFileAsVectorOfVectors() - \r\nEXPECTED: This
              is a test file.
              - \r\nRECEIVED
            : "
              + CSV[ 0 ][ 0 ] );
     dessert( ( CSV[ 1 ][ 0 ] == "It will be read into the testing program." ) )
       << ( "\r\n\r\nERROR: readFileAsVectorOfVectors() - \r\nEXPECTED: It
              will be read into the testing program.
              - \r\nRECEIVED
            : "
              + CSV[ 0 ][ 0 ] );
     dessert( ( CSV[ 2 ][ 0 ] == "The output here should appear." ) )
       << ( "\r\n\r\nERROR: readFileAsVectorOfVectors() - \r\nEXPECTED: The
              output here should appear.
              - \r\nRECEIVED
            : "
              + CSV[ 0 ][ 0 ] );

     for ( char c = 'a'; c <= 'z'; ++c )
     {
       dessert( ( CSV[ c - 'a' + 3 ][ 0 ] == std::string( 1, c ) ) )
         << ( "\r\n\r\nERROR: readFileAsVectorOfVectors() - \r\nEXPECTED: "
              + std::string( 1, c )
              + " - \r\nRECEIVED: " + CSV[ c - 'a' + 3 ][ 0 ] );
     }

     fio.resetInputToFileStart( "testFile" );

     fio.reset();*/
  }

  // Check FIO can read a full file in
  void FIO_UnitTests::runWholeFileReadTest()
  {
    SyntaxHandler syntax;

    fio.setDirID( "data", fio.getRoot() + pathSep + "data" );
    fio.setFileID(
      "testFile", fio.getPath( "data" ) + pathSep + "fileIOTestData.txt" );

    auto & fs = fio.openWO( "testFile" );

    for ( int i = 0; i < numberOfLinesToTest; ++i )
    { fs << std::to_wstring( i ) << std::endl; }

    fio.closeWO( "testFile" );

    auto line = fio.readFile( "testFile" );
    syntax.stripChar( line, '\n' );
    syntax.stripChar( line, '\r' );

    fio.setFileID(
      "testFile1", fio.getPath( "data" ) + pathSep + "fileIOTestData.txt" );
    fio.openWI( "testFile1" );

    auto line2 = fio.readFile( "testFile1" );
    fio.closeWI( "testFile1" );
    syntax.stripChar( line2, '\n' );
    syntax.stripChar( line2, '\r' );

    dessert( ( ( line == line2 ) && ( line2 == "0123456789" ) ) )
      << ( "\r\n\r\nERROR: readFile() - \r\nEXPECTED: 123456789 - "
           "\r\nRECEIVED: "
           + line + " AND: " + line2 );

    fio.setFileID(
      "testFile2", fio.getPath( "data" ) + pathSep + "fileIOTestData.txt" );

    auto & ofs = fio.openWO( "testFile2" );

    for ( int i = 0; i < numberOfLinesToTest; ++i )
    { ofs << "a,b,c,d,e,f,g,h,i,j" << std::endl; }

    fio.closeWO( "testFile2" );

    fio.reset();
  }

  // Check FIO can read a csv file in
  void FIO_UnitTests::runCSVTest()
  {
    fio.setDirID( "data", fio.getRoot() + pathSep + "data" );
    fio.setFileID(
      "testFile", fio.getPath( "data" ) + pathSep + "csvTest.csv" );

    std::cout << "File: " << std::endl << std::endl;

    std::cout << fio.readFile( "testFile" ) << std::endl << std::endl;

    std::cout << "Vector:" << std::endl << std::endl;

    auto vec = fio.readFileToVector( "testFile" );

    for ( auto & s : vec )
    {
      std::cout << s << std::endl
                << "=======================================" << std::endl
                << std::endl;
    }

    std::cout << "Matrix:" << std::endl;

    auto mat = fio.readFileToMatrix( "testFile", ",", "\n\r" );

    for ( auto & r : mat )
    {
      for ( auto & s : r ) { std::cout << s << "|"; }
      std::cout << std::endl
                << "--------------------------------------" << std::endl;
    }

    fio.reset();
  }

  FIO_UnitTests::FIO_UnitTests() : fio( FIO::getInstance() ) {}
} // namespace FIOTests