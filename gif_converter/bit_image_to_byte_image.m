function imgbyte = bit_image_to_byte_image(imgbit)
    % decode
    
    [height, width] = size(imgbit);
    width = width *8;
    
    imgbyte = uint8(zeros(height,width));
    for j=1:height % cols
        rowi = 7;
        for i=1:1:(width/8) % rows
            byte = zeros(1,8);
            byte(1, (8:-1: (8-size(de2bi(imgbit(j,i)),2)+1) ) ) = de2bi(imgbit(j,i));
            imgbyte(j, ((i*8)-7) :1: (i*8) ) = flip(byte);
            rowi = rowi - 1;
        end
    end
    %imshow(imgbyte(:,:,1))
    
end