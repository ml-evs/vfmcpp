function [xres0,yres0,zres0,vindres0] =CalcMeshAdjustment(delta,xt,yt,zt,vind)

for kkk=min(vind):max(vind)
    ind0=find(kkk==vind);
    x=xt(ind0);
    y=yt(ind0);
    z=zt(ind0);
    ell=CalcMeshLengths(x,y,z);
    N=length(x);
    

    
    % Remove points if spacing is less than delta/2
    ind=find(ell>=delta/2);
    if length(ind)~=N
        disp(['Removed points due to proximity: ',num2str(N-length(ind))]);
        
        if isempty(ind)
            x=x(1:2:N);
            y=y(1:2:N);
            z=z(1:2:N);
            N=length(x);
            %ell=CalcMeshLengths(x,y,z);
        else
            x=x(ind);
            y=y(ind);
            z=z(ind);
            N=length(x);
            %ell=CalcMeshLengths(x,y,z);
        end
    end    
    


    xres{kkk}=x;
    yres{kkk}=y;
    zres{kkk}=z;
    vindres{kkk}=kkk.*ones(1,length(xres{kkk}));
    N=length(x);
    %N1=0;
    %N2=N;
    
    %while N1~=N2 % Keep adding new points
    %    N1=N2;
    %    xold=x;
    %    yold=y;
    %    zold=z; 
        % Calculate local curvature at each point and only keep points with curvature <=1.9/delta;
        %ell=CalcMeshLengths(x,y,z);
        %N=length(ell);
        %[xpp,ypp,zpp]=CalcSDoublePrime(x,y,z,ell);
        %curv=sqrt(xpp.*xpp+ypp.*ypp+zpp.*zpp);
        %ind=find(curv<=1.9/delta);
        %x=x(ind);
        %y=y(ind);
        %z=z(ind);
   
        ell=CalcMeshLengths(x,y,z);
        N=length(ell);
        %if length(ind)~=N
        %    disp(['Removed points due to high curvature: ',num2str(N-length(ind))]);
        %end
        
        
        % Add points if spacing is greater than delta
        %ell=CalcMeshLengths(x,y,z);
        ind=find(ell>=delta);    
            if ~isempty(ind)
                disp(['Add new points: ',num2str(length(ind))]);
                [xpp,ypp,zpp]=CalcSDoublePrime(x,y,z,ell);
                N=length(ell);
                for k=1:length(ind) %insert new points, slow using for loop but shouldn't be required often
                    newi=ind(k);
            
                if newi~=1
                    newi=newi-1;
                else
                    newi=N;
                end
                
                xnew=[];
                ynew=[];
                znew=[];
                indnew=[];
                
                if newi~=N
                    xppp=(xpp(newi)+xpp(newi+1))/2;
                    yppp=(ypp(newi)+ypp(newi+1))/2;
                    zppp=(zpp(newi)+zpp(newi+1))/2;
                    R=1/sqrt(xppp^2+yppp^2+zppp^2);
                    
                    if R>delta/1.9
                        xnew=[xnew 0.5.*(x(newi)+x(newi+1))+(sqrt(R^2-ell(newi+1)^2/4)-R)*xppp*R];
                        ynew=[ynew 0.5.*(y(newi)+y(newi+1))+(sqrt(R^2-ell(newi+1)^2/4)-R)*yppp*R];
                        znew=[znew 0.5.*(z(newi)+z(newi+1))+(sqrt(R^2-ell(newi+1)^2/4)-R)*zppp*R];
                        indnew=[indnew newi+1/2];
                    end
                else    
                    xppp=(xpp(N)+xpp(1))/2;
                    yppp=(ypp(N)+ypp(1))/2;
                    zppp=(zpp(N)+zpp(1))/2;
                    R=1/sqrt(xppp^2+yppp^2+zppp^2);
                    
                    if R>delta/1.9
                        xnew=[xnew 0.5.*(x(N)+x(1))+(sqrt(R^2-ell(1)^2/4)-R)*xppp*R];
                        ynew=[ynew 0.5.*(y(N)+y(1))+(sqrt(R^2-ell(1)^2/4)-R)*yppp*R];
                        znew=[znew 0.5.*(z(N)+z(1))+(sqrt(R^2-ell(1)^2/4)-R)*zppp*R];
                        indnew=[indnew newi+1/2];
                    end
                end
            end
                % put new points in the correct places within the array.
                
                if ~isempty(find(isreal(xnew)==0,1))
                   disp('Dodgy remesh has happened!') 
                end
                
                xres2=[x xnew];
                yres2=[y ynew];
                zres2=[z znew];
                indres=[1:length(x) indnew];
            
                [~,sort_ind]=sort(indres);
                xres{kkk}=xres2(sort_ind);
                yres{kkk}=yres2(sort_ind);
                zres{kkk}=zres2(sort_ind);
                vindres{kkk}=kkk.*ones(1,length(xres{kkk}));
                %x=xres{kkk};
                %y=yres{kkk};
                %z=zres{kkk};
                %N2=length(x);
                
                %figure(2)
                %plot3(xold,yold,zold,'r+',x,y,z,'b-o',xnew,ynew,znew,'g^')
                %drawnow
                %pause
                
            %else
            %    N2=N1;
            end
    %end

    


end

xres0=[];
yres0=[];
zres0=[];
vindres0=[];

for m=1:kkk
    xres0=[xres0 xres{m}];
    yres0=[yres0 yres{m}];
    zres0=[zres0 zres{m}];
    vindres0=[vindres0 vindres{m}];
end

end    

