% Input file template (csv):
% Accel X, Accel Y, Accel X, Gyro X, Gyro Y, Gyro Z, Comp X, Comp Y, Comp Z
function mplot(filename)
Ts = 0.005;
D = dataset('File', filename, 'Delimiter', ',');
s = size(D, 2);
t = (1:size(D, 1)) * Ts;
figure
subplot(3,2,1)
plot(t, D.AccelX, t, D.AccelY, t, D.AccelZ)
legend('X', 'Y', 'Z')
title('Acceleration Output');
ylabel('Accel - g');
xlabel('Time (seconds)');

subplot(3,2,2)
plot(t, D.Pitch, t, D.Roll)
legend('Pitch Angle', 'Roll Angle')
title('Acceleration Output');
ylabel('Degrees');
xlabel('Time (seconds)');

subplot(3,2,3)
plot(t, D.GyroX, t, D.GyroY, t, D.GyroZ) 
grid
title('Gyroscope Output');
ylabel('Degrees Per Second');
xlabel('Time (seconds)');
subplot(3,2,4)
plot(t, D.IntGyroZ, '-r', t, D.Yaw)
grid
legend('Gyro Z', 'Yaw (Mag)')
title('Integrated Gyro');
ylabel('Degrees (Position)');
xlabel('Time (seconds)');

subplot(3,2,5)
plot(t, D.MagX, t, D.MagY, t, D.MagZ)
grid
title('Magnetometer Output');
ylabel('Gauss');
xlabel('Time (seconds)');

subplot(3,2,6)
plot(t, D.Heading)
grid
title('Magnetometer Heading');
ylabel('Degrees');
xlabel('Time (seconds)');

