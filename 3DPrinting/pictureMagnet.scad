$fa=6;
$fs=0.1;
EPS=0.01;

PictureHeight=30;
PictureWidth=40;

Width=6;
MagnetThickness=2.1;
MagnetHeight=10.5;
HandleWidth=2;
HandleHeight=10;


Height=PictureHeight+2;
Depth=PictureWidth+HandleWidth+2;

difference() {
    union() {
        cube(size=[Width,Depth,Height]);
        translate([Width-EPS,0,0])
            cube(size=[HandleHeight,HandleWidth,Height]);
    }

    translate([1, -EPS, (Height-MagnetHeight)/2])
        cube(size=[MagnetThickness,Depth-5,MagnetHeight]);

    translate([Width-2, HandleWidth+1, 1])
        cube(size=[1, PictureWidth+10, PictureHeight]);

    translate([Width-1, HandleWidth+3, 3])
        cube(size=[10, PictureWidth-4, PictureHeight-4]);

    translate([HandleHeight+Width,0,0]) rotate(90, [1,0,0])
        cylinder(r=HandleHeight, h=10, center=true);
    translate([HandleHeight+Width,0,Height]) rotate(90, [1,0,0])
        cylinder(r=HandleHeight, h=10, center=true);
}

