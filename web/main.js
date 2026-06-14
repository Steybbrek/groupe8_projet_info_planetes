import * as THREE from 'three';
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import { GLTFLoader } from 'three/addons/loaders/GLTFLoader.js';
import { CSS2DRenderer, CSS2DObject } from 'three/addons/renderers/CSS2DRenderer.js';
import {fetchJSONData, createPlanets, Planete} from "./loader.js";
import {createScene, createCamera, createRenderer, createControls, createStars, createLight, createTextRenderer} from "./draw.js";
import {createRaycaster, detect_click, createPointer, reset_pointer} from "./Collision.js"

async function main() {
    const w = window.innerWidth;
    const h = window.innerHeight;

    window.addEventListener('resize', () => {
        camera.aspect = window.innerWidth / window.innerHeight;
        renderer.setSize( window.innerWidth, window.innerHeight);
        labelRenderer.setSize( window.innerWidth, window.innerHeight);
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

    //draw planets / Orbits / Halo
    planets.forEach(planet => {
        scene.add(planet.mesh);
        let path = './3D_texture/' + planet.name + ".glb"
        planet.load_3D_model(path);
        planet.createName()
        //Sun case
        const orbite = planet.drawOrbite()
        if(orbite != undefined) scene.add(orbite);
        const halo = planet.createHalo()
        if(halo != undefined) scene.add(halo)
    });
    
    //draw stars
    const Star = createStars(3000, 200)
    scene.add(Star)

    //Raycasting
    const pointer = createPointer()
    const raycaster = createRaycaster()
    detect_click(pointer)
    
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

        planets.forEach(planet => {
            const halo = planet.updateHalo()
            if(halo != undefined) halo.quaternion.copy(camera.quaternion);
        });
  

        planets.forEach(planet => planet.update_name())

        const meshesPlanetes = planets.map(p => p.mesh);
        raycaster.setFromCamera(pointer, camera)
        const intersects = raycaster.intersectObjects(meshesPlanetes, false);

        if (intersects.length > 0) {
            reset_pointer(pointer)
            OnclickPlanet(intersects[0].object)
        }
        if(time%60 == 0) menu(planets, Star)
        labelRenderer.render(scene, camera);
        renderer.render(scene, camera);
        controls.update();
        time += 1
    }
    animate(0)
}

function menu(planets, Star) {
    if (ischeck("checkbox_names")) {
        planets.forEach(planet => planet.show_name())
    }
    else {
        planets.forEach(planet => planet.hide_name())
    }
    if (ischeck("checkbox_etoile")) {
        Star.visible = true
    }
    else {
        Star.visible = false
    }
    if (ischeck("checkbox_orbites")) {
        planets.forEach(planet => planet.show_orbite())
    }
    else {
        planets.forEach(planet => planet.hide_orbite())
    }
    if (ischeck("checkbox_planets")) {
        planets.forEach(planet => planet.show_planet())
    }
    else {
        planets.forEach(planet => planet.hide_planet())
    }
    if (ischeck("checkbox_halo")) {
        planets.forEach(planet => planet.show_halo())
    }
    else {
        planets.forEach(planet => planet.hide_halo())
    }
}

function ischeck(id) {
    return document.getElementById(id).checked
}

function OnclickPlanet(mesh) {
    console.log(mesh.userData.planet.name)
    //window.location.href = "planete.html";
}
main();