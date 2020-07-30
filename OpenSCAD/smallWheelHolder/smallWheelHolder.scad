$fn=90;

outterDiameter = 12;
overallHeight = 24;
difference(){
	cylinder(h=overallHeight, d1=outterDiameter, d2=outterDiameter-5);
	#cylinder(h=14, d=6);
}

cubeLength = 3*outterDiameter;
cubeWidth = outterDiameter;
cubeHeight = 3;
difference(){
	translate([-0.5*cubeLength, -0.5*cubeWidth, overallHeight-cubeHeight])
		cube([cubeLength, cubeWidth, cubeHeight]);
	translate([0.5*cubeLength-5, 0, overallHeight-cubeHeight])
		#cylinder(h=cubeHeight, d=3.5);
	translate([-0.5*cubeLength+5, 0, overallHeight-cubeHeight])
		#cylinder(h=cubeHeight, d=3.5);
}

transitionHeight=4;
translate([0,0,overallHeight-transitionHeight-cubeHeight])
	cylinder(h=transitionHeight, d1=outterDiameter-5, d2=outterDiameter);
