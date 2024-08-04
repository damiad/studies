% Sparse function in octave
N = 100;
size_m = (N-1)^2;

% Empty list for non zero elements
x = [];
y = [];
v = [];

% Generating Nonzero elements
for i = 1:size_m-N+1
    x = [x; i];
    x = [x; i + N-1];
    y = [y; i + N-1];
    y = [y; i];
    v = [v; -1];
    v = [v; -1];
end

% First SubSquare
sx=[];
sy=[];
sv=[];

for i=1:N-1
    sx = [sx; i];
    sy = [sy; i];
    sv = [sv; 4];
end

for i=1:N-2
    sx = [sx; i];
    sx = [sx; i+1];
    sy = [sy; i+1];
    sy = [sy; i];
    sv = [sv; -1];
    sv = [sv; -1];
end

% Other SubSquares
elements_number = size(sx,1);
for i=0:N-2
    for j=1:elements_number
        x = [x; sx(j)+i*(N-1)];
        y = [y; sy(j)+i*(N-1)];
        v = [v; sv(j)];
    end
end


sparse_matrix = sparse(x,y,v, size_m, size_m);

% Example function f(x, y)
function result = f(x, y)
    result = x; 
end


function [x, residuals] = conjgrad(A, b, x0)
    r = b - A * x0;
    w = -r;
    z = A * w;
    a = (r' * w) / (w' * z);
    x = x0 + a * w;
    
    residuals = [norm(r)];

    for i = 1:size(A, 1)
        r = r - a * z;
        if (norm(r) < 1e-10)
            break;
        end
        B = (r' * z) / (w' * z);
        w = -r + B * w;
        z = A * w;
        a = (r' * w) / (w' * z);
        x = x + a * w;
        residuals = [residuals, norm(b - A * x)];
    end
end

x0 = zeros(size_m, 1);
% Creating b vector using function f(x, y)
b = zeros(size_m, 1);
for i = 1:size_m
    x_val = mod(i - 1, N - 1) + 1; % Coords x
    y_val = floor((i - 1) / (N - 1)) + 1; % Coords y
    b(i) = f(x_val, y_val);
end
disp(b);

[x, residuals] = conjgrad(sparse_matrix, b, x0);
disp(x);    

% Vizualization of vector residuals
figure;
iteration = 1:length(residuals);
plot(iteration, residuals, '-o');
xlabel('Iteration');
ylabel('Norm of residual vector');
title('Norm of residual vector in each iteration of conjugate gradient method');
pause;
