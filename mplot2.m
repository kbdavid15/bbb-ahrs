% Input file template (csv):
% Accel X, Accel Y, Accel X, Gyro X, Gyro Y, Gyro Z, Comp X, Comp Y, Comp Z
function mplot2(filename, samplerate)
D = dataset('File', filename, 'Delimiter', ',');
t = (1:size(D, 1)) * samplerate;

figure
subplot(3,1,1)
plot(t, D.AccelX, t, D.AccelY, t, D.AccelZ)
grid
legend('X', 'Y', 'Z')
title('Acceleration Output');
ylabel('Accel - g');

subplot(3,1,2)
plot(t, D.GyroX, t, D.GyroY, t, D.GyroZ) 
grid
title('Gyroscope Output');
ylabel('Degrees Per Second');

subplot(3,1,3)
plot(t, D.MagX, t, D.MagY, t, D.MagZ)
grid
title('Magnetometer Output');
ylabel('Gauss');
xlabel('Time (seconds)');
