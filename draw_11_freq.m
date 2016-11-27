close all; clc; clear

figure;
data = load('./txtData2/data.txt');

sampling_rate = 8000;
len = length(data);
x_axis = (0:(len-1))/8000;

seconds_len_data2 = len/8000.0;

start_index = round(4.581/seconds_len_data2*len)
end_index = round(4.794/seconds_len_data2*len)

t_data = data(start_index:end_index);
fft_data = fft(t_data);
len = length(fft_data);
x_axis = (1:len)*8000/len;
plot(x_axis, abs(fft_data))
xlabel('Frequency/Hz')
ylabel('Amp')
xlim([0 8000])
set(gca,'fontsize',16,'xtick',0:500:8000,'yscale','linear')