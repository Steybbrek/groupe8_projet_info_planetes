import * as THREE from 'three';
import { OrbitControls } from "three/addons/controls/OrbitControls.js";

class Planete {
    constructor(data, position) {
        this.name = data["Name"];
        this.diametre = data["Diametre"];
        this.color = data["Color"]
        this.position = position
        if(this.diametre / 1e6 > 0.1) {
            this.geo = new THREE.SphereGeometry(this.diametre / 1e6, 32, 32);
        }
        else {
        this.geo = new THREE.SphereGeometry(0.1, 32, 32);
        }
        this.mat = new THREE.MeshBasicMaterial({
            color: this.color.toString()
        });
        this.mesh = new THREE.Mesh(this.geo, this.mat);
        this.scale = 1e10;

    }

    print() {
        console.log(`Name = ${this.name} | Diametre = ${this.Diametre} | Color = ${this.color} | Position 0 = ${this.position[0][0]}`);
    }

    update_rotation(t) {
        this.mesh.rotation.y = t * 0.0001
    }

    update_position(i) {
        if(this.name != "Sun") {
            const x = this.position[0][0][0] / this.scale
            const y = this.position[0][0][1] / this.scale
            const z = this.position[0][0][2] / this.scale
            this.mesh.position.set(x, y, z)
        }
    }

    drawOrbite() {
        if(this.name != "Sun") {
            let points = []
            this.position.forEach(pos => {
                let point_position = new THREE.Vector3
                point_position.x = pos[0][0] / this.scale
                point_position.y = pos[0][1] / this.scale
                point_position.z = pos[0][2] / this.scale
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