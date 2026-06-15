import * as THREE from 'three';
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import { CSS2DRenderer, CSS2DObject } from 'three/addons/renderers/CSS2DRenderer.js';

/**
 * Initialise le moteur de rendu WebGL et l'ajoute au document HTML.
 * @param {number} w - La largeur du canevas de rendu.
 * @param {number} h - La hauteur du canevas de rendu.
 * @returns {THREE.WebGLRenderer} Le moteur de rendu (avec antialiasing).
 */
function createRenderer(w, h) {
    const renderer = new THREE.WebGLRenderer({"antialias" : true});
    renderer.setSize(w, h);
    document.body.appendChild(renderer.domElement);
    return renderer;
}

/**
 * Crée la scene principal de l'univers 3D.
 * @returns {THREE.Scene} La scene.
 */
function createScene() {
    const scene = new THREE.Scene();
    return scene;
}

/**
 * Configure une camera avec une perspective réaliste.
 * @param {number} fov - Le champ de vision en degrés.
 * @param {number} aspect - Le ratio de la fenêtre (largeur / hauteur).
 * @param {number} near - La distance de rendu minimale.
 * @param {number} far - La distance de rendu maximale.
 * @returns {THREE.PerspectiveCamera} La camera.
 */
function createCamera(fov, aspect, near, far) {
    const camera = new THREE.PerspectiveCamera(fov, aspect, near, far);
    camera.position.z = 50;
    return camera;
}

/**
 * Ajoute les contrôles à la souris permettant de pivoter, zoomer et se déplacer autour du centre.
 * @param {THREE.Camera} camera - La camera.
 * @param {THREE.WebGLRenderer} renderer - Le moteur de rendu.
 * @returns {OrbitControls} Les contrôles avec inertie.
 */
function createControls(camera, renderer) {
    const controls = new OrbitControls(camera, renderer.domElement)
    controls.enableDamping = true;
    return controls;
}

/**
 * Génère un décor étoilé sphérique en utilisant un système de points.
 * @param {number} nb - Le nombre d'étoiles à générer.
 * @param {number} distance_min - La distance minimale à laquelle les étoiles commencent à apparaître.
 * @returns {THREE.Points} L'objet 3D contenant toutes les étoiles.
 */
function createStars(nb, distance_min) {
    const geo = new THREE.BufferGeometry;
    const position_star = new Float32Array(nb * 3)
    const vecteur = new THREE.Vector3
    for(let i = 0; i < nb; i++) {
        vecteur.randomDirection();
        const distance = distance_min + (Math.random() * 2000);
        vecteur.multiplyScalar(distance);

        position_star[i * 3] = vecteur.x;
        position_star[i * 3 + 1] = vecteur.y;
        position_star[i * 3 + 2] = vecteur.z;
    };
    geo.setAttribute('position', new THREE.BufferAttribute(position_star, 3));
    const mat = new THREE.PointsMaterial({
        color: 0xFFFFFF,
        sizeAttenuation: true
    });
    const star = new THREE.Points(geo, mat);
    return star;
}

/**
 * Crée une lumière au centre de la scène (comme le Soleil).
 * @returns {THREE.PointLight} La source de lumière.
 */
function createLight() {
    const color = 0xFFFFFF;
    const intensity = 1000;
    const light = new THREE.PointLight(color, intensity);
    light.position.set(0, 0, 0);
    return light;
}

/**
 * Initialise un moteur de rendu dédié à l'affichage du texte.
 * @returns {CSS2DRenderer} Le moteur de rendu HTML (avec pointerEvents = 'none').
 */
function createTextRenderer() {
    const labelRenderer = new CSS2DRenderer();
    labelRenderer.setSize(window.innerWidth, window.innerHeight);
    labelRenderer.domElement.style.position = 'absolute';
    labelRenderer.domElement.style.top = '0px';
    //Raycasting (Laisse passer le clic à travers la div invisible vers le canvas 3D)
    labelRenderer.domElement.style.pointerEvents = 'none'; 
    return labelRenderer;
}

/**
 * À FAIRE : Fonction de génération d'astéroïdes.
 * @param {number} nb - Le nombre d'astéroïdes.
 * @param {THREE.Vector3|Array} position - Les coordonnées de base ou l'orbite.
 */
function createAsteroid(nb, position) {

}

export {createScene, createCamera, createRenderer, createControls, createStars, createLight, createTextRenderer};