async function loadData(){

let r=await fetch(
"http://localhost:5000/api/water/latest"
);

let d=await r.json();

document.getElementById("device").innerHTML=d.device;

document.getElementById("liters").innerHTML=d.liters;

document.getElementById("flow").innerHTML=d.flow;

document.getElementById("time").innerHTML=d.time;

}

async function loadStat(){

let r=await fetch(
"http://localhost:5000/api/stat"
);

let d=await r.json();

document.getElementById("total").innerHTML=d.total;

document.getElementById("max_flow").innerHTML=d.max_flow;

}

loadData();

loadStat();

setInterval(function(){

loadData();

loadStat();

},5000);
