$fn=90;

outterDiameter = 12;
overallHeight = 24;
difference(){
	union(){
		cylinder(h=overallHeight, d=outterDiameter);
		transitionHeight=4;
		translate([0,0,overallHeight-0.1])
			hull(){
				translate([0, 0, -20])
					cylinder(h=0.1,d=outterDiameter);
				translate([-2.5*outterDiameter/2, -outterDiameter/2, 0])
					cube([2.5*outterDiameter, outterDiameter, 0.1]);
					//cylinder(h=0.1, d=outterDiameter);
			}
	}
	#cylinder(h=14, d=6.05);
	#cylinder(h=4, d=6.3);
}

screwHoleDiameter = 3.5;
cubeLength = 30+2*screwHoleDiameter+10+2;
cubeWidth = outterDiameter;
cubeHeight = 3;
difference(){
	translate([-0.5*cubeLength, -0.5*cubeWidth, overallHeight-cubeHeight])
		cube([cubeLength, cubeWidth, cubeHeight]);
	translate([0.5*cubeLength-5, 0, overallHeight-cubeHeight])
		#cylinder(h=cubeHeight, d=screwHoleDiameter);
	translate([-0.5*cubeLength+5, 0, overallHeight-cubeHeight])
		#cylinder(h=cubeHeight, d=screwHoleDiameter);
}

/*transitionHeight=4;
translate([0,0,overallHeight-0.1])
	hull(){
		translate([0, 0, -20])
			cylinder(h=0.1,d=outterDiameter);
		translate([-2.5*outterDiameter/2, -outterDiameter/2, 0])
			cube([2.5*outterDiameter, outterDiameter, 0.1]);
			//cylinder(h=0.1, d=outterDiameter);
	}*/
/*translate([0,0,overallHeight-transitionHeight-cubeHeight])
	scale([2, 1, 1])
		cylinder(h=transitionHeight, d=outterDiameter);*/
