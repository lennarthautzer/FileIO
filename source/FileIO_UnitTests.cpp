//*

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE FileIOTest

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <algorithm>
#include "FileIO.h"
#include "SyntaxHandler.h"

int constexpr numberOfFilesToTest = 10;
int constexpr numberOfLinesToTest = 10;

//Check FileIO constructs
BOOST_AUTO_TEST_CASE(ConstructionAndDestruction)
{
  FileIO fileIO;
}

//Check FileIO can find directories
BOOST_AUTO_TEST_CASE(DirectoriesTest)
{
  FileIO fileIO;

  BOOST_REQUIRE_MESSAGE(
    fileIO.getBaseDirectory() == std::string(std::filesystem::current_path()), 
    "ERROR: getBaseDirectory() - EXPECTED: " 
    + std::string(std::filesystem::current_path())
    + " RECEIVED: " + fileIO.getBaseDirectory());

    fileIO.setDirectoryPath("data", fileIO.getBaseDirectory() + "/data");

    BOOST_REQUIRE_MESSAGE(
    fileIO.getDirectoryPath("data") == 
      std::string(std::filesystem::current_path()) + "/data", 
    "ERROR: getDirectoryPath() - EXPECTED: " 
    + std::string(std::filesystem::current_path()) + "/data"
    + " RECEIVED: " + fileIO.getDirectoryPath("data"));
}

//Check FileIO can find files recursively
BOOST_AUTO_TEST_CASE(FileSearchTest)
{
  FileIO fileIO;
  fileIO.setDirectoryPath("data", fileIO.getBaseDirectory() + "/data");

  auto filePaths = fileIO.findAllFiles( fileIO.getDirectoryPath("data"), ".txt", recursiveSearchTrue);

  for(auto & file : filePaths)
  {
    std::cout << file << std::endl;
  }

  BOOST_REQUIRE_MESSAGE(
    std::find(filePaths.begin(), filePaths.end(), 
    fileIO.getDirectoryPath("data") + "/" + "fileIOTestData.txt") != filePaths.end(), 
    "ERROR: findAllFiles() - EXPECTED: " 
    + fileIO.getDirectoryPath("data") + "/" + "fileIOTestData.txt");
  
  BOOST_REQUIRE_MESSAGE(
    std::find(filePaths.begin(), filePaths.end(), 
    fileIO.getDirectoryPath("data") + "/testSubFolder/fileIOSubTestData.txt") != filePaths.end(), 
    "ERROR: findAllFiles() - EXPECTED: " 
    + fileIO.getDirectoryPath("data") + "/testSubFolder/fileIOSubTestData.txt");
  
  BOOST_REQUIRE_MESSAGE(
    std::find(filePaths.begin(), filePaths.end(), 
    fileIO.getDirectoryPath("data") + "/anotherTestSubFolder/fileIOAnotherSubTestData.txt") != filePaths.end(), 
    "ERROR: findAllFiles() - EXPECTED: " 
    + fileIO.getDirectoryPath("data") + "/anotherTestSubFolder/fileIOAnotherSubTestData.txt");
  
  BOOST_REQUIRE_MESSAGE(
    std::find(filePaths.begin(), filePaths.end(), 
    fileIO.getDirectoryPath("data") + "/testSubFolder/yetAnotherTestSubFolder/fileIOYetAnotherSubTestData.txt") != filePaths.end(), 
    "ERROR: findAllFiles() - EXPECTED: " 
    + fileIO.getDirectoryPath("data") + "/testSubFolder/yetAnotherTestSubFolder/fileIOYetAnotherSubTestData.txt");
}

//Check FileIO can read a file
BOOST_AUTO_TEST_CASE(FileReadTest)
{
  FileIO fileIO;
  fileIO.setDirectoryPath("data", fileIO.getBaseDirectory() + "/data");

  SyntaxHandler syntax;

  auto filePaths = fileIO.findAllFiles( fileIO.getDirectoryPath("data"), ".txt", recursiveSearchTrue);

  std::string const fileToRead = *std::find(filePaths.begin(), filePaths.end(), 
    fileIO.getDirectoryPath("data") + "/" + "fileIOTestData.txt");

  std::string line;
  std::string line2;
  auto & fileStream = fileIO.openInputStream("testFile", fileIO.getDirectoryPath("data") + "/fileIOTestData.txt");

  getline(fileStream, line);
  syntax.stripChar(line, '\n');
  syntax.stripChar(line, '\r');

  BOOST_REQUIRE_MESSAGE(line == "This is a test file.", 
    "ERROR: openInputStream() - EXPECTED: This is a test file - RECEIVED: " + line);
  
  fileIO.resetInputStreamToFileStart("testFile");

  getline(fileStream, line2);
  syntax.stripChar(line2, '\n');
  syntax.stripChar(line2, '\r');
  
  BOOST_REQUIRE_MESSAGE(line == line2, 
    "ERROR: resetInputStreamToFileStart() - EXPECTED: This is a test file - RECEIVED: " + line2);

  fileIO.resetInputStreamToFileStart("testFile");

  fileIO.readLine("testFile", line2);
  syntax.stripChar(line2, '\n');
  syntax.stripChar(line2, '\r');
  
  BOOST_REQUIRE_MESSAGE(line == line2, 
    "ERROR: readLine(streamIDIn, lineToWriteTo) - EXPECTED: This is a test file - RECEIVED: " + line2);

  fileIO.resetInputStreamToFileStart("testFile");

  line2 = fileIO.readLine("testFile");
  syntax.stripChar(line2, '\n');
  syntax.stripChar(line2, '\r');
  
  BOOST_REQUIRE_MESSAGE(line == line2, 
    "ERROR: readLine(streamIDIn) - EXPECTED: This is a test file - RECEIVED: " + line2);
}

//Check FileIO can write a file
BOOST_AUTO_TEST_CASE(FileWriteTest)
{
  FileIO fileIO;
  fileIO.setDirectoryPath("data", fileIO.getBaseDirectory() + "/data");
  SyntaxHandler syntax;

  for(int i = 0; i < numberOfFilesToTest / 2; ++i)
  {
    std::string const fileToWriteTo = fileIO.getDirectoryPath("data") + "/" + "fileIOTest" + std::to_string(i) + ".txt";

    auto & fileStream = fileIO.openOutputStream("testWriteFile" + std::to_string(i), fileToWriteTo);

    for(int j = 0; j < numberOfLinesToTest; ++j)
    { fileStream << j << std::endl; }
  }

  for(int i = numberOfFilesToTest / 2; i < numberOfFilesToTest; ++i)
  {
    std::string const fileToWriteTo = fileIO.getDirectoryPath("data") + "/" + "fileIOTest" + std::to_string(i) + ".txt";

    fileIO.openOutputStream("testWriteFile" + std::to_string(i), fileToWriteTo);

    for(int j = 0; j < numberOfLinesToTest; ++j)
    { fileIO.writeLine("testWriteFile" + std::to_string(i), std::to_string(j) + "\n"); }
  }

  for(int i = 0; i < numberOfFilesToTest; ++i)
  {
    std::string const fileToRead = fileIO.getDirectoryPath("data") + "/" + "fileIOTest" + std::to_string(i) + ".txt";

    fileIO.openInputStream( "testWriteFile" + std::to_string(i) );
    std::string line = fileIO.readLine("testWriteFile" + std::to_string(i) );

    for(int j = 0; line != ""; ++j, fileIO.readLine("testWriteFile" + std::to_string(i), line))
    {
      syntax.stripChar(line, '\n');
      syntax.stripChar(line, '\r');
    
      BOOST_REQUIRE_MESSAGE(line == std::to_string(j), "ERROR: openInputStream() - EXPECTED:" + std::to_string(j) + 
        " - RECEIVED: " + line);
    }
  }
}

//Check FileIO function overloads
BOOST_AUTO_TEST_CASE(OverloadTest)
{
  FileIO fileIO;
  fileIO.setDirectoryPath("data", fileIO.getBaseDirectory() + "/data");
  SyntaxHandler syntax;

  char const * const testingFileChar = "testFile";
  char const * const testingFilePathChar = (fileIO.getDirectoryPath("data") + "/fileIOTestData").c_str();
  std::string const testingFileString = "testFile";
  std::string const testingFilePathString = fileIO.getDirectoryPath("data") + "/fileIOTestData";

  auto & fileStreamOut = fileIO.openOutputStream(testingFileChar, testingFilePathChar);

  fileStreamOut << "1" << std::endl;

  fileIO.closeOutputStream("testFile");

  fileIO.openInputStream(testingFileChar, testingFilePathChar);
  auto line = fileIO.readLine("testFile");

  BOOST_REQUIRE_MESSAGE( line == "1", "ERROR: openInputStream() - EXPECTED: 1" << " - RECEIVED: " + line);
  
  fileIO.closeInputStream("testFile");

  auto & fileStreamOut2 = fileIO.openOutputStream(testingFileChar, testingFilePathString);

  fileStreamOut2 << "2" << std::endl;

  fileIO.closeOutputStream("testFile");

  fileIO.openInputStream(testingFileChar, testingFilePathString);
  line = fileIO.readLine("testFile");

  BOOST_REQUIRE_MESSAGE( line == "2", "ERROR: openInputStream() - EXPECTED: 2" << " - RECEIVED: " + line);
  
  fileIO.closeInputStream("testFile");

  auto & fileStreamOut3 = fileIO.openOutputStream(testingFileString, testingFilePathChar);

  fileStreamOut3 << "3" << std::endl;

  fileIO.closeOutputStream("testFile");

  fileIO.openInputStream(testingFileString, testingFilePathChar);
  line = fileIO.readLine("testFile");

  BOOST_REQUIRE_MESSAGE( line == "3", "ERROR: openInputStream() - EXPECTED: 3" << " - RECEIVED: " + line);
  
  fileIO.closeInputStream("testFile");

  auto & fileStreamOut4 = fileIO.openOutputStream(testingFileString, testingFilePathString);

  fileStreamOut4 << "4" << std::endl;

  fileIO.closeOutputStream("testFile");

  fileIO.openInputStream(testingFileString, testingFilePathString);
  line = fileIO.readLine("testFile");

  BOOST_REQUIRE_MESSAGE( line == "4", "ERROR: openInputStream() - EXPECTED: 4" << " - RECEIVED: " + line);
  
  fileIO.closeInputStream("testFile");
}

//Check FileIO can read a full file in
BOOST_AUTO_TEST_CASE(WholeFileReadTest)
{
  FileIO fileIO;
  SyntaxHandler syntax;
  fileIO.setDirectoryPath("data", fileIO.getBaseDirectory() + "/data");
  
  std::string const testingFileString = "testFile";
  std::string const testingFilePathString = fileIO.getDirectoryPath("data") + "/fileIOTestData";

  auto & fileStreamOut = fileIO.openOutputStream(testingFileString, testingFilePathString);

  for(int i = 0; i < numberOfLinesToTest; ++i)
  { fileStreamOut << i << std::endl; }

  fileIO.closeOutputStream("testFile");

  auto line = fileIO.readFileAsString("testFile");
  syntax.stripChar(line, '\n');
  syntax.stripChar(line, '\r');

  fileIO.openInputStream(testingFileString + "1", testingFilePathString);
  auto line2 = fileIO.readFileAsString("testFile1");
  fileIO.closeInputStream(testingFileString + "1");

  syntax.stripChar(line2, '\n');
  syntax.stripChar(line2, '\r');

  BOOST_REQUIRE_MESSAGE( line == line2 && line2 == "0123456789", 
    "ERROR: readFileAsString() - EXPECTED: 123456789" << " - RECEIVED: " + line + " AND: " + line2);
  
  fileIO.openOutputStream(testingFileString + "2", testingFilePathString);

  for(int i = 0; i < numberOfLinesToTest; ++i)
  { fileStreamOut << "a,b,c,d,e,f,g,h,i,j" << std::endl; }

  fileIO.closeOutputStream(testingFileString + "2");

  auto CSV = fileIO.readCSV("testFile2");

  for(auto & col : CSV)
  {
    for(auto & row : col)
    {
      std::cout << row << "|";
    }
    std::cout << std::endl;
  }
}

//*/