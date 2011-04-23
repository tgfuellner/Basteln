plexiDicke=10.4;
plexiLaenge=100.5;
plexiInKonsole=15;

BattHalterDicke=32;
BattHalterLaenge=61;
BattZ=17;

SchalterBreite=19;
SchalterLaenge=22;
SchalterEindringZ=20; // Ohne Bearbeitung 20 und mit Abzwicken 15

Rand=10;

konsZ=30;
konsY=Rand+plexiDicke+BattHalterDicke+Rand;
konsX=plexiLaenge+2*Rand;

LEDTiefe=10;


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

konsole();
//oben2D();
//unten2D();
//mitte2D();
