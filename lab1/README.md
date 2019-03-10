#how to run  
Generate 2 matrix files, 800x900 - first one. 900-820 - second one.

     lab1.exe matrix1.txt matrix2.txt 800 900 820
     
Run calculations
     
     lab1.exe matrix1.txt matrix2.txt 0 1

Where 0 is a runmode and 1 is a chunk size.
## runmodes
- 0 - 1 thread.  
- 1 - static schedule.  
- 2 - dynamic schedule.  
- 3 - guided schedule.  
## chunksize
Define chunksize for the omp parallel schedulers. If NULL then chunk size will be 

    rows * columns (of the result matrix) / threads 
    
