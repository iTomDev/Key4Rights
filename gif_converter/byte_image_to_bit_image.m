function imgbit = byte_image_to_bit_image(imgbyte)
    % encode
    
    [height, width] = size(imgbyte);
    
    imgbit = zeros(height,(width/8),1);
    %
    for j=1:height % cols
        rowi = 1;
        for i=1:8:width % rows
            imgbit(j,rowi) = bi2de(imgbyte(j,i:i+7));
            rowi = rowi + 1;
        end
    end
end


