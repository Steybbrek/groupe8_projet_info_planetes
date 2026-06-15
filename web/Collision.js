import * as THREE from 'three';

/**
 * Écoute les événements de la souris pour détecter un clic.
 * @param {THREE.Vector2} pointer - Le vecteur 2D qui stocke les coordonnées du clic.
 */
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

/**
 * Réinitialise la position du curseur en dehors de l'écran.
 * Empêche le Raycaster de continuer à détecter la planète à chaque frame après que le premier clic.
 * @param {THREE.Vector2} pointer - Le vecteur 2D du curseur à réinitialiser.
 */
function reset_pointer(pointer) {
    pointer.x = -1000;
    pointer.y = -1000;
}

/**
 * Crée un Raycaster utilisé pour détecter les collisions entre la souris et les objets 3D.
 * @returns {THREE.Raycaster} L'objet Raycaster.
 */
function createRaycaster() {
    const raycaster = new THREE.Raycaster();
    return raycaster;
}

/**
 * Convertit les coordonnées du clic en pixels vers des coordonnées normalisées.
 * @param {PointerEvent} event - L'événement de la souris contenant les coordonnées X et Y.
 * @param {THREE.Vector2} pointer - Le vecteur 2D mis à jour.
 */
function onPointerClick(event, pointer) {
    // Calcul mathématique pour passer d'une grille en pixels à une grille de [-1, 1]
    pointer.x = ( event.clientX / window.innerWidth ) * 2 - 1;
    pointer.y = - ( event.clientY / window.innerHeight ) * 2 + 1;
}

/**
 * Initialise le vecteur qui stockera la position de la souris.
 * @returns {THREE.Vector2} Le vecteur 2D initialisé hors champ.
 */
function createPointer() {
    const pointer = new THREE.Vector2(-1000, -1000);
    return pointer;
}

export {createRaycaster, detect_click, createPointer, reset_pointer};