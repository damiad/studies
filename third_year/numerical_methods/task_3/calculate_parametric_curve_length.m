N = 14; % Starting number of intervals between a and b
n = 3; % Degree of the B-spline curve
a = n; % Begining of derivative interval
b = N - n; % End of derivative interval
tolerance = 1e-6; % Tolerance for comparing quadrature results

u = (0:N);
d = [   
    -0.7, 0;
    -0.333333333333, 0;
    -0.9, 0;
    -0.166666666667, 0;
    -0.083333333333, 0;
    0.0, 0;
    0.083333333333, 0;
    0.166666666667, 0;
    0.9, 0;
    0.333333333333, 0;
    0.416666666667, 0
];

d = d';

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

function ds_dt = parametric_derivative(x, n, u, d)
    ds_dt = [0; 0];
    N = length(u) - 1;

    for i = 1:(N-n-1)
        ds_dt = ds_dt + ((n / (u(i+n) - u(i))) * (d(:, i+1) - d(:, i)) * Mansfield_de_Boor_Cox(x, i + 1, n - 1, u));
    end
    ds_dt = sqrt(sum(ds_dt.^2));
end

function result = calculate_quadrature(a, b, h, n, u, d)
    result = 0;
    for i = a:h:(b-h)
        result = result + quad(@(t) (parametric_derivative(t, n, u, d)), i, i+h);
    end
end

result_prev = 0;
for i = 1:1000
    h = 1/i;
    result = calculate_quadrature(a, b, h, n, u, d);
    disp(['h: ', num2str(h), '  Curve length: ', num2str(result), '  Difference: ', num2str(abs(result - result_prev))]);
    if abs(result - result_prev) < tolerance
        break;
    else
        result_prev = result;
    end
end

disp('-------- FINAL RESULT --------');
disp(['h: ', num2str(h), '  Curve length: ', num2str(result)]);
