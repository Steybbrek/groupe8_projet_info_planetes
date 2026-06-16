# groupe8_projet_info_planetes

## Avant de tester

#### **Avoir un serveur local**

Nous vous conseillons d'utiliser l'extension VS code [Live Server](https://marketplace.visualstudio.com/items?itemName=ritwickdey.LiveServer) qui est plutôt simple à utiliser.

#### **Le fichier web/methodes.json**

Le fichier web/methodes.json est déjà généré, cependant si vous souhaitez tester avec des paramètres différents, nous vous conseillons d'être sur linux / de l'adapter le fichiers_c/makefile à votre système d'exploitation

---

## Guide simplifié

#### **1 - Changer le fichier fichiers_c/planetes.h**

<pre><code>// bools
#define LOADING_BAR     1                           // Affichage d'une barre de chargement (impact très léger sur les perfs)
#define ENERGIE         0                           // Calcul de l'énergie du système (impact fort sur les perfs)
#define EULER_T         0                           // Euler Terminal
#define EULER           1                           // Euler Interact
#define EULER_ASSYM     1                           // Euler Assymétrique
#define RK4             1                           // RK4

// constantes modifiables / paramètres
#define N_PLANETS                    14             // nombre de planètes / objets
#define PAS_REEL                     300            // fréquence des calculs (sec)
#define PAS_SAUVEGARDE               86400          // fréquence de sauvegarde (sec)
#define PERIODE_ENREGISTREMENT       3652.5         // période de calculs à partir du 10 juin 2026 00:00:00 TDB (jours)
#define MARGE_ENERGIE                1              // marge d'erreur pour l'énergie (en %)</code></pre>

#### **2 - Executer dans le terminal (linux)**

A la racine du projet :
<pre><code>[user]$ cd fichiers_c
[user]$ make planetes
</code></pre>

#### **3 - S'assurer que le serveur local est lancé**

#### **4 - Dans le navigateur**

Aller à http://127.0.0.1:[port]/chemin/vers/racine/web  
(normalement vous arriverez sur la page index.html, si vous voyez le contenu du dossier web, cliquez sur index.html)

#### **5 - Explorez le système solaire**

Différentes options sont visibles en haut à gauche de l'écran.  
Vous pouvez également vous déplacer en maintenant le clique droit et déplaçant la souris.  
Vous pouvez tourner la caméra en maintenant le clique gauche et déplaçant la souris.  
Vous pouvez aussi zoomer ou dézoomer avec la molette.  
En cliquant sur une planète vous pouvez avoir accès à des information supplémentaires sur celle-ci.  
Bonne exploration !

---

## Informations sur les fichiers : 

### web/methodes.json
<pre><code>{
    "planete_methode":[
        [[x,y,z],[vx,vy,vz],0],  
        [[x,y,z],[vx,vy,vz],1],
        ...
        [[x,y,z],[vx,vy,vz],t]
    ],
    ...
}</code></pre>

<p><bold>ATTENTION :</bold> Le fichier json d'exemple possède 2 fois t = 0 par méthode, surement 1 d'initialisation et 1 du premier calcul.  
On a donc supprimé le 1er t=0 pour avoir des données plus adaptées et simplifier les opérations
</p>

---

### fichiers_c/makefile
Nous avons 3 options pour la compilation :   
 - *make planetes* : compile les fichiers c, supprime les fichiers .o, execute le programme et le supprime  
 - *make planetes_noDelete* : make planetes mais conserve le programme compilé  
 - *make vplanetes* : make planetes mais execute avec valgrind  

<p><bold>ATTENTION :</bold>Le makefile a été écrit et adapté pour Linux</p>

 ---

### fichiers_c/planetes.h

Selections d'options et de parametres à changer avant la compilation :
<pre><code>// bools
#define LOADING_BAR     1                           // Affichage d'une barre de chargement (impact très léger sur les perfs)
#define ENERGIE         0                           // Calcul de l'énergie du système (impact fort sur les perfs)
#define EULER_T         0                           // Euler Terminal
#define EULER           1                           // Euler Interact
#define EULER_ASSYM     1                           // Euler Assymétrique
#define RK4             1                           // RK4

// constantes modifiables / paramètres
#define N_PLANETS                    14             // nombre de planètes / objets
#define PAS_REEL                     300            // fréquence des calculs (sec)
#define PAS_SAUVEGARDE               86400          // fréquence de sauvegarde (sec)
#define PERIODE_ENREGISTREMENT       3652.5         // période de calculs à partir du 10 juin 2026 00:00:00 TDB (jours)
#define MARGE_ENERGIE                1              // marge d'erreur pour l'énergie (en %)</code></pre>

Booléens : 
 - **LOADING_BAR** : Barre de chargement
 - **ENERGIE** : Calculs d'énergie et vérification de conservation d'énergie - affiche à la fin du programme les étapes dépassant la MARGE_ENERGIE 
 - **EULER_T** : Affiche dans le terminal dix étapes de la méthode d'Euler sans interactions avec les autres planètes
 - **EULER** : Enregistre dans le fichiers "web/methodes.json" les données calculées par la méthode d'Euler (interactions avec les autres planètes)
 - **EULER_ASSYM** : Enregistre dans le fichiers "web/methodes.json" les données calculées par la méthode d'Euler assymétrique (interactions avec les autres planètes)
 - **RK 4** : Enregistre dans le fichiers "web/methodes.json" les données calculées par la méthode Runge-Kutta 4 (interactions avec les autres planètes)

Paramètres :
 - **N_PLANETS** : Le nombre de planètes
 - **PAS_REEL** : Le pas de calcul, à l'échelle du système (sec)
 - **PAS_SAUVEGARDE** : Le pas entre chaque sauvegarde de valeurs dans le fichier json (sec)
 - **PERIODE_ENREGISTRE** : Période des calculs, à partir du 10 juin 2026 00:00:00 TDB (en jours)
 - **MARGE_ENERGIE** : Marge d'erreur pour l'énergie du système (variation max entre 2 calculs) en %
---

## Données planétaires : 

### Position initiale : 

Nous avons décidé de partir du [site de la NASA](https://ssd.jpl.nasa.gov/horizons/) afin d'obtenir les positions initiales de chaque planète.  
Le temps de départ que nous avons choisi est le 10 Juin 2026 à 00:00 TDB (Temps Dynamique Barycentrique)  
Le TDB est l'unité de temps par défaut du site, il est surtout utilisé en astronomie.  
Il est indépendant des irrégularités de la rotation terrestre.

---

### Ajouter une autre planète :

*On considère que l'utilisateur souhaite ajouter une planète à une liste de n planètes, dont le nom est [NomPlanete]*

#### **1 - fichiers_c/planetes.h**

Ajouter 1 au nombre de planètes :
<pre><code>#define N_PLANETS  n+1</code></pre>

#### **2 - fichiers_c/planetes.c**

Ajouter les informations de la planète :  
<pre><code>Planet * reset(Planet * planets){

    ...

    planets[n] = initPlanet(m, x, y, z, vx, vy, vz); // m en kg, x,y et z en m et vx,vy et vz en m/s
    
    return planets;
}

...

TempFILE * initFiles(TempFILE * files, char * prefixe){

    ...

    initiateFileData(&files[n], prefixe, "[nomplanete]"); // nom en minuscule

    return files;
}</code></pre>

#### **3 - web/data_info.bdd**

Ajouter le reste des informations :  
<pre><code>...
    "[nomplanete]": {                           // nom en minuscule
        "Name": "[NomPlanete]",                 // nom en majuscule
        "Color": "rgb([couleurPlanete])",
        "Diametre": [diametre],
        "Masse": "[m] kg",
        "Gravite": "[v] m/s²",
        "Densite": "[densite] g/cm³",
        "Distance_soleil": "[distanceSoleil]",
        "Periode_orbitale": "[tempsOrbite]",
        "Vitesse_orbitale": "[vitesseOrbite]",
        "Temperature_moyenne": "[temperature] (surface)",
        "Composition": "[composition : 100%]"
    }</code></pre>

#### **4 - web/3D_texture/**

Ajouter un modèle 3D de la planète :
<pre><code>[NomPlanete].glb      # nom en majuscule</code></pre>

---