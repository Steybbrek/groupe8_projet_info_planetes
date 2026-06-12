import * as THREE from 'three';
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import { GLTFLoader } from 'three/addons/loaders/GLTFLoader.js';
import { CSS2DRenderer, CSS2DObject } from 'three/addons/renderers/CSS2DRenderer.js';
import {fetchJSONData, createPlanets, Planete} from "./loader.js";
import {createScene, createCamera, createRenderer, createControls, createStars, createLight, createTextRenderer} from "./draw.js";
import {createRaycaster, onPointerMove, createPointer} from "./Collision.js"

async function main() {
    const w = window.innerWidth;
    const h = window.innerHeight;

    window.addEventListener('resize', () => {
        camera.aspect = window.innerWidth / window.innerHeight;
        renderer.setSize( window.innerWidth, window.innerHeight);
        camera.updateProjectionMatrix();
    });

    //Slider
    const slider = document.getElementById("stage_slider");
    const StageValue = document.getElementById("stage_value");
    const button = document.getElementById("stage_autoplay")
    let stage = 1;
    //TO DO : AUTOGENERATE
    slider.max = 3651
    slider.addEventListener('input', (event) => {
        stage = parseInt(event.target.value);
        StageValue.innerText = stage;
    });
    let autoplay = false; 
    button.addEventListener("click", (event) => {
        if(autoplay) {
            button.innerText = "►";
            autoplay = false;
        }
        else {
            button.innerText = "||";
            autoplay = true;
        }
    });


    //extract and create class
    const data_pos = await fetchJSONData('./methodes.json');
    const data_info = await fetchJSONData('./data_info.json');
    const planets = createPlanets(data_pos, data_info);
    planets.forEach(planet => planet.print());

    //set-up draw scene
    const renderer = createRenderer(w, h)
    const scene = createScene()
    const camera = createCamera(45, w/h, 0.1, 5000)
    const controls = createControls(camera, renderer)

    //draw planets / Orbits
    planets.forEach(planet => {
        scene.add(planet.mesh);
        let path = './3D_texture/' + planet.name + ".glb"
        planet.load_3D_model(path);
        planets.forEach(planet => planet.createName())
        //Sun case
        const temp = planet.drawOrbite()
        if(temp != undefined) scene.add(planet.drawOrbite());
    });
    
    //draw stars
    const Star = createStars(3000, 200)
    scene.add(Star)

    //Raycasting
    const pointer = createPointer()
    const raycaster = createRaycaster()
    window.addEventListener('pointermove', (event) => {
        onPointerMove(event, pointer);
    });
    
    // create Light (for the 3d asset)
    const light = createLight()
    scene.add(light)

    const labelRenderer = createTextRenderer()
    document.body.appendChild(labelRenderer.domElement);

    let time = 0
    function animate(t) {
        requestAnimationFrame(animate);
        if(autoplay == true) {
            if(time%60 == 0) {
                StageValue.innerText = stage+1;
                stage += 1
                planets.forEach(planet => planet.update_position(stage));
            }
            else {
                planets.forEach(planet => planet.interpolation(stage, stage+1, time%60, 60));
            }
        }
        else {
            planets.forEach(planet => planet.update_position(document.getElementById('stage_value').textContent));
        }
        planets.forEach(planet => planet.update_rotation(t));
        planets.forEach(planet => planet.update_name())
        const meshesPlanetes = planets.map(p => p.mesh);
        raycaster.setFromCamera(pointer, camera)
        const intersects = raycaster.intersectObjects(meshesPlanetes, false);

        if (intersects.length > 0) {
            console.log("Tu survoles une planète !");
            console.log(intersects[0].object)
        }

        labelRenderer.render(scene, camera);
        renderer.render(scene, camera);
        controls.update();
        time += 1
    }
    animate(0)
}

main();