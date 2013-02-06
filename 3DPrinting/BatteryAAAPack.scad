$fn=100;
EPS=0.01;


PackRadius=23.5/2;
PackHeight=8;
PackWallThickness=2.1;

AAARadius=10.5/2;
RivetHoleRadius=2.5;

CenterRadius=0.5;

Rubber=1.2;


difference() {
    cylinder(r=PackRadius,h=PackHeight,center=false);
    batteries();
    translate([0,0,0]) cylinder(r=CenterRadius,h=2,center=false);

    // Rubber guides
    translate([PackRadius-Rubber/2,0,0]) cube(size=[Rubber,Rubber,PackHeight*3],center=true);
    translate([-PackRadius+Rubber/2,0,0]) cube(size=[Rubber,Rubber,PackHeight*3],center=true);
}


module batteries() {
    d=PackRadius-AAARadius;
    translate([d,0,PackWallThickness])
        battery();
    rotate(120,[0,0,1]) translate([d,0,PackWallThickness])
        battery();
    rotate(240,[0,0,1]) translate([d,0,PackWallThickness])
        battery();
}

module battery() {
    cylinder(r=AAARadius,h=PackHeight,center=false);
    translate([0,0,-PackHeight+EPS])
    cylinder(r=RivetHoleRadius,h=PackHeight,center=false);
}
