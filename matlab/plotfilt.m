% Input file template (csv):
% X, Y, Z, Filt X, Filt Y, Filt Z
function plotfilt(filename)
Ts = 0.01;
D = importdata(filename, ',');   % D.data, D.colheaders, D.textdata
x = D.data(:, 1);
y = D.data(:, 2);
z = D.data(:, 3);

x_filt = D.data(:, 4);
y_filt = D.data(:, 5);
z_filt = D.data(:, 6);

s = size(D.data);
t = (1:length(D.data(:, 1))) * Ts;
figure
subplot(3,1,1)
plot(t, x, t, x_filt)
legend('X', 'X Filt')
title('X Axis');
xlabel('Time (seconds)');

subplot(3,1,2)
plot(t, y, t, y_filt)
legend('Y', 'Y Filt')
title('Y Axis');
xlabel('Time (seconds)');

subplot(3,1,3)
plot(t, z, t, z_filt)
legend('Z', 'Z Filt')
title('Z Axis');
xlabel('Time (seconds)');

