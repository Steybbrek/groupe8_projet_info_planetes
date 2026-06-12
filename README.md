# groupe8_projet_info_planetes

## Mise en forme du fichier json : 

### methodes.json
<pre><code>{
    "planete_methode":[
        [[x,y,z],[vx,vy,vz],0],  
        [[x,y,z],[vx,vy,vz],1],
        ...
        [[x,y,z],[vx,vy,vz],t]
    ],
    ...
}</code></pre>

**ATTENTION :** Le fichier json d'exemple possède de fois t = 0 par méthode, surement 1 d'initialisation et 1 du premier calcul.  
On a donc supprimé le 1er t=0 pour avoir des données plus adaptées et simplifier les opérations
---

## Données planétaires : 

### Position initiale : 

Nous avons décidé de partir du [site de la NASA](https://ssd.jpl.nasa.gov/horizons/app.html/) afin d'obtenir les positions inititales de chaque planète.  
Le temps de départ que nous avons choisi est le 10 Juin 2026 à 00:00 TDB (Temps Dynamique Barycentrique)  
Le TDB est l'unité de temps par défaut du site, il est surtout utilisé en astronomie.  
Il est indépendant des irrégularités de la rotation terrestre.

---