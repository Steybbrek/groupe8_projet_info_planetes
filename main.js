import * as THREE from 'three';
import { OrbitControls } from "jsm/controls/OrbitControls.js";
import {fetchJSONData, createPlanets, Planete} from "./loader.js";
import {createScene, createCamera, createRenderer, createControls} from "./draw.js";


async function main() {
    const w = window.innerWidth;
    const h = window.innerHeight;

    //extract and create class
    const data_pos = await fetchJSONData('./data_pos.json');
    const data_info = await fetchJSONData('./data_info.json');
    const planets = createPlanets(data_pos, data_info);
    planets.forEach(planet => planet.print());
    console.log(planets)

    //set-up draw scene
    const renderer = createRenderer(w, h)
    const scene = createScene()
    const camera = createCamera(45, w/h, 0.1, 500)
    const controls = createControls(camera, renderer)

    //draw planets
    planets.forEach(planet => {
        scene.add(planet.mesh);
    });
    function animate(t) {
        requestAnimationFrame(animate);

        planets.forEach(planet => planet.update_rotation(t));
        planets.forEach(planet => planet.update_position(t));
        
        renderer.render(scene, camera);
        controls.update();
    }
    animate(0)
}

main();