import * as THREE from 'three';
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import { GLTFLoader } from 'three/addons/loaders/GLTFLoader.js';
import { CSS2DRenderer, CSS2DObject } from 'three/addons/renderers/CSS2DRenderer.js';

/**
 * Classe représentant une planète.
 * Gère la géométrie 3D, l'orbite, l'étiquette HTML, le modèle 3D et l'interpolation des mouvements.
 */
class Planete {
    /**
     * Initialise une nouvelle planète avec ses données de base.
     * @param {Object} data - Les informations de la planète (Nom, Diamètre, Couleur).
     * @param {Array} position - Le tableau contenant toutes les coordonnées [x, y, z] précalculées de l'orbite.
     */
    constructor(data, position) {
        this.scale_distance = 1e10;
        this.scale_taille = 5e5;
        this.camera_pos = new THREE.Vector3;
        this.camera_recul = 10;

        this.name = data["Name"];
        this.diametre = data["Diametre"];
        this.color = data["Color"];
        this.position = position;
        
        this.label = null;
        this.orbite_mesh = null;
        this.modele3D = null;
        this.halo = null;

        // Création de la géométrie de base (sphère de couleur unie)
        if(this.diametre / 1e6 > 0.5) {
            this.geo = new THREE.SphereGeometry(this.diametre / this.scale_taille, 32, 32);
        } else {
            this.geo = new THREE.SphereGeometry(0.5, 32, 32);
        }
        
        this.mat = new THREE.MeshBasicMaterial({ color: this.color });
        this.mesh = new THREE.Mesh(this.geo, this.mat);
        
        // Permet de retrouver l'instance de la classe lors du Raycasting
        this.mesh.userData.planet = this; 
    }

    /**
     * Affiche les informations de la planète dans la console.
     */
    print() {
        console.log(`Name = ${this.name} | Diametre = ${this.diametre} | Color = ${this.color} | Position 0 = ${this.position[0][0]}`);
    }

    /**
     * Fait tourner la planète sur elle-même.
     * @param {number} t - Le temps écoulé depuis le début de l'animation.
     */
    update_rotation(t) {
        this.mesh.rotation.z = t * 0.0001;
    }

    /**
     * Place la planète à un index précis de son tableau de positions.
     * @param {number} i - L'index de l'étape.
     */
    update_position(i) {
        if(this.name != "Soleil") {
            const x = this.position[i][0][0] / this.scale_distance;
            const y = this.position[i][0][1] / this.scale_distance;
            const z = this.position[i][0][2] / this.scale_distance;
            this.mesh.position.set(x, y, z);
            this.camera_pos = new THREE.Vector3(x, y, z)
        }
    }

    /**
     * Déplace de manière fluide la planète entre deux étapes.
     * @param {number} start - L'index de la position de départ.
     * @param {number} end - L'index de la position d'arrivée.
     * @param {number} frame - La frame d'animation actuelle.
     * @param {number} frametonext - Le nombre total de frames a la prochaine étape.
     */
    interpolation(start, end, frame, frametonext) {
        if(this.name != "Soleil") {
            let vector_start = new THREE.Vector3(
                this.position[start][0][0] / this.scale_distance,
                this.position[start][0][1] / this.scale_distance,
                this.position[start][0][2] / this.scale_distance
            );
            let vecteur_end = new THREE.Vector3(
                this.position[end][0][0] / this.scale_distance,
                this.position[end][0][1] / this.scale_distance,
                this.position[end][0][2] / this.scale_distance
            );

            // Interpolation linéaire (LERP) pour trouver le point intermédiaire exact
            vector_start.lerp(vecteur_end, frame/frametonext);
            this.mesh.position.copy(vector_start); 
            this.camera_pos.copy(vector_start)
        }
    }

    /**
     * Dessine la ligne représentant la l'orbite d'une planète.
     * @returns {THREE.Line|undefined} Le mesh de la ligne d'orbite (ou undefined si c'est le Soleil).
     */
    drawOrbite() {
        if(this.name != "Soleil") {
            let points = [];
            this.position.forEach(pos => {
                let point_position = new THREE.Vector3();
                point_position.x = pos[0][0] / this.scale_distance;
                point_position.y = pos[0][1] / this.scale_distance;
                point_position.z = pos[0][2] / this.scale_distance;
                points.push(point_position);
            });
            const geo = new THREE.BufferGeometry().setFromPoints(points);
            const mat = new THREE.LineBasicMaterial({ 
                color: this.color,
                transparent: true,
                opacity: 0.3
            });
            this.orbite_mesh = new THREE.Line(geo, mat);
            return this.orbite_mesh;
        }
    }

    /**
     * Charge et adapte un modèle 3D détaillé pour remplacer la sphère de base.
     * @param {string} fichier - Le chemin vers le fichier .glb.
     * @param {boolean} realiste - Si false, force une taille minimale pour que la planète reste visible.
     */
    load_3D_model(fichier, realiste) {
        const loader = new GLTFLoader();
        loader.load(fichier, (gltf) => {
            this.modele3D = gltf.scene;
            
            const box = new THREE.Box3().setFromObject(this.modele3D);
            const tailleActuelle = box.getSize(new THREE.Vector3()).x;
            
            let tailleCible = this.diametre / this.scale_taille;
            if (!realiste) {
                if (tailleCible < 0.1) {
                    tailleCible = 1;
                }
            }
            
            const ratio = tailleCible / tailleActuelle;
            this.modele3D.scale.set(ratio, ratio, ratio); 
            this.mat.visible = false;
            this.mesh.add(this.modele3D);
            this.camera_recul = tailleCible * 25
        });
    }

    /**
     * Crée une étiquette textuelle HTML pour afficher le nom de la planète.
     */
    createName() {
        const label_name = document.createElement('div');
        label_name.className = 'planet-name';
        label_name.textContent = this.name;
        
        label_name.style.color = 'white';
        label_name.style.fontFamily = 'sans-serif';
        label_name.style.fontSize = '12px';
        label_name.style.marginTop = '-1em';

        this.label = new CSS2DObject(label_name);
        this.label.position.set(0, this.diametre / 1e7, 0); 
        this.mesh.add(this.label);
    }

    /**
     * Crée un halo de sélection autour de la planète.
     * @returns {THREE.LineSegments|undefined} Le mesh du halo.
     */
    createHalo() {
        if(this.name != "Soleil") {
            const geo = new THREE.CircleGeometry(0.5, 64);
            const bordure = new THREE.EdgesGeometry(geo);
            const mat = new THREE.LineBasicMaterial({
                color: 0xDEDEDE,
                transparent: true,
                opacity: 0.5
            });
            
            this.halo = new THREE.LineSegments(bordure, mat);
            this.halo.position.copy(this.mesh.position);
            
            return this.halo;
        }
    }

    /**
     * Met à jour la position et la taille du halo pour qu'il soit toujours visible face caméra.
     * @param {boolean} realiste - Si true, la taille du halo s'adapte dynamiquement à la distance de la caméra.
     * @param {THREE.Camera} camera - La camera active de la scène.
     * @returns {THREE.LineSegments|undefined} Le halo mis à jour.
     */
    updateHalo(realiste, camera) {
        if(this.name != "Soleil" && this.halo) {
            this.halo.position.copy(this.mesh.position);
            
            if (realiste) {
                const scaleConstante = camera.position.distanceTo(this.halo.position) / 50; 
                this.halo.scale.set(scaleConstante, scaleConstante, scaleConstante);
            } else {
                this.halo.scale.set(1, 1, 1);
            }
        }
        return this.halo;
    }

    /**
     * Met à jour la position de l'étiquette de nom.
     */
    update_name() {
        if(this.label) this.label.position.set(1, this.diametre / 1e7, 0); 
    }

    // FONCTIONS D'INTERFACE (Afficher / Masquer des éléments)
    hide_name() { if(this.label) this.label.visible = false; }
    hide_orbite() { if(this.orbite_mesh) this.orbite_mesh.visible = false; }
    hide_planet() { if(this.modele3D) this.modele3D.visible = false; }
    hide_halo() { if(this.halo) this.halo.visible = false; }

    show_name() { if(this.label) this.label.visible = true; }
    show_orbite() { if(this.orbite_mesh) this.orbite_mesh.visible = true; }
    show_planet() { if(this.modele3D) this.modele3D.visible = true; }
    show_halo() { if(this.halo) this.halo.visible = true; }
}

/**
 * Récupère et parse un fichier JSON depuis une URL.
 * @param {string} url - Le chemin du fichier JSON.
 * @returns {<Object>} les données JSON parsées.
 */
function fetchJSONData(url) {
    return fetch(url)
        .then(response => {
            if (!response.ok) {
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
            return response.json();  
        }); 
}

/**
 * Génère un tableau d'instances de la classe Planete.
 * @param {Object} data_pos - Le fichier JSON contenant toutes les positions calculées.
 * @param {Object} data_info - Le fichier JSON contenant les propriétés physiques.
 * @param {string} method - La méthode mathématique pour trouver la bonne clef d'orbite.
 * @returns {Planete[]} Un tableau contenant tous les objets Planete.
 */
function createPlanets(data_pos, data_info, method) {
    const planets = [];
    for (let planete_name in data_info) {
        const info = data_info[planete_name];
        let positions;
        
        if (planete_name == "soleil") {
            // Le soleil reste au centre (0,0,0)
            positions = [ [[0, 0, 0], [0, 0, 0], 0] ];
        } else {
            // Reconstruit dynamiquement la clef (ex: "terre_RK4")
            const key = `${planete_name}_${method}`;
            positions = data_pos[key];
        }
        planets.push(new Planete(info, positions));
    }
    return planets;
}

export {fetchJSONData, createPlanets, Planete};