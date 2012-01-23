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

ThicknessOfCircuitBoard=2.3;
MaterialThicknessOverBoard=2.5;
MaterialThicknessBelowBoard=12;

MaterialIntoBoard=10;
MaterialBeforeBoard=3;

//----------------
totalWidth=LeftOfConMaterial+ConWidth+RightOfConMaterial;

module solderGap() {
    solderWidth=3.2;
    translate([-solderWidth/2,MaterialIntoBoard+MaterialBeforeBoard-4+EPS,
                 MaterialThicknessBelowBoard+ThicknessOfCircuitBoard-EPS])
        cube(size=[solderWidth,4,1.2]);
}

difference() {
    cube(size=[totalWidth,
           MaterialIntoBoard+MaterialBeforeBoard,
           MaterialThicknessBelowBoard+ThicknessOfCircuitBoard+MaterialThicknessOverBoard]);
    translate([LeftOfConMaterial,MaterialBeforeBoard,-EPS])
        cube(size=[ConWidth,MaterialIntoBoard+EPS,ThicknessOfCircuitBoard+MaterialThicknessBelowBoard]);
    translate([-EPS,MaterialBeforeBoard+EPS,MaterialThicknessBelowBoard])
        cube(size=[totalWidth+2*EPS,MaterialIntoBoard,ThicknessOfCircuitBoard]);
    translate([LeftOfConMaterial+ConEmptyInset,0,0]) solderGap();
    translate([LeftOfConMaterial+ConPlusInset,0,0]) solderGap();
    translate([LeftOfConMaterial+ConMinusInset,0,0]) solderGap();
}



module pocket() {
  h=PocketEdge*tan(BatAngle);
  linear_extrude(height = BoardThickness, center = false, convexity = 10, twist = 0)
  polygon(points=[[-10*EPS,BottomEdge+h],[PocketEdge,BottomEdge],[PocketEdge,BoardHeight+10*EPS]
		 ,[-10*EPS, BoardHeight+10*EPS]],
          	  paths=[[0,1,2,3]]);
}

module screwHole() {
  union() {
    cylinder(h = 20, r=screwHead/2);
    translate([0,9,5]) cylinder(h=20, r=screwHead/2);
    translate([0,9,0]) cylinder(h=20, r=screwDia/2);
    translate([-screwDia/2,0,0]) cube(size=[screwDia,9,20]);
    translate([-screwHead/2,0,5]) cube(size=[screwHead,9,20]);
  }
}

module back() {
  difference() {
    cube(size=[BoardWidth,BoardHeight,BoardThickness]);
    translate([0,0,RestBoardWidth]) pocket();
    translate([2*PocketEdge+BoardMiddleWidth,0,BoardThickness+RestBoardWidth])
		rotate(a=[0,180,0]) pocket();
    translate([PocketEdge+BoardMiddleWidth/2,BoardHeight-20,-EPS*5]) screwHole();
    translate([PocketEdge+BoardMiddleWidth/2,20,-EPS*5]) screwHole();
  }
}

module front() {
  difference() {
    cube(size=[BoardWidth,BoardHeight,BoardThickness]);
    translate([0,0,RestBoardWidth]) pocket();
    translate([2*PocketEdge+BoardMiddleWidth,0,BoardThickness+RestBoardWidth])
		rotate(a=[0,180,0]) pocket();
  }
}

module battery(){
  h=PocketEdge*tan(BatAngle);
  r=AADiameter/2;
  translate([-BoardMiddleWidth,h+BottomEdge+r*2,RestBoardWidth+r]) rotate(a=[0,90,-BatAngle])
    color(Aluminum) cylinder(h=AALength, r=AADiameter/2, $fs=RoundAccuracy);
}

module all() {
    back();
    //front();
    //battery();
    //translate([BoardWidth+10,0,0]) back();
}


// 2D Oben für DXF Export
module oben2D() {
  projection(cut=true) translate([0,0,-9]) all();
  //projection(cut=true) translate([0,0,-4]) all();
  //projection(cut=true) translate([0,0,-2]) all();
}

module seite2D() {
  //projection(cut=true) translate([0,0,9]) rotate(a=[0,90,0]) all();
  projection(cut=true) translate([0,0,BoardWidth/2]) rotate(a=[0,90,0]) all();
}

//oben2D();
//all();
//seite2D();
