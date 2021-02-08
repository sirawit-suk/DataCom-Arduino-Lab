close all;
clear all;

time = 20
t = [0:0.001:time] % make very small time of x to max_time
alpha = 3;   % this is volt
f = 1 % Hz

y = alpha*sin(2*pi*f*t)

%formula : A*sin(wt)
%formula : A*sin(2*pi*t/T)
%formula : A*sin(2*pi*f*t)

% sin(0) == 0
% sin(pi/2) == 1
% sin(pi) == 0
% sin(pi*3/2) == -1
% sin(2*pi) == 0

figure(1)
plot(t,y)
