length = 14.9;
baseHeight = 1;

cubeLength = 9.8;

frontCubeWidth = 2.2;
frontCubeHeight = 3;
translate([(length-frontCubeWidth)/2, 0, (baseHeight+frontCubeHeight)/2])
	cube([frontCubeWidth, cubeLength, frontCubeHeight], center=true);

backCubeWidth = 3.2;
backCubeHeight = 2.4;
translate([-(length-backCubeWidth)/2, 0, (baseHeight+backCubeHeight)/2])
	cube([backCubeWidth, cubeLength, backCubeHeight], center=true);
	
difference(){
	cube([length, 12, baseHeight], center=true);
	holeWidth = 5;
	#translate([-(length-holeWidth)/2+backCubeWidth ,0 ,0])
		cube([holeWidth, 8, baseHeight], center=true);
}
