clear

for k=1:4861
    load(['Data\Data_',num2str(k),'.mat']);
    figure(1)
    clf; hold all
    
    for m=min(vind):max(vind)
       ind=find(vind==m);
       plot3(x([ind ind(1)]),y([ind ind(1)]),z([ind ind(1)]))
    end
    axis tight
    %axis equal
    %xlim([-3e-6 3e-6])
    %ylim([-3e-6 3e-6])
    %zlim([-10e-6 10e-6])
    
    %axis square
    %view(3)
    view([0 10])
    drawnow
    
    tm(k)=t;
    
    ind1=find(vind==1);
    ind2=find(vind==2);
    
    r1(k)=(max(x(ind1))-min(x(ind1)))/2;
    r2(k)=(max(x(ind2))-min(x(ind2)))/2;
    z1(k)=mean(z(ind1));
    z2(k)=mean(z(ind2));
    x1(k)=mean(x(ind1));
    x2(k)=mean(x(ind2));
    y1(k)=mean(y(ind1));
    y2(k)=mean(y(ind2));
end