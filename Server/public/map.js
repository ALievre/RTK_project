import { initializeApp } from "https://www.gstatic.com/firebasejs/9.6.2/firebase-app.js";
import { getDatabase, ref, onValue } from 'https://www.gstatic.com/firebasejs/9.6.2/firebase-database.js';
const firebaseConfig = {

    apiKey: "AIzaSyAfZm7f4_eq7XbNIClE8KJR0LaNkCOb-vg",
    authDomain: "rtk-not-lost.firebaseapp.com",
    databaseURL: "https://rtk-not-lost-default-rtdb.europe-west1.firebasedatabase.app",
    projectId: "rtk-not-lost",
    storageBucket: "rtk-not-lost.appspot.com",
    messagingSenderId: "614359693080",
    appId: "1:614359693080:web:cfce4bff04022e1f9e106d"

};

const firebase = initializeApp(firebaseConfig);
const database = getDatabase(firebase);
var position = new Object();
var longitude =0;
var latitude =0;

var base_long =0.0;
var base_lat=0.0;
console.log(database);
const startCountRef = ref(database, '/RTK_corrected_position');
onValue(startCountRef, (snapshot) => {
    const data = snapshot.val();
    position = data;
    console.log(data);
    longitude = parseFloat(data.longitude);
    latitude = parseFloat(data.latitude);
    // map.getSource('rover').setData(pointOnCircle(longitude,latitude));
})

mapboxgl.accessToken = 'pk.eyJ1IjoiYXNzaWFuZ3V5ZW4iLCJhIjoiY2t5ZDB2ZnkyMHVnbTJ2cGJ1MnE0MDQ1MiJ9.2prkgS_zZ1OEtyjTMe2vtw';
const map = new mapboxgl.Map({
    container: 'map', // container ID
    style: 'mapbox://styles/mapbox/satellite-streets-v11', // style URL
    center: [1.444209, 43.604652], // starting position on Toulouse[lng, lat]
    zoom: 11 // starting zoom
});

// position.longitude = "1.444209";
// position.latitude = "43.604652"

function pointOnCircle(lng, lat) {
    return {
        'type': 'Point',
        'coordinates': [lng, lat],
    };
}

map.on('load', ()=>{
    
    map.addSource('rover', {
        'type': 'geojson',
        'data': pointOnCircle(parseFloat(position.longitude), parseFloat(position.latitude))
    });

    map.addSource('base', {
        'type': 'geojson',
        'data': pointOnCircle(parseFloat(base_long), parseFloat(base_lat))
    });
    map.addLayer({
        'id' : 'rover',
        'source' : 'rover',
        'type' : 'circle',
        'paint' :{
            'circle-radius' : 6,
            'circle-color' : '#F44336' 
        }
    })

    map.addLayer({
        'id' : 'base',
        'source' : 'base',
        'type' : 'circle',
        'paint' :{
            'circle-radius' : 8,
            'circle-color' : '#2986cc' 
        }
    })
    setInterval(()=>{
        map.getSource('rover').setData(pointOnCircle(longitude,latitude));
        map.getSource('base').setData(pointOnCircle(base_long,base_lat));
    },100);
    

})

$(".send").on('click', ()=>{
    base_lat = parseFloat($('.latitude-i')[0].value)
    base_long= parseFloat($('.longitude-i')[0].value)
})

