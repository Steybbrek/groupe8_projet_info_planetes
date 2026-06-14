import * as THREE from 'three';

function detect_click(pointer) {
    let startX = 0;
    let startY = 0;
    window.addEventListener('pointerdown', (event) => {
        startX = event.clientX;
        startY = event.clientY;
    });
    window.addEventListener('pointerup', (event) => {
        const diffX = Math.abs(event.clientX - startX);
        const diffY = Math.abs(event.clientY - startY);
        if (diffX < 5 && diffY < 5) {
            onPointerClick(event, pointer);
        }
    });
}

function reset_pointer(pointer) {
    pointer.x = -1000
    pointer.y = -1000
}

function createRaycaster() {
    const raycaster = new THREE.Raycaster()
    return raycaster
}

function onPointerClick(event, pointer) {
    pointer.x = ( event.clientX / window.innerWidth ) * 2 - 1;
    pointer.y = - ( event.clientY / window.innerHeight ) * 2 + 1;
}

function createPointer() {
    const pointer = new THREE.Vector2(-1000, -1000);
    return pointer
}

export {createRaycaster, detect_click, createPointer, reset_pointer}