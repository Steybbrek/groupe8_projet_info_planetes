import * as THREE from 'three';
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import { GLTFLoader } from 'three/addons/loaders/GLTFLoader.js';
import { CSS2DRenderer, CSS2DObject } from 'three/addons/renderers/CSS2DRenderer.js';

class Planete {
    constructor(data, position) {
        this.scale_distance = 1e10;
        this.scale_taille = 5e5

        this.name = data["Name"];
        this.diametre = data["Diametre"];
        this.color = data["Color"]
        this.position = position
        this.label = null
        this.orbite_mesh = null
        this.modele3D = null
        this.halo = null

        if(this.diametre / 1e6 > 0.5) {
            this.geo = new THREE.SphereGeometry(this.diametre / this.scale_taille, 32, 32);
        }
        else {
        this.geo = new THREE.SphereGeometry(0.5, 32, 32);
        }
        this.mat = new THREE.MeshBasicMaterial({
            color: this.color
        });
        this.mesh = new THREE.Mesh(this.geo, this.mat);
        this.mesh.userData.planet = this; //Permet de retrouver ces donnée apres le raycasting dans le intersect

    }

    print() {
        console.log(`Name = ${this.name} | Diametre = ${this.diametre} | Color = ${this.color} | Position 0 = ${this.position[0][0]}`);
    }

    update_rotation(t) {
        this.mesh.rotation.z = t * 0.0001
    }

    update_position(i) {
        if(this.name != "Soleil") {
            const x = this.position[i][0][0] / this.scale_distance;
            const y = this.position[i][0][1] / this.scale_distance;
            const z = this.position[i][0][2] / this.scale_distance;
            this.mesh.position.set(x, y, z)
        }
    }

    interpolation(start, end, frame, frametonext) {
        if(this.name != "Soleil") {
            let vector_start = new THREE.Vector3(
                this.position[start][0][0] / this.scale_distance,
                this.position[start][0][1] / this.scale_distance,
                this.position[start][0][2] / this.scale_distance,
            )
            let vecteur_end = new THREE.Vector3(
                this.position[end][0][0] / this.scale_distance,
                this.position[end][0][1] / this.scale_distance,
                this.position[end][0][2] / this.scale_distance,
            )

            vector_start.lerp(vecteur_end, frame/frametonext);
            this.mesh.position.copy(vector_start); // on a pas 3 valeur mais un vecteur donc c'est copy
        }
    }

    drawOrbite() {
        if(this.name != "Soleil") {
            let points = []
            this.position.forEach(pos => {
                let point_position = new THREE.Vector3
                point_position.x = pos[0][0] / this.scale_distance
                point_position.y = pos[0][1] / this.scale_distance
                point_position.z = pos[0][2] / this.scale_distance
                points.push(point_position)
            });
            const geo = new THREE.BufferGeometry().setFromPoints(points)
            const mat = new THREE.LineBasicMaterial({ 
                color: this.color,
                transparent: true,
                opacity: 0.3
            });
            this.orbite_mesh = new THREE.Line(geo, mat)
            return this.orbite_mesh
        }
    }

    load_3D_model(fichier) {
        const loader = new GLTFLoader();
        loader.load(fichier, (gltf) => {
            this.modele3D = gltf.scene;
            console.log(this.name)
            //Mesure la taille du l'asset pour pouvoir reduire sa taille a "this.diametre / 1e7"
            const box = new THREE.Box3().setFromObject(this.modele3D);
            const tailleActuelle = box.getSize(new THREE.Vector3()).x;
            console.log(tailleActuelle)
            let tailleCible = this.diametre / this.scale_taille;

            const ratio = tailleCible / tailleActuelle;
            console.log(ratio)
            this.modele3D.scale.set(ratio, ratio, ratio); 
            this.mat.visible = false;
            this.mesh.add(this.modele3D);
        })
    }

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

    createHalo() {
        if(this.name != "Soleil") {
            const geo = new THREE.CircleGeometry(0.5, 64);
            const bordure = new THREE.EdgesGeometry(geo);
            const mat = new THREE.LineBasicMaterial({ 
                color: 0xDEDEDE,
            });
            this.halo = new THREE.LineSegments(bordure, mat);
            this.halo.position.copy(this.mesh.position);
            
            return this.halo;
        }
    }

    updateHalo() {
        if(this.name != "Soleil") {
            this.halo.position.copy(this.mesh.position);
        }
        return this.halo
    }

    update_name() {
        this.label.position.set(1, this.diametre / 1e7, 0); 
    }

    hide_name() {
        this.label.visible = false;
    }

    hide_orbite() {
        if(this.orbite_mesh) {
            this.orbite_mesh.visible = false;
        }
    }

    hide_planet() {
        if(this.modele3D) {
            this.modele3D.visible = false
        }
    }

    hide_halo() {
        if(this.halo) {
            this.halo.visible = false
        }
    }

    show_name() {
        this.label.visible = true;
    }

    show_orbite() {
        if(this.orbite_mesh) {
            this.orbite_mesh.visible = true;
        }
    }

    show_planet() {
        if(this.modele3D) {
            this.modele3D.visible = true
        }
    }

    show_halo() {
        if(this.halo) {
            this.halo.visible = true
        }
    }
}

function fetchJSONData(url) {
    return fetch(url)
        .then(response => {
            if (!response.ok) {
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
            return response.json();  
        }); 
}

function createPlanets(data_pos, data_info, method) {
    const planets = [];
    for (let planete_name in data_info) {
        const info = data_info[planete_name];
        let positions;
        if (planete_name == "soleil") {
            positions = [ [[0, 0, 0], [0, 0, 0], 0] ];
        } else {
            //recreer le chemin avec "planete_name" + "_" + "methode_de_calcul"
            const key = `${planete_name}_${method}`;
            positions = data_pos[key];
        }
        planets.push(new Planete(info, positions));
    }
 
    return planets;
}

export {fetchJSONData, createPlanets, Planete};