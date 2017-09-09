%durata de rulare a scriptului este de aproximativ 4 secunde

pkg load image
    
%demo pentru inverse_mapping
    
    %pentru scalare
    img = inverse_mapping('flapping_bird.png', 0.4 * eye(2));
    imwrite(mat2gray(img), 'inverse_mapping_bird_0.4.png');
    
    img = inverse_mapping('flapping_duck.png', 0.4 * eye(2));
    imwrite(mat2gray(img), 'inverse_mapping_duck_0.4.png');
    
    img = inverse_mapping('flapping_bird.png', 2 * eye(2));
    imwrite(mat2gray(img), 'inverse_mapping_bird_2.png');
    
    img =inverse_mapping('flapping_duck.png', 2 * eye(2));
    imwrite(mat2gray(img), 'inverse_mapping_duck_2.png');
    
%pentru rotatie

    x = pi/4;
    T = [cos(x) -sin(x); sin(x) cos(x)];
    img = inverse_mapping('flapping_bird.png', T);
    imwrite(mat2gray(img), 'inverse_mapping_bird_45.png');
    
    x = pi/4;
    T = [cos(x) -sin(x); sin(x) cos(x)];
    img = inverse_mapping('flapping_duck.png', T);
    imwrite(mat2gray(img), 'inverse_mapping_duck_45.png');
    
    x = 3 * pi/4;
    T = [cos(x) -sin(x); sin(x) cos(x)];
    img = inverse_mapping('flapping_bird.png', T);
    imwrite(mat2gray(img), 'inverse_mapping_bird_135.png');
    
    x = 3 * pi/4;
    T = [cos(x) -sin(x); sin(x) cos(x)];
    img = inverse_mapping('flapping_duck.png', T);
    imwrite(mat2gray(img), 'inverse_mapping_duck_135.png');
    
    x = 5 * pi/4;
    T = [cos(x) -sin(x); sin(x) cos(x)];
    img = inverse_mapping('flapping_bird.png', T);
    imwrite(mat2gray(img), 'inverse_mapping_bird_225.png');
    
    x = 5 * pi/4;
    T = [cos(x) -sin(x); sin(x) cos(x)];
    img = inverse_mapping('flapping_duck.png', T);
    imwrite(mat2gray(img), 'inverse_mapping_duck_225.png');
    
    x = 7 * pi/4;
    T = [cos(x) -sin(x); sin(x) cos(x)];
    img = inverse_mapping('flapping_bird.png', T);
    imwrite(mat2gray(img), 'inverse_mapping_bird_315.png');
    
    x = 7 * pi/4;
    T = [cos(x) -sin(x); sin(x) cos(x)];
    img = inverse_mapping('flapping_duck.png', T);
    imwrite(mat2gray(img), 'inverse_mapping_duck_315.png');
    
    x = pi;
    T = [cos(x) -sin(x); sin(x) cos(x)];
    img = inverse_mapping('flapping_bird.png', T);
    imwrite(mat2gray(img), 'inverse_mapping_bird_180.png');
    
    x = pi;
    T = [cos(x) -sin(x); sin(x) cos(x)];
    img = inverse_mapping('flapping_duck.png', T);
    imwrite(mat2gray(img), 'inverse_mapping_duck_180.png');
    
%demo pentru transform_image

    img = transform_image('flapping_duck.png', 12.3);
    imwrite(mat2gray(img), 'blurred_duck.png');
    
    img = transform_image('flapping_bird.png', 12.3);
    imwrite(mat2gray(img), 'blurred_bird.png');
    
    
    
    
