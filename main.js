import * as THREE from 'three';
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import {fetchJSONData, createPlanets, Planete} from "./loader.js";
import {createScene, createCamera, createRenderer, createControls, createStars} from "./draw.js";
import {createRaycaster, onPointerMove, createPointer} from "./Collision.js"

async function main() {
    const w = window.innerWidth;
    const h = window.innerHeight;

    //extract and create class
    const data_pos = await fetchJSONData('./data_pos.json');
    const data_info = await fetchJSONData('./data_info.json');
    const planets = createPlanets(data_pos, data_info);
    planets.forEach(planet => planet.print());

    //set-up draw scene
    const renderer = createRenderer(w, h)
    const scene = createScene()
    const camera = createCamera(45, w/h, 0.1, 5000)
    const controls = createControls(camera, renderer)

    //draw planets
    planets.forEach(planet => {
        scene.add(planet.mesh);
    });
    
    //draw stars
    const Star = createStars(2000, 200)
    scene.add(Star)

    //Raycasting
    const pointer = createPointer()
    const raycaster = createRaycaster()
    window.addEventListener('pointermove', (event) => {
        onPointerMove(event, pointer);

    //Orbite
    planets.forEach(planet => {
        //Attention au Soleil
        const temp = planet.drawOrbite()
        if(temp != undefined) scene.add(planet.drawOrbite());
    });


    });

    function animate(t) {
        requestAnimationFrame(animate);

        planets.forEach(planet => planet.update_rotation(t));
        planets.forEach(planet => planet.update_position(t));

        const meshesPlanetes = planets.map(p => p.mesh);
        raycaster.setFromCamera(pointer, camera)
        const intersects = raycaster.intersectObjects(meshesPlanetes, false);

        if (intersects.length > 0) {
            console.log("Tu survoles une planète !");
            console.log(intersects[0].object)
        }

        renderer.render(scene, camera);
        controls.update();
    }
    animate(0)
}

main();