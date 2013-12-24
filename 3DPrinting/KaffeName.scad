/*

Stammt von http://www.thingiverse.com/thing:168745

OpenSCAD-File to generate a Customized luggage label
2013-10-19 - kowomike

Needs write.scad as found here:
https://github.com/rohieb/Write.scad

or here:
http://www.thingiverse.com/thing:16193

Uses font for write.scad as found here:
http://www.thingiverse.com/thing:100779

*/

use <Write.scad/Write.scad>  //Change to directory where write.scad resides; Put the font (stencil2.dxf) in same directory (were other fonts are as well)


text=[				 
//"YOUR NAME"
//,"ADDRESS"
"MARIA"
//,"COUNTRY"

];						  

//Other fonts need heavy changes for spacing, size etc.
font="Write.scad/stencil2.dxf";
fontlengthfactor=.682;

// **** START CUSTOMIZING HERE ****

// Fontheight of text (too small will be hard to print)
fontsize=13;

// Spacing between letters (too little will be hard to print)
spacing=0.9;		 

// Spacing between lines as factor of fontsize
linespacing=0.85;	 

// Corner radius of plate [mm]
cornerradius=3;   

// Thickness of plate [mm]
platethickness=3; 

// emgraving depth of text in % of plate (-66 is nice; -100 is through-hole)
// Use positive values to emboss instead of engraving 
textdepth=-100;      

// Length of slitted hole [mm]
slitlength=10;		 

// Width of slitted hole [mm]
slitwidth=4;		 

// Put text inside the plate so it can only be seen against light; sets textdepth automatically to 50%; !! You won't see text in preview anymore so first be sure to have it right
hidetext=false;    

// Enter Text; UPPERCASE looks best
// Use up to 10 lines as needed; for alignment to center or right, use spaces


// **** END CUSTOMIZING HERE ****

//Find out maximum characters for first 10 lines for centering
maxtextchars = max(len(text[0]),len(text[1]),len(text[2]),len(text[3]),len(text[4]),len(text[5]),len(text[6]),len(text[7]),len(text[8]),len(text[9]));
textlines=len(text);

//Calculate Platesize based on textlength and fontsize
textlength=maxtextchars*spacing*fontlengthfactor*fontsize;
platelength=textlength+slitwidth+fontlengthfactor*fontsize/2+6+sqrt(cornerradius); //+slitwidth-sqrt(corner_radius)+6;
platewidth=textlines*0.9*fontsize+cornerradius/2;

//Avoid having slit larger than platewidth
slitlengthuse=min(slitlength,(platewidth-cornerradius*2)*0.9);

//Change text depth and text height when hidden is true
textdepthuse=((hidetext==true)? -platethickness*1/4 : 0.01);
textthickness=((hidetext==true)? -platethickness/2 : textdepth/100*platethickness-0.02);

//Module for plate
module plate (length,width,thickness,cornerradius) {
linear_extrude(height = thickness, center = true, convexity = 10) {
	minkowski () {
	square ([length-2*cornerradius,width-2*cornerradius],center=true);
	circle (r=max(0.1,cornerradius),$fn=100);  //Max-function to avoid malfunction with cornerradius 0
	}
}
}


//Module for slit
module slit (slitlength,slitwidth,slitheight) { 
linear_extrude(height = slitheight+1, center = true, convexity = 10) {
	minkowski () {
	square ([0.1,slitlength],center=true);
	circle (r=slitwidth/2,$fn=100);
	}
}
}


//Generate complete plate
difference () {
	//plate(platelength,platewidth,platethickness,cornerradius);
    cylinder(r=35,center=true,h=thickness,$fn=50);

	//Slit for attaching
	//translate ([-platelength/2+4+slitwidth/2,0,0])
	//slit(slitlengthuse,slitwidth,platethickness);

	//Engrave text if textdepth negative
	if (textdepth<0) {
		translate ([fontlengthfactor*fontsize/2,-fontsize*0.75,platethickness/2])  //Adjust text start 
		translate ([-textlength/2,textlines*linespacing*fontsize/2,textdepthuse])
		for (i=[0:textlines-1]) {
			translate ([0,-fontsize*linespacing*i,0])
            //TG
			write(text[i],t=textthickness,h=fontsize,font=font,space=spacing);
		}
	}

//Uncomment to show inside of plate for checking hidden text position
//translate ([0,0,-50])
//cube ([100,100,100]);

}

//Emboss text if textdepth positive
if (textdepth>0) {  
		color ("YellowGreen")
		translate ([fontlengthfactor*fontsize/2,-fontsize*0.75,platethickness/2])  //Adjust text start 
		translate ([-textlength/2+2+slitwidth/2,textlines*linespacing*fontsize/2,0])
		for (i=[0:textlines-1]) {
			translate ([0,-fontsize*linespacing*i,0])
			write(text[i],t=textthickness,h=fontsize,font=font,space=spacing);
		}
}

