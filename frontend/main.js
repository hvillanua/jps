const elems = document.getElementsByClassName("grid");

// let vw = window.innerWidth;
// let vh = window.innerHeight;
let vw = document.documentElement.clientWidth;
let vh = document.documentElement.clientHeight;
let vmin = Math.min(vw, vh);

var mouseDownID = -1;
var mouseAction = null;

let cell_size_prop = 3;
let cell_size = vmin * cell_size_prop / 100;
// TODO: figure out how many rows and cols we can fit
let col_cells = Math.floor(vw / cell_size) - 1;
let row_cells = Math.floor(vh / cell_size) - 4;
// console.log("Columns: ", col_cells);
// console.log("Rows: ", row_cells);
for(i=0; i<row_cells; i++){
  for(j=0; j<col_cells; j++){
    let cell = document.createElement("div");
    cell.className = "cell";
    cell.id = i * col_cells + j;
    cell.setAttribute("x", j);
    cell.setAttribute("y", i);
    cell.setAttribute("path", false);
    cell.setAttribute("wall", false);
    cell.ondrop = (e) => {drop(e);};
    cell.ondragover = (e) => {allowDrop(e);};
    cell.onmousedown = (e) => {mouseDown(e);};
    cell.onmouseup = (e) => {mouseUp(e);};
    cell.onmouseover = (e) => {mouseOver(e);};
    //touchstart
    elems[0].appendChild(cell);
  }
}
elems[0].style.setProperty("grid-template-columns", `repeat(${col_cells}, ${cell_size_prop}vmin)`);
elems[0].style.setProperty("grid-template-rows", `repeat(${row_cells}, ${cell_size_prop}vmin)`);

let default_start;
let default_goal;

let imgs = ["resources/start.png", "resources/goal.png"];
for(i=0; i<imgs.length; i++){
  let col;
  if(i%2 == 0){
    col = Math.floor((col_cells / 2) - 0.2 * col_cells);
  }
  else{
    col = Math.floor((col_cells / 2) + 0.2 * col_cells);
  }
  let row = Math.floor((row_cells / 2));
  let id = col + row * col_cells;
  let img = document.createElement("img")
  let cell = document.getElementById(id);
  if(i%2 == 0){
    cell.setAttribute("start", true);
    default_start = cell;
    img.id = "start";
  }
  else{
    cell.setAttribute("goal", true);
    default_goal = cell;
    img.id = "goal";
  }
  img.src = imgs[i];
  img.width = 0.85 * cell_size;
  img.height = 0.85 * cell_size;
  img.draggable = true;
  img.ondragstart = (e) => {drag(e);};
  cell.appendChild(img);
}

const api = new URL("http://127.0.0.1:18080");

function sendGrid(){
  let url = new URL("/grid", api);
  const walls = Array.from(document.querySelectorAll("[wall=true]")).map(elem => {
    return new Cell(parseInt(elem.getAttribute("x")), parseInt(elem.getAttribute("y")));
  });
  const json_data = {
    "width": col_cells,
    "height": row_cells,
    "walls": walls};
  fetch(url, {method: "PUT", body: JSON.stringify(json_data)})
  .then(response => {
    return response.json();
  })
  .then(json => {
    requestRun();
  });
}

function requestRun(){
  let url = new URL("/run", api);
  let start_cell = document.querySelector("[start=true]");
  let goal_cell = document.querySelector("[goal=true]");
  url.searchParams.append("startx", start_cell.getAttribute("x"));
  url.searchParams.append("starty", start_cell.getAttribute("y"));
  url.searchParams.append("goalx", goal_cell.getAttribute("x"));
  url.searchParams.append("goaly", goal_cell.getAttribute("y"));
  fetch(url, {method: "GET"})
  .then(response => {
    return response.json();
  })
  .then(json => {
    drawPath(json["path"], start_cell);
  });
}

function runJPS(){
  sendGrid();
}

function clearGrid(){
  let url = new URL("grid/clear", api);
  url.searchParams.append("width", col_cells);
  url.searchParams.append("height", row_cells);
  fetch(url, {method: "PUT"})
  .then(response => {
    return response.json();
  })
  .then(json => {
    clearPath();
    restoreLocations();
    clearWalls();
  });
}

class Cell {
  constructor(x, y) {
    this.x = x;
    this.y = y;
  }
  static add(a, b){
    return new Cell(a.x+b.x, a.y+b.y);
  }

  static sub(a, b){
    return new Cell(a.x-b.x, a.y-b.y);
  }

  static dir(a){
    return new Cell(a.x>0 ? 1 : (a.x<0 ? -1 : 0), a.y>0 ? 1 : (a.y<0 ? -1 : 0))
  }

  static equals(a, b){
    return a.x === b.x && a.y === b.y;
  }
}

function restoreLocations(){
  let start_cell = document.querySelector("[start=true]");
  start_cell.removeAttribute("start");
  let goal_cell = document.querySelector("[goal=true]");
  goal_cell.removeAttribute("goal");
  default_start.setAttribute("start", true);
  default_start.appendChild(start_cell.firstChild);
  default_goal.setAttribute("goal", true);
  default_goal.appendChild(goal_cell.firstChild);
}

function clearPath(){
  document.querySelectorAll("[path=true]").forEach(elem => {
    elem.setAttribute("path", false);
  });
}

function clearWalls(){
  document.querySelectorAll("[wall=true]").forEach(elem => {
    elem.setAttribute("wall", false);
  })
}

let interval;

function drawPath(path, start_cell){
  // cells are in order from start to goal, not including start
  if(path == null){
    return;
  }
  clearPath();
  let array = [];
  let last_cell = new Cell(
    parseInt(start_cell.getAttribute("x")),
    parseInt(start_cell.getAttribute("y")));
  array.push(document.querySelector(`[x="${last_cell.x}"][y="${last_cell.y}"]`));
  for(i=0; i<path.length; i++){
    let pivot_cell = new Cell(path[i]["x"], path[i]["y"]);
    let dir = Cell.dir(Cell.sub(pivot_cell, last_cell));
    last_cell = Cell.add(last_cell, dir);
    array.push(document.querySelector(`[x="${last_cell.x}"][y="${last_cell.y}"]`));
    while(!Cell.equals(last_cell, pivot_cell)){
      last_cell = Cell.add(last_cell, dir);
      array.push(document.querySelector(`[x="${last_cell.x}"][y="${last_cell.y}"]`));
    }
  }
  interval = setInterval(drawPathAnimate, 50, array);
}

function drawPathAnimate(path){
  if(path.length == 0){
    clearInterval(interval);
  }
  else{
    let cell_elem = path.shift()
    cell_elem.setAttribute("path", true);
  }
}

function allowDrop(e) {
  // location can't be wall nor start/goal
  if(e.target.tagName.toLowerCase() === "div"
    && e.target.getAttribute("wall") === "false"
    && e.target.getAttribute("start") == null
    && e.target.getAttribute("goal") == null){
      e.preventDefault();
    }
}

function drag(e) {
  e.dataTransfer.setData("text", e.target.id);
}

function drop(e) {
  e.preventDefault();
  let data = e.dataTransfer.getData("text");
  if(data === ""){
    return;
  }
  // when moving start or goal, remove previous cell attribute to false and new to true
  const elem = document.getElementById(data);
  elem.parentElement.removeAttribute(elem.id);
  e.target.appendChild(document.getElementById(data));
  e.target.setAttribute(elem.id, true);
  mouseDownID = -1;
}

function isToggleWall(elem){
  return elem.tagName.toLowerCase() === "div"
    && elem.getAttribute("start") == null
    && elem.getAttribute("goal") == null;
}

function mouseDown(e) {
  if(mouseDownID == -1){
    if(isToggleWall(e.target) && e.target.getAttribute("wall") === "false"){
      mouseAction = "put";
    }
    else if(isToggleWall(e.target) && e.target.getAttribute("wall") === "true"){
      mouseAction = "remove";
    }
    clickWall(e);
    mouseDownID = 1;
  }
}

function mouseUp(e) {
  //Only stop if exists
  if(mouseDownID != -1) {
    mouseDownID = -1;
    mouseAction = null;
  }
}

function mouseOver(e) {
  if(mouseDownID != -1) {
    if(isToggleWall(e.target)){
      clickWall(e);
    }
  }
}

function clickWall(e) {
  if(mouseAction === "put"){
    e.target.setAttribute("wall", true);
  }
  else if(mouseAction === "remove"){
    e.target.setAttribute("wall", false);
  }
}