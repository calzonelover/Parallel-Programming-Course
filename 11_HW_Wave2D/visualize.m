clear;clc
nx = 201;
ny = 201;
fid = fopen('wave2d_raw_map.dat','r');
u = fread(fid,nx*ny,'float32');
fclose(fid);
u = reshape(u,nx,ny);
imagesc(u)
