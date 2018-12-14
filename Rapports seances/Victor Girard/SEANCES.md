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
