clear all;

data = dlmread("Solution.txt");

[X,Y,U] = setup(data);


plot = surf(X,Y,U);
xlabel('\(x\)','Interpreter','latex','FontSize',20)
ylabel('\(y\)','Interpreter','latex','FontSize',20)
zlabel('{\(u_h(x,y)\)}','Interpreter','latex','FontSize',20)
title('DDMRes Solution','FontSize',20)
set(gcf,'Position',[25 25 900 900]);

xlim([0 1]);
ylim([0 1]);

fig = get(groot,'CurrentFigure');

while ~isempty(fig)
    load Solution.txt
    check = Solution(1:3,1);
    if check ~= data(1:3,1)
        data = dlmread("Solution.txt");
        [X,Y,U] = setup(data);
        set(plot,'Xdata',X,'YData',Y,'ZData',U);
        drawnow();
    end
    fig = get(groot,'CurrentFigure');
    pause(0.5);
end


function [X,Y,U] = setup(data)
u = data(:,1);
x = data(:,2);
y = data(:,3);

n = 50;

it1 = 1;
it2 = 1;
U = zeros(n);

systemsize = size(x);
for i = linspace(x(1),x(systemsize(1)),n)
    x1(it1) = i;
    for j = linspace(y(1),y(systemsize(1)),n)
        y1(it2) = j;
        U(it2,it1) = griddata(x,y,u,i,j,'linear');
        
        it2 = it2 + 1;
    end
    it2 = 1;
    it1 = it1 + 1;
end

[X,Y] = meshgrid(x1,y1);


end