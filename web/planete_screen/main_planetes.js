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
        renderer.setSize( window.innerWidth, window.innerHeight);
        camera.updateProjectionMatrix();
    });

    //set-up draw scene
    const renderer = createRenderer(w, h)
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

    function animate(t) {
        requestAnimationFrame(animate);
        
        planet.update_rotation(t);

        renderer.render(scene, camera);
        controls.update();
    }
    animate(0)
}

main();