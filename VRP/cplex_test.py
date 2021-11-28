import cplex

var=['x','y']
b=[20,30]
c=[800,1800,1500]
A=[
    [var,[1,2]],
    [var,[3,4]],
    [var,[3,1]],
]

prob=cplex.Cplex()
prob.objective.set_sense(prob.objective.sense.maximize)
prob.variables.add(obj=b,names=var)
prob.linear_constraints.add(lin_expr=A,senses=['L']*3,rhs=c)
prob.solve()

x=prob.solution.get_values()
for i in range(len(var)):
    print(var[i],"=",x[i])
print(prob.solution.get_objective_value())