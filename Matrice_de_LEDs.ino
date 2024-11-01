# include <Adafruit_NeoPixel.h>
# include <Adafruit_GFX.h>
# include <Adafruit_NeoMatrix.h>


int donnees = 2;

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


uint16_t red = matrix.Color(255, 0, 0);
uint16_t green = matrix.Color(0, 255, 0);
uint16_t blue = matrix.Color(0, 0, 255);
uint16_t yellow = matrix.Color(127, 127, 0);
uint16_t black = matrix.Color(0, 0, 0);

const uint16_t colors[] = {red, green, blue, yellow};


void reset ()
{
  matrix.fillScreen (black);
}


int bp1 = 4;
int bp2 = 5;

bool menu;

int ligne;
int colonne;

bool rebond = false;
int direc = 0;


void carre (int taille, uint16_t coul)
{
  reset ();

  for (int i = 0; i < taille * 2; i ++)
  {
    // haut
    ligne = 4 - taille;
    colonne = ligne + i;
    matrix.drawPixel (ligne, colonne, coul);

    // bas
    ligne = 3 + taille;
    matrix.drawPixel (ligne, colonne, coul);

    // gauche
    colonne = 4 - taille;
    ligne = colonne + i;
    matrix.drawPixel (ligne, colonne, coul);

    // droite
    colonne = 3 + taille;
    matrix.drawPixel (ligne, colonne, coul);
  }

  matrix.show ();
}


void bouton_gauche ()
{
  Serial.println ("gauche");
  menu = false;
  
  if (! rebond)
  {
    direc --;
    
    rebond = true;
    delay (100);
    rebond = false;
  }
}

void bouton_droit ()
{
  Serial.println ("droit");
  menu = false;

  if (! rebond)
  {
    direc ++;
    
    rebond = true;
    delay (100);
    rebond = false;
  }
}


void setup ()
{
  Serial.begin (9600);
  
  matrix.begin ();
  matrix.setBrightness (5);
  reset ();
  matrix.show ();

  pinMode (bp1, INPUT_PULLUP);
  attachInterrupt (digitalPinToInterrupt (bp1), bouton_gauche, FALLING);
  pinMode (bp2, INPUT_PULLUP);
  attachInterrupt (digitalPinToInterrupt (bp2), bouton_droit, FALLING);
}


void loop()
{
  reset ();
  menu = true;

  while (menu)
  {

    // 3.2 - Carrés concentriques
    
    for (int taille = 1; taille <= 4; taille ++)
    {
      carre (taille, colors[taille % 3]);
      delay (500);
    }
  
  
    // 4 - Promenade sur un carré
  
    for (int cote = 1; cote <= 4; cote ++)
    {
      for (int i = 0; i <= 6; i ++)
      {
        reset ();
        switch (cote)
        {
          
          // haut
          case 1:
          
            ligne = 0;
            colonne = i;
            break;
  
          // droite
          case 2:
  
            ligne = i;
            colonne = 7;
            break;
  
          // bas
          case 3:
  
            ligne = 7;
            colonne = 7 - i;
            break;
  
          // gauche
          case 4:
  
            ligne = 7 - i;
            colonne = 0;
            break;
        }
  
        matrix.drawPixel (ligne, colonne, colors[cote % 4]);
        matrix.show ();
        delay (100);
      }
    }
    
  }


  // 5 - Snake

  // 5.1 - Fonctionnement de base

  // gauche
  ligne = 3;
  colonne = 0;

  while (true)
  {
    reset ();
    
    switch (direc)
    {
      // droite
      case 0:

        colonne ++;
        break;

      // bas
      case 1:

        ligne ++;
        break;

      // gauche
      case 2:

        colonne --;
        break;

      // haut
      case 3:

        ligne --;
        break;
    }

    // Snake touche un mur
    if (ligne < 0 || ligne > 7 || colonne < 0 || colonne > 7)
    {
      matrix.fillScreen (red);
      break;
    }

    matrix.drawPixel (ligne, colonne, red);
    matrix.show ();
    delay (500);
  }
}
