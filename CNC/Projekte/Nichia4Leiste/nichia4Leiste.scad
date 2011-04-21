EPS=0.01;

aluRailLength=1660;
aluRailWidth=22.4;
aluRailDebth=10;
aluRailBorderWidth=1.5;

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

rail();

module konsole() {
  difference() {
    cube(size=[konsX,konsY,konsZ]);

    // Plexi
    translate([Rand,Rand,konsZ-plexiInKonsole]) {
        cube(size=[plexiLaenge,plexiDicke,plexiInKonsole+0.2]);
    }

    // Schalter
    translate([Rand, konsY-Rand-SchalterBreite, konsZ-SchalterEindringZ]) {
        cube(size=[SchalterLaenge, SchalterBreite, SchalterEindringZ+0.1]);
    }

    // Batteriefach
    translate([Rand+SchalterLaenge-0.1, Rand+plexiDicke-0.1, -0.01]) {
        cube(size=[BattHalterLaenge, BattHalterDicke, BattZ]);
    }

    // Aussparung für LED
    translate([konsX/2, Rand+plexiDicke/2, konsZ-plexiInKonsole-LEDTiefe/2]) {
        cube(size=[plexiDicke, plexiDicke, LEDTiefe+0.1], center=true);
    }
  }
}


// 2D Oben für DXF Export
module oben2D() {
  projection(cut=true) translate([0,0,-konsZ+0.1]) konsole();
}
// 2D Unten für DXF Export
module unten2D() {
  projection(cut=true) translate([0,0,-0.1]) konsole();
}
// 2D Mitte für DXF Export
module mitte2D() {
  projection(cut=true) translate([0,0,-konsZ+plexiInKonsole+0.1]) konsole();
}

