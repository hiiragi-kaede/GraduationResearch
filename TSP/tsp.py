import module.tsp_module as tsp_mod

#fname="rand100.txt"
fname="a280_use.txt"

###main function###
with open(fname) as f:
    if fname!="a280_use.txt":
        data=[list(map(float,i.split())) for i in f.readlines()]
    else:
        data=[list(map(float,i.split()[1:])) for i in f.readlines()]
    size=len(data)

    #ans=tsp_mod.nn_method(data,size)
    ans=tsp_mod.insertion_method(data,size,True)
    #ans=tsp_mod.random_route(data,size)

    tsp_mod.draw_graph(ans,"before")
    print("before_cost:",tsp_mod.total_move_cost(ans))

    ans=tsp_mod.two_opt_method(ans,size)

    tsp_mod.draw_graph(ans,"after")
    print("after_cost:",tsp_mod.total_move_cost(ans))