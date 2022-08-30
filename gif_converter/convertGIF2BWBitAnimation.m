
% Thomas Pile
% August 2022
% Convert animated GIF into a sequence of binary b&w images
% currently only supported in Matlab

clc
clear

%%
% some examples

% import gif                           % dims after resizing
filepath = 'gifs/triangle_sign';
filepath = 'gifs/laptop';               % 53 55 11
filepath = 'gifs/disk';                 % 60 60 8
filepath = 'gifs/smash_keyboard';       % 61 77 9
filepath = 'gifs/microcontroller';
filepath = 'gifs/driving';
filepath = 'gifs/juggle';
filepath = 'gifs/stairs';
filepath = 'gifs/eye_star';
filepath = 'gifs/crowd';
filepath = 'gifs/angry_user';
filepath = 'gifs/jet';
filepath = 'gifs/anime';
filepath = 'gifs/motion_green';         % 60 109 120
filepath = 'gifs/motion_red';
filepath = 'gifs/globe';                % 62 109 100
filepath = 'gifs/earth_seasons';
filepath = 'gifs/motion_digital';
filepath = 'gifs/';

%%
% configuration

centre_image==1

% scale image to fit display
scalefactor = 0.64;
scalefactor = 0.17;

% invert b&w or not
invert = 0;
padtype = 0;

% binarisation mode
binmode = 'adaptive';
%binmode = 'otsu';

% set size of display
dwidth = 128;
dheight = 64;

% limit length to 30 frames
shorts = 1;

%%
% load image and convert to b&w frames

[imgs,map]=imread([filepath '.gif'],'frames','all');

% make array for b&w image version and define the x,y size and num of
% frames
imgscaled = imresize(imgs(:,:,1,1),scalefactor);
y = size(imgscaled,1);
x = size(imgscaled,2);
fmax = size(imgs,4);
bwimgs = zeros(y,x,fmax);

if y > dheight
    disp( 'Error: Resize the image') 
    disp ([num2str(y) ' is too high'])
end
if x > dwidth
   disp 'Error: Resize the image'
   disp ([num2str(x) ' is too wide'])
end

% loop through frames and convert to b&w
for i=1:fmax
    % convert to b&W
    if strcmp(binmode, 'adaptive')
        bwimgs(:,:,i) = imbinarize(imresize(imgs(:,:,1,i),scalefactor), 'adaptive'); 
    else
        bwimgs(:,:,i) = imbinarize(imresize(imgs(:,:,1,i),scalefactor)); 
    end
    %bwimgs(:,:,i) = imbinarize(imresize(imgs(:,:,1,i),scalefactor), 0.045); 
    %imshow(bwimgs(:,:,i))
    %pause(0.1);
end

%%
% simple b&w coding scheme. Store only the pixels to be drawn in the image
% by location. Matches that used by the display output itself i.e 
% (width*height)/8
% then use a single bit to store the pixel value

% bit image to display frame size
if padtype==1
    bwimgspad = uint8(ones(dheight, dwidth, fmax));
else
    bwimgspad = uint8(zeros(dheight, dwidth, fmax));
end

if (centre_image==1)
    dy = round( ((dheight-y)/2), 0);
    dx = round( ((dwidth-x)/2), 0);
    bwimgspad((1+dy):(y+dy), (1+dx):(x+dx), :) = bwimgs(:,:,:);
else
    bwimgspad(1:y, 1:x, :) = bwimgs(:,:,:);
end


% invert b&w

if (invert== 1)
    bwimgspad = ~bwimgspad;
end

% b&w
gifbit = uint8((zeros(dheight, (dwidth/8), fmax)));

% verify
%imshow(bwimgspad (:,:,1))

% convert gif animation to bit format
% loop through frames
for k=1:fmax
    imgbyte = bwimgspad(:,:,k);
    imgbit = byte_image_to_bit_image(imgbyte);
    gifbit(:,:,k) = imgbit;
end

% convert back to play and verify
gifbitverify = uint8(ones(dheight, dwidth, fmax));
for k=1:fmax
    imgbit = gifbit(:,:,k);
    imgbyte2 = bit_image_to_byte_image(imgbit);
    gifbitverify(:,:,k) = imgbyte2;
end

% display final result
for k=1:fmax
    imshow( imresize( uint8(gifbitverify(:,:,k)*255), 5) );
    pause(0.2);
end

%%
% create header file format of the GIF file for importation into code

% unroll the animation into a sequence of still images
% height is the height of the display, animation frames are sequentially
% going right 

% force fmax limit to constrain file size

[gheight, gwidth, fmax] = size(gifbit);

if shorts==1
    fmax = 30;
end

img = uint8(zeros(gheight, gwidth*fmax));
for k=1:fmax
    img(:,(gwidth*k)-(gwidth-1) : (gwidth*k) ) = gifbit(:,:,k);
end

% formatted as a header file.
% hex = packed hex pairs e.g 0xFF 0xAB  0x3C => FF3CAB
% this is because the 8 bytes of b&w are compacted into a single byte as
% bits
fid = fopen([filepath 's.h'], 'wt');
for j=1:gheight
    for i=1:gwidth*fmax
        fprintf( fid, '%02x', img(j,i));
    end
    fprintf( fid, '\n');
end
fclose(fid);
disp('saved')

% c code to extract this ( - todo!




%%
% create raw data form for programming onto flash chips


%%
