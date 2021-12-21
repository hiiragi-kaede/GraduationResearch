cd .\VRP\cpp\
g++ -Wno-format-security -O3 -o main main.cpp src/construct.cpp src/util.cpp src/improve.cpp src/tools.cpp -pthread
main.exe < ../CVRP_Data/tsplib/eilA101.txt > ../../research_out/eilA101_out.txt
cd ../../