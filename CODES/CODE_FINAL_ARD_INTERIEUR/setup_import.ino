
// *********************************************************************
// INCLUDES
// *********************************************************************

#include <LCDMenuLib2.h>
#include <U8glib.h>


// *********************************************************************
// INITIALISATION DU PORT DE COMMUNICATION
// *********************************************************************

#include <SoftwareSerial.h>
#define RX 8
#define TX 9
SoftwareSerial ard(RX,TX);

#define RX2 10
#define TX2 11
SoftwareSerial esp(RX2,TX2);



// *********************************************************************
// VARIABLES
// *********************************************************************
String choixVille="Nice";
String codePostale="06000";
String Ville; 
String Tempera;
String Humidi;
String VentVi;
String VentDir;
String message;
String s = "";
int k=0;
int port=0;
int enc=0;
float dir;
// *********************************************************************
// INITIALISATION DES SPECS DE L'ECRAN
// *********************************************************************

U8GLIB_ST7920_128X64 u8g(5, 6, 7, U8G_PIN_NONE);


//reglage de la lib et definition de l'ecran
#define _LCDML_DISP_w                 128            // Largeur LCD
#define _LCDML_DISP_h                 64             // hauteur LCD

// definition de la police
#define _LCDML_DISP_font              u8g_font_6x13  // police u8glib 
#define _LCDML_DISP_font_w            6              // largeur police
#define _LCDML_DISP_font_h            13             // hauteur police

// reglage du curseur de selection
#define _LCDML_DISP_cursor_char       ">"            // caractere curseur
#define _LCDML_DISP_cur_space_before  2              // espace entre curseur et bord d'ecran
#define _LCDML_DISP_cur_space_behind  4              // espace entre curseur et texte

// taille et position du menu
#define _LCDML_DISP_box_x0            0              // Point de depart(x0,...)
#define _LCDML_DISP_box_y0            0              // Point de depart(..., y0)
#define _LCDML_DISP_box_x1            128            // largeur en x  (x0 + larg.)
#define _LCDML_DISP_box_y1            64             // hauteur en y  (y0 + haut.)
#define _LCDML_DISP_draw_frame        1              // dessine un rectangle autour du menu

// largeur de la barre de defilement
#define _LCDML_DISP_scrollbar_w       6  

//definition du max de colonnes et max lignes
#define _LCDML_DISP_cols_max          ((_LCDML_DISP_box_x1-_LCDML_DISP_box_x0)/_LCDML_DISP_font_w)
#define _LCDML_DISP_rows_max          ((_LCDML_DISP_box_y1-_LCDML_DISP_box_y0-((_LCDML_DISP_box_y1-_LCDML_DISP_box_y0)/_LCDML_DISP_font_h))/_LCDML_DISP_font_h)

//definition des lignes+colonnes
#define _LCDML_DISP_rows              _LCDML_DISP_rows_max  // max rows
#define _LCDML_DISP_cols              20                   // max cols



// *********************************************************************
// PROTOTYPES
// *********************************************************************

void lcdml_menu_display();
void lcdml_menu_clear();
void lcdml_menu_control();


// *********************************************************************
// OBJETS MENU
// *********************************************************************
LCDMenuLib2_menu LCDML_0 (255, 0, 0, NULL, NULL); // element racine (!!NE PAS RETIRER!!)
LCDMenuLib2 LCDML(LCDML_0, _LCDML_DISP_rows, _LCDML_DISP_cols, lcdml_menu_display, lcdml_menu_clear, lcdml_menu_control);


// *********************************************************************
// MENU / DISPOSITION
// *********************************************************************

// LCDML_0        => couche racine
// LCDML_0_X      => couche 1
// LCDML_0_X_X    => couche 2
// LCDML_0_X_X_X  => couche 3
// LCDML_0_...    => couche ...

//ajout d'un element
// LCDML_add(id, couche_prece,nouvel_id, nom_a_lecran, fonction_du_sous_menu)

//les fonction de sous menu sont dans display_menuFunction
  LCDML_add         (0  , LCDML_0         , 1  , "Meteo locale"     , mFunc_tem);       
  LCDML_add         (1  , LCDML_0_1       , 1  , "Retour"           , mFunc_back);
  LCDML_add         (2  , LCDML_0         , 2  , "Meteo en Ligne"   , NULL); 
  LCDML_add         (3  , LCDML_0_2       , 1  , "Acces meteo"      , mFunc_li);
  LCDML_add         (4  , LCDML_0_2       , 2  , "CHANGER"          , NULL);
  LCDML_addAdvanced (5  , LCDML_0_2_2     , 1  , NULL,         ""   , mCod_post, 0, _LCDML_TYPE_dynParam);
  LCDML_add         (6  , LCDML_0_2_2     , 2  , "Retour"           , mFunc_back);
  LCDML_add         (7  , LCDML_0_2       , 3  , "Retour"           , mFunc_back);
 


//Cette valeur doit etre la meme que la plus grande valeur ID
#define _LCDML_DISP_cnt    7

//Creation/ du menu
LCDML_createMenu(_LCDML_DISP_cnt);



// *********************************************************************
// SETUP GLOBALE
// *********************************************************************

void setup()
{
  //Initialisation du port serie et des ports de communication avec l'ESP et la seconde arduino
  Serial.begin(9600);                
  esp.begin(9600);
  ard.begin(9600);
  //Setup de la lib basé sur le nombre d'elements a afficher 
  LCDML_setup(_LCDML_DISP_cnt);

  //Activation du defilement de l'ecran
  LCDML.MENU_enRollover();

  //Desactivation de l'option screensaver( inactivité de 10sec affiche un message de veille sur l'ecran)
  LCDML.SCREEN_disable();
}

// *********************************************************************
// LOOP
// *********************************************************************
void loop()
{
  //cette fonction est a appeler ici (!!NE PAS RETIRER!!)
  LCDML.loop();
  if(port==0){
    ard.listen();
  }
  else if(port==1){
    esp.listen();
  }
  
  
  }


// *********************************************************************
//VERIFICATION ET CORRECTION EN CAS D ERREUR
// *********************************************************************
# if(_LCDML_glcd_tft_box_x1 > _LCDML_glcd_tft_w)
# error _LCDML_glcd_tft_box_x1 is to big
# endif

# if(_LCDML_glcd_tft_box_y1 > _LCDML_glcd_tft_h)
# error _LCDML_glcd_tft_box_y1 is to big
# endif
