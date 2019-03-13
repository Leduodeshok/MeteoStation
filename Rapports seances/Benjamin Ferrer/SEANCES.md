BENJAMIN FERRER
==

COMPTE RENDU SEANCE 1 (14/12/18):
--

Debut de séance un peu de recherche pour voir ce que la communauté a produit comme station météo cela nous a donné quelques idées
comme le format des menus.

Pour le début je me suis concentré sur le rotary encoder qui va nous permettre de nous balader dans les différents menus et ainsi pouvoir
selectioner les villes dans lesquels on affichera la météo (et toutes autres informations complementaires)

		#define encoder0PinA 2 // les Pin 2 et 3 sont important car sinon la fonction attachInterrupt ne marche pas 
		#define encoder0PinB 3
		#define encoder0Btn 4
		int encoder0Pos = 0;

		void setup() {
		  Serial.begin(9600);
		  pinMode(encoder0PinA, INPUT_PULLUP);
		  pinMode(encoder0PinB, INPUT_PULLUP);
		  pinMode(encoder0Btn, INPUT_PULLUP);
		  attachInterrupt(0, doEncoder, CHANGE);
		}
		int valRotary,lastValRotary;

		void loop() {
		  int btn = digitalRead(encoder0Btn);
		  Serial.print(btn);
		  Serial.print(" ");
		  Serial.print(valRotary);
		  
		  if(valRotary>lastValRotary){ // sens des aiguilles d'une montre
		    Serial.print("  CW");
		  }
		  
		  if(valRotary<lastValRotary){ // sens inverse 
		    Serial.print("  CCW");
		  }
		  
		  lastValRotary = valRotary;
		  Serial.println(" ");
		  delay(100);
		}
		void doEncoder(){
		  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)){
		    encoder0Pos++;
		  }
		  else{
		    encoder0Pos--;
		  }
		  valRotary = encoder0Pos/10; //le fait de diviser le encoder0Pos permet de faire derouler plus vite l'encodeur
		}                             //donc plus le chiffre est petit plus il sera vite

		J'ai donc fini de faire le rotary encoder et j'ai compris les principales fonctions (le bouton et les sens de rotation entre aiguille d'une montre ou inverse ) cela nous sera utile pour naviguer dans nos futurs menu.

		Pour le menu on va se servir d'une petite fleche qui va nous permettre de se diriger dans les differents menu 
		Et pour l'instant j'ai convertit en hexadecimal des nombre binaire pour que grace a la fonction Bitmap la fleche apparait sur l'écran lcd 

		00000000 --> 0x00
		00010000 --> 0x10
		00011000 --> 0x18
		00011100 --> 0x1C             et donc sur l'ecran lcd cela va afficher une fleche pleine 
		00011110 --> 0x1E
		00011100 --> 0x1C
		00011000 --> 0x18
		00010000 --> 0x10

La fin de séance se resumera a de la documentation sur la librairie u8glib pour creer des menus 


COMPTE RENDU SEANCE 2 (18/12/18):
--

Mon objectif de séance sera de rendre L'anémomètre et la girouette fonctiels

Tout d'abord de la documentation sur comment fonctionne des Modules Grove car l'anémomètre fonctionne avec celui et des connecteurs RJ11
Je trouve un [site](https://www.lextronic.fr/temperature-meteo/19999-capteur-anenometre.html "le susdit site").
M'expliquant le montage et pour faire le test (Eteindre la diode quand l'anémomètre tourne) [vidéo](https://youtu.be/Ru0aI_2J2dI "la susdite video") montrant que cela fonctionne : 
J'ai un peu modifier le code car ne fonctionner pas bien aus debut 

### Code pour allumer la diode : # 

		void setup() {
		  pinMode(4, OUTPUT);
		  pinMode(2, INPUT);
		  Serial.begin(9600);
		}
		// Boucle principale:
		void loop() {

		  int BP = digitalRead(2); // Lecture du capteur

		  if (BP == LOW) {
		    digitalWrite(4, HIGH); // Allume la Led

		  }
		  else {
		    digitalWrite(4, LOW); // Eteind la Led

		  }
		}

Je me suis ensuite consacré a la girouette une ancienne élève m'a expliquer son fonctionnement 
et j'ai ensuite reussi a la faire fonctionner et ajuster suivant les valeurs que recevait l'arduino pour que le programme 
nous renvoi les points cardinaux.

### Code de la girouette : # 

		void setup() {
		  Serial.begin(9600);
		}
		void loop() {
		  int sensorValue = analogRead(A1);
		  Serial.print("La direction du vent est :");
		  if(sensorValue>200 && sensorValue<300){
		    Serial.print("SUD " );
		    Serial.println(sensorValue);
		  }
		  else if(sensorValue>100 && sensorValue<200){
		    Serial.print("SUD-EST ");
		    Serial.println(sensorValue);
		  }
		  else if(sensorValue>10 && sensorValue<100){
		    Serial.print("EST ");
		    Serial.println(sensorValue);
		  }
		  else if(sensorValue>400 && sensorValue<500){
		    Serial.print("NORD EST ");
		    Serial.println(sensorValue);
		  }
		  else if(sensorValue>750 && sensorValue<800){
		    Serial.print("NORD ");
		    Serial.println(sensorValue);
		  }
		  else if(sensorValue>800 && sensorValue<900){
		    Serial.print("NORD-OUEST");
		    Serial.println(sensorValue);
		  }
		  else if(sensorValue>900 && sensorValue<1023){
		    Serial.print("OUEST");
		    Serial.println(sensorValue);
		  }
		  else if(sensorValue>600 && sensorValue<700){
		    Serial.print("SUD-OUEST ");
		    Serial.println(sensorValue);
		  }
		  //Serial.println(sensorValue);
		  delay(100); 
		}

Je me suis ensuite consacré de nouveau a l'anémomètre pour que maintenant il m'affiche la vitesse du vent je me suis aidé d'un donnateur
allemand son [site](http://www.aeq-web.com/?ref=arduinoide "deutsh kalität").


### Code pour l'anémomètre : #

		const int m_time = 1;  
		int wind_ct = 0;
		float wind = 0.0;
		unsigned long time = 0;

		void setup()
		{
		  Serial.begin(9600);
		  time = millis();
		}
		void loop()
		{
		  meassure();
		  Serial.print("VITESSE DU VENT ");
		  Serial.print(wind);       //Speed in Km/h
		  Serial.print(" km/h - ");
		  Serial.print(wind / 3.6); //Speed in m/s
		  Serial.println(" m/s");
		}
		void countWind() {
		  wind_ct ++;
		}
		void meassure() {
		  wind_ct = 0;
		  time = millis();
		  attachInterrupt(1, countWind, RISING);
		  delay(1000 * m_time);
		  detachInterrupt(1);
		  wind = (float)wind_ct / (float)m_time * 2.4/3;
		}

Cela fonctionnement parfaitement j'ai juste du modifier la dernier ligne, les valeurs étaient biaisés car compté triple.
Comme l'anémomètrea possède 3 branches et que l'on utilise la fonction attachInterrupt l'arduino compté le resultat par 3.
J'ai donc divisé le resultat par 3. 

Pour finir j'ai regroupé les 2 codes et le montage pour que cela soit plus propre.
![image](https://github.com/Leduodeshok/MeteoStation/blob/rapports-seances/Rapports%20seances/Benjamin%20Ferrer/18.12.2018/IMG_8142.JPG)

### Code final : #

		const int m_time = 1;  
		int wind_ct = 0;
		float wind = 0.0;
		unsigned long time = 0;

		void setup() {
		  Serial.begin(9600);
		  time = millis();
		}

		void loop() {
		  meassure();
		  girouette();
		  delay(100);
		}

		void countWind() {
		  wind_ct ++;
		}

		void meassure() {
		  wind_ct = 0;
		  time = millis();
		  attachInterrupt(1, countWind, RISING);
		  delay(1000 * m_time);
		  detachInterrupt(1);
		  wind = (float)wind_ct / (float)m_time * 2.4/3;
		  Serial.print("VITESSE DU VENT ");
		  Serial.print(wind);       //Speed in Km/h
		  Serial.print(" km/h - ");
		  Serial.print(wind / 3.6); //Speed in m/s
		  Serial.println(" m/s");
		}

		void girouette(){
		  int sensorValue = analogRead(A1);
		  Serial.print("La direction du vent est :");
		  if(sensorValue>200 && sensorValue<300){
		    Serial.print("SUD " );
		    Serial.println(sensorValue);
		  }
		  else if(sensorValue>100 && sensorValue<200){
		    Serial.print("SUD-EST ");
		    Serial.println(sensorValue);
		  }
		  else if(sensorValue>10 && sensorValue<100){
		    Serial.print("EST ");
		    Serial.println(sensorValue);
		  }
		  else if(sensorValue>400 && sensorValue<500){
		    Serial.print("NORD EST ");
		    Serial.println(sensorValue);
		  }
		  else if(sensorValue>750 && sensorValue<800){
		    Serial.print("NORD ");
		    Serial.println(sensorValue);
		  }
		  else if(sensorValue>800 && sensorValue<900){
		    Serial.print("NORD-OUEST");
		    Serial.println(sensorValue);
		  }
		  else if(sensorValue>900 && sensorValue<1023){
		    Serial.print("OUEST");
		    Serial.println(sensorValue);
		  }
		  else if(sensorValue>600 && sensorValue<700){
		    Serial.print("SUD-OUEST ");
		    Serial.println(sensorValue);
		  }
		}

Tout marche plutot bien, il reste quand meme a faire des ajustements sur la girouette car peu precise sur les directions.
Et quand on debranche puis rebranche la carte les valeurs ont tendance a se renitialiser.
Ce qui va changer changer les directions.

### Ecriture post seance: #

Pour les indications Nord/EST ...de la girouette.
le principe est que la girouette renvoie une plage de valeur ( c-a-d : quand elle est dans une certaine position elle renvoie des valeurs 
entre 60-80 c'est un exemple)
donc j'ai positionné la girouette dans les 8 positions de la boussole en prenant un Nord arbitraire comme la girouette est analogique cela 
renvoie en 0 et 1023 donc pour les differentes directions j'ai choisie une plage de 100 valeurs.
Le seul probleme de cette pratique c'est que la girouette est dependante de la position initial.




COMPTE RENDU SENACE 3 (11/01/19):
--

Pour cette séance j'avais pour objectif de rendre tous les instrument météorologique extérieur fonctionnels ensemble sur une même carte.   Le probleme etant que nous n'avons pas recu le pluviomètre. J'ai donc du me contenté de l'anémometre, de la girouette et du capteur humidité et temperature. 

Le code etant ci dessus j'ai du rajouter une delay de 1000ms dans la fonction principale pour laisser le sensor de temps de tout faire.


		void humidity() {
		  sensors_event_t event;  
		  dht.temperature().getEvent(&event);
		  Serial.print("Temperature: ");
		  Serial.print(event.temperature);
		  Serial.println(" *C");
		  dht.humidity().getEvent(&event);
		  Serial.print("Humidity: ");
		  Serial.print(event.relative_humidity);
		  Serial.println("%");
		}

et le montage est classique donc pas de difficulté particuliere
Ainsi j'etais en charge de faire des rendu 3D des 2 boitiers celui interieur (Ecran + encodeur) et le boitier exterieur (tout les instrument : girouette, anémomètre, pluviomètre et capteur pression/temperature).  

J'ai donc fait de multiples mesures (dans le dossier mesures) pour avoir un rendu d'une bonne qualité et dans les bonnes dimensions mais suite un probleme de sauvegarde je n'ai que le rendu 3D du boitier interieur.  


Pour le rendu les cavité sont pour montrer ou va se placer les differents instrument
le grand rectangle sera l'ecran LCD 
le trou sera l'endroit ou l'on place l'encodeur 
suite sur le cote arrière il y a 2 petits trou c'est pour faire passer les cables qui reliront les 2 cartes arduino pour qu'elle puisse 
echanger entre elle.




COMPTE RENDU SEANCE 4 (18/01/19):
--

Durant cette séance mon but a été de concevoir les plans pour nos 2 boites (l'interieur et l'exterieur) Nous avons décidé de faire une découpe laser cela était plus interresant d'un point vue temps et économique.  
J'ai donc utilisé le logiciel "Inkscape". J'ai mit beaucoup de temps a faire les schèmas car je devais apprendre comment fonctionne le logiciel et de ce fait durant mes test j'ai fait pas mal d'erreur (probleme de convertion d'unité ect. ) qui ont largement retardé ce que je voulais faire.  
De plus nous voulions que notre boite interieur possede un angle non droit. De ce fait tous les extentions du logiciel que j'avais installé et apprit a m'en servir ne servais pas. J'ai donc du un peu tricher en faisant 2 boites et de prendre seulement ce qui m'interresse.  

Finalement j'ai reussi a faire l'esquisse de nos 2 boites. Il ne manque plus que de faire decouper au laser les planches et de les assembler (le document des esquisse se situe dans le dossiers "Boite" ).

Ensuite en fin de séance je ne me suis concentré qu'a optimiser mon programme et de pouvoir l'afficher tous les données sur l'ordinateur en vu de l'oral mais aussi de pouvoir switch rapidement dans un modèle plus compacte que j'enverrai a la carte mère qui va gérer l'affichage des données sur l'écran LCD.





COMPTE RENDU SEANCE 5 (06/02/19):
--

j'avais pour objectif durant cette séance de m'occuper de la boite donc aller au fablab malheureusement étant fermer au momnet ou j'y suis allé, j'ai donc changé de cap et décidé d'aider mon collegue sur l'aspect technique, et il est resté sur interface. je me suis occupé de l'aspect "communication entre les cartes".  
Le hic étant que durant toute la séance je me suis principalement documenté sur le module esp32 et j'ai pas fait grand chose.  
J'ai effectué quelques tests trouvé sur internet pour comprendre le principe, et j'ai mit pas mal de temps a comprendre car certains programme de test était bugé et j'ai du les corrigé sauf que le temps de trouver ou se situe l'erreur et comment la corriger sur des notions que je viens d'apprendre ont fait cette séance n'a pas été fructueuse en terme de contenu physique mais plus intellectuelle. 


### Update : #

Normalement nous allons recevoir le pluviomètre avant mercredi je vais pouvoir m'occuper de celui-ci et je retournerai au fablab pour enfin 
avoir nos boites et faire les ajustements si necessaires. Tant dis que pour l'esp32 Victor a continué de son coté ayant plus de notions que 
moi la dessus.




COMPTE RENDU SEANCE 6 (13/02/19):
--

Pour cette séance je me suis occupé de la connexion de entre les deux cartes. J'ai donc passé du temps (trop) a soudé le cable qui va relier les instruments xterieurs et la carte qui va afficher les infos sur l'écran. je devais donc "trafiquer" un cable téléphonique qui passe des pings de mon arduino a la platine grove. Ensuite j'ai du modifier mon code des instruments exterieux.  

C'est a dire, on va passer par la biblothèque Software serial pour faire passer toutes les données puis ensuite pour que la carte comprend que ces données la represent telle type d'instrument. On a decidé d'un peu **"triché"** car avant chaque donnée on envoie une lettre qui va indiquer a quoi la suite correspond. 
>Un exemple vaut 1000 mots : 


	void meassure() {
	  wind_ct = 0;
	  time = millis();
	  attachInterrupt(1, countWind, RISING);
	  delay(1000 * m_time);
	  detachInterrupt(1);
	  wind = (float)wind_ct / (float)m_time * 2.4/3;
	  ////////////////////////envoie/////////////////////////////////
	  ac.print("C");
	  ac.print(wind);
  

Voila le code de l'anemometre j'ai donc supprimé les Serial print puisque maintenant voire sur l'ordinateur les données n'est plus interesant. En premiere partie j'ai le code dit classique qui va recuperer ce que l'anemometre recois. et pour la deuxieme partie c'est ce que j'envoie par le cable telephonique ac correspond au nom que j'ai donné a mon SoftwareSerial et d'abord on envoie la lettre "C" puis ensuite la varibla "wind" qui est pour le cas present la variable contenant la vitesse du vent. Par contre pour une raison obscure je suis obligé de faire 2  

	ac.print(...)

car si j'envoie les 2 informations en même temps a la reception notre carte ne comprend pas c'est d'ailleurs mon objectif de la séance prochaine. Pour voir si cela ne peut pas nous porter prejudice.



COMPTE RENDU SEANCE 7 (27/02/19):
--

### Objectif du jour: #

Pour moi c'était de terminer 2-3 lignes de code des instruments extérieurs pour que lorsque j'envoie les données Elle sait parfaitement se qu'elle reçoit et 	ainsi clarifier les rapports sans pour autant avoir des problèmes de communication Ensuite ayant totalement finie ma partie, j'ai décidé de préparer notre oral et pour cela j'ai commencé a réorganiser notre github Car il y avait plein d'informations plus utile ou pas au bonne endroit ect. C'était donc un travail rébarbatif et assez long mais néanmois nécéssaire. J'ai donc mit les différents plannings sachant que pour le planning final j'ai du me replonger dans tous nos rapports de séance ne l'ayant pas au fur et a mesure, j'ai mit aussi en ligne notre cahier des charges et j'ai fait quelques consignes que vous stipuler dans votre mail (mettre les differents module independant les uns des l'autres ect.) et au final cela m'a pris plus de temps que prévu. C'est pour cela que ma séance ne ressemble pas a un travail (futur) d'ingénieur cela reste néanmoins nécéssaire et cela m'a appris un truc. C'est de faire ce genre de tache de façon periodique et pas tout d'un coup.


COMPTE RENDU SEANCE 8 (06/03/19):
--

C'est notre derniere séance avant l'oral, pendant que mon camarade tente de resoudre son problème d'esp personnellement je m'occuper 
de peaufiner les derniers details plus particulierement sur les boitiers. Nous avons pris pour le boitier exterieur, une boite hermétique étant donner qu'il y a des composants electroniques s'il pleut cela peut emdommager nos composants mais cela souleve un problème, le capteur humidité / température se situe dans la boite ext. mais en contact avec les autres composants eclectronique (carte arduino ect.) Or principe physique, tout ce materiel chauffe en fonctionnement ce qui a pour consequence de fausser un peu les mesure du capteur de temperature pour cela j'ai appliquer du scotch pour coller le capteur sur une paroi et tenter de faire une petite cage de protection pour minimiser ce probleme de temperature.
J'ai du faire quelques branchements diverses et refaire un peu de soudure pour finaliser tout notre projet
Une fois cela fini je me suis atteler a notre futur oral qui se deroulera la semaine prochaine en commençant par exemple le rapport ecrit qu'il faudra rendre ect.


COMPTE RENDU SEANCE 9 (13/03/2019):
--


Fin du projet, Soutenance a l'oral devant le jury. Merci M.Masson et Nassim  Abderrahmane.




