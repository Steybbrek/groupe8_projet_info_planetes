class Planete {
    constructor(bodie, position) {
        this.id = bodie.id;
        this.name = bodie.name;
        this.radius = bodie.radius_display;
        this.color = bodie.color
        this.position = position
    }

    print() {
        console.log(`${this.id} : Name = ${this.name} | Radius = ${this.radius} | Color = ${this.color} | Position = ${this.position[0][0]}`);
    }

    get_id() {
        return this.id;
    }

    get_name() {
        return this.name;
    }

    get_radius() {
        return this.radius;
    }

    get_color() {
        return this.color;
    }

    get_position(i) {
        return this.position[i]
    }
}

function fetchJSONData(url) {
    return fetch(url)
        .then(response => {
            if (!response.ok) {
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
            return response.json();  
        }); 
}

function createPlanets(data) {
    console.log(data.info)
    let planets = [];

    // A MODIFIER quand j'aurais le vrai format json
    for (let i = 0; i < 1; i++) {
        planets.push(new Planete(data.info, data.mercuryRK));
    }
    return planets;
}

async function test() {
    try {
        const data = await fetchJSONData("./Data.json"); 
        const planets = createPlanets(data);
        planets.forEach(planet => planet.print());
        console.log("DATA :", data)
        console.log("Bodies:", data.bodies); 
        console.log("Meta:", data.meta);
        
    } catch (error) {
        console.error('Failed to process data:', error);
    }
}

export {fetchJSONData, createPlanets, Planete};