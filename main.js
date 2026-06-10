import {fetchJSONData, createPlanets, Planete} from "./loader.js";

async function main(animate = false) {
    const data = await fetchJSONData('./Data.json');
    const planets = createPlanets(data);
    planets.forEach(planet => planet.print());
}

main();