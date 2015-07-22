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
    hold all
    plot(gyro_x)
    plot(gyro_y)
    plot(gyro_z)
    title('Gyroscope Output');
    ylabel('Degrees Per Second');
    if (s(2) >=9)
        mag_x = D.data(:, 7);
        mag_y = D.data(:, 8);
        mag_z = D.data(:, 9);
        subplot(3,1,3)
        hold all
        plot(mag_x)
        plot(mag_y)
        plot(mag_z)
        title('Magnetometer Output');
        ylabel('Gauss');
    end
end



%plot(accel_x, t, accel_y, t, accel_z, t)