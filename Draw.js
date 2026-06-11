import * as THREE from 'three';
import { OrbitControls } from "jsm/controls/OrbitControls.js";

function createRenderer(w, h) {
    const renderer = new THREE.WebGLRenderer();
    renderer.setSize(w, h);
    document.body.appendChild(renderer.domElement);
    return renderer
}

function createScene() {
    const scene = new THREE.Scene();
    return scene
}

function createCamera(fov, aspect, near, far) {
    const camera = new THREE.PerspectiveCamera(fov, aspect, near, far);
    camera.position.z = 50;
    return camera
}

function createControls(camera, renderer) {
    const controls = new OrbitControls(camera, renderer.domElement)
    controls.enableDamping = true;
    return controls
}

function moveCamera(camera) {
    camera.position.z = 3;
    return camera
}

export {createScene, createCamera, createRenderer, createControls};