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
BoardMiddleWidth=5;

PocketEdge=(BoardWidth-BoardMiddleWidth)/2;
RestBoardWidth=BoardThickness-AADiameter/2;
BatAngle=30;
BottomEdge=10;

// Colors
Aluminum = [0.77, 0.77, 0.8];
BlackPaint = [0.2, 0.2, 0.2];
BrightYellow = [1, 1, 0];
Transparent = [0.3, 0.3, 1, 0.8];

module pocket() {
  h=PocketEdge*tan(BatAngle);
  linear_extrude(height = BoardThickness, center = false, convexity = 10, twist = 0)
  polygon(points=[[-EPS,BottomEdge+h],[PocketEdge,BottomEdge],[PocketEdge,BoardHeight+EPS]
		 ,[-EPS, BoardHeight+EPS]],
          	  paths=[[0,1,2,3]]);
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
  translate([0,h+BottomEdge+r,RestBoardWidth+r]) rotate(a=[0,90,-BatAngle])
    color(Aluminum) cylinder(h=AALength, r=AADiameter/2, $fs=RoundAccuracy);
}

front();
battery();


/* aluminum rails
 */
module rail() {
  rotate(a=[180,0,0])
  difference() {
    cube(size=[aluRailLength,aluRailWidth,aluRailDebth]);	
    translate([-EPS,aluRailBorderWidth,aluRailBorderWidth]) {
      cube(size=[aluRailLength+50*EPS,aluRailWidth-2*aluRailBorderWidth,
                 aluRailDebth-aluRailBorderWidth+EPS]);
    }
  }
}

/* Luxeon Star
 */
module luxeonDrill() {
  DrillCenterDist=9.4;
  DrillR=1.5;
  RoundAccuracy=0.1;
    translate([-DrillCenterDist,0,-EPS]) union() {
      cylinder(h = 10, r=DrillR, $fs=RoundAccuracy);
      translate([-5,-DrillR,-EPS]) cube(size = [5,2*DrillR,2]);
    }
}
module luxeonStar() {
  R=11.547; // So that Flat side to side = 20
  Angles=[0,60,120,180,240,300];

  difference() {
    union() {
      color(Aluminum) hexagon_prism(1.5, R);
      color(BlackPaint) translate([0,0,1.5]) hexagon_prism(0.1, R);
      color(BrightYellow) translate([0,0,1.6]) cube(size=[5,5, 1.3], center=true);
      for (a=Angles) {
	rotate(a=[0,0,a+30]) translate([-7.25,0,1.7]) 
		color(Aluminum) cube(size=[2.5,4,0.1], center=true);
      }
      
    }
    for (a=Angles) {
      rotate(a=[0,0,a]) luxeonDrill();
    }
  }
}

module lampshade() {
  difference() {
    color(Transparent) roundedBox([44, 30, 10.4], 2, true);
    translate([16,0,-10]) cylinder(h=20, r=DowelR);
    translate([-16,0,-10]) cylinder(h=20, r=DowelR);
  }
}


module all() {

  rail();

  for(i=[0:NumLEDs-1]) {
    echo("Led=",LedEdgeOffset+DistanceBetweenLEDs*i);
    translate([LedEdgeOffset+DistanceBetweenLEDs*i,-aluRailWidth/2,0]) luxeonStar();
    translate([LedEdgeOffset+DistanceBetweenLEDs*i,-aluRailWidth/2,5.2+4]) lampshade();
    translate([LedEdgeOffset/2+DistanceBetweenLEDs*i,-aluRailWidth/2,-10]) {
      echo("DübelL=",LedEdgeOffset/2+DistanceBetweenLEDs*i);
      cylinder(h = 20, r=DowelR);
    }
    translate([3*LedEdgeOffset/2+DistanceBetweenLEDs*i,-aluRailWidth/2,-10]) {
      echo("DübelR=",3*LedEdgeOffset/2+DistanceBetweenLEDs*i);
      cylinder(h = 20, r=DowelR);
    }
  }

  for (i=[1:NumLEDs-1]) {
    translate([LedEdgeOffset-DistanceBetweenLEDs/2+DistanceBetweenLEDs*i,
		-aluRailWidth/2,-10]) {
      echo("DübelM=",LedEdgeOffset-DistanceBetweenLEDs/2+DistanceBetweenLEDs*i);
      cylinder(h = 20, r=DowelR);
    }
  }
}


// 2D Oben für DXF Export
module oben2D() {
  //projection(cut=true) translate([0,0,-2]) all();
  //projection(cut=true) translate([0,0,2]) all();
  projection(cut=true) translate([0,0,2]) lampshade();
}
//oben2D();

//all();
//lampshade();
