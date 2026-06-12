import * as THREE from 'three';
import { OrbitControls } from "three/addons/controls/OrbitControls.js";

function createRenderer(w, h) {
    const renderer = new THREE.WebGLRenderer({"antialias" : true});
    renderer.setSize(w, h);
    document.body.appendChild(renderer.domElement);
    return renderer
}

function createScene() {
    const scene = new THREE.Scene();
    return scene;
}

function createCamera(fov, aspect, near, far) {
    const camera = new THREE.PerspectiveCamera(fov, aspect, near, far);
    camera.position.z = 50;
    return camera;
}

function createControls(camera, renderer) {
    const controls = new OrbitControls(camera, renderer.domElement)
    controls.enableDamping = true;
    return controls;
}

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
    geo.setAttribute( 'position', new THREE.BufferAttribute(position_star, 3));
    const mat = new THREE.PointsMaterial({
        color: 0xFFFFFF,
        sizeAttenuation: true
    });
    const star = new THREE.Points(geo, mat);
    return star
}

function createLight() {
    const color = 0xFFFFFF;
    const intensity = 1000;
    const light = new THREE.PointLight(color, intensity);
    light.position.set(0, 0, 0);
    return light
}

export {createScene, createCamera, createRenderer, createControls, createStars, createLight};