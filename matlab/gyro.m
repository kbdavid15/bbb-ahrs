function [ output_args ] = gyro( filename )
%GYRO Summary of this function goes here
%   Detailed explanation goes here
Ts = 0.01;
D = dataset('File', filename, 'Delimiter', ',');
t = (1:size(D, 1)) * Ts;

figure
subplot(2,1,1)
plot(t, D.GyroX, t, D.GyroY, t, D.GyroZ) 
grid
legend('X', 'Y', 'Z')
title('Gyroscope Output');
ylabel('deg/sec');

subplot(2,1,2)
plot(t, D.IntGyroX, t, D.IntGyroY, t, D.IntGyroZ)
grid
title('Gyroscope Integrated');
ylabel('degrees');
xlabel('Time (seconds)');

end

