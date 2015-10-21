#ifndef SCRUTIL_H_INCLUDED
#define SCRUTIL_H_INCLUDED

namespace ScrUtil {

/** Colores */
enum Color {
    Black, Blue, Red, Magenta,
    Green, Cyan, Yellow, White,
    UndefinedColor
};

/**
    Describe una pos. en la pantalla
*/
struct Position {
    short int row;
    short int column;
};

struct Attributes {
    Color paper;
    Color ink;
};

/**
    Borra la pantalla
*/
void clear();

/**
    Indica los colores del texto a escribir
    @param tinta Color de la tinta
    @param papel Color de fondo
*/
void setColors(Color tinta, Color papel);

/**
    Indica los colores del texto a escribir
    @param color Color de la tinta y el papel
    @see Attributes
*/
void setColors(Attributes color);

/**
    Obtiene los atributos en uso
    @return Los colores como una estructura Attributes
    @see Attributes
*/
Attributes getCurrentAttributes();

/**
    Obtiene el char. en una pos. de la pantalla
    @param La pos. como una estructura Position
    @return El valor del char.
    @see Attributes
*/
int getCharacterAt(Position pos);

/**
    Mover el cursor a una pos. determinada
    @param fila La fila donde colocar cursor
    @param columna La columna donde colocar el cursor
*/
void moveCursorTo(unsigned short int fila, int unsigned short columna);

/**
    Mover el cursor a una pos. determinada
    @param pos Estructura Position conteniendo la pos.
    @see Position
*/
void moveCursorTo(Position pos);

/**
    Devuelve el num. de filas y columnas max.
    @return La info. como estructura Position
    @see Position
    @note en Unix siempre devuelve 25x80
*/
Position getConsoleSize();

/**
    Devuelve el num. de filas
    @return El max. num. de filas
    @see Position
    @note En caso de que la funcionalidad no se soporte,
          devuelve -1 en ambos campos de Position
*/
short int getMaxRows();

/**
    Devuelve el num. de columnas
    @return El max. num. de columnas
    @see getConsoleSize
*/
short int getMaxColumns();

/**
    Devuelve la pos. del cursor
    @return La pos. del cursor
    @see getConsoleSize
    @note en Unix siempre devuelve -1, -1
*/
Position getCursorPosition();

/**
    Esconde o visualiza el cursor
    @param see Si es verdadero, lo visualiza, si es falso lo esconde.
*/
void showCursor(bool see);

}

#endif // SCRUTIL_H_INCLUDED
