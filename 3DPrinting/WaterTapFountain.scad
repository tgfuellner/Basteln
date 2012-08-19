$fn=64;
EPS=0.01;

// Big Pipe
MountingDia=24;
WallThickness=2;
Height=23;

// Fountain Pipe
FountainDia=10;
FountainWall=2;
FountainHeight=15;

HullCylinderDisplacement=5;

difference() {
    union() {
        cylinder(h=Height, r=WallThickness+MountingDia/2, center=true);

        hull() {
            translate([0,-HullCylinderDisplacement,-Height/2+EPS])
                cylinder(h=EPS, r=MountingDia/2-HullCylinderDisplacement, center=true);
            translate([0,-MountingDia/2,-FountainDia/2]) rotate(50,[1,0,0])
                cylinder(h=FountainHeight, r=FountainDia/2);
        }
    }

    cylinder(h=Height+EPS, r=MountingDia/2, center=true);
    translate([0,-MountingDia/2,-FountainDia/2]) rotate(50,[1,0,0]) translate([0,0,FountainHeight/2])
        cylinder(h=FountainHeight+WallThickness*4, r=FountainDia/2-FountainWall, center=true);

}


module pipe(h, r, wall, c=true) {
    difference() {
        cylinder(h=Height, r=r, center=c);

        //cylinder(h=Height+EPS, r=r-wall, center=c);
    }
}
