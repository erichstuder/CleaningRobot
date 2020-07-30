$fn = 360;

turbineOutterDiameter = 107;
holderWidth = 15;
holderLength = 50;

difference(){
	cube([holderLength, holderWidth, 20]);
	
	translate([holderLength/2, holderWidth+0.5, turbineOutterDiameter/2+5])
		rotate([90, 0, 0])
			#cylinder(h=holderWidth+1, d=turbineOutterDiameter);
	
	zipTieChannelWidth = 4.1;
	translate([-0.5, holderWidth/2-zipTieChannelWidth/2, 1])
		#cube([holderLength+1, zipTieChannelWidth, 1.6]);
	
	screwHoleDiameter = 3.6;
	screwHeadDiameter = 6.6;
	screwHoleHeight = 20;
	translate([15, holderWidth/2, -1])
		#cylinder(h=screwHoleHeight, d=screwHoleDiameter);
	translate([holderLength-15, holderWidth/2, -1])
		#cylinder(h=screwHoleHeight, d=screwHoleDiameter);
	translate([5, holderWidth/2-screwHeadDiameter/2, 1])
		#cube([holderLength-10, screwHeadDiameter, 20]);
	
}
