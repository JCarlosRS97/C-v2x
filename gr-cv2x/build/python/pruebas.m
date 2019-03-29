Ts = 1/15000/2048;
c=read_complex_binary('capture.dat');
t = (0:(length(c)-1)).*Ts;
plot(t, c)