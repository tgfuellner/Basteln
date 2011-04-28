// AA Battery Dispenser
// First in First out

EPS=0.01;
$fs=0.2;

// AA Battery
AADiameter=14.3;
AALength=51;
Gap=0.5;

// Two Boards needes for assembly
BoardThickness=10.4;
BoardWidth=100.5;
BoardHeight=200;
BoardMiddleWidth=15;

PocketEdge=(BoardWidth-BoardMiddleWidth)/2;
RestBoardWidth=BoardThickness-AADiameter/2-Gap;
BatAngle=20;
BottomEdge=10;

screwDia=4;
screwHead=screwDia+5;

// Colors
Aluminum = [0.77, 0.77, 0.8];
BlackPaint = [0.2, 0.2, 0.2];
BrightYellow = [1, 1, 0];
Transparent = [0.3, 0.3, 1, 0.8];

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
all();
//seite2D();
