EPS=0.01;

OverallHeight=18;
PlateHeight=6;

// Messwerte Aussen
AussenWidth=90;
AussenDepth=130;

// Messwerte Innen
InnenWidth=45.5;
InnenDepth=98;

// Wandstärke Rand
E=2.4;


difference() {
  translate([0,0,OverallHeight/2])
    cube(size=[AussenWidth+E*2,AussenDepth+E*2,OverallHeight], center=true);

  translate([0,0,OverallHeight/2+PlateHeight])
    cube(size=[AussenWidth,AussenDepth,OverallHeight], center=true);

  translate([0,0,OverallHeight/2-EPS])
    cube(size=[InnenWidth,InnenDepth,OverallHeight], center=true);
}
