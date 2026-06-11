import * as THREE from 'three';
import { OrbitControls } from "jsm/controls/OrbitControls.js";

class Planete {
    constructor(data, position) {
        this.name = data["Name"];
        this.diametre = data["Diametre"];
        this.color = data["Color"]
        this.position = position
        this.geo = new THREE.SphereGeometry(0.5, 32, 32);
        this.mat = new THREE.MeshBasicMaterial({
            color: this.color.toString()
        });
        this.mesh = new THREE.Mesh(this.geo, this.mat);
    }

    print() {
        console.log(`Name = ${this.name} | Diametre = ${this.Diametre} | Color = ${this.color} | Position 0 = ${this.position[0][0]}`);
    }

    update_rotation(t) {
        this.mesh.rotation.y = t * 0.0001
    }

    update_position(i) {
        if(this.name != "Sun") {
            const scale = 1e10;
            const x = this.position[0][0][0] / scale
            const y = this.position[0][0][1] / scale
            const z = this.position[0][0][2] / scale
            this.mesh.position.set(x, y, z)
            console.log(this.position[0][0])
        }
    }

    get_name() {
        return this.name;
    }

    get_diametre() {
        return this.diametre
    }

    get_radius() {
        return this.diametre / 2;
    }

    get_color() {
        return this.color;
    }

    get_position(i) {
        return this.position[i]
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