import * as THREE from 'three';
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import { GLTFLoader } from 'three/addons/loaders/GLTFLoader.js';
import {fetchJSONData, createPlanet, Planete} from "./loader.js";
import {createScene, createCamera, createRenderer, createControls, createStars, createLight, createTextRenderer} from "../draw.js";

async function main() {
    const w = window.innerWidth-(window.innerWidth/3);
    const h = window.innerHeight;
    const parametresUrl = new URLSearchParams(window.location.search);
    const cible = parametresUrl.get('cible');

    //extract and create class
    const data_info = await fetchJSONData('../data_info.bdd');
    const planet = createPlanet(data_info, cible);

    window.addEventListener('resize', () => {
        camera.aspect = window.innerWidth / window.innerHeight;
        renderer.setSize( window.innerWidth-(window.innerWidth/3), window.innerHeight);
        camera.updateProjectionMatrix();
    });

    //set-up draw scene
    const renderer = createRenderer(w, h)
    document.getElementById('canvas_zone').appendChild(renderer.domElement);
    const scene = createScene()
    const camera = createCamera(45, w/h, 0.1, 5000)
    camera.position.z = 5;
    const controls = createControls(camera, renderer)

    scene.add(planet.mesh);
    let path = '../3D_texture/' + planet.name + ".glb"
    planet.load_3D_model(path);
    
    //draw stars
    const Star = createStars(3000, 200)
    scene.add(Star)
    
    const color = 0xFFFFFF;
    const intensity = 1;
    const light = new THREE.AmbientLight(color, intensity);
    scene.add(light);

    document.getElementById("planet_name").innerHTML = planet.name
    document.getElementById("info_diametre").innerHTML = planet.diametre + " km"
    document.getElementById("info_masse").innerHTML = planet.masse
    document.getElementById("info_gravite").innerHTML = planet.gravity
    document.getElementById("info_densite").innerHTML = planet.densite
    document.getElementById("info_gravite").innerHTML = planet.gravity
    document.getElementById("info_distance").innerHTML = planet.distance_soleil
    document.getElementById("info_periode").innerHTML = planet.periode_orbitale
    document.getElementById("info_vitesse").innerHTML = planet.vitesse_orbitale
    document.getElementById("info_atmo").innerHTML = planet.composition
    document.getElementById("info_temp").innerHTML = planet.temperature_moyenne

    function animate(t) {
        requestAnimationFrame(animate);
        
        planet.update_rotation(t);

        renderer.render(scene, camera);
        controls.update();
    }
    animate(0)
}

main();