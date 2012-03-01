//This is a parametric drill or tap block
//The hole diameters are specified below
//Max hole diameter is 
//2 * (rBlock - rHoles) - 2mm

nHoles = 10; //number of holes
$fn=nHoles;

rBlock = 20; //radius of block in mm

rHoles = 15; //holes will be laid-out on a 
//circle of this radius

tBlock = 10; //thickness of the block

// Vorsicht: Bohrerdurchmesser / 2.0
rMin = 1.55; //Minimum hole radius
rMax = 2; //Maximum hole radius

rStep = (rMax-rMin)/(nHoles-1); //hole
//radius step size

sepHoles = 360 / nHoles; //angular
//separation of holes

difference() {
	cylinder(tBlock, rBlock, rBlock, center = true);

	union() {
		for (i = [1:nHoles])
				{
    			assign (angle = i*360/nHoles, distance = rHoles, r = (i-1) * rStep + rMin)
    					{
        		rotate(angle, [0, 0, 1])
        		translate([0, distance, 1.5])
        		cylinder(h=tBlock + 1, r=r+0.1, center = true, $fn=24);
    					}
				}
			}
			};

