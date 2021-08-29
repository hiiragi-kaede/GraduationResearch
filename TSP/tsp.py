import sys
sys.path.append("../")
from module import tsp_module as mod
from pprint import pprint

#fname="rand100.txt"
fname="a280_use.txt"

###main function###
with open(fname) as f:
    if fname!="a280_use.txt":
        data=[list(map(float,i.split())) for i in f.readlines()]
    else:
        data=[list(map(float,i.split()[1:])) for i in f.readlines()]

    dis_mat=[[mod.dis(data[i],data[j]) for i in range(len(data))] for j in range(len(data))]
    size=len(data)

    print("解法タイプを入力してください")
    print("0:ランダムルート(入力データの添字順)")
    print("1:nearest neighbor法で解く")
    print("2:挿入法")
    print("3:焼きなまし法")
    
    print("type:",end="")
    state=int(input())
    order = mod.random_route(size)
    if state==0:
        pass
    elif state==1:
        order=mod.nn_method(size,dis_mat)
    elif state==2:
        print("挿入順タイプを入力してください")
        print("0:添字順")
        print("1:x座標昇順")
        print("2:y座標昇順")

        print("insert type:",end="")
        ins_state=int(input())
        if ins_state<0 or ins_state>2:
            ins_state=0
        
        order=mod.insertion_method(data,dis_mat,ins_state)
    elif state==3:
        order=mod.sa_method(data,size,dis_mat)
    else:
        print("指定された数値の範囲で入力してください")
        exit(1)
    
    mod.draw_graph(data,order,"before")
    print("before_cost:",mod.total_move_cost(order,dis_mat))

    order=mod.two_opt_method(dis_mat,order)

    mod.draw_graph(data,order,"after")
    print("after_cost:",mod.total_move_cost(order,dis_mat))

    #mod.draw_nx_graphs(data,order)