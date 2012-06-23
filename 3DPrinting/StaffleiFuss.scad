my_fn=30;
EPS=0.01;


// Alu Abmessungen
Alu_width = 25;
Alu_length = 42;
Alu_rein = 45;
wall_thickness = 3;
DickeUnten = 5;
ZulageSchraege = 13;


Total_height = Alu_rein+DickeUnten+ZulageSchraege;
width_out = Alu_width + (2 * wall_thickness);
length_out = Alu_length + (2 * wall_thickness);


rotate(180, [0,1,0]) intersection() {
    theBox();
    abrundung();
}


module abrundung() {
    R=85;
    
    translate([0,-Alu_length/2,R])
    rotate(90,[0,1,0]) cylinder($fn=400,center=true,r=R, h=Alu_width+(2*wall_thickness+EPS));
}

module theBox() {
  difference() {
    translate([0,0,Total_height/2])
        roundedBox([width_out,length_out,Total_height],wall_thickness,center=true,$fn=my_fn);
    translate([0,0,Alu_rein/2+DickeUnten+ZulageSchraege+EPS])
      cube([Alu_width,Alu_length,Alu_rein], center=true);
    translate([0,0,Total_height])
      cube([Alu_width+20,Alu_length+20,8], center=true);
  }
}

module schraege() {
    // Wird zum wegnehmen genuzt, deshalb einfach ein gutes StUeck groesser
    hull() {
        cube(size=[Alu_width*2,Alu_length+2*wall_thickness+0.5,EPS], center=true);
        translate([0,length_out/2,ZulageSchraege/2])
            cube(size=[Alu_width*2,EPS,ZulageSchraege], center=true);
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
