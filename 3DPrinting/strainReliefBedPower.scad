// Strain Relief for heated bed Powercable

EPS=0.01;
$fs=0.2;

// 3-way screw connector
ConWidth=15.8;

// Inset measured from right edge of Connector
ConPlusInset=12.4;
ConMinusInset=7.2;
ConEmptyInset=2.3;

RightOfConMaterial=2;
LeftOfConMaterial=3;
MaterialToCon=3;


ThicknessOfCircuitBoard=2.3;
MaterialThicknessOverBoard=2.5;
MaterialThicknessBelowBoard=12;

MaterialIntoBoard=10;
MaterialBeforeBoard=3;

WireRadius=1.1;

//----------------
totalWidth=LeftOfConMaterial+ConWidth+RightOfConMaterial;

module solderGap() {
    solderWidth=3.2;
    translate([-solderWidth/2,MaterialIntoBoard+MaterialBeforeBoard-4+EPS,
                 MaterialThicknessBelowBoard+ThicknessOfCircuitBoard-EPS])
        cube(size=[solderWidth,4,1.2]);
}

module wireHole() {
    rotate(120, [1,0,0]) translate([0,0,-5]) cylinder(h = 20, r=WireRadius);
}

module bevel() {
    rotate(-55, [1,0,0])
      translate([-EPS,MaterialBeforeBoard,0])
        cube(size=[totalWidth+10, MaterialIntoBoard+MaterialBeforeBoard+10, 90]);
}

module relief() {
    difference() {
        cube(size=[totalWidth,
               MaterialIntoBoard+MaterialBeforeBoard,
               MaterialThicknessBelowBoard+ThicknessOfCircuitBoard+MaterialThicknessOverBoard]);
        translate([LeftOfConMaterial,MaterialBeforeBoard+MaterialToCon,-EPS])
            cube(size=[ConWidth,MaterialIntoBoard+EPS,ThicknessOfCircuitBoard+MaterialThicknessBelowBoard]);
        translate([-EPS,MaterialBeforeBoard+EPS,MaterialThicknessBelowBoard])
            cube(size=[totalWidth+2*EPS,MaterialIntoBoard,ThicknessOfCircuitBoard]);
        translate([LeftOfConMaterial+ConEmptyInset,0,0]) solderGap();
        translate([LeftOfConMaterial+ConPlusInset,0,0]) solderGap();
        translate([LeftOfConMaterial+ConMinusInset,0,0]) solderGap();
        translate([LeftOfConMaterial+ConPlusInset,MaterialBeforeBoard+MaterialToCon,MaterialThicknessBelowBoard-3])
            wireHole();
        translate([LeftOfConMaterial+ConMinusInset,MaterialBeforeBoard+MaterialToCon,MaterialThicknessBelowBoard-3])
            wireHole();
        translate([LeftOfConMaterial+ConEmptyInset,MaterialBeforeBoard+MaterialToCon,MaterialThicknessBelowBoard-3])
            wireHole();
        bevel();
    }
}

rotate(90,[1,0,0]) relief();

