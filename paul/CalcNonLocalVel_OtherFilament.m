function [vx,vy,vz] =CalcNonLocalVel_OtherFilament(x,y,z,x1,x2,x3)
N=length(x);

parfor k=1:N
    [vx(k),vy(k),vz(k)]=SegmentTerm(x(k),y(k),z(k),x1,x2,x3);
end

end

function [xres,yres,zres]=SegmentTerm(xi,yi,zi,x,y,z);
kappa=9.98e-8;
kappa4pi=kappa/(4*pi);

N=length(x);

px=x-xi;
py=y-yi;
pz=z-zi;

qx=x([2:N,1])-xi;
qy=y([2:N,1])-yi;
qz=z([2:N,1])-zi;
        
A=sqrt(px.*px+py.*py+pz.*pz);
B=(px.*qx+py.*qy+pz.*qz);
C=sqrt(qx.*qx+qy.*qy+qz.*qz);

D=kappa4pi.*(A+C)./(A.*C.*(A.*C+B));

xcross=py.*qz-pz.*qy;
ycross=pz.*qx-px.*qz;
zcross=px.*qy-py.*qx;

xres=sum(D.*xcross);
yres=sum(D.*ycross);
zres=sum(D.*zcross);
end
