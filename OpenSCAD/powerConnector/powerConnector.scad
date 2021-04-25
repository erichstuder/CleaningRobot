$fn = 90;

innerDiameter = 4;
coverLength = 7; //TODO: maybe add 1 or 2 mm
length = 10+coverLength;
secondHoleX = 6;
screwHole = 3.5;

male();
translate([0, 20, 0])
	female();


module male(){
	outterDiameter = 12;
	difference(){
		union(){
			
			cylinder(d=outterDiameter, h=length, center=true);
			translate([secondHoleX, 0, 0])
				cube([outterDiameter, outterDiameter, length], center=true);
			translate([outterDiameter/2, 0, 0])
				cube([outterDiameter, outterDiameter, length], center=true);
		}
		
		for(x = [0, secondHoleX]){
			translate([x, 0 , 0]){
				#cylinder(d=innerDiameter, h=length, center=true);
				
				translate([0, -outterDiameter/4, -length/2+3])
					rotate([90, 0, 0])
						#cylinder(d=screwHole, h=outterDiameter/2, center=true);
			}
		}
		
		coverDiameter = 9;
		translate([0, 0, (length-coverLength)/2]){
			#cylinder(d=coverDiameter, h=coverLength, center=true);
			translate([secondHoleX, 0, 0])
				#cube([coverDiameter, coverDiameter, coverLength], center=true);
			translate([coverDiameter/2, 0, 0])
				#cube([coverDiameter, coverDiameter, coverLength], center=true);
		}
	}
}


module female(){
	outterDiameter = 8;
	difference(){
		union(){
			cylinder(d=outterDiameter, h=length, center=true);
			translate([secondHoleX, 0, 0])
				cube([outterDiameter, outterDiameter, length], center=true);
			translate([outterDiameter/2, 0, 0])
				cube([outterDiameter, outterDiameter, length], center=true);
		}
		
		for(x = [0, secondHoleX]){
			translate([x, 0 , 0]){
				#cylinder(d=innerDiameter, h=length, center=true);
				
				translate([0, outterDiameter/4, -length/2+3+coverLength])
					rotate([90, 0, 0])
						#cylinder(d=screwHole, h=outterDiameter/2, center=true);
			}
		}
	}
}
