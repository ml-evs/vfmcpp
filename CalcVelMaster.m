function [vxt,vyt,vzt,ellt] = CalcVelMaster(xt,yt,zt,vind,alpha)
%constants
kappa=9.98e-8;
kappa4pi=kappa/(4*pi);
a0=1.3e-10;
a1=exp(0.5)*a0;

for count=min(vind):max(vind)
    ind=find(vind==count);
    ind2=find(vind~=count);


    x=xt(ind);
    y=yt(ind);
    z=zt(ind);
    x_other=xt(ind2);
    y_other=yt(ind2);
    z_other=zt(ind2);
    

    N=length(x);
    %length of filament segments
    ell=CalcMeshLengths(x,y,z);
    %calc vectors
    [xp,yp,zp]=CalcSPrime(x,y,z,ell);
    [xpp,ypp,zpp]=CalcSDoublePrime(x,y,z,ell);
    xcross=yp.*zpp-zp.*ypp;
    ycross=zp.*xpp-xp.*zpp;
    zcross=xp.*ypp-yp.*xpp;
    %self-induced velcities
    lvx=zeros(1,N);
    lvy=zeros(1,N);
    lvz=zeros(1,N);
    
    lvx(1:N-1)=kappa4pi.*log(2.*sqrt(ell(1:N-1).*ell(2:N))./a1).*xcross(1:N-1);
    lvx(N)=kappa4pi.*log(2.*sqrt(ell(N).*ell(1))./a1).*xcross(N);
    lvy(1:N-1)=kappa4pi.*log(2.*sqrt(ell(1:N-1).*ell(2:N))./a1).*ycross(1:N-1);
    lvy(N)=kappa4pi.*log(2.*sqrt(ell(N).*ell(1))./a1).*ycross(N);
    lvz(1:N-1)=kappa4pi.*log(2.*sqrt(ell(1:N-1).*ell(2:N))./a1).*zcross(1:N-1);
    lvz(N)=kappa4pi.*log(2.*sqrt(ell(N).*ell(1))./a1).*zcross(N);

    %non-local components of velocity due to segments on the same ring
    [nvx,nvy,nvz]=CalcNonLocalVel(x,y,z);
    %non-local components of velocity due to segments on the other ring
    if ~isempty(ind2)
        [nvx2,nvy2,nvz2]=CalcNonLocalVel_OtherFilament(x,y,z,x_other,y_other,z_other);
        vx=lvx+nvx+nvx2;
        vy=lvy+nvy+nvy2;
        vz=lvz+nvz+nvz2;
    else
        vx=lvx+nvx;
        vy=lvy+nvy;
        vz=lvz+nvz;
    end
    
    vx=vx+alpha.*(zp.*vy-yp.*vz);
    vy=vy+alpha.*(xp.*vz-zp.*vx);
    vz=vz+alpha.*(yp.*vx-xp.*vy);
    
    
    vxt(ind)=vx;
    vyt(ind)=vy;
    vzt(ind)=vz;
    ellt(ind)=ell;

end


end

