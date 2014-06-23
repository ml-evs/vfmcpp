clear;

%Constants
kappa=9.98e-8;
kappa4pi=kappa/(4*pi);
kappa2pi=kappa/(2*pi);
a0=1.3e-10;
a1=exp(0.5)*a0;

% mutual friction
alpha=0;

%Number of points on each ring
N=100;

%Initial radius of both rings
r0=[1e-6 0.9e-6];

%Coordinates for the rings initial position
x0=[0 0.45e-6];
y0=[0 0];
z0=[2e-6 0];

theta=linspace(0,2*pi,N+1);
theta=theta(1:end-1);

x1=x0(1)+r0(1).*cos(theta);
y1=y0(1)+r0(1).*sin(theta);
z1=z0(1)+zeros(1,N);

x2=x0(2)+r0(2).*cos(theta);
y2=y0(2)+r0(2).*sin(theta);
z2=z0(2)+zeros(1,N);

ell=CalcMeshLengths(x1,y1,z1);
%spatial resolution

delta=4*mean(ell)/3;
%Time step
maxdt=(delta/2)^2/(kappa*log(delta/(2*pi*a0)));
dt=maxdt/25; % As Baggaley, Barenghi PRB, 2010

x=[x1 x2];
y=[y1 y2];
z=[z1 z2];

vind=[1*ones(1,length(x1)) 2*ones(1,length(x2))];

% How often to save a file
file_cycle=100;

t=0;
firstloop=0;
count=0;
filenum=0;
Nt=length(x);
% MAIN TIME LOOP STARTS HERE
while t<1

[vx,vy,vz]=CalcVelMaster(x,y,z,vind,alpha);
%[vx2,vy2,vz2,ell2]=CalcVelMaster(x2,y2,z2,x1,y1,z1);


if firstloop==0;
    vxm2=mean(vx).*ones(1,Nt);
    vxm1=mean(vx).*ones(1,Nt);
    vym2=mean(vy).*ones(1,Nt);
    vym1=mean(vy).*ones(1,Nt);
    vzm2=mean(vz).*ones(1,Nt);
    vzm1=mean(vz).*ones(1,Nt);
    vxn=vx;
    vyn=vy;
    vzn=vz;
    firstloop=1;
else
    vxm2=vxm1;
    vxm1=vxn;
    vxn=vx;
    vym2=vym1;
    vym1=vyn;
    vyn=vy;
    vzm2=vzm1;
    vzm1=vzn;
    vzn=vz;     
end

x=x+CalcAdamsBashworth(dt,vxn,vxm1,vxm2);
y=y+CalcAdamsBashworth(dt,vyn,vym1,vym2);
z=z+CalcAdamsBashworth(dt,vzn,vzm1,vzm2);


if count==file_cycle
    count=0;
    filenum=filenum+1;
    save(['Data\Data_',num2str(filenum),'.mat'],'t','x','y','z','vind');
    disp(['time= ', num2str(t), ' N= ',num2str(length(x))])
    
    
    figure(1)
    clf; hold all
    for k=min(vind):1:max(vind)
        ind=find(vind==k);
        plot3(x([ind ind(1)]),y([ind ind(1)]),z([ind ind(1)]))
    end
    xlim([min(x) max(x)])
    ylim([min(y) max(y)])
    zlim([min(z) max(z)])
    axis square
    view(3)
    drawnow
end

count=count+1;
t=t+dt;

%[xn,yn,zn,vindn]=CalcMeshAdjustment(delta,x,y,z,ell,vind);
%[xn,yn,zn,vindn]=CalcReconnection(delta,xn,yn,zn,vindn);

nnn=1;
nn=length(x);
xn=x;
yn=y;
zn=z;
vindn=vind;


reconn_count=0;
while nn~=nnn
    nnn=nn;
    
    [xn,yn,zn,vindn]=CalcReconnection(delta,xn,yn,zn,vindn);
    
    nn=length(xn);
    reconn_count=reconn_count+1;
    if reconn_count==20
        disp('Reconnection loop limit exceeded');
        nn=nnn;
    end    
end

[xn,yn,zn,vindn]=CalcSmooth(delta,xn,yn,zn,vindn);
[xn,yn,zn,vindn]=CalcMeshAdjustment(delta,xn,yn,zn,vindn);
[xn,yn,zn,vindn]=FuncLoopKiller(6,xn,yn,zn,vindn);

if length(x)~=length(xn)
   Fvxn=scatteredInterpolant(x',y',z',vxn'); 
   Fvyn=scatteredInterpolant(x',y',z',vyn'); 
   Fvzn=scatteredInterpolant(x',y',z',vzn');
   Fvxm1=scatteredInterpolant(x',y',z',vxm1'); 
   Fvym1=scatteredInterpolant(x',y',z',vym1'); 
   Fvzm1=scatteredInterpolant(x',y',z',vzm1');
   x=xn;
   y=yn;
   z=zn;
   vind=vindn;
   
   vxn=Fvxn(x,y,z);
   vyn=Fvyn(x,y,z);
   vzn=Fvzn(x,y,z);
   vxm1=Fvxm1(x,y,z);
   vym1=Fvym1(x,y,z);
   vzm1=Fvzm1(x,y,z);
end

end

