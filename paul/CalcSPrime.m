function [xres,yres,zres] = CalcSPrime(x,y,z,ell)

N=length(x);

A(2:N-2)=CalcA(ell(1:N-3),ell(2:N-2),ell(3:N-1),ell(4:N));
A(1)=CalcA(ell(N),ell(1),ell(2),ell(3));
A(N-1)=CalcA(ell(N-2),ell(N-1),ell(N),ell(1));
A(N)=CalcA(ell(N-1),ell(N),ell(1),ell(2));

B(2:N-2)=CalcB(ell(1:N-3),ell(2:N-2),ell(3:N-1),ell(4:N));
B(1)=CalcB(ell(N),ell(1),ell(2),ell(3));
B(N-1)=CalcB(ell(N-2),ell(N-1),ell(N),ell(1));
B(N)=CalcB(ell(N-1),ell(N),ell(1),ell(2));

D(2:N-2)=CalcD(ell(1:N-3),ell(2:N-2),ell(3:N-1),ell(4:N));
D(1)=CalcD(ell(N),ell(1),ell(2),ell(3));
D(N-1)=CalcD(ell(N-2),ell(N-1),ell(N),ell(1));
D(N)=CalcD(ell(N-1),ell(N),ell(1),ell(2));

E(2:N-2)=CalcE(ell(1:N-3),ell(2:N-2),ell(3:N-1),ell(4:N));
E(1)=CalcE(ell(N),ell(1),ell(2),ell(3));
E(N-1)=CalcE(ell(N-2),ell(N-1),ell(N),ell(1));
E(N)=CalcE(ell(N-1),ell(N),ell(1),ell(2));

C=CalcC(A,B,D,E);

xres(3:N-2)=A(3:N-2).*x(1:N-4)+B(3:N-2).*x(2:N-3)+C(3:N-2).*x(3:N-2)+D(3:N-2).*x(4:N-1)+E(3:N-2).*x(5:N);
xres(1)=A(1)*x(N-1)+B(1)*x(N)+C(1)*x(1)+D(1)*x(2)+E(1)*x(3);
xres(2)=A(2)*x(N)+B(2)*x(1)+C(2)*x(2)+D(2)*x(3)+E(2)*x(4);
xres(N-1)=A(N-1)*x(N-3)+B(N-1)*x(N-2)+C(N-1)*x(N-1)+D(N-1)*x(N)+E(N-1)*x(1);
xres(N)=A(N)*x(N-2)+B(N)*x(N-1)+C(N)*x(N)+D(N)*x(1)+E(N)*x(2);

yres(3:N-2)=A(3:N-2).*y(1:N-4)+B(3:N-2).*y(2:N-3)+C(3:N-2).*y(3:N-2)+D(3:N-2).*y(4:N-1)+E(3:N-2).*y(5:N);
yres(1)=A(1)*y(N-1)+B(1)*y(N)+C(1)*y(1)+D(1)*y(2)+E(1)*y(3);
yres(2)=A(2)*y(N)+B(2)*y(1)+C(2)*y(2)+D(2)*y(3)+E(2)*y(4);
yres(N-1)=A(N-1)*y(N-3)+B(N-1)*y(N-2)+C(N-1)*y(N-1)+D(N-1)*y(N)+E(N-1)*y(1);
yres(N)=A(N)*y(N-2)+B(N)*y(N-1)+C(N)*y(N)+D(N)*y(1)+E(N)*y(2);

zres(3:N-2)=A(3:N-2).*z(1:N-4)+B(3:N-2).*z(2:N-3)+C(3:N-2).*z(3:N-2)+D(3:N-2).*z(4:N-1)+E(3:N-2).*z(5:N);
zres(1)=A(1)*z(N-1)+B(1)*z(N)+C(1)*z(1)+D(1)*z(2)+E(1)*z(3);
zres(2)=A(2)*z(N)+B(2)*z(1)+C(2)*z(2)+D(2)*z(3)+E(2)*z(4);
zres(N-1)=A(N-1)*z(N-3)+B(N-1)*z(N-2)+C(N-1)*z(N-1)+D(N-1)*z(N)+E(N-1)*z(1);
zres(N)=A(N)*z(N-2)+B(N)*z(N-1)+C(N)*z(N)+D(N)*z(1)+E(N)*z(2);


end

function res=CalcA(lm1,l,lp1,lp2)
res=(l.*lp1.*lp1+l.*lp1.*lp2)./(lm1.*(lm1+l).*(lm1+l+lp1).*(lm1+l+lp1+lp2));
end

function res=CalcB(lm1,l,lp1,lp2)
res=(-lm1.*lp1.*lp1-l.*lp1.*lp1-lm1.*lp1.*lp2-l.*lp1.*lp2)./(lm1.*l.*(l+lp1).*(l+lp1+lp2));
end

function res=CalcD(lm1,l,lp1,lp2)
res=(lm1.*l.*lp1+l.*l.*lp1+lm1.*l.*lp2+l.*l.*lp2)./(lp1.*lp2.*(l+lp1).*(lm1+l+lp1));
end

function res=CalcE(lm1,l,lp1,lp2)
res=(-lp1.*l.*l-lm1.*l.*lp1)./(lp2.*(lp1+lp2).*(l+lp1+lp2).*(lm1+l+lp1+lp2));
end

function res=CalcC(A,B,D,E)
res=-(A+B+D+E);
end