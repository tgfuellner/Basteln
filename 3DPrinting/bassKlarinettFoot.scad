$fa=6;
$fs=0.1;
EPS=0.01;

rSphere=15;
rClarinettRod=3.5;
rFooter=6;
FooterDepth=0.6;

unImportantLength=100;

difference() {
    sphere(r=rSphere);

    scale([1,1,-1])
        cylinder(r=rClarinettRod, h=unImportantLength);

    translate([0,0,-unImportantLength/2-(rSphere/4)*3])
        cube(size=[unImportantLength,unImportantLength,unImportantLength], center=true);
    translate([0,0,unImportantLength/2+(rSphere/7)*6])
        cube(size=[unImportantLength,unImportantLength,unImportantLength], center=true);
    translate([0,0,(rSphere/7)*6-FooterDepth])
        cylinder(r=rFooter, h=unImportantLength);
}
