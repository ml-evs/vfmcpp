function [resx,resy,resz,resvind] = CalcReconnection(delta,xt,yt,zt,vind)

N=length(xt);

x=xt;
y=yt;
z=zt;

% build index information of the different vortex loops
start_of_loop=1;
for k=1:N-1
    if vind(k)==vind(k+1)
        in(k)=k+1;
        ib(k+1)=k;
    else
        in(k)=start_of_loop;
        ib(start_of_loop)=k;
        start_of_loop=k+1;
    end  
end
in(N)=start_of_loop;
ib(N)=N-1;
ib(start_of_loop)=N;

in0=in;
ib0=ib;


nr=0;
rp1=[];
rp2=[];
% calculate the distance of every point to every other point
for k=1:N
    distx=x-x(k);
    disty=y-y(k);
    distz=z-z(k);
    dist=sqrt(distx.*distx+disty.*disty+distz.*distz);
    ind=find(dist<=0.5.*delta);
    if length(ind)~=1
        for m=1:length(ind)
            if ind(m)==k % same point
            elseif ib(ind(m))==k || in(ind(m))==k % neighbouring point
            %elseif length(find(rp1==k))>=1 || length(find(rp2==k))>=1 || length(find(rp1==ind(m)))>=1 || length(find(rp2==ind(m)))>=1 % already reconnected points
            %elseif length(find(rp1==k+1))>=1 || length(find(rp2==k+1))>=1 || length(find(rp1==ind(m)+1))>=1 || length(find(rp2==ind(m)+1))>=1 % next to reconnected points
            %elseif length(find(rp1==k-1))>=1 || length(find(rp2==k-1))>=1 || length(find(rp1==ind(m)-1))>=1 || length(find(rp2==ind(m)-1))>=1 % next to reconnected points
            else
                
                nr=nr+1;
                rp1(nr)=k;
                rp2(nr)=ind(m);
                nr_dist(nr)=dist(ind(m));
            end
        end    
    end
end
    
    
if nr==0 % if there are no reconnections, no change
    resx=xt;
    resy=yt;
    resz=zt;
    resvind=vind;
else
    [~,rci]=min(nr_dist);
    
    for k=min(vind):max(vind)
       ind=find(k==vind);
       ell(ind)=CalcMeshLengths(x(ind),y(ind),z(ind));
       [xp(ind),yp(ind),zp(ind)]=CalcSPrime(x(ind),y(ind),z(ind),ell(ind)); 
    end
    
    ell_total=sum(ell);
    
    k=rci; % only choose closest two points to reconnect.
        dot=xp(rp1(k)).*xp(rp2(k))+yp(rp1(k)).*yp(rp2(k))+zp(rp1(k)).*zp(rp2(k));
        
        if dot<0.9
            % switch flags
            disp('reconnection')
            % new code - 25/07/13
            % haven't done anything with the before flags as they're not
            % used later on in this code.
            in(ib(rp2(k)))=in(rp1(k));
            in(ib(rp1(k)))=in(rp2(k));
            
            % old code - changed 25/07/13
            % has a bug if rp1=1 ot rp2=end - duh!
            %in(rp1(k)-1)=in(rp2(k));
            %ib(rp2(k)+1)=ib(rp1(k));
        
            %in(rp2(k)-1)=in(rp1(k));
            %ib(rp1(k)+1)=ib(rp2(k));
        
        
            % flags so that points will be deleted.
            in(rp1(k))=-1;
            in(rp2(k))=-1;
            %ib(rp1(k))=-1;
            %ib(rp2(k))=-1;
        end
    

    % rebuild & retrace vectors from index flags - not very elegant!
    resz=zeros(1,N);
    resy=zeros(1,N);
    resx=zeros(1,N);
    resvind=zeros(1,N);
    flags=zeros(1,N);
    vcount=0;
    counter=0;
    
    while ~isempty(find(flags==0, 1))
        point_ind=find(flags==0,1);
        if in(point_ind)==-1
            counter=counter+1;
            flags(point_ind)=1;
            resvind(counter)=-1; % marker to show point needs to be discarded
        else
            counter=counter+1;
            vcount=vcount+1;
            first_point=point_ind;
            resx(counter)=x(point_ind);
            resy(counter)=y(point_ind);
            resz(counter)=z(point_ind);
            resvind(counter)=vcount;
            flags(point_ind)=1;
            point_ind=in(point_ind);
            
            % Trace each loop
            while point_ind~=first_point;
                counter=counter+1;
                
                %if point_ind>N || point_ind<1
                %    disp('oops!')
                %end
                
                if point_ind==-1;
                    disp('oops!')
                    point_ind=first_point;
                    counter=counter-1;
                else
                    resx(counter)=x(point_ind);
                    resy(counter)=y(point_ind);
                    resz(counter)=z(point_ind);
                    resvind(counter)=vcount;
                    flags(point_ind)=1;
                    point_ind=in(point_ind);
                end
            end
            
            
        end
            
            
        
    end
   
    
    % only keep points that do not have the -1 marker
    ind=find(resvind~=-1);
    resx=resx(ind);
    resy=resy(ind);
    resz=resz(ind);
    resvind=resvind(ind);
    
    % check there are a minimum of 5 points in each loop, otherwise the
    % loop is removed (because derivatives need 2 points both in front and
    % behind to make any sense).
    xout=[];
    yout=[];
    zout=[];
    vout=[];
    vc=0;
    
    for k=1:max(resvind)
        ind=find(resvind==k);
        if length(ind)>=6
           vc=vc+1; 
           loopx=resx(ind);
           loopy=resy(ind);
           loopz=resz(ind);
           
           %ell=CalcMeshLengths(loopx,loopy,loopz);
           %[loopxpp,loopypp,loopzpp]=CalcSDoublePrime(loopx,loopy,loopz,ell);
           %curv=sqrt(loopxpp.*loopxpp+loopypp.*loopypp+loopzpp.*loopzpp);
    
           %ind2=find(curv<=1.9/delta);
           %loopx=loopx(ind2);
           %loopy=loopy(ind2);
           %loopz=loopz(ind2);
           
           if length(loopx)>=6
                xout=[xout loopx];
                yout=[yout loopy];
                zout=[zout loopz];
                vout=[vout vc.*ones(1,length(loopx))];
           else
                disp(['loop removed: ',num2str(length(loopx))]);
           end
        else
           disp(['loop removed: ',num2str(length(ind))]); 
            
        end
        
    end
    resx=xout;
    resy=yout;
    resz=zout;
    resvind=vout;
    
    %for k=min(resvind):max(resvind)
    %   ind=find(k==resvind);
    %   ell(ind)=CalcMeshLengths(resx(ind),resy(ind),resz(ind)); 
    %end
    
    % if length has increased revert to original configuration
    %if sum(ell)>ell_total
    %    resx=xt;
    %    resy=yt;
    %    resz=zt;
    %    resvind=vind;    
    %end
    
    figure(1)
    clf; hold all
    for k=min(resvind):1:max(resvind)
        ind=find(resvind==k);
        plot3(resx([ind ind(1)]),resy([ind ind(1)]),resz([ind ind(1)]))
    end
    xlim([min(resx) max(resx)])
    ylim([min(resy) max(resy)])
    zlim([min(resz) max(resz)])
    axis square
    view(3)
    drawnow
    %pause
end
end
