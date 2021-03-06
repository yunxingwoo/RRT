function dxdt = tunnel_diode(t,x,p);
    C=2;    %pF
    L=5;    %uH
    R=1.5;  %kOhm
    u=1.2;  %V
    h = 17.76*x(1)-103.79*(x(1)^2)+229.62*(x(1)^3)-226.31*(x(1)^4)+83.72*(x(1)^5);
    dxdt=[(1/C)*(x(2)-(h+p(1)));
       (1/L)*(-x(1)-R*x(2)+u+p(2))]; 
end