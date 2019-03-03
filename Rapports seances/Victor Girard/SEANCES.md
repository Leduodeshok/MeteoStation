///////////////////////////////////////////////////
//////////////VICTOR GIRARD////////////////////////
///////////////////////////////////////////////////


COMPTE RENDU SEANCE 1 (14/12/2018):
==

Premiere seance nous avons donc etabli les grandes lignes du projet:
  une station meteo connectée qui nous permet d'afficher sur un ecran la meteo local 
  ou d'une ville via le passage d'une api.

Pour l'affichage nous utilisons un ecran lcd 128x64 pixels, a la base nous voulions le controlé via un 
controlleur I2C cependant n'ayant pas trouvé comment utilisé l'I2C nous nous rabatons vers une librairie 
la U8Glib qui nous permet de faire a peu pres tous ce qui est necessaire a l'affichage sur l'ecran.
Contrairement a un ecran a caractere (ex: 16x2) on a beaucoup plus de liberté dans les affichage possible 
(creer des blocks,les placer ou l'on veut ).

Cette seance etait donc principalement acces sur le fonctionnement de la librairie UG8 grace a une page trouvé sur internet.
http://www.zonnepanelen.wouterlood.com/an-128x64-graphic-lcd-display-with-st7920-controller-for-the-arduino-displaying-temperature-and-relative-humidity/
celui ci reprend les grandes lignes de son code sous forme de texte, que j'ai adapté a l'utilisation des librairies DHT pour pouvoir afficher en live temperature et humitidée que je recupere en temps reel grace a la sonde DHT11(temperature+pression). Les librairies DHT prennent en charges le fais que toutes les infos passent sur un seul fil via du onewire. on recoit donc des "bus" de data.
L'interet d'utiliser le meme branchement que sur la page web ci dessus, c'est qu'on limite le nombre de fils branchés a l'arduino, ici 3 suffisent pour l'ecran et 1 pour le capteur de temperature (plus 5v et GND pour les deux).

J'ai aussi commencé a me panché sur la connection au wifi via l'ESP cependant il est necessaire d'avoir une wifi avec mot de passe classique et non une page de connection (comme le unice hotspot). Il faut donc que j'approfondisse comment creer une communication entre l'ESP et l'arduino.


COMPTE RENDU SEANCE 2 (18/12/2018):
==

Durant cette seance je me suis occuper de poursuivre ma partie sur la recuperation de la meteo sur internet.

Afin de pouvoir récupérer les informations de météo de n'importe quelle ville du monde nous allons passer par une API. Lorsqu'on utilise son ordinateur, pour avoir des infos on cherche sur un site, qui a en général une interface graphique, cependant son on veut automatiser la recherche et récupération d'info sur tel ou tel sujet (en l'occurrence ici la meteo d'une ville) un programme ne parvient pas a interagir avec l'interface graphique. Dans ce cas la on passe par une API, elle permet de recuperer des informations complètes par le biais de requêtes HTTP/URL simple.
Dans notre cas nous allons utilisé un site compatible API, il s'agit de https://openweathermap.org/.
Apres avoir créer un compte le site vous attribue une (ou plusieurs) "clé" , cette clé va vous permettre de faire les requetes que vous voulez. Avec un compte gratuit nous sommes tout de même limité :
-60 requetes par minutes max (largement suffisent a notre échelle)
-accès a la meteo du moment, ou au prévision sur 5 jours
-rafraichissement des données toutes les 2 heures max

Prenons un exemple de requête URL, si on veut la meteo actuelle a Nice;
http://api.openweathermap.org/data/2.5/weather?q=06000,fr&appid=0494f3cb8bbf30444e7a6042419a7068

Cette requête nous renvoi un texte en JSON :
{"coord":{"lon":7.27,"lat":43.7},"weather":[{"id":500,"main":"Rain","description":"light rain","icon":"10n"}],"base":"stations","main":{"temp":276.203,"pressure":949.79,"humidity":90,"temp_min":276.203,"temp_max":276.203,"sea_level":1027.42,"grnd_level":949.79},"wind":{"speed":1.67,"deg":20.0002},"rain":{"3h":2.49},"clouds":{"all":92},"dt":1544993336,"sys":{"message":0.0021,"country":"FR","sunrise":1544943528,"sunset":1544975700},"id":6454924,"name":"Nice","cod":200}

Qu'il faut ensuite parser, c’est-à-dire, découper de tel sorte a ne recuperer que les infos dont nous avons besoin.

Une fois cela realiser il a fallut travailler sur la communication entre la carte arduino est l'ESP via du RX/TX entre ces deux cartes. Pour que cela soit possible il faut passer par un pont diviseur de tension sur la sortie TX de l'arduino afin d'abaisser la tension max de 5v a une tension de 3,3v pour eviter de griller l'ESP qui fonctionne, elle, en 3,3v.



COMPTE RENDU SEANCE 3 (11/01/19):
==

Seance un peu compliqué..

Avec mon binome nous avons bien avancé durant les semaines precedentes, de ce fait nous avons reussi a savoir comment fonctionnais chacun des capteur dont nous disposons afin de pouvoir mesurer la meteo locale (ayant personnellement traité la meteo en ligne dans le rapport de seance precedent). Vient donc l'etape delicate d'incorporer ces magnifique mesure a l'ecran 128x64 fournit, et la les choses ce compliquent...

Pour nous faciliter la tacher de coder cet ecran nous avons decider de passer par une librairie genial, LCDmenulib2 (plus complette que la LCDmenulib1, logique). Lorsqu'on charge l'exemple de cette librairie compatible avec notre ecran on se retrouve face a une page avec 6 onglet;
-LCDML_u8glib: le u8glib permet de gerer l'ecran donc tout ce qui touche 
      a l'affichage de ligne, carré, texte,.... Dans cette partie on doit dans un premier temps entrer les n° des pins utilisés pour l'ecran sur l'arduino, ainsi que creer une "liste" qui permet l'affichage des menu et leur indentation, sous forme "d'ardre". On ajoute donc couche par couche les different item de menu
      LCDML_add         (0  , LCDML_0         , 1  , "Meteo locale"     , mFunc_tem);       // this menu function can be found on "LCDML_display_menuFunction" tab
        LCDML_add         (1  , LCDML_0_1       , 1  , "Retour"           , mFunc_back);
        LCDML_add         (2  , LCDML_0         , 2  , "Meteo en Ligne"   , NULL); 
      LCDML_add         (3  , LCDML_0_2       , 1  , "CHANGER"          , NULL);
        LCDML_addAdvanced (5  , LCDML_0_2_1     , 1  , "Code postale"     , 
        LCDML_add         (4  , LCDML_0_2       , 2  , "Retour"           , mFunc_back);
        cette liste permet d'afficher le menu de la photo ECRANMENU1_SEANCE3
        avec comme premiere couche meteo locale et meteo en ligne (LCDML_0)

-LCDML_condition; on ne modifie rien ici, il s'agit d'une page de la  
      librairie qui permet de dire si oui ou non un item est affiché a l'ecran.

-LCDML_control; permet de controler de differente maniere la navigation,on se contente de mettre*#define _LCDML_CONTROL_cfg      3*
      qui dit que nous allons controler le menu avec un encodeur rotetif, ce menu declare donc que tourner vers la droite equivaut a aller en haut, vers la gauche a aller en bas, qu'un clique cours correspond a entrer, et qu'un clique long (+ de 800ms) correspond a retour.

-LCDML_display_dynFunction; Menu qui gere la creation des fonction a 
      affichage dynamique , on s'en servira pour l'affichage du choix du code postale. Mais il me faudra encore un peu de temps pour comprendre le bon fonctionnement de cette partie.

-LCDML_display_menu; On ne change rien ici, il s'agit de la partie qui 
      gere l'affichage.

-LCDML_display_menuFunction; Ici on peut implanter les fonctions pour les       different capteur par exemple. 

Dans la seance d'aujourd'hui c'est ce dernier onglet que j'ai modifier pour y implanter une fonction qui recuprer l'humidité et la temperature via le capteur DHT11 pour l'afficher sur l'ecran lorsqu'on est sur le menu principale et qu'on clique sur meteo locale. Le resultat est plutot satisfaisant; les deux données s'actualisent bien en temps reel et les données sont correctes (cf. photo ECRANMENU2_SEANCE3).

Puisque l'affichage est une chaine de caractere dynamique j'utilise la fonction sprintf;

//on recupere la temperature comme un entier
dht.temperature().getEvent(&event);
int temp=event.temperature;

//on recupere l'humidité domme un entier
dht.humidity().getEvent(&event);
int humi=event.relative_humidity;

//on declare deux chaine de caractere d'une longueur maximale de 20 caractere, pour l'humidité et la temperature
char hum[20];
char tem[20];

//sprintf acualise en temps reel les chaine de caractere en remplacant le %d par temp ou humi respectivement dans les chaine tem et hum.
sprintf (tem, "Temperature: %d *C", temp);
sprintf (hum, "Humidite: %d prcts", humi);*

par la suite on a plus qu'a affiché chacune de ces chaine de caractere a x=0 et y=h\*nb de ligne 

 u8g.firstPage();
*do {
  u8g.drawStr( 0, (_LCDML_DISP_font_h * 1),tem);
  u8g.drawStr( 0, (_LCDML_DISP_font_h * 2), hum);
 } while( u8g.nextPage() );*




COMPTE RENDU SEANCE 4 (18/01/19):
==

Aujourd'hui encore des difficultés..
Avec la petite presentation en vu je me suis dis la veille, tient il faudrait que je pense a verifier si le programme pour l'ESP fonctionne bien . Quelle idee genial puisqu'il s'avere que notre premier ESP etait completement buguer est qu'il fonctionnait quand il avait envie. Je me suis donc atteler au debugage de ce dernier sans grand succes et malgré le nouvel ESP, pas de resultats probans nomplu, surement du a des erreurs dans mon programme qui marchait parfaitement il y a de ca la 1ere seance. Je vais revenir dessu durant la semaine prochaine pour tenter de resoudre ce probleme qui pourait s'averer tres enuyeux puisque L'ESP represente tout de meme la moitiée de notre projet.
Apres m'etre battu avec l'ESP pendant bien 1h30 et ayant eu un semblant d'amelioration je suis passer a l'etude de l'affichage sur l'ecran 128x64 et encore la de grosses difficultés, notament dans la definition de fonction qui gererons la recuperation des infos locale et en ligne (via SoftwareSerial, dans le meme principe qu'avec le bluetooth).

En clair, une seance qui n'a pas beaucoup avancé de mon coté, heureusement que benjamin a lui terminer la modelisations de nos boitier!



COMPTE RENDU SEANCE 5 (06/02/19):
==


Pour notre projet et notamment sur la partie meteo en ligne j'aimerai que l'on puisse choisir la ville via un menu déroulant. Par chance la librairie que nous utilisons propose une fonctionnalité qui me permetterai de faire cela !

Pour l'affichage d'un element sur l'ecran cela fonctionne par couche de menu le code ce presente de la sorte:

 
    LCDML_add         (0  , LCDML_0         , 1  , "Meteo locale"     , mFunc_tem);      
    LCDML_add         (1  , LCDML_0_1       , 1  , "Retour"           , mFunc_back);
    LCDML_add         (2  , LCDML_0         , 2  , "Meteo en Ligne"   , NULL); 
    LCDML_add         (3  , LCDML_0_2       , 1  , "CHANGER"          , NULL);
    LCDML_addAdvanced (4  , LCDML_0_2_1     , 1  , NULL,         ""   , mCod_post, 0, _LCDML_TYPE_dynParam);
    LCDML_add         (5  , LCDML_0_2_1     , 2  , "Retour"           , mFunc_back);
    LCDML_add         (6  , LCDML_0_2       , 3  , "Retour"           , mFunc_back);


Le premier parametre est juste le n° global de l'element, le 2eme est la couche precedente (LCDML_0 etant le root), le 3eme est le n° de l'element sur sa couche : par exemple on a Meteo locale (id global:0/id local:1) qui est sur la premiere couche avec Meteo en ligne (id global:2/id local:2). ces principes d'ID permettent d'attribuer un sous menu a un element en particulier, par exemple CHANGER est un element du sous menu de Meteo en Ligne, donc sa couche précédente est bien LCDML_0_2 (2 étant l'ID local de meteo en ligne).

Pour permettre de creer un menu defilant on utilise LCDML_addAdvenced qui permettent d'ajouter des fonction a affichage dynamique sur un element du menu.
Le code d'une telle fonction ressemble a ca, sachant que la le fonctionne sur le plan de l'affichage mais je n'arrive pas encore a le faire sauvgarder le nouveau code postale.

  void mCod_post(uint8_t line){
  //on initialise les variables necessaires pour l'affichage du message
    String message = "";
    String Ville = "Nice"; 
    String CP = "";


    //on initialise les variables pour le curseur
    int pos=0;
    int enc=0;


    //verifie si le curseur est bien sur la ligne a contenu dynamique
    if(line==LCDML.MENU_getCursorPos()){
  
    //si on est sur la ligne on verifie s'il y a toute interaction avec un des boutons
    if(LCDML.BT_checkAny()){

    //verifie s'il y a une interaction avec le bouton entrer(click sur l'encodeur)
      if(LCDML.BT_checkEnter()){
        //dans ce cas la on inverse l'etat du scroll de l'ecran (s'il etait activé on le desactive et inversement) cela permet de rester sur la 
        //meme ligne pour interagir maintenant avec la fonction dynamique
          if(LCDML.MENU_getScrollDisableStatus()==0){
            LCDML.MENU_disScroll();
          }
          else{
            LCDML.MENU_enScroll();
          }
          //on reset enter
          LCDML.BT_resetEnter();
        }
      //verifie si on tourne l'encodeur vers la droite

        if(LCDML.BT_checkUp()){
          if(enc<10){
            enc++;
          }
          else{
          enc=0;
          }
          if(enc==0){
            Ville="Nice";
            CP="06600";
          }

          if(enc==1){
            Ville="Paris";
            CP="";
          }
        }
        //on reset up
          LCDML.BT_resetUp();
          }
          
          //on verifie si on tourne l'encodeur vers la gauche (ici pour l'instant ne sert a rien)
        if(LCDML.BT_checkDown()){
          pos++;
          if(pos>4){
            pos=0;
          }
          LCDML.BT_resetDown();
        }
        
      }
    
    //on cree une chaine de caractere dynamique qui change selon la position de l'encodeur et donc de la ville selectionner
    char codpo[20];
    message = "CodPost: " + Ville;
    message.toCharArray(codpo, sizeof(codpo) - 1);
    char eff[20]="CodPost: ";
    
    //on fini par afficher au bon endroit le message variant
    u8g.drawStr( _LCDML_DISP_box_x0+_LCDML_DISP_font_w + _LCDML_DISP_cur_space_behind,  (_LCDML_DISP_font_h * (1+line)), codpo);
    u8g.drawStr( _LCDML_DISP_box_x0+_LCDML_DISP_font_w + _LCDML_DISP_cur_space_behind,  (_LCDML_DISP_font_h * (1+line)), eff);
    
    
}



COMPTE RENDU SEANCE 6 (13/02/19)(en retard):
==

Cette seance a ete principalement consacrer a la discussion entre les differentes cartes de notre projet.
Afin de nous "simplifier" la vie et surtout par manque de place sur une seule carte arduino nous avons decider de decouper notre projet en petits modules suivants:

-l'ESP ; qui s'occupe de recuperer la meteo en ligne qui revient sous la forme d'une chaine JSON (cf. CR seance 2).
-Une carte **(1)** arduino qui s'occupe de recuperer la meteo locale, grace a nos instruments de mesure (vitesse/direction du vent, humidité, temperature,).
-Une carte **(2)** arduino pour gerer l'affichage de toutes les infos, celle ci gere aussi la creation/disposition des menu (cf. CR seance 5).


pour que les cartes puissent discuter entre elles on utilise la librairie SoftwareSerial, compatible arduino nano et ESP32, donc ideal ici.
Cette librairie fonctionne de la meme maniere que le lorsqu'on utilise le port serie de l'ordi. Pour faire dans les grandes lignes,sur l'esp est la carte **1** on cree un nouveau port serie que l'on nomme comme on veut *(on l'appelera ard. ici)*, on l'initialise, avec ses port RX et TX dediés.

le code s'articule comme suit :

    //on inclue la bibliotheque necessaire a la discussion entre les boards
    #include<SoftwareSerial.h>

    //on utilise des define ici car plus precis et ne prenans pas de place sur la carte surtout pour des constates qui ne sont pas utilisé dans la loop mais juste pour l'initialisation de la lib
    #define RX 7
    #define TX 6

    //on "cree" un port serie qu'on appel esp avec RX pin 7 et TX pin 6
    SoftwareSerial esp(RX,TX);

    //variables utilisé dans le programme
    String Ville; 
    String Temp;
    String Hum;
    String Vent;
    String VentDir;

    void setup() {
      //on lance le port serie a 9600 bauds, pas besoin de plus, pour afficher sur l'ordinateur les infos recu par l'arduino de l'esp
      Serial.begin(9600);
      
      //on lance le "port serie" virtuelle pour que l'ard. et l'ESP puisse communiquer, idem, 9600 bauds suffisent amplement vu que l'on rafraichit uniquement 1f/sec
      esp.begin(9600);
    }

    void loop() {

      //dans la boucle je compare juste chaque caractere pour verifier s'il s'agit d'unz nouvelle info envoyer au format maj+info tel que :Ainfo.
      char c;
      int Read=0;
      boolean first_char =true;
      String s = "";
      while (esp.available()) {
        c = char(esp.read());
        if(Read!=0){
        s=s+String(c);
        }
        if (first_char){
          
          if(c=='C'){
            Read=1;
          }
          if(c=='T'){
            Read=2;
          }
          if(c=='H'){
            Read=3;
          }
          if(c=='V'){
            Read=4;
          }
          if(c=='D'){
            Read=5;
          }
          first_char=false;
        }
        
      }

        switch(Read){
        case 1:Ville=s;   break;
        case 2:Temp=s;    break;
        case 3:Hum=s;     break;
        case 4:Vent=s;    break;
        case 5:VentDir=s; break;
      }
      Serial.print(Ville);
      Serial.print(Temp);
      Serial.print(Hum);
      Serial.print(Vent);
      Serial.print(VentDir);
      
      
     }






COMPTE RENDU SEANCE 7 (27/02/19):
==

Objectif du jour, debugger l'ESP qui fait des siennes.
Je reprend le code de la semaine derniere. du coté de l'ESP on prend le code de pars au quel on ajoute la librairie SoftwareSerial, compatible avec les deux cartes.
Dans le setup on demarre le port serie, on l'appelera ici ard, Ard.begin(9600); cette vitesse est largement suffisante, et puis il faut que l'on mette la meme que du coté de l'arduino.
A la fin du code de l'esp on rajoute les lignes suivante (dans la loop):

    ard.println("C"+location);
    ard.println("T"+String(temperature));
    ard.println("H"+String(humidity));
    ard.println("V"+String(wind));
    ard.println("D"+String(windDir));
  
qui permet d'envoyer les informations qu'on a selectionné, entre autre ville, temperature, humidité, direction et force du vent.

Après quelque ajustement l'esp me fait des siennes, et decide de ne plus fonctionner, a voir dans la semaine si le probleme est du coté du code ou encore du coté materiel.


Je passe à la comunication Arduino/Arduino, afin de discuter entre la carte de l'ecran et celle des instruments de mesures. C'est le meme principe que du coté arduino/esp, de plus, l'arduino est bien moins capricieuse que l'esp !
le code est donc tres similaire, on mesure ce que l'on veut puis on envoi les info précédé d'un caractere precis.

Nous avons decidé de relié nos deux boitier grace a un cable RJ45(cable internet), l'interet ?

-ayant un boitier a l'exterieur et un a l'interieur, il est necessaire d'avoir un cable (long si possible), comportant 4 fils independant au moins pour la comunication entre les deux cartes (RX,TX), et l'alimentation de la deuxieme carte (GND,5V). un cable RJ45 possede 8 fils à l'interieur.

-question de modularité/simplicité .A l'interieur sur le boitier de l'ecran, on installe une prise RJ45 recuperer sur une ancienne livebox et ressoudé sur un petit pcb avec des pin male. ce qui nous permetera d'installer les boitier plus simplement.

-question d'esthetique.


A faire pour la prochaine seance ; 
-
      
      reussir enfin la discussion entre les cartes arduino/esp et arduino/arduino, et entamer l'interface final.


