use <../../../../MCAD/regular_shapes.scad>

EPS=0.01;


aluRailLength=1660;
aluRailWidth=22.4;
aluRailDebth=10;
aluRailBorderWidth=1.5;

LedEdgeOffset=100;

NumLEDs=4;
DistanceBetweenLEDs=(aluRailLength-2*LedEdgeOffset)/(NumLEDs-1);

// Colors
Aluminum = [0.77, 0.77, 0.8];
BlackPaint = [0.2, 0.2, 0.2];
BrightYellow = [1, 1, 0];

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


module all() {

  rail();

  for(i=[0:NumLEDs-1]) {
    echo("Led=",LedEdgeOffset+DistanceBetweenLEDs*i);
    translate([LedEdgeOffset+DistanceBetweenLEDs*i,-aluRailWidth/2,0]) luxeonStar();
    translate([LedEdgeOffset/2+DistanceBetweenLEDs*i,-aluRailWidth/2,-10]) {
      echo("DübelL=",LedEdgeOffset/2+DistanceBetweenLEDs*i);
      cylinder(h = 20, r=1.75, $fs=0.2);
    }
    translate([3*LedEdgeOffset/2+DistanceBetweenLEDs*i,-aluRailWidth/2,-10]) {
      echo("DübelR=",3*LedEdgeOffset/2+DistanceBetweenLEDs*i);
      cylinder(h = 20, r=1.75, $fs=0.2);
    }
  }

  for (i=[1:NumLEDs-1]) {
    translate([LedEdgeOffset-DistanceBetweenLEDs/2+DistanceBetweenLEDs*i,
		-aluRailWidth/2,-10]) {
      echo("DübelM=",LedEdgeOffset-DistanceBetweenLEDs/2+DistanceBetweenLEDs*i);
      cylinder(h = 20, r=1.75, $fs=0.2);
    }
  }
}


// 2D Oben für DXF Export
module oben2D() {
  projection(cut=true) translate([0,0,-2]) all();
  projection(cut=true) translate([0,0,2]) all();
}

//oben2D();

all();
