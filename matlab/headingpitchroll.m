function headingpitchroll( filename )
% Plot heading, pitch, and roll from file
%   Detailed explanation goes here
Ts = 0.01;
D = dataset('File', filename, 'Delimiter', ',');
t = (1:size(D, 1)) * Ts;

figure
subplot(3,1,1)
plot(t, D.Heading) 
grid
title('Heading Output From Magnetometer');
ylabel('degrees');

subplot(3,1,2)
plot(t, D.Pitch)
grid
title('Pitch from Accelerometer');
ylabel('degrees');

subplot(3,1,3)
plot(t, D.Roll)
grid
title('Roll from Accelerometer');
ylabel('degrees');
xlabel('Time (seconds)');

end

