$fa=6;
$fs=0.1;
EPS=0.01;

PictureHeight=30;
PictureWidth=40;

Width=6;
MagnetThickness=1.8;
MagnetHeight=15;
HandleWidth=0;
HandleHeight=10;


Height=PictureHeight+2;
Depth=PictureWidth+HandleWidth+2;


rotate(90, [1,0,0]) PictureMagnet();


module window() {
    W=PictureWidth-4;
    H=PictureHeight-4;

    translate([Width-1, HandleWidth+3, 3])
      rotate(90,[1,0,0]) rotate(90, [0,1,0]) translate([W/2,H/2,10/2-EPS])
      roundCornersCube(W,H,10,13);
}


module PictureMagnet() {
  difference() {
    union() {
        cube(size=[Width,Depth,Height]);
        /*
        translate([Width-EPS,0,0])
            cube(size=[HandleHeight,HandleWidth,Height]);
        */
    }

    translate([1, -EPS, (Height-MagnetHeight)/2])
        cube(size=[MagnetThickness,Depth-5,MagnetHeight]);

    translate([Width-2, HandleWidth+1, 1])
        cube(size=[1, PictureWidth+10, PictureHeight]);

    window();


    // Einrundung
    translate([HandleHeight+Width,0,0]) rotate(90, [1,0,0])
        cylinder(r=HandleHeight, h=10, center=true);
    translate([HandleHeight+Width,0,Height]) rotate(90, [1,0,0])
        cylinder(r=HandleHeight, h=10, center=true);
  }
}


/*
http://codeviewer.org/view/code:1b36 
Copyright (C) 2011 Sergio Vilches
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
Contact: s.vilches.e@gmail.com


    ----------------------------------------------------------- 
                 Round Corners Cube (Extruded)                
      roundCornersCube(x,y,z,r) Where:                        
         - x = Xdir width                                     
         - y = Ydir width                                     
         - z = Height of the cube                             
         - r = Rounding radious                               
                                                              
      Example: roundCornerCube(10,10,2,1);                    
     *Some times it's needed to use F6 to see good results!   
 	 ----------------------------------------------------------- 
*/
// Test it!
// roundCornersCube(10,5,2,1);


module createMeniscus(h,radius) // This module creates the shape that needs to be substracted from a cube to make its corners rounded.
difference(){        //This shape is basicly the difference between a quarter of cylinder and a cube
   translate([radius/2+0.1,radius/2+0.1,0]){
      cube([radius+0.2,radius+0.1,h+0.2],center=true);         // All that 0.x numbers are to avoid "ghost boundaries" when substracting
   }

   cylinder(h=h+0.2,r=radius,$fn = 20,center=true);
}


module roundCornersCube(x,y,z,r)  // Now we just substract the shape we have created in the four corners
difference(){
   cube([x,y,z], center=true);

translate([x/2-r,y/2-r]){  // We move to the first corner (x,y)
      rotate(0){  
         createMeniscus(z,r); // And substract the meniscus
      }
   }
   translate([-x/2+r,y/2-r]){ // To the second corner (-x,y)
      rotate(90){
         createMeniscus(z,r); // But this time we have to rotate the meniscus 90 deg
      }
   }
      translate([-x/2+r,-y/2+r]){ // ... 
      rotate(180){
         createMeniscus(z,r);
      }
   }
      translate([x/2-r,-y/2+r]){
      rotate(270){
         createMeniscus(z,r);
      }
   }
}





