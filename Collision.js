import * as THREE from 'three';

function createRaycaster() {
    const raycaster = new THREE.Raycaster()
    return raycaster
}

function onPointerMove(event, pointer) {
    pointer.x = ( event.clientX / window.innerWidth ) * 2 - 1;
    pointer.y = - ( event.clientY / window.innerHeight ) * 2 + 1;
}

function createPointer() {
    const pointer = new THREE.Vector2(-1000, -1000);
    return pointer
}

export {createRaycaster, onPointerMove, createPointer}