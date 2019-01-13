///////////////////////////////////////////////////
//////////////VICTOR GIRARD////////////////////////
///////////////////////////////////////////////////



	COMPTE RENDU SEANCE 1 (14/12/2018);


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


	COMPTE RENDU SEANCE 2 (18/12/2018);


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



	COMPTE RENDU SEANCE 3 (11/01/19);

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

-LCDML_control; permet de controler de differente maniere la navigation, 			on se contente de mettre 
			*#define _LCDML_CONTROL_cfg      3*

			qui dit que nous allons controler le menu avec un encodeur rotetif, ce menu declare donc que tourner vers la droite equivaut a aller en haut, vers la gauche a aller en bas, qu'un clique cours correspond a entrer, et qu'un clique long (+ de 800ms) correspond a retour.

-LCDML_display_dynFunction; Menu qui gere la creation des fonction a 
			affichage dynamique , on s'en servira pour l'affichage du choix du code postale. Mais il me faudra encore un peu de temps pour comprendre le bon fonctionnement de cette partie.

-LCDML_display_menu; On ne change rien ici, il s'agit de la partie qui 
			gere l'affichage.

-LCDML_display_menuFunction; Ici on peut implanter les fonctions pour les 			different capteur par exemple. 

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





