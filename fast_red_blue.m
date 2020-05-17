clc;
clear all;
close all;
s=serial('COM3','Baudrate',9600);%opening serial port
fopen(s);

vid=ipcam('http://192.168.43.1:8080/video');

while(1)
    red=0;
    blue=0;
    
    data=snapshot(vid);
    [a, b, c]=size(data);
    y=a;
    x=b-40;
    % center region of the frame
        %the offset 40 is because the cell phone camera was placed a bit
        %rightward from the center of the robot.
        %The resolution used was 320x240...hence an offset of 40 pixels 
        %was enough in this case
    
    diff_imR = imsubtract(data(:,:,1), rgb2gray(data));%subtracting red component
    diff_imB = imsubtract(data(:,:,3), rgb2gray(data));%subtracting blue component
    
    diff_imR = medfilt2(diff_imR, [3 3]);
    diff_imB = medfilt2(diff_imB, [3 3]);
    
    diff_imR = im2bw(diff_imR,0.25);
    diff_imB = im2bw(diff_imB,0.25);
    
    diff_imR = bwareaopen(diff_imR,300);
    diff_imB = bwareaopen(diff_imB,300);
    
     bwR = bwlabel(diff_imR, 8);
    bwB = bwlabel(diff_imB, 8);
    
    statsR = regionprops(bwR,'Area', 'BoundingBox', 'Centroid');
    statsB = regionprops(bwB,'Area', 'BoundingBox', 'Centroid');
    
    figure(1)
    imshow(data)
    
    hold on
    
     for g = 1:length(statsR)
        
        bb = statsR(g).BoundingBox;
        centroidR = statsR(g).Centroid;
        red=1;
        rectangle('Position',bb,'EdgeColor','r','LineWidth',2)
        plot(centroidR(1),centroidR(2), '-m+')
        a=text(centroidR(1),centroidR(2), strcat('X: ', num2str(round(centroidR(1))), '    Y: ', num2str(round(centroidR(2)))));
        set(a, 'FontName', 'Arial', 'FontWeight', 'bold', 'FontSize', 12, 'Color', 'yellow');
     hold on;
     end
     
     for v = 1:length(statsB)
        
        dd = statsB(v).BoundingBox;
        centroidB = statsB(v).Centroid;
        blue=1;
        rectangle('Position',dd,'EdgeColor','b','LineWidth',2)
           plot(centroidB(1),centroidB(2), '-m+')
        b=text(centroidB(1),centroidB(2), strcat('X: ', num2str(round(centroidB(1))), '    Y: ', num2str(round(centroidB(2)))));
        set(b, 'FontName', 'Arial', 'FontWeight', 'bold', 'FontSize', 12, 'Color', 'yellow');
        
     hold on 
     end
     hold off
    
     
  
        if red==1
        
        cenRx=centroidR(1);
        cenRy=centroidR(2);
        X1=x/2+20; 
        Y1=y/2+20;
        X2=x/2-20;
        Y2=y/2-20;
        if (cenRx>X2 && cenRx<X1)
            fprintf(s,'F');
        elseif (cenRx>X1)                             % 1st Quadrant
            fprintf(s,'R');
        elseif (cenRx<X2)                         % 2nd Quadrant
            fprintf(s,'L');
        end
        
        
        elseif (red==0)&&(blue==0)
        fprintf(s,'N');
   
        
         elseif (red==0)&&(blue==1)
        
        cenBx=centroidB(1);
        cenBy=centroidB(2);
        X1=x/2+20; % center region of the frame
        Y1=y/2+20;
        X2=x/2-20;
        Y2=y/2-20;
        if (cenBx>X2 && cenBx<X1)
            fprintf(s,'f');
        elseif (cenBx>X1)                             % 1st Quadrant
            fprintf(s,'r');
        elseif (cenBx<X2)                         % 2nd Quadrant
            fprintf(s,'l');
        end
        
        
    end
          
   
   
   
 pause(0.15);   
end %while