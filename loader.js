import * as THREE from 'three';
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import { GLTFLoader } from 'three/addons/loaders/GLTFLoader.js';

class Planete {
    constructor(data, position) {
        this.scale_distance = 1e10;
        this.scale_taille = 5e5
        this.name = data["Name"];
        this.diametre = data["Diametre"];
        this.color = data["Color"]
        this.position = position
        if(this.diametre / 1e6 > 0.1) {
            this.geo = new THREE.SphereGeometry(this.diametre / this.scale_taille, 32, 32);
        }
        else {
        this.geo = new THREE.SphereGeometry(0.1, 32, 32);
        }
        this.mat = new THREE.MeshBasicMaterial({
            color: this.color.toString()
        });
        this.mesh = new THREE.Mesh(this.geo, this.mat);

    }

    print() {
        console.log(`Name = ${this.name} | Diametre = ${this.Diametre} | Color = ${this.color} | Position 0 = ${this.position[0][0]}`);
    }

    update_rotation(t) {
        this.mesh.rotation.z = t * 0.0001
    }

    update_position(i) {
        if(this.name != "Sun") {
            const x = this.position[i][0][0] / this.scale_distance
            const y = this.position[i][0][1] / this.scale_distance
            const z = this.position[i][0][2] / this.scale_distance
            this.mesh.position.set(x, y, z)
        }
    }

    drawOrbite() {
        if(this.name != "Sun") {
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
            const mesh = new THREE.Line(geo, mat)
            return mesh
        }
    }

    load_3D_model(fichier) {
        const loader = new GLTFLoader();
        loader.load(fichier, (gltf) => {
            const modele3D = gltf.scene;
            console.log(this.name)
            //Mesure la taille du l'asset pour pouvoir reduire sa taille a "this.diametre / 1e7"
            const box = new THREE.Box3().setFromObject(modele3D);
            const tailleActuelle = box.getSize(new THREE.Vector3()).x;
            console.log(tailleActuelle)
            let tailleCible = this.diametre / this.scale_taille;
            console.log(tailleCible)
            if (tailleCible < 0.1) {
                tailleCible = 1;
            }
            console.log(tailleCible)
            const ratio = tailleCible / tailleActuelle;
            console.log(ratio)
                modele3D.scale.set(ratio, ratio, ratio); 
            this.mat.visible = false;
            this.mesh.add(modele3D);
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

function createPlanets(data_pos, data_info) {
    let planets = [];
    for (var planete_name in data_info) {
        planets.push(new Planete(data_info[planete_name], data_pos[planete_name]));
    }
    return planets;
}

async function test() {
    try {
        const data = await fetchJSONData("./Data.json"); 
        const planets = createPlanets(data);
        planets.forEach(planet => planet.print());
        console.log("DATA :", data)
        console.log("Bodies:", data.bodies); 
        console.log("Meta:", data.meta);
        
    } catch (error) {
        console.error('Failed to process data:', error);
    }
}

export {fetchJSONData, createPlanets, Planete};