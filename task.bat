@echo off
cd research_out
mkdir %~n1_out 2>NUL
cd ../VRP/cpp
g++ -Wno-format-security -O3 -o main main.cpp src/construct.cpp src/util.cpp src/improve.cpp src/tools.cpp -pthread
main.exe -n=ilsc -k=f -t=0 -l=0 -h=0 < ../CVRP_Data/%1 > ../../research_out/%~n1_out/cross_normal.txt 2>NUL
main.exe -n=ilsc -k=f -t=0 -l=0 -h=1 < ../CVRP_Data/%1 > ../../research_out/%~n1_out/cross_h.txt 2>NUL
main.exe -n=ilsc -k=f -t=0 -l=1 -h=0 < ../CVRP_Data/%1 > ../../research_out/%~n1_out/cross_l.txt 2>NUL
main.exe -n=ilsc -k=f -t=0 -l=1 -h=1 < ../CVRP_Data/%1 > ../../research_out/%~n1_out/cross_lh.txt 2>NUL
main.exe -n=ilsc -k=f -t=1 -l=0 -h=0 < ../CVRP_Data/%1 > ../../research_out/%~n1_out/cross_t.txt 2>NUL
main.exe -n=ilsc -k=f -t=1 -l=0 -h=1 < ../CVRP_Data/%1 > ../../research_out/%~n1_out/cross_th.txt 2>NUL
main.exe -n=ilsc -k=f -t=1 -l=1 -h=0 < ../CVRP_Data/%1 > ../../research_out/%~n1_out/cross_tl.txt 2>NUL
main.exe -n=ilsc -k=f -t=1 -l=1 -h=1 < ../CVRP_Data/%1 > ../../research_out/%~n1_out/cross_tlh.txt 2>NUL
echo; >> ../../research_out/%~n1_out/cross_normal.txt
echo; >> ../../research_out/%~n1_out/cross_h.txt
echo; >> ../../research_out/%~n1_out/cross_l.txt
echo; >> ../../research_out/%~n1_out/cross_lh.txt
echo; >> ../../research_out/%~n1_out/cross_t.txt
echo; >> ../../research_out/%~n1_out/cross_th.txt
echo; >> ../../research_out/%~n1_out/cross_tl.txt
echo; >> ../../research_out/%~n1_out/cross_tlh.txt

for /l %%n in (1,1,3) do (
    main.exe -n=ilsc -k=f -t=0 -l=0 -h=0 -s=%%n< ../CVRP_Data/%1 >> ../../research_out/%~n1_out/cross_normal.txt 2>NUL
    main.exe -n=ilsc -k=f -t=0 -l=0 -h=1 -s=%%n< ../CVRP_Data/%1 >> ../../research_out/%~n1_out/cross_h.txt 2>NUL
    main.exe -n=ilsc -k=f -t=0 -l=1 -h=0 -s=%%n< ../CVRP_Data/%1 >> ../../research_out/%~n1_out/cross_l.txt 2>NUL
    main.exe -n=ilsc -k=f -t=0 -l=1 -h=1 -s=%%n< ../CVRP_Data/%1 >> ../../research_out/%~n1_out/cross_lh.txt 2>NUL
    main.exe -n=ilsc -k=f -t=1 -l=0 -h=0 -s=%%n< ../CVRP_Data/%1 >> ../../research_out/%~n1_out/cross_t.txt 2>NUL
    main.exe -n=ilsc -k=f -t=1 -l=0 -h=1 -s=%%n< ../CVRP_Data/%1 >> ../../research_out/%~n1_out/cross_th.txt 2>NUL
    main.exe -n=ilsc -k=f -t=1 -l=1 -h=0 -s=%%n< ../CVRP_Data/%1 >> ../../research_out/%~n1_out/cross_tl.txt 2>NUL
    main.exe -n=ilsc -k=f -t=1 -l=1 -h=1 -s=%%n< ../CVRP_Data/%1 >> ../../research_out/%~n1_out/cross_tlh.txt 2>NUL
    echo; >> ../../research_out/%~n1_out/cross_normal.txt
    echo; >> ../../research_out/%~n1_out/cross_h.txt
    echo; >> ../../research_out/%~n1_out/cross_l.txt
    echo; >> ../../research_out/%~n1_out/cross_lh.txt
    echo; >> ../../research_out/%~n1_out/cross_t.txt
    echo; >> ../../research_out/%~n1_out/cross_th.txt
    echo; >> ../../research_out/%~n1_out/cross_tl.txt
    echo; >> ../../research_out/%~n1_out/cross_tlh.txt
)
cd ../../