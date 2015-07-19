% Input file template (csv):
% Accel X, Accel Y, Accel X, Gyro X, Gyro Y, Gyro Z, Comp X, Comp Y, Comp Z
function plot9dof(filename)
D = importdata(filename, ',');   % D.data, D.colheaders, D.textdata
s = size(D.data);
accel_x = D.data(:, 1);
accel_y = D.data(:, 2);
accel_z = D.data(:, 3);

figure
subplot(3,1,1)
hold all
plot(accel_x)
plot(accel_y)
plot(accel_z)
legend('Accel X', 'Accel Y', 'Accel Z')
title('Acceleration Output');
ylabel('Accel - g');

if (s(2) >=6)
    gyro_x = D.data(:, 4);
    gyro_y = D.data(:, 5);
    gyro_z = D.data(:, 6);
    subplot(3,1,2)
    plot(gyro_x)
    plot(gyro_y)
    plot(gyro_z)
    title('Gyroscope Output');
    ylabel('Degrees Per Second');
end



%plot(accel_x, t, accel_y, t, accel_z, t)