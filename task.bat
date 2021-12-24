@echo off
cd research_out
mkdir %~n1_out 2>NUL
cd ../VRP/cpp
g++ -Wno-format-security -O3 -o main main.cpp src/construct.cpp src/util.cpp src/improve.cpp src/tools.cpp -pthread
main.exe -n=ilsc -k=d< ../CVRP_Data/%1 > ../../research_out/%~n1_out/cross_bridge.txt 2>NUL
main.exe -n=ilsc -k=f< ../CVRP_Data/%1 > ../../research_out/%~n1_out/cross_four.txt 2>NUL
echo; >> ../../research_out/%~n1_out/cross_bridge.txt
echo; >> ../../research_out/%~n1_out/cross_four.txt
for /l %%n in (1,1,3) do (
    main.exe -n=ilsc -k=d -s=%%n< ../CVRP_Data/%1 >> ../../research_out/%~n1_out/cross_bridge.txt 2>NUL
    main.exe -n=ilsc -k=f -s=%%n< ../CVRP_Data/%1 >> ../../research_out/%~n1_out/cross_four.txt 2>NUL
    echo; >> ../../research_out/%~n1_out/cross_bridge.txt
    echo; >> ../../research_out/%~n1_out/cross_four.txt
)
cd ../../