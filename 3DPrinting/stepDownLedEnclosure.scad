my_fn=30;
EPS=0.01;


// Box Parameter
width_outer = 44;
length_outer = 65;
height_outer = 35;
wall_thickness = 2.1;


SockelDim=10;
ScrewDia=2.5;


theBox();
ledKuehlkoerperHalterungLeft(0);
translate([58,-19,0])  ledKuehlkoerperHalterungLeft(180);


module ledKuehlkoerperHalterungLeft(angle) {
    translate([-width_outer/2-SockelDim/2-2,length_outer/2-14,0])
    rotate(angle, [0,0,1])
    difference() {
        hull() {
        translate([0,0,height_outer-6]) boardSockel();
        translate([SockelDim/2+2.5,0,height_outer-30]) 
           cube([1,SockelDim,SockelDim], center=true);
        }
        translate([0,0,height_outer-6-SockelDim+1])
           cylinder(r=ScrewDia/2,h=SockelDim-2, $fn=20, center=true);
    }
}


module boardSockel() {
  translate([0,0,SockelDim/2 - height_outer/2 +wall_thickness -EPS])
    difference() {
      cube([SockelDim,SockelDim,SockelDim], center=true);
      
      translate([0,0,1+EPS])
       cylinder(r=ScrewDia/2,h=SockelDim-2, $fn=20, center=true);
    }
}


module theBox() {
  width_inner = width_outer - (2 * wall_thickness);
  length_inner = length_outer - (2 * wall_thickness);
  height_inner = height_outer - (2 * wall_thickness);
  BoardHoleDistX=30;
  BoardHoleDistY=41;

  difference() {
    roundedBox([width_outer,length_outer,height_outer],wall_thickness,center=true,$fn=my_fn);
    cube([width_inner,length_inner,height_inner],center=true);
    translate([0,0,height_outer-wall_thickness-EPS])
      cube([width_outer+EPS,length_outer+EPS,height_outer], center=true);
  }

  h=wall_thickness+SockelDim/2-EPS;
  translate([-h+width_outer/2,h-length_outer/2,0]) 
    boardSockel();
  translate([-BoardHoleDistX,0,0]) translate([-h+width_outer/2,h-length_outer/2,0]) 
    boardSockel();

  translate([0,BoardHoleDistY,0]) {
      translate([-h+width_outer/2,h-length_outer/2,0]) 
        boardSockel();
      translate([-BoardHoleDistX,0,0]) translate([-h+width_outer/2,h-length_outer/2,0]) 
        boardSockel();
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
