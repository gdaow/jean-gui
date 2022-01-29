#include <clang-c/Index.h>

int main( int argc, char** argv )
{
  if( argc < 2 ) {
    return -1;
  }

  CXIndex index        = clang_createIndex( 0, 1 );
  CXTranslationUnit tu = clang_createTranslationUnit( index, argv[1] );

  if(tu == nullptr) {
    return -1;
  }
  
  clang_disposeTranslationUnit( tu );
  clang_disposeIndex( index );
  return 0;
}
