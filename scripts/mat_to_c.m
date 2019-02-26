mat = '3GPP_36213_Table7_1_7_2_1_1';
load(strcat(mat,'.mat'),'raw');
fd = fopen(strcat(mat,'.cpp'), 'wt');
fprintf(fd, 'const int %s[%i][%i] = {\n', mat, length(raw(:,1)), length(raw(1,:)));


for i = 1:length(raw(:,1))
    fprintf(fd, '{%i',raw(i,1));
    for j = 2: length(raw(1,:))
        fprintf(fd, ', %i', raw(i,j));
    end;
    fprintf(fd,'},\n');
end;
fclose(fd);
