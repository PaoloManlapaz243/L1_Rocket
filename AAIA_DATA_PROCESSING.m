FILEPREF = "/data";
FILESUF = ".csv";

%Find the latest version of the data file
i = 1;
FILENAME = strcat(FILEPREF, num2str(i), FILESUF);
while exist(FILENAME)
    i = i + 1;
    FILENAME = strcat(FILEPREF, num2str(i), FILESUF);
end

FILENAME = strcat(FILEPREF, num2str(i-1), FILESUF);

close all;

global gtime gtemp gpressure galtitude;

T = readtable(FILENAME);
gtime =      T{:,1};
gtemp =      T{:,2};
gpressure =  T{:,3};
galtitude =  T{:,4};

regplot();
movmeanplot(4);
sgolayplot(1,3);


% 
% sgolayplot(2, 15);
% sgolayplot(2, 127);
% 
% movmeanplot(64);
movmeanplot(128);
% 
% sgolayplot(2, 63);
% sgolayplot(2, 127);
% 
% sgolayplot(5, 63);
sgolayplot(5, 127);

% 
% figure;
% x = 2:9;
% y = 1:8;
% [X, Y] = meshgrid(x, y);
% Z = squeeze(sgolayOptim(1,2:9,1:8));
% surf(X,Y,Z);

SNRcheck();

% Fs = 1000/50;
% L=length(gpressure);
% fvec = Fs*(0:(L/2))/L;
% dft = (abs(fft(gpressure - mean(gpressure)))/L);
% 
% plot(fvec, dft(1:L/2 + 1));


function SNR = snrhelper(filtered, noisy)
    SNR = snr(filtered, noisy - filtered);
end

function helperplot(temp, pressure, altitude)
    hold on;
    global gtime gtemp gpressure galtitude;

    %Temp vs. Time
    subplot(2,2,1);
    a= plot(gtime, temp);
    title('Rocket Temp');
    ylabel('Temperature (F)');
    xlabel('Time (ms)');
    legend(sprintf([ ...
        'Temp Max = %.1fF\n' ...
        'Temp Min = %.1fF\n' ...
        'SNR = %.1fdB' ...
        ], max(temp), min(temp), snrhelper(temp, gtemp)), 'Location', 'southeast');

    %Pressure vs. Time
    subplot(2,2,2);
    plot(gtime, pressure);
    title('Rocket Pressure');
    ylabel('Pressure (hPa)');
    xlabel('Time (ms)');
    legend(sprintf([ ...
        'Pressure Max = %4.3e hPa\n' ...
        'Pressure Min = %4.3e hPa\n' ...
        'SNR = %.1fdB' ...
        ], max(pressure), min(pressure), snrhelper(pressure, gpressure)), 'Location', 'southeast');

    %Altitude vs. Time
    subplot(2,2,3);
    plot(gtime, altitude);
    title('Rocket Altitude');
    ylabel('Altitude (m)');
    xlabel('Time (ms)');
    legend(sprintf([ ...
        'Altitude Max = %.1fm\n' ...
        'Altitude Min = %.1fm\n' ...
        'SNR = %.1fdB'...
        ], max(altitude), min(altitude), snrhelper(altitude, galtitude)), 'Location', 'southeast');

    hold off;
end


function sgolayplot(order, window)
    figure;
    sgtitle(['Savitzky-Golay, Order: ', num2str(order), ', Window: ', num2str(window)]);

    global gtemp gpressure galtitude;

    temp =      sgolayfilt(gtemp, order, window);
    pressure =  sgolayfilt(gpressure, order, window);
    altitude =  sgolayfilt(galtitude, order, window);

    helperplot(temp, pressure, altitude);
end

function movmeanplot(window)
    figure;
    sgtitle(['Moving Average, Window: ',num2str(window)]);

    global gtemp gpressure galtitude;

    temp =      movmean(gtemp, window);
    pressure =  movmean(gpressure, window);
    altitude =  movmean(galtitude, window);

    helperplot(temp, pressure, altitude);
end

function regplot()
    figure;
    sgtitle('Raw Data Plot');

    global gtemp gpressure galtitude;

    helperplot(gtemp, gpressure, galtitude);
end

function SNRcheck()
    global gtemp gpressure galtitude;

    sgolayOptim = zeros(3,9,8);
    for i = 2:9
        for j = 1:i-1
            sgolayOptim(1,i,j) = snrhelper(sgolayfilt(gtemp, j, (2^i)-1), gtemp);
            sgolayOptim(2,i,j) = snrhelper(sgolayfilt(gpressure, j, (2^i)-1), gpressure);
            sgolayOptim(3,i,j) = snrhelper(sgolayfilt(galtitude, j, (2^i)-1), galtitude);
        end
    end

    movMeanOptim = zeros(3,9);
    for i = 1:9
        movMeanOptim(1,i) = snrhelper(movmean(gtemp, (2^i)-1), gtemp);
        movMeanOptim(2,i) = snrhelper(movmean(gpressure, (2^i)-1), gpressure);
        movMeanOptim(3,i) = snrhelper(movmean(galtitude, (2^i)-1), galtitude);
    end

    figure;
    x = 2:9;
    y = 1:8;

    Z = nan(length(y), length(x));
    [X, Y] = meshgrid(x, y);
    
    %Temp SNR
    for col = 1:length(x)
        i_val = x(col);
        max_j = i_val - 1;
        Z(1:max_j, col) = sgolayOptim(1, i_val, 1:max_j); 
    end

    subplot(2,2,1);
    surf(X, Y, Z);
    xlabel("Window Size (2^x-1)");
    ylabel("Polynomial Order");
    zlabel("SNR (dB)");
    title("Temp SNR for varying S-golay Filter");

    %pressure SNR
    for col = 1:length(x)
        i_val = x(col);
        max_j = i_val - 1;
        Z(1:max_j, col) = sgolayOptim(2, i_val, 1:max_j); 
    end

    subplot(2,2,2);
    surf(X, Y, Z);
    xlabel("Window Size (2^x-1)");
    ylabel("Polynomial Order");
    zlabel("SNR (dB)");
    title("Pressure SNR for varying S-golay Filter");

    %altitude snr
    for col = 1:length(x)
        i_val = x(col);
        max_j = i_val - 1;
        Z(1:max_j, col) = sgolayOptim(3, i_val, 1:max_j); 
    end

    subplot(2,2,3);
    surf(X, Y, Z);
    xlabel("Window Size (2^x-1)");
    ylabel("Polynomial Order");
    zlabel("SNR (dB)");
    title("Altitude SNR for varying S-golay Filter");

    figure;

    %temp MA snr
    subplot(2,2,1);
    plot(1:9, movMeanOptim(1,:));
    xlabel("Window Size (2^x)");
    ylabel("SNR (dB)");
    title("Temp SNR for MA Filters");

    %pressure MA snr
    subplot(2,2,2);
    plot(1:9, movMeanOptim(2,:));
    xlabel("Window Size (2^x)");
    ylabel("SNR (dB)");
    title("Pressure SNR for MA Filters");

    %altitude MA snr
    subplot(2,2,3);
    plot(1:9, movMeanOptim(3,:));
    xlabel("Window Size (2^x)");
    ylabel("SNR (dB)");
    title("Altitude SNR for MA Filters");
end