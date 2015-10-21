#include <cstdio>
#include "scrutil.h"

using namespace std;

int main()
{
    char nombre[255];

    ScrUtil::setColors( ScrUtil::Yellow, ScrUtil::Blue );
    ScrUtil::clear();

    ScrUtil::showCursor( true );

    ScrUtil::moveCursorTo( 0, 0 );
    ScrUtil::setColors( ScrUtil::Yellow, ScrUtil::Black );
    printf( "scrutil Demo" );
    ScrUtil::setColors( ScrUtil::Yellow, ScrUtil::Blue );

    ScrUtil::moveCursorTo( 5, 10 );
    printf( "Max Filas: %d\n", ScrUtil::getMaxRows() );
    ScrUtil::moveCursorTo( 6, 10 );
    printf( "Max Columnas: %d\n", ScrUtil::getMaxColumns() );


    ScrUtil::moveCursorTo( 10, 10 );
    printf( "Introduce tu nombre:" );
    ScrUtil::moveCursorTo( 10, 50 );
    scanf( "%s", nombre );

    ScrUtil::setColors( ScrUtil::Yellow, ScrUtil::Red );
    ScrUtil::moveCursorTo( ScrUtil::getMaxRows() - 5, 50 );
    printf( "Tu nombre es %s\n", nombre );
    ScrUtil::setColors( ScrUtil::White, ScrUtil::Black );
    ScrUtil::showCursor( false );
    return 0;
}
