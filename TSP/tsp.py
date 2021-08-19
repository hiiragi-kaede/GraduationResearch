import module.tsp_module as mod

fname="rand100.txt"
#fname="a280_use.txt"

###main function###
with open(fname) as f:
    if fname!="a280_use.txt":
        data=[list(map(float,i.split())) for i in f.readlines()]
    else:
        data=[list(map(float,i.split()[1:])) for i in f.readlines()]

    print("解法タイプを入力してください")
    print("0:ランダムルート(入力データの添字順)")
    print("1:nearest neighbor法で解く")
    print("2:挿入法")
    print("3:焼きなまし法")
    
    print("type:",end="")
    state=int(input())
    ans = mod.random_route(data)
    if state==0:
        pass
    elif state==1:
        ans=mod.nn_method(data)
    elif state==2:
        print("挿入順タイプを入力してください")
        print("0:添字順")
        print("1:x座標昇順")
        print("2:y座標昇順")

        print("insert type:",end="")
        ins_state=int(input())
        if ins_state<0 or ins_state>2:
            ins_state=0
        
        ans=mod.insertion_method(data,ins_state)
    elif state==3:
        ans=mod.sa_method(data)
    else:
        print("指定された数値の範囲で入力してください")
        exit(1)
    
    mod.draw_graph(ans,"before")
    print("before_cost:",mod.total_move_cost(ans))

    ans=mod.two_opt_method(ans)

    mod.draw_graph(ans,"after")
    print("after_cost:",mod.total_move_cost(ans))