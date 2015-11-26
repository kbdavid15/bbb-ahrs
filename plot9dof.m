% Input file template (csv):
% Accel X, Accel Y, Accel X, Gyro X, Gyro Y, Gyro Z, Comp X, Comp Y, Comp Z
function plot9dof(filename)
Ts = 0.005;
D = importdata(filename, ',');   % D.data, D.colheaders, D.textdata
s = size(D.data);
t = (1:length(D.data(:, 1))) * Ts;
accel_x = D.data(:, 1);
accel_y = D.data(:, 2);
accel_z = D.data(:, 3);
% mean(accel_x)
% mean(accel_y)
% mean(accel_z)
int_accel_x = D.data(:,10);
figure
subplot(3,2,1)
plot(t, accel_x, t, accel_y, t, accel_z)
legend('Accel X', 'Accel Y', 'Accel Z')
title('Acceleration Output');
ylabel('Accel - g');
xlabel('Time (seconds)');

if (s(2) >=6)    
    gyro_x = D.data(:, 4);
    gyro_y = D.data(:, 5);
    gyro_z = D.data(:, 6);
    trapgyroz = D.data(:,13);
%     Z = cumtrapz(gyro_z)*0.005;
%     mean(gyro_x)
%     mean(gyro_y)
%     mean(gyro_z)
    subplot(3,2,3)
    plot(t, gyro_x, t, gyro_y, t, gyro_z) 
    grid
    title('Gyroscope Output');
    ylabel('Degrees Per Second');
    xlabel('Time (seconds)');
    subplot(3,2,4)
    plot(t, trapgyroz, '-r')
    grid
    title('Integrated Gyro');
    ylabel('Degrees (Position)');
    xlabel('Time (seconds)');
    
    if (s(2) >=9)
        mag_x = D.data(:, 7);
        mag_y = D.data(:, 8);
        mag_z = D.data(:, 9);
        subplot(3,2,5)
        plot(t, mag_x, t, mag_y, t, mag_z)
        title('Magnetometer Output');
        ylabel('Gauss');
        xlabel('Time (seconds)');
    end
end



%plot(accel_x, t, accel_y, t, accel_z, t)