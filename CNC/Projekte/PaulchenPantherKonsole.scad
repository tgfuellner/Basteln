plexiDicke=10.4;
plexiLaenge=100.5;
plexiInKonsole=15;

BattHalterDicke=32;
BattHalterLaenge=61;

Rand=10;

konsZ=30;
konsY=Rand+plexiDicke+BattHalterDicke+Rand;
konsX=plexiLaenge+2*Rand;


difference() {
    cube(size=[konsX,konsY,konsZ]);
    translate([Rand,Rand,konsZ-plexiInKonsole]) {
        cube(size=[plexiLaenge,plexiDicke,plexiInKonsole+0.2]);
    }
}
