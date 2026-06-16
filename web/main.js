import * as THREE from 'three';
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import { GLTFLoader } from 'three/addons/loaders/GLTFLoader.js';
import { CSS2DRenderer, CSS2DObject } from 'three/addons/renderers/CSS2DRenderer.js';
import {fetchJSONData, createPlanets, Planete} from "./loader.js";
import {createScene, createCamera, createRenderer, createControls, createStars, createLight, createTextRenderer} from "./draw.js";
import {createRaycaster, detect_click, createPointer, reset_pointer} from "./Collision.js"


/**
 * Fonction principale qui initialise le système solaire. Gère la scène 3D, l'interface utilisateur, le raycasting et la boucle d'animation.
 */
async function main() {
    const w = window.innerWidth;
    const h = window.innerHeight;
    const parametresUrl = new URLSearchParams(window.location.search);
    let method = parametresUrl.get('method');
    if (!method) {
        method = "RK4"
    }
    document.getElementById("radio_" + method.toLowerCase()).checked = true;

    //extract and create class
    const data_pos = await fetchJSONData('./methodes.json');
    const data_info = await fetchJSONData('./data_info.bdd');
    const planets = createPlanets(data_pos, data_info, method);

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
    //[1] parce que [0] c'est le soleil
    slider.max = planets[1].position.length - 1;
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

    //set-up draw scene
    const renderer = createRenderer(w, h)
    const scene = createScene()
    const camera = createCamera(45, w/h, 0.0001, 5000)
    const controls = createControls(camera, renderer)

    //draw planets / Orbits / Halo
    planets.forEach(planet => {
        scene.add(planet.mesh);
        let path = './3D_texture/' + planet.name + ".glb"
        planet.load_3D_model(path, ischeck("checkbox_planete_realiste"));
        planet.createName()
        //soleil case
        const orbite = planet.drawOrbite()
        if(orbite != undefined) scene.add(orbite);
        const halo = planet.createHalo()
        if(halo != undefined) scene.add(halo)
    });
    add_menu_planete(planets)
    
    //draw stars
    const Star = createStars(3000, 200)
    scene.add(Star)

    //Raycasting
    const pointer = createPointer()
    const raycaster = createRaycaster()
    detect_click(pointer)
    
    // create Light (for the 3d asset)
    const light = createLight(100)
    scene.add(light)
    const camera_light = createLight(0.02)
    scene.add(camera_light)

    const labelRenderer = createTextRenderer()
    document.body.appendChild(labelRenderer.domElement);

    change_methode(planets, scene);


    let time = 0
    /**
     * Boucle de rendu exécutée à chaque frame.
     * @param {number} t - Le temps écoulé depuis le lancement de la page.
     */
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
            const halo = planet.updateHalo(ischeck("checkbox_planete_realiste"), camera)
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
        const followplanet = document.getElementById("planet_select").value
        if (followplanet != "Soleil") camerafollow(followplanet, camera, planets, controls)

        reset_pointer(pointer)

        camera_light.position.copy(camera.position)

        if(time%60 == 0) menu(planets, Star)
        labelRenderer.render(scene, camera);
        renderer.render(scene, camera);
        controls.update();
        time += 1
    }
    animate(0)
}

/**
 * Gère l'affichage (visible/invisible) des différents éléments de la scène selon les Checkboxs.
 * @param {Planete[]} planets - Le tableau contenant toutes les planetes.
 * @param {THREE.Points} Star - Le nuage de points représentant les etoiles.
 */
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

/**
 * Vérifie si une case à cocher (checkbox) HTML est cochée ou non.
 * @param {string} id - L'identifiant HTML de la case à cocher.
 * @returns {boolean} True si cochée, False sinon.
 */
function ischeck(id) {
    return document.getElementById(id).checked
}

/**
 * Redirige l'utilisateur vers la page de la planète cliquée.
 * @param {THREE.Mesh} mesh - Le mesh 3D de la planète cliquée.
 */
function OnclickPlanet(mesh) {
    console.log(mesh.userData.planet.name.toLowerCase());
    const url = `planete_screen/planete.html?cible=${mesh.userData.planet.name.toLowerCase()}`;
    window.location.href = url;
}

/**
 * Associe les boutons de l'interface aux différentes méthodes mathématiques d'intégration (et vrai taille).
 * @param {Planete[]} planets - Le tableau des planetes de la scène.
 * @param {Object} data_pos - Les données JSON des trajectoires.
 * @param {THREE.Scene} scene - La scène principale.
 */
function change_methode(planets, scene) {
    document.getElementById("radio_euler")?.addEventListener("change", () => appliquerMethode("euler"));
    document.getElementById("radio_eulerassym")?.addEventListener("change", () => appliquerMethode("eulerAssym"));
    document.getElementById("radio_rk4")?.addEventListener("change", () => appliquerMethode("RK4"));
    document.getElementById("checkbox_planete_realiste")?.addEventListener("change", () => reload3D(planets, scene));
}

/**
 * Applique la nouvelle méthode mathématique.
 * @param {string} Method - Le nom de la méthode.
 */
function appliquerMethode(Method) {
    window.location.href = window.location.pathname + "?method=" + Method;
}

/**
 * Recharge les modèles 3D des planètes pour passer de la taille réelle à la taille agrandi.
 * @param {Planete[]} planets - Le tableau des planetes.
 * @param {THREE.Scene} scene - La scène 3D.
 */
function reload3D(planets, scene) {
    planets.forEach(planet => {
        if (planet.modele3D) {
            planet.mesh.remove(planet.modele3D);
            planet.modele3D = null
        }
        let path = './3D_texture/' + planet.name + ".glb"
        planet.load_3D_model(path, ischeck("checkbox_planete_realiste"));
    });
}

function add_menu_planete(planets) {
    const menu = document.getElementById("planet_select");

    planets.forEach(planet => {
        if (planet.name !== "Soleil") {
            const option = document.createElement("option");
            option.value = planet.name;
            option.textContent = planet.name;
            menu.appendChild(option);
        }
    });
}

function camerafollow(followplanet, camera, planets, controls){
    planets.forEach(planet => {
        if(planet.name == followplanet) {
            const direction = new THREE.Vector3();
            const sun_pos = new THREE.Vector3(1, 0, 1)
            direction.subVectors(camera.position, sun_pos).normalize();
            camera.position.copy(planet.camera_pos).addScaledVector(direction, planet.camera_recul);
            controls.target.copy(planet.camera_pos);
            controls.update();
        }
    })
}

main();