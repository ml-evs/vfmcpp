function res= CalcMeshLengths(x,y,z)
% see Baggaley and Barenghi, JLTP 166,3 (2012). BOTTOM OF PAGE 11
N=length(x);

xdiff=diff(x);
ydiff=diff(y);
zdiff=diff(z);

xdiff(2:N)=xdiff;
ydiff(2:N)=ydiff;
zdiff(2:N)=zdiff;

xdiff(1)=x(1)-x(end);
ydiff(1)=y(1)-y(end);
zdiff(1)=z(1)-z(end);

res=sqrt(xdiff.*xdiff+ydiff.*ydiff+zdiff.*zdiff);

end

