$fn=90;

*cube([25, 25, 8]);

rodDiameter = 4.8;
rodLength = 7.7;

translate([-5,-rodDiameter/2-1,0])
	cube([40, rodDiameter+2, 1]);
cylinder(h=rodLength+7, d=rodDiameter);
translate([10,0,0])
	cylinder(h=rodLength+6, d=rodDiameter-0.1);
translate([20,0,0])
	cylinder(h=rodLength+5, d=rodDiameter-0.2);
translate([30,0,0])
	cylinder(h=rodLength+4, d=rodDiameter-0.3);

