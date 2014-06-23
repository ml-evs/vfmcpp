function [vx,vy,vz] =CalcNonLocalVel(x,y,z)
N=length(x);

parfor k=1:N
    [vx(k),vy(k),vz(k)]=SegmentTerm(x,y,z,k);
end

end

function [xres,yres,zres]=SegmentTerm(x,y,z,k);
kappa=9.98e-8;
kappa4pi=kappa/(4*pi);

N=length(x);
    
xi=x(k);
yi=y(k);
zi=z(k);

if k==1
    px=[x(2:N-1)]-xi;
    py=[y(2:N-1)]-yi;
    pz=[z(2:N-1)]-zi;
else    
    px=[x(1:k-2),x(k+1:N)]-xi;
    py=[y(1:k-2),y(k+1:N)]-yi;
    pz=[z(1:k-2),z(k+1:N)]-zi;
end

if k==N
    qx=[x(2:N-1)]-xi;
    qy=[y(2:N-1)]-yi;
    qz=[z(2:N-1)]-zi;
elseif k==1
    qx=x(3:N)-xi;
    qy=y(3:N)-yi;
    qz=z(3:N)-zi;    
else
    qx=[x(2:k-1),x(k+2:N),x(1)]-xi;
    qy=[y(2:k-1),y(k+2:N),y(1)]-yi;
    qz=[z(2:k-1),z(k+2:N),z(1)]-zi;
end
        
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
