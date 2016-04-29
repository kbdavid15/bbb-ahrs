function pitchroll( filename )
% Plot heading, pitch, and roll from file
%   Detailed explanation goes here
Ts = 0.01;
D = dataset('File', filename, 'Delimiter', ',');
t = (1:size(D, 1)) * Ts;

figure
subplot(2,1,1)
plot(t, D.Pitch)
grid
title('Pitch from Accelerometer');
ylabel('degrees');

subplot(2,1,2)
plot(t, D.Roll)
grid
title('Roll from Accelerometer');
ylabel('degrees');
xlabel('Time (seconds)');

end

