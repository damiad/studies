b=119;
a=10;

N=13;
c=5;
n=3;

h = (b-a)/(N-2*n);
u = a + ((0:N)-n)*h;

M = c*(N-2*n);
h_p = (b-a)/M;
x = a + (0:M)*h_p;

function y = f(x,a,b)
    y = abs(x - (a+b)/2);
end

% function y = f(x,a,b)
%     y = x^a+b;
% end

function y = f_vec(x, a, b)
    y = arrayfun(@f, x, a, b);
end

y = f_vec(x, a, b);

function y = Mansfield_de_Boor_Cox(x, i, n, u)
    if n == 0
        if u(i) <= x && x < u(i+1)
            y = 1;
        else
            y = 0;
        end
    else
        y = ((x-u(i))/(u(i+n)-u(i)) * Mansfield_de_Boor_Cox(x, i, n-1,u)) + ((u(i+n+1)-x)/(u(i+n+1)-u(i+1)) * Mansfield_de_Boor_Cox(x, i+1, n-1,u));    
    end
end


% Calculate the coefficients of the spline function
A = zeros(M+1, N-3);
for j = 1:M+1
    for i = 1:(N-3)
        A(j, i) = Mansfield_de_Boor_Cox(x(j), i, 3, u);
    end
end

d = pinv(A' *A) * A' * y';

function y = spline(x, d, u)
    y = 0;
    for i = 1:length(d)
        y = y + d(i) * Mansfield_de_Boor_Cox(x, i, 3, u);
    end
end

% Number of points for the plot
num_points = 2000;
x_plot = linspace(a, b, num_points);

% Evaluate the spline function for the plot points
y_spline = zeros(1, num_points);
for k = 1:num_points
    y_spline(k) = spline(x_plot(k), d, u);
end

% Plot the spline function curve
figure;
plot(x_plot, y_spline, 'b-', 'LineWidth', 2);
hold on;

% Highlight the data points on the curve
scatter(x, y, 10, 'r', 'filled');
title('Spline Function with Highlighted Points');
xlabel('x');
ylabel('y');
legend('Spline Curve', 'Data Points');
grid on;
hold off;
pause;

