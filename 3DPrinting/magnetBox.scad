my_fn=30;
EPS=0.01;


// Box Parameter
width_outer = 40;
length_outer = 60;
height_outer = 50;
wall_thickness = 2;


// Give Dimension of the magnet:
Magnet_width = 18.5;
Magnet_Length = 37;
Magnet_Heigth = 2;
Magnet_MaterialThicknessArround = 1.5;


theBox();


module theBox() {
  width_inner = width_outer - (2 * wall_thickness);
  length_inner = length_outer - (2 * wall_thickness);
  height_inner = height_outer - (2 * wall_thickness);

  A = Magnet_MaterialThicknessArround;
  IntoWall=0.49;
  HolderGapLength=8;

  difference() {
    union() {
      difference() {
            roundedBox([width_outer,length_outer,height_outer],wall_thickness,center=true,$fn=my_fn);
            cube([width_inner,length_inner,height_inner],center=true);
            translate([0,0,height_outer-wall_thickness-EPS])
                cube([width_outer+EPS,length_outer+EPS,height_outer], center=true);
      }
      translate([-(Magnet_Heigth+A-IntoWall-width_inner-EPS)/2,0,0])
        cube([Magnet_Heigth+A-IntoWall,Magnet_width+2*A,Magnet_Length+A], center=true);
    }
    translate([-(Magnet_Heigth+A-width_inner-EPS)/2,0,0])
    translate([A/2+IntoWall,0,A/2+EPS])
      cube([Magnet_Heigth,Magnet_width,Magnet_Length], center=true);

    translate([-(A-width_inner-EPS)/2,0,(Magnet_Length-HolderGapLength+A+EPS)/2])
      cube([A,Magnet_width+2*A+EPS,HolderGapLength], center=true);
  }
}


// roundedBox([width, height, depth], float radius, bool sidesonly);

// EXAMPLE USAGE:
// roundedBox([20, 30, 40], 5, true);

// size is a vector [w, h, d]
module roundedBox(size, radius, sidesonly)
{
  rot = [ [0,0,0], [90,0,90], [90,90,0] ];
  if (sidesonly) {
    cube(size - [2*radius,0,0], true);
    cube(size - [0,2*radius,0], true);
    for (x = [radius-size[0]/2, -radius+size[0]/2],
           y = [radius-size[1]/2, -radius+size[1]/2]) {
      translate([x,y,0]) cylinder(r=radius, h=size[2], center=true);
    }
  }
  else {
    cube([size[0], size[1]-radius*2, size[2]-radius*2], center=true);
    cube([size[0]-radius*2, size[1], size[2]-radius*2], center=true);
    cube([size[0]-radius*2, size[1]-radius*2, size[2]], center=true);

    for (axis = [0:2]) {
      for (x = [radius-size[axis]/2, -radius+size[axis]/2],
             y = [radius-size[(axis+1)%3]/2, -radius+size[(axis+1)%3]/2]) {
        rotate(rot[axis])
          translate([x,y,0])
          cylinder(h=size[(axis+2)%3]-2*radius, r=radius, center=true);
      }
    }
    for (x = [radius-size[0]/2, -radius+size[0]/2],
           y = [radius-size[1]/2, -radius+size[1]/2],
           z = [radius-size[2]/2, -radius+size[2]/2]) {
      translate([x,y,z]) sphere(radius);
    }
  }
}
