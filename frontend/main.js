const elems = document.getElementsByClassName("grid");

// let vw = window.innerWidth;
// let vh = window.innerHeight;
let vw = document.documentElement.clientWidth;
let vh = document.documentElement.clientHeight;
let vmin = Math.min(vw, vh);

let cell_size_prop = 3;
let cell_size = vmin * cell_size_prop / 100;
let col_cells = Math.floor(vw / cell_size) - 1;
let row_cells = Math.floor(vh / cell_size) - 3;
console.log("Columns: ", col_cells);
console.log("Rows: ", row_cells);
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
function runJPS(){
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

function drawPath(path, start_cell){
  // cells are in order from start to goal, not including start
  if(path == null){
    return;
  }
  clearPath();
  let last_cell = new Cell(
    parseInt(start_cell.getAttribute("x")),
    parseInt(start_cell.getAttribute("y")));
  document.querySelector(`[x="${last_cell.x}"][y="${last_cell.y}"]`)
  .setAttribute("path", true);
  for(i=0; i<path.length; i++){
    let pivot_cell = new Cell(path[i]["x"], path[i]["y"]);
    let dir = Cell.dir(Cell.sub(pivot_cell, last_cell));
    last_cell = Cell.add(last_cell, dir);
    document.querySelector(`[x="${last_cell.x}"][y="${last_cell.y}"]`)
      .setAttribute("path", true);
    while(!Cell.equals(last_cell, pivot_cell)){
      last_cell = Cell.add(last_cell, dir);
      document.querySelector(`[x="${last_cell.x}"][y="${last_cell.y}"]`)
      .setAttribute("path", true);
    }
  }
}

function allowDrop(e) {
  // location can't be wall nor start/goal
  if(e.target.tagName.toLowerCase() == "div"
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
  var data = e.dataTransfer.getData("text");
  // when moving start or goal, remove previous cell attribute to false and new to true
  const elem = document.getElementById(data);
  elem.parentElement.removeAttribute(elem.id);
  e.target.appendChild(document.getElementById(data));
  e.target.setAttribute(elem.id, true);
}