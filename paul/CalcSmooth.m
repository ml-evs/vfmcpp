function [xres,yres,zres,vindres] = CalcSmooth(delta,xt,yt,zt,vindt)

xres=[];
yres=[];
zres=[];
vindres=[];

for k=min(vindt):max(vindt)
    ind=find(vindt==k);
    x=xt(ind);
    y=yt(ind);
    z=zt(ind);
    ell=CalcMeshLengths(x,y,z);
    N=length(ell);
    [xpp,ypp,zpp]=CalcSDoublePrime(x,y,z,ell);
    curv=sqrt(xpp.*xpp+ypp.*ypp+zpp.*zpp);
    ind2=find(curv<=1.9/delta);
    x=x(ind2);
    y=y(ind2);
    z=z(ind2);
    if length(ind2)~=N
        disp(['Removed points due to high curvature: ',num2str(N-length(ind2))]);
    end
    xres=[xres x];
    yres=[yres y];
    zres=[zres z];
    vindres=[vindres k.*ones(1,length(x))];
end

end

