$fn=90;

cubeThickness = 8;
cubeSideLength = 24;

difference(){
	cube([cubeSideLength, cubeSideLength, cubeThickness]);
	holeDiameter = 3.8;
	holeBorderDistance = 5;
	#translate([holeBorderDistance, cubeSideLength-holeBorderDistance, -0.5])
		cylinder(h=cubeThickness+1, d=holeDiameter);
	#translate([cubeSideLength-holeBorderDistance, cubeSideLength-holeBorderDistance, -0.5])
		cylinder(h=cubeThickness+1, d=holeDiameter);
}

rodDiameter = 4.8;
rodLength = 7.7;
rodDistance = 8;

translate([0, 5, cubeThickness]){
	numberOfRods = 3;
	firstRodX = (cubeSideLength - (numberOfRods-1)*rodDistance)/2;
	translate([firstRodX, 0, 0])
		cylinder(h=rodLength, d=rodDiameter);
	translate([firstRodX+rodDistance, 0, 0])
		cylinder(h=rodLength, d=rodDiameter);
	translate([firstRodX+2*rodDistance, 0, 0])
		cylinder(h=rodLength, d=rodDiameter);
}

