EPS=0.1;
xCut=10;
CubeDim=100;

module leftPart() {
    union() {
        difference() {
            import("VR_Goggles.stl");
            translate([xCut,-CubeDim/2,-EPS])
                cube(size=[CubeDim,CubeDim,CubeDim]);
        }

        difference() {
            translate([0,-36,0])
                cube(size=[20,6,33]);

            rightPart();
        }
    }
}

module rightPart() {
    difference() {
        import("VR_Goggles.stl");
        translate([xCut-CubeDim,-CubeDim/2,-EPS])
            cube(size=[CubeDim,CubeDim,CubeDim]);
    }
}


rightPart();
leftPart();
