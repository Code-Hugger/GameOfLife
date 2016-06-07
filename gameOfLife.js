/* 
* Conway's game of life in javascript
* Usage: include this script at then end of an HTML file with a canvas element with ID 'canvasGOL'.
* - Must be included at end because the HTML document must be loaded before the script executes.
*
* Thanks to Mr. Doob for teaching me everything I know about JS animation:
* http://mrdoob.com/lab/javascript/requestanimationframe/
*/

var canvas, context, imageData, new_map, width, height;

init();
animate();

// Get canvas element, array representation of pixels, and dimensions of canvas
function init(){
	canvas = document.getElementById("canvasGOL");
	width = canvas.width;
	height = canvas.height;

	context = canvas.getContext("2d");
	imageData = context.getImageData(0, 0, width, height);

	// Need some memory allocated so that a new map can be created on each iteration before being drawn on canvas
	new_map = new Array(window.width);
	for(x = 0; x < width; x++){
		new_map[x] = new Array(height);
		for(y = 0; y < height; y++){
			new_map[x][y] = 0;
		}
	}

	// Call an initialization function to set the initial pixels on the canvas
	//initBlinker();
	//initRandom();
	//initGossiperGun();
	initGossiperArray();
	updateCanvas();
}

function animate(){
	requestAnimationFrame(animate);
	draw();
}

// This function loops continuously after the call to the animate() function
function draw(){
	iterate();
	updateCanvas();
}

// Helper function to turn on a single pixel in the canvas at position (x, y)
function setPX(x, y) {
    var index = (x + y * width) * 4;
    imageData.data[index + 0] = 0; // red
    imageData.data[index + 1] = 255; // green
    imageData.data[index + 2] = 0; // blue
    imageData.data[index + 3] = 255; // a
}

// Helper function to turn off a single pixel in the canvas at position (x, y)
function unsetPX(x, y){
	var index = (x + y * width) * 4;
    imageData.data[index + 0] = 0; // red
    imageData.data[index + 1] = 0; // green
    imageData.data[index + 2] = 0; // blue
    imageData.data[index + 3] = 255; // a
}

// Helper function to determine if a single pixel in the canvas is turned on or off
function issetPX(x, y){
	// Return 1 if cell at x, y is alive; 0 if x, y is dead
	if(imageData.data[(((x + y * width) * 4) + 1)] == 255){
		return 1;
	}else{
		return 0;
	}
}

// Helper function to propogate changes in the imageData array to the HTML canvas
function updateCanvas(){
	context.putImageData(imageData, 0, 0);
}

// Main game of life iteration function. Kills/Grows cells based on Conway's rules
function iterate(){
	// Note: we actually iterate over every cell except for a single layer of cells around the margin
	for(x = 1; x < window.width - 1; x++){
		for(y = 1; y < window.height - 1; y++){
			// For each cell, determine how many of its eight neighbors are alive
			var live_neighbors = 0;
			live_neighbors += issetPX(x + 1, y);
			live_neighbors += issetPX(x - 1, y);
			live_neighbors += issetPX(x, y + 1);
			live_neighbors += issetPX(x, y - 1);
			live_neighbors += issetPX(x + 1, y + 1);
			live_neighbors += issetPX(x - 1, y + 1);
			live_neighbors += issetPX(x + 1, y - 1);
			live_neighbors += issetPX(x - 1, y - 1);

			if(live_neighbors < 2){ // If cell has less than 2 living neighbors, it dies
				new_map[x][y] = 0;
			}else if(live_neighbors > 3){ // If cell has greater than 3 living neighbors, it dies
				new_map[x][y] = 0;

			}else if(live_neighbors == 3){ // If an empty space has 3 living neighbors, a new cell is created
				new_map[x][y] = 1;

			}else{ // If any space has 2 neighbors, it stays in its last state
				new_map[x][y] = issetPX(x, y);
			}

		}
	}

	// The entire next generation has been stored in the new_map array, changes are propagated to canvas
	for(x = 1; x < width - 1; x++){
		for(y = 1; y < height - 1; y++){
			if(new_map[x][y] == 0){
				unsetPX(x, y);
			}else if(new_map[x][y] == 1){
				setPX(x, y);
			}
		}
	}
}

// init functions belllow here.
// CAUTION, these may break when canvas size is very small. Should probably implement bounds checking...

// A simple 'blinker' oscillator of only three pixels
function initBlinker(){
	setPX(50, 50);
	setPX(51, 50);
	setPX(52, 50);
}

// Randomly turns on a portion of the cells
function initRandom(){
	var portion_alive = 0.75; // portion of cells initialized alive (1 = %100, 0 = %0)

	for(x = 0; x < width; x++){
		for(y = 0; y < height; y++){
			if(Math.random() < portion_alive){ // Pseudo-random coinflip
				setPX(x, y);
			}
		}
	}
}

// A single Gossiper Gun
function initGossiperGun(){
	var coords = [[5, 9], [6, 9], [5, 10], [6, 10],
	[17, 7], [18, 7], [16, 8], [15, 9], [15, 10], [15, 11], [16, 12], [17, 13], [18, 13],
	[19, 10],
	[20, 8], [21, 9], [21, 10], [21, 11], [20, 12], [22, 10],
	[25, 7], [25, 8], [25, 9], [26, 7], [26, 8], [26, 9], [27, 6], [27, 10],
	[29, 6], [29, 5], [29, 10], [29, 11],
	[39, 7], [39, 8], [40, 7], [40, 8]];

	for(idx = 0; idx < coords.length; idx++){
		setPX(coords[idx][0], coords[idx][1]);
	}
}

// Create 5 Gossiper Guns in a horizontal row at the top
// This will DEFINITELY break if the canvas gets smaller; TODO: compute number of guns in array based on width.
function initGossiperArray(){
	var guns_in_array = 10;
	var coords = [[5, 9], [6, 9], [5, 10], [6, 10],
	[17, 7], [18, 7], [16, 8], [15, 9], [15, 10], [15, 11], [16, 12], [17, 13], [18, 13],
	[19, 10],
	[20, 8], [21, 9], [21, 10], [21, 11], [20, 12], [22, 10],
	[25, 7], [25, 8], [25, 9], [26, 7], [26, 8], [26, 9], [27, 6], [27, 10],
	[29, 6], [29, 5], [29, 10], [29, 11],
	[39, 7], [39, 8], [40, 7], [40, 8]];

	for(jdx = 0; jdx < guns_in_array; jdx++){
		for(idx = 0; idx < coords.length; idx++){
			setPX(coords[idx][0], coords[idx][1]);
			coords[idx][0] += 50;
		}
	}
}