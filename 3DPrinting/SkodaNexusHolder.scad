// Skoda Galaxy Nexus Holder

EPS=0.01;
$fn=50;

MountWidth=12;
MountLength=50;
Thickness=2.4;

HolderLength=24;
HakenThickness=1.5;
HakenDepth=4;

StlLength=150;
StlHight=10;
StlEdgeLength=30;
LeftCut=7;
BackCut=18;

// For Middle Part
    Inset=5;
    Length=StlLength-2*LeftCut-Inset;
    GlueLength=MountWidth-Inset/2;
    MiddlePlaceMent=2*(Length/5)-MountWidth;

module haken() {
    translate([0,MountLength,0]) rotate(-30,[1,0,0])
       union() {
              cube(size=[MountWidth,HolderLength,Thickness]);
              translate([0,HolderLength-HakenThickness,Thickness-EPS])
                  cube(size=[MountWidth,HakenThickness,HakenDepth]);
       }
    cube(size=[MountWidth,MountLength,Thickness]);

    // Close the gap
    hull() {
    translate([0,MountLength,0]) rotate(-30,[1,0,0])
       cube(size=[MountWidth,EPS,Thickness]);
    cube(size=[MountWidth,MountLength,Thickness]);
    }
}


module baseLeft() {
    difference() {
        import("Galaxy_Nexus_Dock_-_Flat_Bottom_-_Low_Poly.stl");

        // Cut left
        translate([-EPS,-EPS,-StlHight-EPS]) cube(size=[LeftCut,40,15]);

        // Cut back end
        translate([3,BackCut,-StlHight-EPS]) cube(size=[150,20,15]);

        // Cut right
        translate([StlEdgeLength+LeftCut,-EPS,-StlHight-EPS]) cube(size=[150,20,15]);
    }
}

module baseRight() {
    difference() {
        import("Galaxy_Nexus_Dock_-_Flat_Bottom_-_Low_Poly.stl");

        // Cut Right
        translate([StlLength-LeftCut,-EPS,-StlHight-EPS]) cube(size=[LeftCut+EPS,40,15]);

        // Cut back end
        translate([-EPS,BackCut,-StlHight-EPS]) cube(size=[150,20,15]);

        // Cut left
        translate([-StlEdgeLength-LeftCut,-EPS,-StlHight-EPS]) cube(size=[150,20,15]);
    }
}

module baseRightWithHaken() {
    translate([StlLength-LeftCut-MountWidth,BackCut,-StlHight]) rotate(80, [1,0,0]) {
        difference() {
            haken();
            translate([GlueLength-Length,StlHight,-EPS])
                middle();
        }
    }
    baseRight();
}

module baseLeftWithHaken() {
    translate([LeftCut,BackCut,-StlHight]) rotate(80, [1,0,0]) {
        difference() {
            haken();
            translate([Inset/2,StlHight,-EPS])
                middle();
        }
    }
    baseLeft();
}

module middle() {

    difference() {
        cube(size=[Length,MountLength-StlHight-Inset,Thickness]);
        translate([0,0,Thickness/2])
            cube(size=[GlueLength,MountLength,Thickness]);
        translate([Length-GlueLength,0,Thickness/2])
            cube(size=[GlueLength,MountLength,Thickness]);

        // middle
        translate([MiddlePlaceMent,0,Thickness/2])
            cube(size=[MountWidth,MountLength,Thickness]);
    }

}

module topHolder() {
    Hight=MountLength-StlHight;
    NexusThickness=13;
    NexusInset=5;
    AddOnZ=5;
    AddOnY=4;

    translate([0,0,Thickness/2])
        cube(size=[MountWidth,Hight,Thickness/2]);
    translate([0,Hight-EPS,0])
        cube(size=[MountWidth,AddOnY,Thickness]);

    translate([0,Hight+AddOnY-EPS,0])
        cube(size=[MountWidth,HakenThickness,NexusThickness+AddOnZ]);

    translate([0,Hight+AddOnY-NexusInset-EPS,NexusThickness])
        cube(size=[MountWidth,NexusInset,HakenThickness]);

    hull() {
    translate([0,Hight+AddOnY-EPS,NexusThickness])
        cube(size=[MountWidth,HakenThickness,AddOnZ]);

    translate([0,Hight+AddOnY-NexusInset-EPS,NexusThickness])
        cube(size=[MountWidth,NexusInset,HakenThickness]);
    }
}


module all() {
    baseLeftWithHaken();
    baseRightWithHaken();
    translate([LeftCut+Inset/2,BackCut+Thickness,0]) rotate(80, [1,0,0]) middle();
    translate([MiddlePlaceMent+LeftCut+Inset/2,BackCut+Thickness,0]) rotate(80, [1,0,0])
        topHolder();
}

//all();

//For printing
// middle();
rotate(90, [0,1,0]) topHolder();
// rotate(-90, [0,1,0]) baseLeftWithHaken();
// rotate(90, [0,1,0]) translate([-110,0,0]) baseRightWithHaken();
