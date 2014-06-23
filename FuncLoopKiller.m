function [resx,resy,resz,resvind] =FuncLoopKiller(MinN,x,y,z,vind)
N=length(x);

resx=zeros(1,N);
resy=zeros(1,N);
resz=zeros(1,N);
resvind=zeros(1,N);


vc=0;
for k=1:min(vind):max(vind)
   ind=find(k==vind);
   if length(ind)>=MinN
       vc=vc+1; 
       resx(ind)=x(ind);
       resy(ind)=y(ind);
       resz(ind)=z(ind);
       resvind(ind)=vc.*ones(1,length(ind));
   end    
end

ind=find(resvind>0);
resx=resx(ind);
resy=resy(ind);
resz=resz(ind);
resvind=resvind(ind);

end

