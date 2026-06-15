import * as THREE from 'three';
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import { GLTFLoader } from 'three/addons/loaders/GLTFLoader.js';
import { CSS2DRenderer, CSS2DObject } from 'three/addons/renderers/CSS2DRenderer.js';

class Planete {
    constructor(data) {
        this.scale_taille = 5e5

        this.name = data["Name"];
        this.diametre = data["Diametre"];
        this.color = data["Color"]
        this.modele3D = null

        
        this.geo = new THREE.SphereGeometry(this.diametre / this.scale_taille, 32, 32);
        this.mat = new THREE.MeshBasicMaterial({
            color: this.color
        });
        this.mesh = new THREE.Mesh(this.geo, this.mat);
    }

    print() {
        console.log(`Name = ${this.name} | Diametre = ${this.diametre} | Color = ${this.color} | Position 0 = ${this.position[0][0]}`);
    }

    update_rotation(t) {
        this.mesh.rotation.z = t * 0.0001
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
            let tailleCible = 1;

            const ratio = tailleCible / tailleActuelle;
            console.log(ratio)
            this.modele3D.scale.set(ratio, ratio, ratio); 
            this.mat.visible = false;
            this.mesh.add(this.modele3D);
        })
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

function createPlanet(data_info, planete_name) {
    const info = data_info[planete_name];
    const planet = new Planete(info);
    return planet;
}

export {fetchJSONData, createPlanet, Planete};