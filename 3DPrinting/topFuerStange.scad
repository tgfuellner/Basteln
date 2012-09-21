$fn=100;
EPS=0.01;

RadiusStange=45/2;
RohrWandStaerke=5;
RohrLaenge=70;
AuflagerDicke=50;
KabelBreite=50;
AuflagerWandStaerke=10;

AuflagerBreite=RadiusStange*2+RohrWandStaerke*2;

rotate(90,[1,0,0]) difference() {
    hull() {
        cylinder(r=RadiusStange+RohrWandStaerke,h=RohrLaenge,center=true);
          translate([0,0,AuflagerDicke/2+RohrLaenge/2-EPS]) 
            cube(size=[10+AuflagerBreite,AuflagerBreite,AuflagerDicke], center=true);
    }
    cylinder(r=RadiusStange,h=RohrLaenge+2*EPS,center=true);
    ringFuerKabelbinder();
    translate([0,0,-15]) ringFuerKabelbinder();
    translate([0,0,-30]) ringFuerKabelbinder();

    translate([0,0,AuflagerWandStaerke+(RohrLaenge+AuflagerDicke)/2]) kabelNut();
}

module ringFuerKabelbinder() {
    difference() {
        cylinder(r=RadiusStange+RohrWandStaerke+100,h=5, center=true);

        cylinder(r=RadiusStange+RohrWandStaerke,h=5+EPS, center=true);
    }
}


module kabelNut() {
    hull() {
    cube(size=[KabelBreite,AuflagerBreite+EPS,AuflagerDicke-AuflagerWandStaerke],center=true);
    translate([0,0,-(AuflagerDicke+AuflagerWandStaerke-KabelBreite)/2]) rotate(90,[1,0,0])
        cylinder(r=KabelBreite/2,h=AuflagerBreite+EPS,center=true);
    }
}
