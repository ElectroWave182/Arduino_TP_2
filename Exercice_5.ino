#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>

int donnees = 6;

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix (8, 8, donnees,
    NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
    NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] =
{
    matrix.Color(0, 0, 0),   // Black
    matrix.Color(255, 0, 0), // Red
    matrix.Color(0, 255, 0), // Green
    matrix.Color(0, 0, 255), // Blue
    matrix.Color(127, 127, 0)  // Yellow
};


// Coordonnees
int ligne = 3;
int colonne = 3;

// Controle
int bp_droite = 2;
int bp_gauche = 3;
int sens = 0;

// Fruit
int fruit_l = 3
int fruit_c = 3;

// Snake
int taille = 3;

void droite ()
{
    if (bouton)
    {
        bouton = false;
        
        sens ++;
        sens %= 4;
        delay (500);
        
        bouton = true;
    }
}

void gauche ()
{
    if (bouton)
    {
        bouton = false;
        
        sens --;
        sens %= 4;
        delay (500);
        
        bouton = true;
    }
}

  
void setup()
{
    matrix.begin ();
    matrix.setBrightness (10);

    pinMode (bp_droite, INPUT_PULLUP);
    pinMode (bp_gauche, INPUT_PULLUP);
    attachInterrupt (digitalPinToInterrupt (bp_droite), droite,
FALLING);
    attachInterrupt (digitalPinToInterrupt (bp_gauche), gauche,
FALLING);
}


void loop()
{
    if (fruit_l == ligne && fruit_c == colonne)
    {
        fruit_l = rand (8);
        fruit_c = rand (8);
        matrix.drawPixel (fruit_l, fruit_c, colors[4]);
        taille ++;
    }

    matrix.drawPixel (ligne, colonne, colors[2]);
    matrix.show ();
    delay (500);
    
    matrix.drawPixel (ligne, colonne, colors[0]); // faire une fonction qui envoie l'effaçage avec du délai (500 ms * taille)
    switch (sens)
    {
        case 0:
            colonne ++;
            break;
            
        case 1:
            ligne --;
            break;
            
        case 2:
            colonne --;
            break;

        case 3:
            ligne ++;
            break;
    }

    if (ligne < 0 || ligne > 7 || colonne < 0 || colonne > 7) // Snake est sorti de la matrice
    {
        matrix.fillScreen (colors[1]);
        matrix.show ();
        exit ();
    }
}
