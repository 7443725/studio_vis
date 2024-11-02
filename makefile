all: temp_api_v2

temp_api_v2: temp_api_main.o temp_api_sourse.o 
	gcc -o temp_api_v2 temp_api_main.o temp_api_sourse.o 
	del *.o
	temp_api_v2 
	temp_api_v2 -h 
	temp_api_v2 -f temperature_small.csv 
	temp_api_v2 -f temperature_small.csv -m 6
	temp_api_v2 -f temperature_small.txt -m 6
	temp_api_v2 -f temperature_small.csv -o 
	temp_api_v2 -f temperature_big.csv 

temp_api_main.o: temp_api_main.c temp_api_h.h
	gcc -c -o temp_api_main.o temp_api_main.c 
	
temp_api_sourse.o: temp_api_sourse.c 
	gcc -c -o temp_api_sourse.o temp_api_sourse.c 

clean:
	del *.o
	del temp_api_v2.exe
