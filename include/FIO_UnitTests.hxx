#ifndef FILE_IO_UNIT_TESTS_H_
#define FILE_IO_UNIT_TESTS_H_

#include "FIO.hxx"
#include "SyntaxHandler.hxx"
#include "dessert.hpp"

#include <algorithm>
#include <chrono>
#include <string>
#include <iostream>
#include <tuple>

#if defined WIN32 || defined _WIN32 || defined __WIN32 && ! defined __CYGWIN__

  #ifndef WINDOWS
    #define WINDOWS
  #endif

  #ifndef NOMINMAX
    #define NOMINMAX
  #endif

#endif

#ifdef WINDOWS
  #include <filesystem>
#else
  #include <experimental/filesystem>
#endif

static int const constexpr numberOfFilesToTest = 10;
static int const constexpr numberOfLinesToTest = 10;

namespace FIOTests
{
  using namespace FileIO;
  using namespace hst;

  class FIO_UnitTests
  {
    public:
    FIO_UnitTests() : fio( "" ) {}
    ~FIO_UnitTests() = default;

    void initFIOTesting()
    {
      fio.clear();

#ifdef WINDOWS
      fio.storePathAtID(
        "data", fio.getRootDir() + PATH_SEP + ".." + PATH_SEP + "data" );
#else
      fio.storePathAtID( "data", fio.getRootDir() + PATH_SEP + "data" );
#endif
    }

    void runStringConstructionTests()
    {
      char const * sc( "A normal string." );
      std::string const s( "A normal string." );
      wchar_t const * wc( L"A normal string." );
      std::wstring const w( L"A normal string." );
      char const narrowChar( 'z' );
      wchar_t const wideChar( L'ß' );

      hstring fromsc( sc );
      hstring froms( s );
      hstring fromwc( wc );
      hstring fromw( w );
      hstring fromf( fromsc );
      hstring fromNarrowChar( narrowChar );
      hstring fromWideChar( wideChar );

      dessert( ( fromsc.str() == froms.str() && froms.str() == fromwc.str() &&
                 fromwc.str() == fromw.str() && fromw.str() == fromf.str() ) )
        << hstring( "hstring construction multi-byte equivalence." );

      dessert(
        ( fromsc.wstr() == froms.wstr() && froms.wstr() == fromwc.wstr() &&
          fromwc.wstr() == fromw.wstr() && fromw.wstr() == fromf.wstr() ) )
        << hstring( "hstring construction wide equivalence." );

      hstring assignsc = sc;
      hstring assigns = s;
      hstring assignwc = wc;
      hstring assignw = w;
      hstring assignf = fromsc;

      dessert(
        ( assignsc.str() == assigns.str() && assigns.str() == assignwc.str() &&
          assignwc.str() == assignw.str() && assignw.str() == assignf.str() ) )
        << hstring( "hstring assignment multi-byte equivalence." );

      dessert( ( assignsc.wstr() == assigns.wstr() &&
                 assigns.wstr() == assignwc.wstr() &&
                 assignwc.wstr() == assignw.wstr() &&
                 assignw.wstr() == assignf.wstr() ) )
        << hstring( "hstring assignment wide equivalence." );

      dessert(
        ( assignsc.str() == fromsc.str() && assigns.str() == froms.str() &&
          assignwc.str() == fromwc.str() && assignw.str() == fromw.str() ) )
        << hstring( "hstring assignment-construction multi-byte equivalence." );

      dessert(
        ( assignsc.wstr() == fromsc.wstr() && assigns.wstr() == froms.wstr() &&
          assignwc.wstr() == fromwc.wstr() && assignw.wstr() == fromw.wstr() ) )
        << hstring( "hstring assignment-construction wide equivalence." );

      dessert( ( fromf == fromsc && fromf == sc && fromf == s && fromf == wc &&
                 fromf == w ) )
        << hstring( "hstring equivalence operator." );

      dessert( ( ! ( fromf != fromsc ) && ! ( fromf != sc ) &&
                 ! ( fromf != s ) && ! ( fromf != wc ) && ! ( fromf != w ) ) )
        << hstring( "hstring inequivalence operator." );

      dessert( ( fromNarrowChar.str() == "z" && fromNarrowChar.wstr() == L"z" &&
                 fromWideChar.str() ==
                   wideToMultiByte( std::wstring( 1, wideChar ) ) &&
                 fromWideChar.wstr() == L"ß" ) )
        << hstring( "hstring char construction." );
    }

    void runStringConversionTests()
    {
      std::wstring ws = L"ééééééééééééßßßçççççßüüüääääûûûûûûûûûööööööèèèèèèè";
      std::string mb = wideToMultiByte( ws );

      dessert( ( ws == multiByteToWide( mb ) ) )
        << hstring( "hstring conversion." );

      hstring w( ws );
      hstring wToS( w.str() );

      dessert( ( ws == wToS.wstr() ) ) << hstring( "hstring conversion." );

      hstring s( mb );
      hstring sToW( s.wstr() );

      dessert( ( s == sToW.str() ) ) << hstring( "hstring conversion." );
    }

    void runStringConcatenationTest()
    {
      char const * sc( "A normal string.0" );
      std::string const s( "A normal string.1" );
      wchar_t const * wc( L"A normal string.2" );
      std::wstring const w( L"A normal string.3" );
      char const narrowChar( '4' );
      wchar_t const wideChar( L'5' );

      hstring fsc( sc );
      hstring fs( s );
      hstring fwc( wc );
      hstring fw( w );
      hstring fnarrowChar( narrowChar );
      hstring fwideChar( wideChar );

      dessert( ( ( fsc + fs ) == "A normal string.0A normal string.1" ) )
        << hstring( "hstring concatenation." );
      dessert( ( fsc + fs == fsc + s ) ) << hstring( "hstring concatenation." );
      dessert( ( fsc + fwc == fsc + wc ) )
        << hstring( "hstring concatenation." );
      dessert( ( fsc + fw == fsc + w ) ) << hstring( "hstring concatenation." );
      dessert( ( fsc + fnarrowChar == fsc + narrowChar ) )
        << hstring( "hstring concatenation." );
      dessert( ( fsc + fwideChar == fsc + wideChar ) )
        << hstring( "hstring concatenation." );

      dessert( ( fs + fsc == s + fsc ) ) << hstring( "hstring concatenation." );
      dessert( ( fwc + fsc == wc + fsc ) )
        << hstring( "hstring concatenation." );
      dessert( ( fw + fsc == w + fsc ) ) << hstring( "hstring concatenation." );
      dessert( ( fnarrowChar + fsc == narrowChar + fsc ) )
        << hstring( "hstring concatenation." );
      dessert( ( fwideChar + fsc == wideChar + fsc ) )
        << hstring( "hstring concatenation." );
    }

    void runPathManipulationTest()
    {
#ifdef WINDOWS
      char const c( '\\' );
      wchar_t const wc( L'\\' );
      char const * const sc( "\\" );
      wchar_t const * const swc( L"\\" );
      std::string s( "\\" );
      std::wstring ws( L"\\" );
#else
      char const c( '/' );
      wchar_t const wc( L'/' );
      char const * const sc( "/" );
      wchar_t const * const swc( L"/" );
      std::string s( "/" );
      std::wstring ws( L"/" );
#endif

      dessert( ( PATH_SEP == c ) ) << hstring( "Path Separator Equivalence." );
      dessert( ( PATH_SEP == wc ) ) << hstring( "Path Separator Equivalence." );
      dessert( ( PATH_SEP == sc ) ) << hstring( "Path Separator Equivalence." );
      dessert( ( PATH_SEP == swc ) )
        << hstring( "Path Separator Equivalence." );
      dessert( ( PATH_SEP == s ) ) << hstring( "Path Separator Equivalence." );
      dessert( ( PATH_SEP == ws ) ) << hstring( "Path Separator Equivalence." );
      dessert( ( s == PATH_SEP ) ) << hstring( "Path Separator Equivalence." );
      dessert( ( ws == PATH_SEP ) ) << hstring( "Path Separator Equivalence." );

      std::wstring pathW( L"C:\\A\\Test\\Path\\To\\A\\File.txt" );
      std::string path( "C:\\A\\Test\\Path\\To\\A\\File.txt" );

      dessert( ( parentDir( pathW ) == L"C:\\A\\Test\\Path\\To\\A" ) )
        << hstring( "Find Parent Directory." );
      dessert( ( parentDir( pathW ) == "C:\\A\\Test\\Path\\To\\A" ) )
        << hstring( "Find Parent Directory." );
      dessert( ( parentDir( path ) == "C:\\A\\Test\\Path\\To\\A" ) )
        << hstring( "Find Parent Directory." );
      dessert( ( parentDir( path ) == L"C:\\A\\Test\\Path\\To\\A" ) )
        << hstring( "Find Parent Directory." );
      dessert( ( parentDir( pathW ) == parentDir( path ) ) )
        << hstring( "Find Parent Directory." );

      pathW = parentDir( pathW );
      dessert( ( parentDir( pathW ) == L"C:\\A\\Test\\Path\\To" ) )
        << hstring( "Find Parent Directory." );
      pathW = parentDir( pathW );
      dessert( ( parentDir( pathW ) == L"C:\\A\\Test\\Path" ) )
        << hstring( "Find Parent Directory." );
      pathW = parentDir( pathW );
      dessert( ( parentDir( pathW ) == L"C:\\A\\Test" ) )
        << hstring( "Find Parent Directory." );
      pathW = parentDir( pathW );
      dessert( ( parentDir( pathW ) == L"C:\\A" ) )
        << hstring( "Find Parent Directory." );
      pathW = parentDir( pathW );
      dessert( ( parentDir( pathW ) == L"C:" ) )
        << hstring( "Find Parent Directory." );
      pathW = parentDir( pathW );
      dessert( ( parentDir( pathW ) == L"C:" ) )
        << hstring( "Find Parent Directory." );
      pathW += PATH_SEP;
      pathW = parentDir( pathW );
      dessert( ( parentDir( pathW ) == L"C:" ) )
        << hstring( "Find Parent Directory." );

      pathW = L"C:\\A\\Test\\Path\\To\\A\\File.txt";
      dessert( ( baseFile( pathW ) == L"File" ) )
        << hstring( "Find Base Filename. " );
      dessert( ( baseFile( pathW, StripExtensionFalse ) == L"File.txt" ) )
        << hstring( "Find Base Filename." );

      pathW = L"C:\\A\\Test\\Path\\To\\A\\File";
      dessert( ( baseFile( pathW ) == L"File" ) )
        << hstring( "Find Base Filename." );
      dessert( ( baseFile( pathW, StripExtensionTrue ) == L"File" ) )
        << hstring( "Find Base Filename." );

      pathW = L"C:";
      dessert( ( baseFile( pathW ) == L"C:" ) )
        << hstring( "Find Base Filename." );

      pathW = L"/file.txt";
      dessert( ( baseFile( pathW ) == L"file" ) )
        << hstring( "Find Base Filename." );

      auto splitWords = splitString( "The,Quick,Brown|Fox", ",|" );
      std::vector< std::string > exVal { "The", "Quick", "Brown", "Fox" };
      std::vector< std::wstring > exValW { L"The", L"Quick", L"Brown", L"Fox" };

      dessert( ( splitWords.size() == exVal.size() &&
                 splitWords.size() == exValW.size() ) )
        << hstring( "Split size mismatch." );
      for ( std::size_t i = 0; i < exVal.size(); ++i )
      {
        dessert( ( splitWords[ i ] == exVal[ i ] ) )
          << hstring( "Split string." );
        dessert( ( splitWords[ i ] == exValW[ i ] ) )
          << hstring( "Split string." );
      }

      splitWords = splitString( "The,Quick,Brown|Fox", "," );
      exVal = { "The", "Quick", "Brown|Fox" };
      exValW = { L"The", L"Quick", L"Brown|Fox" };

      dessert( ( splitWords.size() == exVal.size() &&
                 splitWords.size() == exValW.size() ) )
        << hstring( "Split size mismatch." );
      for ( std::size_t i = 0; i < exVal.size(); ++i )
      {
        dessert( ( splitWords[ i ] == exVal[ i ] ) )
          << hstring( "Split string." );
        dessert( ( splitWords[ i ] == exValW[ i ] ) )
          << hstring( "Split string." );
      }

      splitWords = splitString( "The,Quick,Brown|Fox" );
      exVal = { "The,Quick,Brown|Fox" };
      exValW = { L"The,Quick,Brown|Fox" };

      dessert( ( splitWords.size() == exVal.size() &&
                 splitWords.size() == exValW.size() ) )
        << hstring( "Split size mismatch." );
      for ( std::size_t i = 0; i < exVal.size(); ++i )
      {
        dessert( ( splitWords[ i ] == exVal[ i ] ) )
          << hstring( "Split string." );
        dessert( ( splitWords[ i ] == exValW[ i ] ) )
          << hstring( "Split string." );
      }

      splitWords = splitString( "The,Quick,Brown|Fox", "" );
      exVal = { "The,Quick,Brown|Fox" };
      exValW = { L"The,Quick,Brown|Fox" };

      dessert( ( splitWords.size() == exVal.size() &&
                 splitWords.size() == exValW.size() ) )
        << hstring( "Split size mismatch." );
      for ( std::size_t i = 0; i < exVal.size(); ++i )
      {
        dessert( ( splitWords[ i ] == exVal[ i ] ) )
          << hstring( "Split string." );
        dessert( ( splitWords[ i ] == exValW[ i ] ) )
          << hstring( "Split string." );
      }

      dessert( ( fio.getPath( "newPath" ) == "newPath" ) )
        << hstring( "Unstored path is returned unaltered." );

      fio.storePathAtID( "newPath", "ANewPath" );
      dessert( ( fio.getPath( "newPath" ) == "ANewPath" ) )
        << hstring( "Stored path is returned correctly." );

      fio.clear();
      dessert( ( fio.getPath( "newPath" ) == "newPath" ) )
        << hstring( "Unstored path is returned unaltered." );

      fio.storePathAtID( "newPath", "ANewPath" );
      dessert( ( fio.getPath( "newPath" ) == "ANewPath" ) )
        << hstring( "Stored path is returned correctly." );

      fio.removePathAtID( "newPath" );
      dessert( ( fio.getPath( "newPath" ) == "newPath" ) )
        << hstring( "Unstored path is returned unaltered." );
    }

    void runFIOConstructionTests()
    {
      FIO fio;
      dessert( ( fio.getRootDir() == fio.findRootDir() ) )
        << hstring( "FIO root assignment." );

      fio.setRootDir( "Bananas" );
      dessert( ( fio.getRootDir() == "Bananas" ) )
        << hstring( "FIO root assignment." );

      fio.storePathAtID( "__root", "NotBananas" );
      dessert( ( fio.getRootDir() == "Bananas" ) )
        << hstring( "FIO root assignment safety check." );

      fio.clear();
      dessert( ( fio.getRootDir() == "Bananas" ) )
        << hstring( "FIO root assignment survives clear." );

      fio.storePathAtID( "test", "NotBananas" );
      fio.setRootDir( "test" );
      dessert( ( fio.getRootDir() == "NotBananas" ) )
        << hstring( "FIO root assignment retrieves stored paths." );

      fio.clear();
      dessert( ( fio.getRootDir() == "NotBananas" ) )
        << hstring( "FIO root assignment survives clear." );

      bool illegalLocaleThrowsError = false;
      try
      {
        FIO fio2( "notALocale" );
      }
      catch ( std::runtime_error const & e )
      {
        illegalLocaleThrowsError = true;
      }
      dessert( ( illegalLocaleThrowsError ) )
        << hstring( "Illegal locale throws error." );
    }

    void runBasicReadTest()
    {
      initFIOTesting();

      fio.openInputStream( fio.getPath( "data" ) + PATH_SEP + "integers.txt" );
      dessert( ( fio.hasInputStream( fio.getPath( "data" ) + PATH_SEP +
                                     "integers.txt" ) ) )
        << hstring( "Input Stream opens." );

      fio.closeInputStream( fio.getPath( "data" ) + PATH_SEP + "integers.txt" );
      dessert( ( ! fio.hasInputStream( fio.getPath( "data" ) + PATH_SEP +
                                       "integers.txt" ) ) )
        << hstring( "Input Stream closes." );

      fio.storePathAtID( "intFile",
                         fio.getPath( "data" ) + PATH_SEP + "integers.txt" );
      fio.openInputStream( "intFile" );
      dessert( ( fio.hasInputStream( "intFile" ) ) )
        << hstring( "Input Stream opens from path ID." );

      fio.closeInputStream( "intFile" );
      dessert( ( ! fio.hasInputStream( "intFile" ) ) )
        << hstring( "Input Stream closes from path ID." );

      auto & i4Stream = fio.openInputStream( "intFile" );

      std::vector< std::wstring > lines;
      for ( int i = 0; i < 100; ++i )
      {
        std::wstring line;
        std::getline( i4Stream, line );
        lines.push_back( line );
      }
      fio.closeInputStream( "intFile" );

      fio.openInputStream( "intFile" );
      for ( int i = 0; i < 100; ++i )
      {
        auto lineRead = fio.readLine( "intFile" );
        dessert( ( lines[ i ] == lineRead ) )
          << hstring( "readLine reads line correctly. " );
      }
      fio.closeInputStream( "intFile" );

      fio.openInputStream( "intFile" );
      for ( int i = 0; i < 50; ++i )
      {
        auto lineRead = fio.readLine( "intFile" );
        dessert( ( std::atoi( lineRead.str().c_str() ) == i ) )
          << hstring( "readLine reads line correctly. " );
      }
      fio.rewindInputStream( "intFile" );
      for ( int i = 0; i < 50; ++i )
      {
        auto lineRead = fio.readLine( "intFile" );
        dessert( ( std::atoi( lineRead.str().c_str() ) == i ) )
          << hstring( "resetInputStream resets stream correctly. " );
      }
      fio.closeInputStream( "intFile" );

      fio.openInputStream( "intFile" );
      for ( int i = 0; i < 50; ++i ) { fio.readLine( "intFile" ); }
      auto f1 = fio.readFile( "intFile" );
      fio.closeInputStream( "intFile" );
      fio.openInputStream( "intFile" );
      auto f2 = fio.readFile( "intFile" );
      fio.closeInputStream( "intFile" );

      dessert( ( f1 == f2 ) )
        << hstring( "readFile correctly resets open input stream." );
    }

    void runBasicWriteTest()
    {
      initFIOTesting();
      fio.storePathAtID( "testFile",
                         fio.getPath( "data" ) + PATH_SEP + "test.txt" );

      fio.openOutputStream( "testFile" );
      dessert( ( fio.hasOutputStream( "testFile" ) ) );
      fio.closeOutputStream( "testFile" );
      dessert( ( ! fio.hasOutputStream( "testFile" ) ) );

      deleteFile( fio.getPath( "testFile" ) );

      bool deletedFileThrowsError = false;

      try
      {
        fio.openInputStream( "testFile" );
      }
      catch ( std::runtime_error & e )
      {
        deletedFileThrowsError = true;
      }

      dessert( ( deletedFileThrowsError ) )
        << hstring( "File deleted successfully" );
    }

    void runExtendedReadWriteTest()
    {
      initFIOTesting();

      auto & stream =
        fio.openOutputStream( fio.getPath( "data" ) + PATH_SEP + "test.txt" );
      for ( int j = 0; j < 100; ++j )
      {
        stream << j;
        if ( j < 99 ) { stream << hstring( "," ); }

        if ( ( j + 1 ) % 10 == 0 && j < 99 ) { stream << std::endl; }
      }

      fio.openOutputStream( fio.getPath( "data" ) + PATH_SEP + "test2.txt" );
      for ( int j = 0; j < 100; ++j )
      {
        fio.writeLine( fio.getPath( "data" ) + PATH_SEP + "test2.txt",
                       std::to_string( j ) );

        if ( j < 99 )
        {
          fio.writeLine( fio.getPath( "data" ) + PATH_SEP + "test2.txt", "," );
        }

        if ( ( j + 1 ) % 10 == 0 && j < 99 )
        {
#ifdef WINDOWS
          fio.writeLine( fio.getPath( "data" ) + PATH_SEP + "test2.txt", "\n" );
#else
          fio.writeLine( fio.getPath( "data" ) + PATH_SEP + "test2.txt",
                         "\r\n" );
#endif
        }
      }

      fio.closeOutputStream( fio.getPath( "data" ) + PATH_SEP + "test.txt" );
      fio.closeOutputStream( fio.getPath( "data" ) + PATH_SEP + "test2.txt" );

      auto v1 =
        fio.readFileToVector( fio.getPath( "data" ) + PATH_SEP + "test.txt" );
      auto v2 =
        fio.readFileToVector( fio.getPath( "data" ) + PATH_SEP + "test2.txt" );

      dessert( ( v1.size() == v2.size() ) )
        << hstring( "File Vectors have same size." );

      for ( std::size_t i = 0; i < v1.size(); ++i )
      {
        dessert( ( v1[ i ] == v2[ i ] ) )
          << hstring( "Vector elements have same size." );
      }

      auto m1 =
        fio.readFileToMatrix( fio.getPath( "data" ) + PATH_SEP + "test.txt" );
      auto m2 =
        fio.readFileToMatrix( fio.getPath( "data" ) + PATH_SEP + "test2.txt" );

      dessert( ( m1.size() == m2.size() ) )
        << hstring( "File Matrices have same size." );

      for ( std::size_t i = 0; i < m1.size(); ++i )
      {
        dessert( ( m1[ i ].size() == m2[ i ].size() ) )
          << hstring( "File Matrices have same size." );

        for ( std::size_t j = 0; j < m1[ i ].size(); ++j )
        {
          dessert( ( m1[ i ][ j ] == m2[ i ][ j ] ) )
            << hstring( "Vector elements have same size." );
        }
      }

      auto & stream3 =
        fio.openOutputStream( fio.getPath( "data" ) + PATH_SEP + "test3.txt" );
      for ( int j = 0; j < 100; ++j )
      {
        stream3 << j << hstring( "," );

        if ( ( j + 1 ) % 10 == 0 ) { stream3 << std::endl; }
      }
      fio.closeOutputStream( fio.getPath( "data" ) + PATH_SEP + "test3.txt" );

      auto m3 =
        fio.readFileToMatrix( fio.getPath( "data" ) + PATH_SEP + "test3.txt" );

      dessert( ( m2.size() == m3.size() ) )
        << hstring( "File Matrices have same size." );

      for ( std::size_t i = 0; i < m2.size(); ++i )
      {
        dessert( ( m2[ i ].size() == m3[ i ].size() ) )
          << hstring( "File Matrices have same size." );

        for ( std::size_t j = 0; j < m2[ i ].size(); ++j )
        {
          dessert( ( m2[ i ][ j ] == m3[ i ][ j ] ) )
            << hstring( "Vector elements have same size." );
        }
      }

      deleteFile( fio.getPath( "data" ) + PATH_SEP + "test.txt" );
      deleteFile( fio.getPath( "data" ) + PATH_SEP + "test2.txt" );
      deleteFile( fio.getPath( "data" ) + PATH_SEP + "test3.txt" );
    }

    void runFileSearchTest()
    {
      initFIOTesting();

      for ( int i = 0; i < 30; ++i )
      {
        fio.openOutputStream( fio.getPath( "data" ) + PATH_SEP + "data" +
                              std::to_string( i % 3 ) + PATH_SEP +
                              std::to_string( i ) + ".txt" );
      }
      for ( int i = 0; i < 30; ++i )
      {
        fio.openOutputStream( fio.getPath( "data" ) + PATH_SEP + "data0" +
                              PATH_SEP + "data0" + PATH_SEP +
                              std::to_string( i ) + ".txt" );
      }
      for ( int i = 0; i < 30; ++i )
      {
        fio.openOutputStream( fio.getPath( "data" ) + PATH_SEP + "data0" +
                              PATH_SEP + "data0" + PATH_SEP + "data0" +
                              PATH_SEP + std::to_string( i ) + ".txt" );
      }
      for ( int i = 0; i < 30; ++i )
      {
        fio.openOutputStream( fio.getPath( "data" ) + PATH_SEP + "data" +
                              std::to_string( i % 3 ) + PATH_SEP +
                              std::to_string( i ) + ".png" );
      }
      for ( int i = 0; i < 30; ++i )
      {
        fio.openOutputStream( fio.getPath( "data" ) + PATH_SEP + "data0" +
                              PATH_SEP + "data0" + PATH_SEP +
                              std::to_string( i ) + ".png" );
      }
      for ( int i = 0; i < 30; ++i )
      {
        fio.openOutputStream( fio.getPath( "data" ) + PATH_SEP + "data0" +
                              PATH_SEP + "data0" + PATH_SEP + "data0" +
                              PATH_SEP + std::to_string( i ) + ".png" );
      }

      fio.clear();
      initFIOTesting();

      auto files = fio.findFiles( ".txt", fio.getPath( "data" ) );

      dessert( ( files.size() != 0 ) ) << hst::hstring( "Files were found." );

      for ( int i = 0; i < 30; ++i )
      {
        auto it = std::find( files.begin(),
                             files.end(),
                             fio.getPath( "data" ) + PATH_SEP + "data" +
                               std::to_string( i % 3 ) + PATH_SEP +
                               std::to_string( i ) + ".txt" );

        dessert( ( it != files.end() ) ) << hst::hstring( "File finding." );
      }
      for ( int i = 0; i < 30; ++i )
      {
        auto it =
          std::find( files.begin(),
                     files.end(),
                     fio.getPath( "data" ) + PATH_SEP + "data0" + PATH_SEP +
                       "data0" + PATH_SEP + std::to_string( i ) + ".txt" );

        dessert( ( it != files.end() ) ) << hst::hstring( "File finding." );
      }
      for ( int i = 0; i < 30; ++i )
      {
        auto it = std::find( files.begin(),
                             files.end(),
                             fio.getPath( "data" ) + PATH_SEP + "data0" +
                               PATH_SEP + "data0" + PATH_SEP + "data0" +
                               PATH_SEP + std::to_string( i ) + ".txt" );

        dessert( ( it != files.end() ) ) << hst::hstring( "File finding." );
      }

      auto files2 = fio.findFiles( ".png", fio.getPath( "data" ) );

      dessert( ( files2.size() != 0 ) ) << hst::hstring( "files2 were found." );

      for ( int i = 0; i < 30; ++i )
      {
        auto it = std::find( files2.begin(),
                             files2.end(),
                             fio.getPath( "data" ) + PATH_SEP + "data" +
                               std::to_string( i % 3 ) + PATH_SEP +
                               std::to_string( i ) + ".png" );

        dessert( ( it != files2.end() ) ) << hst::hstring( "File finding." );
      }
      for ( int i = 0; i < 30; ++i )
      {
        auto it =
          std::find( files2.begin(),
                     files2.end(),
                     fio.getPath( "data" ) + PATH_SEP + "data0" + PATH_SEP +
                       "data0" + PATH_SEP + std::to_string( i ) + ".png" );

        dessert( ( it != files2.end() ) ) << hst::hstring( "File finding." );
      }
      for ( int i = 0; i < 30; ++i )
      {
        auto it = std::find( files2.begin(),
                             files2.end(),
                             fio.getPath( "data" ) + PATH_SEP + "data0" +
                               PATH_SEP + "data0" + PATH_SEP + "data0" +
                               PATH_SEP + std::to_string( i ) + ".png" );

        dessert( ( it != files2.end() ) ) << hst::hstring( "File finding." );
      }

      auto files3 = fio.findFiles( ".*", fio.getPath( "data" ) );

      dessert( ( files3.size() != 0 ) ) << hst::hstring( "files3 were found." );

      for ( int i = 0; i < 30; ++i )
      {
        auto it = std::find( files3.begin(),
                             files3.end(),
                             fio.getPath( "data" ) + PATH_SEP + "data" +
                               std::to_string( i % 3 ) + PATH_SEP +
                               std::to_string( i ) + ".txt" );

        dessert( ( it != files3.end() ) ) << hst::hstring( "File finding." );
      }
      for ( int i = 0; i < 30; ++i )
      {
        auto it =
          std::find( files3.begin(),
                     files3.end(),
                     fio.getPath( "data" ) + PATH_SEP + "data0" + PATH_SEP +
                       "data0" + PATH_SEP + std::to_string( i ) + ".txt" );

        dessert( ( it != files3.end() ) ) << hst::hstring( "File finding." );
      }
      for ( int i = 0; i < 30; ++i )
      {
        auto it = std::find( files3.begin(),
                             files3.end(),
                             fio.getPath( "data" ) + PATH_SEP + "data0" +
                               PATH_SEP + "data0" + PATH_SEP + "data0" +
                               PATH_SEP + std::to_string( i ) + ".txt" );

        dessert( ( it != files3.end() ) ) << hst::hstring( "File finding." );
      }
      for ( int i = 0; i < 30; ++i )
      {
        auto it = std::find( files3.begin(),
                             files3.end(),
                             fio.getPath( "data" ) + PATH_SEP + "data" +
                               std::to_string( i % 3 ) + PATH_SEP +
                               std::to_string( i ) + ".png" );

        dessert( ( it != files3.end() ) ) << hst::hstring( "File finding." );
      }
      for ( int i = 0; i < 30; ++i )
      {
        auto it =
          std::find( files3.begin(),
                     files3.end(),
                     fio.getPath( "data" ) + PATH_SEP + "data0" + PATH_SEP +
                       "data0" + PATH_SEP + std::to_string( i ) + ".png" );

        dessert( ( it != files3.end() ) ) << hst::hstring( "File finding." );
      }
      for ( int i = 0; i < 30; ++i )
      {
        auto it = std::find( files3.begin(),
                             files3.end(),
                             fio.getPath( "data" ) + PATH_SEP + "data0" +
                               PATH_SEP + "data0" + PATH_SEP + "data0" +
                               PATH_SEP + std::to_string( i ) + ".png" );

        dessert( ( it != files3.end() ) ) << hst::hstring( "File finding." );
      }

      for ( int i = 0; i < 30; ++i )
      {
        deleteFile( fio.getPath( "data" ) + PATH_SEP + "data" +
                    std::to_string( i % 3 ) + PATH_SEP + std::to_string( i ) +
                    ".txt" );
      }
      for ( int i = 0; i < 30; ++i )
      {
        deleteFile( fio.getPath( "data" ) + PATH_SEP + "data0" + PATH_SEP +
                    "data0" + PATH_SEP + std::to_string( i ) + ".txt" );
      }
      for ( int i = 0; i < 30; ++i )
      {
        deleteFile( fio.getPath( "data" ) + PATH_SEP + "data0" + PATH_SEP +
                    "data0" + PATH_SEP + "data0" + PATH_SEP +
                    std::to_string( i ) + ".txt" );
      }
      for ( int i = 0; i < 30; ++i )
      {
        deleteFile( fio.getPath( "data" ) + PATH_SEP + "data" +
                    std::to_string( i % 3 ) + PATH_SEP + std::to_string( i ) +
                    ".png" );
      }
      for ( int i = 0; i < 30; ++i )
      {
        deleteFile( fio.getPath( "data" ) + PATH_SEP + "data0" + PATH_SEP +
                    "data0" + PATH_SEP + std::to_string( i ) + ".png" );
      }
      for ( int i = 0; i < 30; ++i )
      {
        deleteFile( fio.getPath( "data" ) + PATH_SEP + "data0" + PATH_SEP +
                    "data0" + PATH_SEP + "data0" + PATH_SEP +
                    std::to_string( i ) + ".png" );
      }
    }

    void runAllUnitTests()
    {
      runStringConstructionTests();
      runStringConversionTests();
      runStringConcatenationTest();
      runPathManipulationTest();
      runFIOConstructionTests();
      runBasicReadTest();
      runBasicWriteTest();
      runExtendedReadWriteTest();
      runFileSearchTest();
    }

    private:
    FIO fio;
  };
} // namespace FIOTests

#endif