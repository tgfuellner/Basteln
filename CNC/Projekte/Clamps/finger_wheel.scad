// Finger Wheel
// Simon Kirkby
// tigger@intethingy.com
// 201005291027

// variables
$fs=0.2;

// boss
shaft_size =  5;
grub_size = 3;
boss_height = 10;

// wheel
wheel_radius = 17;
wheel_height = 10;

// grips 
finger_size = 7;
grip_count = 7;
grip_flatten = 0.7;
cap_scale = 1.1;

// constants 
PI = 3.1415927;

// derived numbers

circum = 2*PI*wheel_radius;
finger_remainder = circum - grip_count*finger_size*2;
//  this is a kludge , does not take into account the chording on the circle
cap_size = (finger_remainder/(2*grip_count))*cap_scale;
boss_size = shaft_size*2;

// structural calculations 


module grip(fs)
{
		scale(v=[grip_flatten,1,1])
		cylinder(wheel_height*3,r1=fs,r2=fs,center=true);
}

module finger_round(fs,c)
{
	inc = 360/c;
	for ( z = [1:c])
	{	
		rotate(z*inc,[0,0,1])		
		translate([wheel_radius,0,0])
		grip(fs);
	}
}

module finger_round_caps(fs,c)
{
	inc = 360/c;
	for ( z = [1:c])
	{	
		rotate(z*inc+(inc/2),[0,0,1])		
		translate([wheel_radius,0,0])
		cylinder(wheel_height,r1=cap_size,r2=cap_size,center=true);
	}
}


module shaft(sr)
{
		cylinder(wheel_height*7,r1=sr,r2=sr,center=true);
}

module grub(gs)
{
	rotate(90,[1,0,0])
	cylinder(wheel_radius*2,r=gs,center=true);
}

module boss(bh,br)
{
	translate([0,0,bh/2+(wheel_height/2)])
	difference()
	{
		cylinder(bh,r=br,center=true);
		grub(gs=grub_size/2.0);
	}
}

module ring(r,th)
{
	difference()
	{
		cylinder(link_size,r=r,center=true);
		cylinder(link_size*2,r=r-link_size,center=true);
	}
}

module wheel(r)
{	
	difference()
	{
		union(){
			cylinder(wheel_height,r1=r,r2=r,center=true);
			boss(bh=boss_height,br=boss_size/2.0);
		}
		shaft(sr=shaft_size/2.0);
	}
}


module finger_wheel()
{
	translate([0,0,wheel_height/2])
	union()
	{
		difference()
		{
			wheel(r=wheel_radius);
			union()
			{
				finger_round(fs=finger_size,c=grip_count);
			}
		}
	finger_round_caps(fs=finger_size,c=grip_count);
	}
}

//wheel(r=wheel_radius);
//ring(r=wheel_radius);
//balls_round(bs=ball_size,c=ball_count);
//boss(bh=boss_height,br=boss_radius);
//finger_round(fs=finger_size,c=grip_count);

projection(cut=false)
  translate([0,0,2])
    finger_wheel();

