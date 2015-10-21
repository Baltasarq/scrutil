/*
    Manejo de la consola
*/

#include "scrutil.h"

#include <cstring>
#include <cstdio>

namespace ScrUtil {

/* Detectar el sistema operativo */
#ifdef _WIN32
    #define SO_WINDOWS
#else
    #ifdef __WIN32__
        #define SO_WINDOWS
    #else
        #ifdef __WINDOWS__
            #define SO_WINDOWS
        #else
            #define SO_UNIX
        #endif
    #endif
#endif

#ifdef SO_WINDOWS
#include <windows.h>

/* WinColors */
/// Colores para tinta
static short int winInkColors[ UndefinedColor + 1 ];
/// Colores para fondo
static short int winPaperColors[ UndefinedColor + 1 ];

static void initWindowsColors()
/**
    Windows distingue entre colores de fondo y colores de tinta,
    y sigue el std. RGB, lo que obliga a definir los colores
    sencillos para construirlos.
*/
{
    winInkColors[ Black ]     = 0;
    winInkColors[ Blue ]      = FOREGROUND_BLUE;
    winInkColors[ Red ]       = FOREGROUND_RED;
    winInkColors[ Magenta ]   = FOREGROUND_BLUE | FOREGROUND_RED;
    winInkColors[ Green ]     = FOREGROUND_GREEN;
    winInkColors[ Cyan ]      = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    winInkColors[ Yellow ]    = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
    winInkColors[ White ]     = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    winInkColors[ UndefinedColor ]  = 0;

    winPaperColors[ Black ]   = 0;
    winPaperColors[ Blue ]    = BACKGROUND_BLUE;
    winPaperColors[ Red ]     = BACKGROUND_RED;
    winPaperColors[ Magenta ] = BACKGROUND_BLUE | BACKGROUND_RED;
    winPaperColors[ Green ]   = BACKGROUND_GREEN;
    winPaperColors[ Cyan ]    = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
    winPaperColors[ Yellow ]  = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
    winPaperColors[ White ]   = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    winPaperColors[ UndefinedColor ] = 0;
}
#else
    // Commands
    static const short int MaxCmdBuffer = 32;
    /// Caracts. por el que empiezan todos los comandos
    static const char * CSI = "\33[";
    static const char * CmdClearScreen = "2J";
    static char cmd[MaxCmdBuffer];

    // Colors
    /// Colores para la tinta
    static char cmdUnixInkColors[ UndefinedColor + 1 ][MaxCmdBuffer];
    /// Colores para el fondo
    static char cmdUnixPaperColors[ UndefinedColor + 1 ][MaxCmdBuffer];
    /// Max. filas en pantalla
    static const short int UnixLastLine = 25;
    static const short int UnixLastColumn = 80;


    static void initUnixColors()
    /**
        Linux sigue los valores de control ANSI que comienzan con
        ESC, y con ellos se puede limpiar la pantalla, cambiar los colores, ...
    */
    {
        sprintf( cmdUnixInkColors[ Black ], "%s%s", CSI, "30m" );
        sprintf( cmdUnixInkColors[ Blue ], "%s%s", CSI, "34m" );
        sprintf( cmdUnixInkColors[ Red ], "%s%s", CSI, "31m" );
        sprintf( cmdUnixInkColors[ Magenta ], "%s%s", CSI, "35m" );
        sprintf( cmdUnixInkColors[ Green ], "%s%s", CSI, "32m" );
        sprintf( cmdUnixInkColors[ Cyan ], "%s%s", CSI, "36m" );
        sprintf( cmdUnixInkColors[ Yellow ], "%s%s", CSI, "93m" );
        sprintf( cmdUnixInkColors[ White ], "%s%s", CSI, "37m" );
        sprintf( cmdUnixInkColors[ UndefinedColor ], "%s%s", CSI, "30m" );

        sprintf( cmdUnixPaperColors[ Black ], "%s%s", CSI, "40m" );
        sprintf( cmdUnixPaperColors[ Blue ], "%s%s", CSI, "44m" );
        sprintf( cmdUnixPaperColors[ Red ], "%s%s", CSI, "41m" );
        sprintf( cmdUnixPaperColors[ Magenta ], "%s%s", CSI, "45m" );
        sprintf( cmdUnixPaperColors[ Green ], "%s%s", CSI, "42m" );
        sprintf( cmdUnixPaperColors[ Cyan ], "%s%s", CSI, "46m" );
        sprintf( cmdUnixPaperColors[ Yellow ], "%s%s", CSI, "103m" );
        sprintf( cmdUnixPaperColors[ White ], "%s%s", CSI, "47m" );
        sprintf( cmdUnixPaperColors[ UndefinedColor ], "%s%s", CSI, "40m" );
    }
#endif

static Attributes libAttrs;
static bool inited = false;

static inline void init()
{
    if ( !inited ) {
        #ifdef SO_WINDOWS
            initWindowsColors();
        #else
            initUnixColors();
        #endif

        libAttrs.ink = White;
        libAttrs.paper = Black;
        inited = true;
    }
}

void clear()
{
    init();

    #ifdef SO_WINDOWS
        COORD pos = { 0, 0 };
        DWORD cars;
        HANDLE hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD dwConSize;

        if( hStdOut != INVALID_HANDLE_VALUE
         && GetConsoleScreenBufferInfo( hStdOut, &csbi ) )
        {
            dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

            FillConsoleOutputCharacter(
                hStdOut,
                ' ',
                dwConSize,
                pos,
                &cars
            );

            FillConsoleOutputAttribute(
                hStdOut,
                winPaperColors[ libAttrs.paper ] | winInkColors[ libAttrs.ink ],
                dwConSize,
                pos,
                &cars
            );
        }
    #else
        strcpy( cmd, CSI );
        strcat( cmd, CmdClearScreen );
        printf( "%s", cmd );
    #endif

    moveCursorTo( 0, 0 );
}

void setColors(Attributes colors)
{
    init();

    libAttrs.paper = colors.paper;
    libAttrs.ink = colors.ink;

    #ifdef SO_WINDOWS
        SetConsoleTextAttribute(
            GetStdHandle( STD_OUTPUT_HANDLE ),
            winPaperColors[ libAttrs.paper ] | winInkColors[ libAttrs.ink ]
        );
    #else
        printf( "%s%s",
            cmdUnixInkColors[ colors.ink ],
            cmdUnixPaperColors[ colors.paper ]
        );
    #endif
}

void setColors(Color tinta, Color papel)
{
    Attributes atrs;

    atrs.paper = papel;
    atrs.ink = tinta;

    setColors( atrs );
}

void moveCursorTo(Position pos)
{
    moveCursorTo( pos.row, pos.column );
}

void moveCursorTo(unsigned short int fila, unsigned short int columna)
{
    init();

    #ifdef SO_WINDOWS
        COORD pos;

        pos.X = columna;
        pos.Y = fila;

        SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), pos );
    #else
        printf( "%s%d;%dH", CSI, fila + 1, columna + 1 );
    #endif
}

Position getConsoleSize()
{
    Position pos;

    init();
    pos.row = pos.column = -1;

    #ifdef SO_WINDOWS
        CONSOLE_SCREEN_BUFFER_INFO infoPantalla;
        GetConsoleScreenBufferInfo(
                GetStdHandle( STD_OUTPUT_HANDLE ), &infoPantalla
        );

        pos.row = infoPantalla.srWindow.Bottom + 1;
        pos.column = infoPantalla.srWindow.Right + 1;
    #else
        pos.row = UnixLastLine;
        pos.column = UnixLastColumn;
    #endif

    return pos;
}

Attributes getCurrentAttributes()
{
    Attributes atr;

    init();

    atr.ink = libAttrs.ink;
    atr.paper = libAttrs.paper;
    return atr;
}

short int getMaxRows()
{
    return getConsoleSize().row;
}

short int getMaxColumns()
{
    return getConsoleSize().column;
}

Position getCursorPosition()
{
    Position toret;

    memset( &toret, 0, sizeof( Position ) );
    init();

    #ifdef SO_WINDOWS
        CONSOLE_SCREEN_BUFFER_INFO infoPantalla;
        GetConsoleScreenBufferInfo(
                GetStdHandle( STD_OUTPUT_HANDLE ),
                &infoPantalla
        );

        toret.row = infoPantalla.dwCursorPosition.Y;
        toret.column = infoPantalla.dwCursorPosition.X;
    #else
        toret.row = -1;
        toret.column = -1;
    #endif

    return toret;
}

void showCursor(bool see)
{
    init();

    #ifdef SO_WINDOWS
        CONSOLE_CURSOR_INFO info;

        info.dwSize = 10;
        info.bVisible = (BOOL) see;
        SetConsoleCursorInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &info );
    #else
        printf( "%s?25%c", CSI, see ? 'h' : 'l' );
    #endif
}

}
