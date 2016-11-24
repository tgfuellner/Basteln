rBall = 21.5+0.5;
bend_radius = 40;
knickWinkel=25;
WandSTaerke=5.5;
RotateRadius=80;
RAussenRohr=51/2;
EPS=0.1;


$fn=150; 

module knick() {
  intersection() {
    translate([0,0,-40]) difference() {
      cube(size = [150,150,80]);
      rotate([0,0,knickWinkel]) cube(size = [150,150,80]);
    }

    difference() {
      rotate_extrude()translate([RotateRadius,0,0])circle(r=rBall+WandSTaerke);

      rotate_extrude()translate([RotateRadius,0,0])circle(r=rBall);
    }
  }
}

module trichter() {
    rWide=rBall+WandSTaerke+8.5;
    hoehe=40;
    GummiLoecherVonAussen=23;

    difference() {
      cylinder(r1=rBall+WandSTaerke, r2=rWide, h=hoehe);

      translate([0,0,-EPS])
      cylinder(r1=rBall, r2=rBall+10, h=40+2*EPS);

      // Löcher für Gummiband
      translate([0,rWide-GummiLoecherVonAussen,hoehe-4])rotate([0,90,0])
      cylinder(r=3, h=2*(rWide+10*EPS), center=true);
      translate([0,-rWide+GummiLoecherVonAussen,hoehe-4])rotate([0,90,0])
      cylinder(r=3, h=2*(rWide+10*EPS), center=true);
    }
}

module knickTrichter() {
    rotate([0,0,knickWinkel-EPS])translate([RotateRadius,0,0])rotate([-90,0,0])
        trichter();
    knick();
}

module all() {
    difference() {
        translate([-RotateRadius,0,0])
            knickTrichter();

        translate([0,-4,0])rotate([-90,0,3.5]) {
            translate([0,0,15])cylinder(r1=RAussenRohr,r2=rBall,h=4);
            cylinder(r=RAussenRohr,h=15);
        }
    }
}

rotate([90,0,0])
  all();
