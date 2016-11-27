close all;clc;clear;

%% Data1
dirpath = './Data1/';
writepath = './txtData1/';
dat = dir( dirpath );
for j = 1 : length( dat )
    if( ~isequal( dat( j ).name, '.' ) &...
        ~isequal( dat( j ).name, '..') )
        datpath = [dirpath dat( j ).name];
        [y, Fs] = audioread(datpath);
        f = fopen([writepath dat( j ).name(1:8) '.txt'],'w');
        fprintf(f, '%f\n', y);
        fclose(f);
    end
end

%% Data2
datpath = './Data2/data.wav';
[y, Fs] = audioread(datpath);
f = fopen('./txtData2/data.txt','w');
fprintf(f, '%f\n', y);
fclose(f);








