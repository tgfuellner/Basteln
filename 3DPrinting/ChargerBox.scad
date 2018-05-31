    /*******************************************************
*
*
*     FB Aka HeartyGFX 2016
*     http://heartygfx.blogspot.com
*     OpenScad Parametric Box
*
*
********************************************************/


////////////////// - Paramètres de la boite - Box parameters - //////////////////////

//Panneau arrière - Back panel  
BPanel=0;// [0:No, 1:Yes]
//Panneau avant - Front panel
FPanel=0;// [0:No, 1:Yes]
// pcb-holder
PHolder=1; // [0:No, 1:Yes]
//demi Coque - Half shell
BShell=0;// [0:No, 1:Yes]
//Coque haut - Top shell
TShell=1;// [0:No, 1:Yes]
//Texte façade - Front text
Text=0;// [0:No, 1:Yes]
//Holes in box [0:No, 1:Yes]
Holes=1;
// Vertical lines as holes [0:No, 1:Yes]
VHoles=1;
//Hole Wdt in box [1=1mm 3=3mm]
Holewdt=1;
// PCB Holder on/off
PCBFix=1;
// PCB Heigh over box bottom
PCBHight=10;
PCBLength=67;
PCBWidth=48;
 // material sickness
PCBThick=1.6;

// show pcb for demo
ShowPCB=0;

// - Text you want
  txt = "Charger 42V";           
// - Font size  
  TxtSize = 5;                 
// - Font  
  Police="Arial Black"; 
// - Longueur - Length  
  Length = 90;       
// - Largeur - Width
  Width = 65;                     
// - Hauteur - Height  
  Height = 53;                                       
// - Diamètre Coin arrondi - Round corner diameter  
  Round = 2;//[0.1:12] 
// - lissage de l'arrondi - Round smoothness  
  Resolution = 50;//[1:100] 
// - Epaisseur - Thickness  
  Thick = 2;//[1:5]
// - Marge - Marging
  m = 0.5;
  
/* [Hidden] */
// - Couleur coque - Shell color  
  Couleur1 = "Yellow";       
// - Couleur coque - Shell color  
  Couleur2 = "Yellow";// "Grey";   
// - Couleur panneaux - Panels color    
  Couleur3 = "Orange";    
// Couleur du circuit imprimé - PCB color
  CouleurPCB = "Green";


/////////// - Boitier générique bord arrondis - Generic rounded box - //////////

module RoundBox($a=Length, $b=Width, $c=Height){// Cube bords arrondis
                    $fn=Resolution;            
                    translate([0,Round,Round]){  
                    minkowski (){                                              
                        cube ([$a-(Length/2),$b-(2*Round),$c-(2*Round)], center = false);
                        rotate([0,90,0]){    
                        cylinder(r=Round,h=Length/2, center = false);
                            } 
                        }
                    }
                }// End of RoundBox Module

      
////////////////////////////////// - Module Coque/Shell - //////////////////////////////////         

module Coque(){//Coque - Shell  
    Thick = Thick*2;  
    VHoles = VHoles*TShell;
    difference(){    
        difference(){//sides decoration
            union(){    
                     difference() {//soustraction de la forme centrale - Substraction rounded box
                      
                        difference(){//soustraction cube median - Median cube slicer
                            union() {//union               
                            difference(){//Coque    
                                RoundBox();
                                translate([Thick/2,Thick/2,Thick/2]){     
                                        RoundBox($a=Length-Thick, $b=Width-Thick, $c=Height-Thick);
                                        }
                                        }//Fin diff Coque                            
                                difference(){//largeur Rails        
                                     translate([Thick+m,Thick/2,Thick/2]){// Rails
                                          RoundBox($a=Length-((2*Thick)+(2*m)), $b=Width-Thick, $c=Height-(Thick*2));
                                                          }//fin Rails
                                     translate([((Thick+m/2)*1.55),Thick/2,Thick/2]){
                                          RoundBox($a=Length-((Thick*3)+2*m), $b=Width-Thick, $c=Height-Thick);
                                                    }           
                                                }//Fin largeur Rails
                                    }//Fin union                                   
                               translate([-Thick,-Thick,Height/2]){// Cube à soustraire
                                    cube ([Length+100, Width+100, Height], center=false);
                                            }                                            
                                      }//fin soustraction cube median - End Median cube slicer
                               translate([-Thick/2,Thick,Thick]){// Forme de soustraction centrale 
                                    RoundBox($a=Length+Thick, $b=Width-Thick*2, $c=Height-Thick);       
                                    }                          
                                }                                          


                difference(){// Fixation box legs
                    union(){
                        translate([3.9*Thick,Thick,Height/2]){
                            rotate([90,0,0]){
                                    $fn=6;
                                    cylinder(d=16,Thick/2);
                                    }   
                            }
                            
                       translate([Length-3.9*Thick,Thick,Height/2]){
                            rotate([90,0,0]){
                                    $fn=6;
                                    cylinder(d=16,Thick/2);
                                    }   
                            }
         
                        }
                            translate([4,Thick+Round,Height/2-57]){   
                             rotate([45,0,0]){
                                   cube([Length,40,40]);    
                                  }
                           }
                           translate([0,-(Thick*1.46),Height/2]){
                                cube([Length,Thick*2,10]);
                           }
                    } //Fin fixation box legs
                    
                difference(){// Opposit box legs
                      union(){
                        translate([Length/2+4,Width-Thick/2,Height/2]){                        
                            rotate([90,0,0]){
                                    $fn=6;
                                    cylinder(d=23 ,Thick/2);
                                    }   
                          }
                         translate([Length/2-4,Width-Thick/2,Height/2]){                        
                            rotate([90,0,0]){
                                    $fn=6;
                                    cylinder(d=23,Thick/2);
                                    }   
                          }
                          
                          
                     }
                        translate([4,Width-Thick-Round,Height/2-57]){   
                             rotate([45,0,0]){
                                   cube([Length,40,40]);    
                                  }
                           }
                           translate([4,Width-Thick-Round,Height/2+7]){   
                             
                                   cube([Length,40,40]);    
                                  
                           }
                        translate([0,Width+(Thick*1.46),Height/2]){
                                cube([Length,Thick*2,10]);
                           }
                    } //Fin box legs
                    
                    if(PCBFix==1 && TShell==0){// PCB Holder
                        
                        // Fixation laterales-Side Stops
                        translate([(Length-PCBLength)/2,Thick/2,0]){
                           union(){
                                cube([Thick/2,(Width-PCBWidth)/2,PCBHight-PCBThick]);
                                cube([Thick/2,(Width-PCBWidth)/2-Thick/2-m/2,PCBHight]);
                                }
                            }
                        translate([Length-(Length-PCBLength)/2-Thick/2,Thick/2,0]){
                           union(){
                                cube([Thick/2,(Width-PCBWidth)/2,PCBHight-PCBThick]);
                                cube([Thick/2,(Width-PCBWidth)/2-Thick/2-m/2,PCBHight]);
                                }
                            }
                        translate([Length-(Length-PCBLength)/2,Width-Thick-0.3,0]){
                            rotate([0,0,180]){
                           union(){
                                cube([Thick/2,(Width-PCBWidth)/3,PCBHight-PCBThick]);
                                cube([Thick/2,(Width-PCBWidth)/3-Thick/2+.2,PCBHight]);
                                }
                            }
                        }
                        translate([(Length-PCBLength)/2+Thick/2,Width-Thick-0.3,0]){
                            rotate([0,0,180]){
                            union(){
                                cube([Thick/2,(Width-PCBWidth)/3,PCBHight-PCBThick]);
                                cube([Thick/2,(Width-PCBWidth)/3-Thick/2+.2,PCBHight]);
                                }
                            }
                        }
                        // Fixation Frontales-Front/Back stops
//                        translate([(Length-PCBLength)/2,(Width-PCBWidth)/2,0]){
                          translate([Thick*2,(Width/2-Thick/2),0]){
                                //cube([(Width-PCBWidth)/2+Thick/2-m/2,Thick,PCBHight]);
                            }
                          translate([(Length/2+PCBLength/2)+m/2,Width/2-Thick/2,0]){
                             //cube([(Width-PCBWidth)/3,Thick,PCBHight]);      
                            }
                    
                }  
            }

        union(){// outbox sides decorations
            for(i=[0:Thick:Length/4]){

                translate([(Length-10) - i,-Thick+0.6,0]){
                    cube([Holewdt,Thick*(VHoles+2),Height/4]);
                    }
                    
                translate([10+i,-Thick+0.6,0]){
                    cube([Holewdt,Thick*(VHoles+2),Height/4]);
                    }    
                    
                translate([10+i,Width-0.6+(VHoles*(-2))-4,0]){
                    cube([Holewdt,Thick+2,Height/3]);
                    } 
                    
                translate([(Length-10) - i,Width-0.6+(VHoles*(-2))-4,0]){
                    cube([Holewdt,Thick+2,Height/4]);
                    }    
                
                    }// fin de for
                }//fin union decoration
            }//fin difference decoration


            union(){ //sides holes
                $fn=50;
                translate([2.6*Thick+5,20,Height/2+4]){
                    rotate([90,0,0]){
                    cylinder(d=2,20);
                    }
                }
                translate([Length-(3.9*Thick),20,Height/2+4]){
                    rotate([90,0,0]){
                    cylinder(d=2,20);
                    }
                }
                translate([2.6*Thick+5,Width+5,Height/2-4]){
                    rotate([90,0,0]){
                    cylinder(d=2,20);
                    }
                }
                translate([Length-(3.9*Thick),Width+5,Height/2-4]){
                    rotate([90,0,0]){
                    cylinder(d=2,20);
                    }
                }
            }//fin de sides holes

        }//fin de difference holes
}// fin coque 

////////////////////////////// - Experiment - ///////////////////////////////////////////
          


///////////////////////////////// - Module Front/Back Panels - //////////////////////////
                           // m=margin 
module Panels(){//Panels
    color(Couleur3){
        translate([Thick+m,m/2,m/2]){
           union(){
             difference(){
                  translate([0,Thick,Thick]){ 
                     RoundBox(Length,Width-((Thick*2)+m),Height-((Thick*2)+m));}
                  translate([Thick,-5,0]){
                     cube([Length,Width+10,Height]);}
                   
                     }
                 }
                 if(PCBFix==1){ // Ajoute les fixations latérales /generate Side PCB-Fix
                
                     // Gauche-Left
                     translate([Thick,Width/2-PCBWidth/2,PCBHight]){
                     cube([(Length-PCBLength)/2-Thick,Thick*3,Thick]);}
                     translate([Thick,Width/2-PCBWidth/2,PCBHight-PCBThick*3]){
                     cube([(Length-PCBLength)/2-Thick*2-m,Thick*3,Thick*3]);}
                     // Droite-Right
                     translate([Thick,Width/2+PCBWidth/2-Thick*3,PCBHight]){
                     cube([(Length-PCBLength)/2-Thick,Thick*3,Thick]);}
                     translate([Thick,Width/2+PCBWidth/2-Thick*3,PCBHight-PCBThick*3]){
                     cube([(Length-PCBLength)/2-Thick*2-m,Thick*3,Thick*3]);}
                 
                 }// fin if
             }
         }
}


///////////////////////////////////// - Main - ///////////////////////////////////////
if(BPanel==1)
//Back Panel
translate ([-m/2,0,0]){
Panels();
}
if(FPanel==1)
//Front Panel
rotate([0,0,180]){
    translate([-Length-m/2,-Width,0]){             
     Panels();
       }
   }

if(Text==1)
// Front text
color(Couleur1){     
     translate([Length-(Thick),Thick*4,(Height-(Thick*4+(TxtSize/2)))]){// x,y,z
          rotate([90,0,90]){
              linear_extrude(height = 0.25){
              text(txt, font = Police, size = TxtSize,  valign ="center", halign ="left");
                        }
                 }
         }
}


if(BShell==1)
// Coque bas - Bottom shell
color(Couleur1){ 
Coque();
}


if(TShell==1)
// Coque haut - Top Shell
color( Couleur2,2){
    translate([0,Width,Height+0.2]){
        rotate([0,180,180]){
                Coque();
                }
        }
}

if(ShowPCB==1 && PCBFix==1)
// Dessiner un PBC - Show PCB
color(CouleurPCB){
    translate([(Length-PCBLength)/2,(Width-PCBWidth)/2,PCBHight-PCBThick]){
        
        cube([PCBLength,PCBWidth,PCBThick]);
        }
}
