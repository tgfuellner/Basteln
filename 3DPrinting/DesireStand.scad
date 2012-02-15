// HTC Desire stand

EPS=0.01;
$fs=0.2;

StandX=80;
StandY=50;
StandZ=10.7;
Inset=7.5;

module samsung() {
  translate([StandX+EPS,0,0]) rotate(70,[1,0,0]) rotate(-90,[0,1,0])
  linear_extrude(height = StandX+2*EPS, center = false, convexity = 10, twist = 0)
  polygon(points=[[0,10],[0,0],[0.5,-2],[1,-3],[1.5,-3.9],[2,-4.1],[2.5,-4.5],[3,-4.8]
        ,[3.5,-5],[4,-5.3],[4.5,-5.5],[5,-5.6],[5.5,-5.6],[6,-5.5]
        ,[6.5,-5.5],[7,-5.4],[7.5,-5.3],[8,-5.2],[8.5,-5.1],[9,-5]
        ,[9.5,-4.5],[10,-4],[10.5,-3.5],[11,-3],[11,1]],
              paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24]]);
}

module desire() {
  scale([1,1.06,1]) translate([StandX+EPS,0,0]) rotate(60,[1,0,0]) rotate(-90,[0,1,0])
  linear_extrude(height = StandX+2*EPS, center = false, convexity = 10, twist = 0)
  polygon(points=[[-1,10],[0,0],[0.5,-2],[1,-3],[1.5,-3.9],[2,-4.1],[2.5,-4.5],[3,-4.8]
        ,[3.5,-5],[4,-5.3],[4.5,-5.5],[5,-5.6],[5.5,-5.6],[6,-5.5]
        ,[6.5,-5.5],[7,-5.4],[7.5,-5.3],[8,-5.2],[8.5,-5.1],[9,-5]
        ,[9.5,-4.5],[10,-4],[10.5,-3.5],[11,-3],[11,1]],
              paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24]]);
}

difference() {
  cube(size=[StandX-EPS,StandY,StandZ]);
  //translate([0,14,4.5]) desire();
  translate([0,14,6]) samsung();
  translate([-EPS,20,3.3]) cube(size=[StandX+2*EPS,StandY,StandZ]);
  translate([Inset,20,-EPS]) cube(size=[StandX-Inset*2,15,StandZ]);
}
