

/*
length = 60;
width = 100;
tablength = 3;
tabwidth = 10;
tablengthperiod = 3;
tabwidthperiod = 4;
  
*/

length = 130;
width = 151.5;
depth = 490.6; //55;

tablength = 2.9;  // material thickness
tabwidth = 15;

tablengthperiod = 3;
tabwidthperiod = 3;
tabdepthperiod = 9;

screwLength = 11.5-tablength;
screwDiameter = 2.95;

hexWidth = 2.4;
hexThickness = 5.5 ;

exploded = true;
explodedplateseperation = 2;

/*
		Panel(length=length,width=width,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tablengthperiod,
			tabwidthperiod=tabwidthperiod,slottype=0,hexWidth=hexWidth,hexThickness=hexThickness);
*/
 

//ScrewSlot(1, 5);

Box(length,width,depth,tablength,tabwidth,tablengthperiod,tabwidthperiod,tabdepthperiod, exploded,hexWidth=hexWidth,hexThickness=hexThickness);



module BottomPanel(length=65 , width =50 , tablength = 3, tabwidth = 6, tablengthperiod = 5, tabwidthperiod = 4, 
	slottype=0, screwLength = 10, screwDiameter = 2,hexWidth = 4,hexThickness = 1)
{

		Panel(length=length,width=width,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tablengthperiod,
			tabwidthperiod=tabwidthperiod, screwLength=screwLength,
			screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness );
		

} 


module TopPanel (
	length=65 , width =50 , tablength = 3, tabwidth = 6, tablengthperiod = 5, tabwidthperiod = 4, 
	slottype=0, screwLength = 10, screwDiameter = 2,hexWidth = 4,hexThickness = 1
)
{

			Panel(length=length,width=width,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tablengthperiod,
			tabwidthperiod=tabwidthperiod, screwLength=screwLength,
			screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness  );



}

module FrontPanel(
	length=65 , width =50 , tablength = 3, tabwidth = 6, tablengthperiod = 5, tabwidthperiod = 4, 
	slottype=0, screwLength = 10, screwDiameter = 2,hexWidth = 4,hexThickness = 1)
{

		Panel(length=depth, width=width,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tablengthperiod,
			tabwidthperiod=tabwidthperiod, slottype=1, screwLength=screwLength,
			screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness  );


}


module BackPanel(	length=65 , width =50 , tablength = 3, tabwidth = 6, tablengthperiod = 5, tabwidthperiod = 4, 
	slottype=0, screwLength = 10, screwDiameter = 2,hexWidth = 4,hexThickness = 1)
{
		Panel(length=depth, width=width,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tablengthperiod,
			tabwidthperiod=tabwidthperiod, slottype=1, screwLength=screwLength,
			screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness  );
}


module LeftPanel(	length=65 , width =50 , tablength = 3, tabwidth = 6, tablengthperiod = 5, tabwidthperiod = 4, 
	slottype=0, screwLength = 10, screwDiameter = 2,hexWidth = 4,hexThickness = 1)
{
		Panel(length=length, width=depth,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tablengthperiod,
			tabwidthperiod= tabwidthperiod, slottype=2, screwLength=screwLength,
			screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness );
}

module RightPanel(length=65 , width =50 , tablength = 3, tabwidth = 6, tablengthperiod = 5, tabwidthperiod = 4, 
	slottype=0, screwLength = 10, screwDiameter = 2,hexWidth = 4,hexThickness = 1)
{
		Panel(length=length, width=depth,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tablengthperiod,
			tabwidthperiod= tabwidthperiod, slottype=2, screwLength=screwLength,
			screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness );
}

module Box(length,width,depth,tablength,tabwidth,tablengthperiod,tabwidthperiod,tabdepthperiod, exploded = false,hexWidth,hexThickness)
{


	if(!exploded)
	{

		BottomPanel(length=length,width=width,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tablengthperiod,
			tabwidthperiod=tabwidthperiod, screwLength=screwLength,
			screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness);




		translate([0 ,0,depth])
		TopPanel(length=length,width=width,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tablengthperiod,
			tabwidthperiod=tabwidthperiod, screwLength=screwLength,
			screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness );


		translate([length/2,0,depth/2])
		rotate([0,-90,0])

		FrontPanel(length=length,width=width,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tabdepthperiod,
			tabwidthperiod=tabwidthperiod, screwLength=screwLength,
			screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness );

		translate([-length/2,0,depth/2])
		rotate([0,90,0])
		BackPanel(length=depth, width=width,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tabdepthperiod,
			tabwidthperiod=tabwidthperiod, slottype=1, screwLength=screwLength,
			screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness  );


		translate([0,width/2,depth/2])
		rotate([90,0,0])
		LeftPanel(length=length, width=depth,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tablengthperiod,
			tabwidthperiod=tabdepthperiod, slottype=2, screwLength=screwLength,
			screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness );

		translate([0,-width/2,depth/2])
		rotate([90,0,0])
		RightPanel(length=length, width=depth,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tablengthperiod,
			tabwidthperiod= tabdepthperiod, slottype=2, screwLength=screwLength,
			screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness);


	}
	else
	{

			BottomPanel(length=length,width=width,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tablengthperiod,
			tabwidthperiod=tabwidthperiod, screwLength=screwLength,
			screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness);




		translate([depth + length + tablength * 4 + explodedplateseperation * 2  ,0,0])
		TopPanel(length=length,width=width,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tablengthperiod,
			tabwidthperiod=tabwidthperiod, screwLength=screwLength,
			screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness );

		translate([ depth * 0.5  + length * 0.5 + tablength * 2 + explodedplateseperation  ,0,0])
		FrontPanel(length=length,width=width,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tabdepthperiod,
			tabwidthperiod=tabwidthperiod, screwLength=screwLength,
			screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness );


		translate([-depth * 0.5  - length * 0.5 - tablength * 2 - explodedplateseperation,0,0])
		BackPanel(length=depth, width=width,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tabdepthperiod,
			tabwidthperiod=tabwidthperiod, slottype=1, screwLength=screwLength,
			screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness  );


		translate([0,width * 0.5 + depth * 0.5  + tablength * 2 + explodedplateseperation,0])
		LeftPanel(length=length, width=depth,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tablengthperiod,
			tabwidthperiod=tabdepthperiod  , slottype=2, screwLength=screwLength,screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness );

		translate([0,-width * 0.5 - depth * 0.5  - tablength * 2 - explodedplateseperation,0])
		RightPanel(length=length, width=depth,tablength = tablength, 
			tabwidth= tabwidth,tablengthperiod=tablengthperiod,
			tabwidthperiod= tabdepthperiod, slottype=2, screwLength=screwLength,
			screwDiameter=screwDiameter,hexWidth=hexWidth,hexThickness=hexThickness);


	}
}

module Panel(length=65 , width =50 , tablength = 3, tabwidth = 6, tablengthperiod = 5, tabwidthperiod = 4, 
	slottype=0, screwLength = 10, screwDiameter = 2,hexWidth = 4,hexThickness = 1)
{


	if(slottype == 1)
	{
		difference()
		{
			square([length+tablength*2,width+tablength*2], center=true);
			PanelTabs(length,width,tabwidthperiod,tablengthperiod, tablength,tabwidth);

			//translate([-((length/2)) - tablength/4,(-width/2)-(width/tabwidthperiod)/2 ])
		//	WidthScrewHoles(width, tabwidthperiod, screwDiameter);

			//translate([((length/2)) + tablength/4,(-width/2)-(width/tabwidthperiod)/2 ])
			//WidthScrewHoles(width, tabwidthperiod, screwDiameter);

			//translate([(-length/2) + (length/tablengthperiod)/2  ,-((width/2)) - tablength/4 ])
			//LengthScrewHoles(length, tablengthperiod, screwDiameter);

			translate([(-length/2) + (length/tablengthperiod)/2  ,((width/2)) + tablength/4 ])
			LengthScrewHoles(length, tablengthperiod, screwDiameter);
		}
	}
	if(slottype == 2)
	{
		difference()
		{
			square([length,width+tablength*2], center=true);

			PositiveLengthTabs(length,width,tablengthperiod,tablength,tabwidth);

			NegativeLengthTabs(length,width,tablengthperiod,tablength,tabwidth);

			translate([-((length/2)) + screwLength/2,(-width/2)-(width/tabwidthperiod)/2 ])

			WidthScrewSlots(width,tabwidthperiod,screwLength,screwDiameter, hexWidth,hexThickness, offsetHex=screwLength*.6666);

			translate([((length/2)) - screwLength/2,(-width/2)-(width/tabwidthperiod)/2 ])
			WidthScrewSlots(width,tabwidthperiod,screwLength,screwDiameter, hexWidth,hexThickness);

			translate([(-length/2) + (length/tablengthperiod)/2  ,-((width/2)) - tablength/4 ])
			LengthScrewHoles(length, tablengthperiod, screwDiameter);

			translate([(-length/2) + (length/tablengthperiod)/2  ,((width/2)) + tablength/4 ])
			LengthScrewHoles(length, tablengthperiod, screwDiameter);
		}

		PositiveWidthTab(length,width, tabwidthperiod, tablength, tabwidth);

		NegativeWidthTab(length,width, tabwidthperiod, tablength, tabwidth);

	}
	if(slottype == 0)
	{
		difference()
		{
		union()
		{
			square([length,width], center=true);
			PanelTabs(length,width,tabwidthperiod,tablengthperiod, tablength,tabwidth);
		} 
			translate([-((length/2)) + screwLength/2,(-width/2)-(width/tabwidthperiod)/2 ])
			WidthScrewSlots(width,tabwidthperiod,screwLength,screwDiameter, hexWidth,hexThickness, offsetHex=screwLength*.6666);

			translate([((length/2)) - screwLength/2,(-width/2)-(width/tabwidthperiod)/2 ])
			WidthScrewSlots(width,tabwidthperiod,screwLength,screwDiameter, hexWidth,hexThickness);

			translate([(-length/2) + (length/tablengthperiod)/2  ,-((width/2) -screwLength/2) ])
			LengthScrewSlots(length, tablengthperiod, screwLength, screwDiameter, hexWidth,hexThickness, offsetHex=screwLength*.6666);

			translate([(-length/2) + (length/tablengthperiod)/2  ,((width/2) -screwLength/2) ])
			LengthScrewSlots(length, tablengthperiod, screwLength, screwDiameter, hexWidth,hexThickness);

		}
	}

}

module PanelTabs(length,width,tabwidthperiod,tablengthperiod, tablength,tabwidth)
{
	PositiveWidthTab(length,width, tabwidthperiod, tablength, tabwidth);

	NegativeWidthTab(length,width, tabwidthperiod, tablength, tabwidth);

	PositiveLengthTabs(length,width,tablengthperiod,tablength,tabwidth);

	NegativeLengthTabs(length,width,tablengthperiod,tablength,tabwidth);
}


module NegativeWidthTab(l = length,w=width, twp = tabwidthperiod, tl = tablength, tw = tabwidth)
{
	translate([-((l/2) + tl/2 ),(-w/2)-(w/twp)/2 ])
	WidthTabs(w,twp,tl,tw);
}

module PositiveWidthTab(l = length,w=width, twp = tabwidthperiod, tl = tablength, tw = tabwidth)

{
	translate([(l/2) + tl/2 ,(-w/2)-(w/twp)/2 ])
	WidthTabs(w,twp,tl,tw);

}

module NegativeLengthTabs(l = length,w=width, tlp = tablengthperiod, tl = tablength, tw = tabwidth)
{
	translate([(-l/2) -(l/tlp)/2,-((w/2) + tl/2 ) ])
	LengthTabs(l,tlp,tl,tw);

}

module PositiveLengthTabs(length,width, tablengthperiod, tablength,  tabwidth)
{
	translate([(-length/2) -(length/tablengthperiod)/2,((width/2) + tablength/2 ) ])
	LengthTabs(length,tablengthperiod,tablength,tabwidth);
}

module WidthScrewSlots(width, tabwidthperiod, screwlength, screwdiameter, hexwidth, hexthickness, offsetHex=0)
{
	for(i = [ 1,4,  tabwidthperiod-1])
	{
		assign(offset = ((width/tabwidthperiod)*i) )
		{
		
			translate([0,offset + (width/tabwidthperiod)/2 ])
			{
				ScrewSlot(screwlength, screwdiameter);

				translate([-screwlength/3 + offsetHex,0,0])
				HexSlot(hexwidth, hexthickness);
			}
		}
	}
}

module WidthScrewHoles(width, tabwidthperiod, screwdiameter)
{
	for(i = [ ])
	{
		assign(offset = ((width/tabwidthperiod)*i) )
		{
		
			translate([0,offset + (width/tabwidthperiod)/2 ])
			circle(r=screwdiameter/2, $fn=50);
			
		}
	}
}

module LengthScrewHoles(length, tablengthperiod, screwdiameter)
{
	for(i = [ 1  ,4, tablengthperiod-1])
	{
		assign(offset = ((length/tablengthperiod)*i) )
		{
		
			translate([offset - (length/tablengthperiod)/2 ,0])
			circle(r=screwdiameter/2, $fn=50);
		}
	}
}

module LengthScrewSlots(length, tablengthperiod, screwlength, screwdiameter, hexwidth, hexthickness, offsetHex=0)
{
	for(i = [ ])
	{
		assign(offset = ((length/tablengthperiod)*i) )
		{
		
			translate([offset - (length/tablengthperiod)/2 ,0])
			{
				ScrewSlot(screwdiameter,screwlength);
				translate([0,-screwlength/3+offsetHex,0])
				HexSlot(hexthickness,hexwidth);
			}
		}
	}
}


module WidthTabs(width, tabwidthperiod, tablength, tabwidth)
{
	for(i = [ 1  : 1 : tabwidthperiod])
	{
		assign(offset = ((width/tabwidthperiod)*i) )
		{
		
			translate([0 ,offset ])
			tab(tablength,tabwidth);
		}
	}
}

module LengthTabs(length,tablengthperiod, tablength, tabwidth)
{
	for(i = [ 1  : 1 : tablengthperiod])
	{
		assign(offset = ((length/tablengthperiod)*i) )
		{
		
			translate([offset , 0])
			tab(tabwidth,tablength);
		}
	}
}


module tab(a,b) 
{
	square([a,b], center=true);
}


module ScrewSlot(a, b)
{
	square([a+2,b], center=true);
	
}

module HexSlot(a, b)
{
	square([a,b], center=true);
}

module ArduinoMegaHoleLayout()
{

	//difference()
	//{
	//	square ([54,102]);

		translate([1,88,0])
		screwhole();

		translate([50,86,0])
		screwhole();

		translate([50,12,0])
		screwhole();

		translate([1,6,0])
		screwhole();
	//}
}

module screwhole()
{
translate([1.6,1.6,0])
circle(r = 1.4, $fn=50);
}
