we only transition when neighborhood function finds a better move

when doing kopt(where symmetry breaking is not proper since i have to do bitwise rotation to check if this sate is already visited) we select the best move out of n opt swaps

for symmetry breaking i should think of using "Don't look bits" technique

we actually transition to a new state
but what i have seen is it kopt tangles the tour further

so kopt when applied to right edge may improve the objective function on generated sequence

now selecting the right edge is the task of <<<search algorithm>>>

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

in kopt we selected edges which were longest but we have to take edge at random.
I have not broken the symmetries properly, need to think more into it using shifting all bits

I am keeping the tabu list of very large sie keep the tabu list of recent nodes or fix the size of the list
instead of storing the states sometime store the transitions

we have a tabu matrix where we have series of operation we do 

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
kopt is actually k swaps for an optimal k
selection procedure for edges to be swapped is select the longest edge

randomization is great many times meaning we start with a random state and then do 
kopt to improve the solution further.

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

in kopt we first generate a tour T then I find the longest edge and try to replace it with edge in S-T
(S as in set of all edges) such that resulting T' new tour is feasible and better than T

in kopt i did a series of exchange such that each gave a gain "g" and all these gains "g" are
additive

i need a stoping rule in kopt as in when gains(individual) start to diminish  

important an edge to be broken can not be an edge previously joined,
same way an edge to be joined can not be an edge previously broken.

i need to maintain a datstructure that stores the edges previously broken
and when looking for new edges to be joined we look in this datastructure and ignore previously broken edges.

[[
  Since in my current algorithm i use index manupulation to make connection between edges
  so I need to maintain a dictionary of index to edge and vice versa
  In one iteration
  when i break an edge i make pair of broken vertices( and not indexes) and store in vector broken_edges storing the edge and not the index
  same way when i join an edge i make pair of joining vertices( and not indexes) and store in vector joined_edges
]]

same way also store edges previously joined and when looking for edges to be broken we look in this store
and ignore previously joined edges and this should happen in single kopt procedure

then do again a random start and do kopt on that.

i can have two data structure [[previously broken]](stores one to one mapping of vartices of whose edges were broken) and [[previously joined]](stores one to one mapping of vartices of whose edge were joined)

in serachNextN i search for next potential n that reduces the distance, can i trace the edge that will be broken and edge that will be joined based on next n

edge that will be joined is (indices[e] indices[n]) and (indices[s] indices[n-1])
edge that will be broken (indices[s] indices[e]) and (indices[n] and indices[n-1])


#######################################################################
kopt is my local search
i need a global heuristic that does not let kopt to stuck in local minima

####################################################
start with initial greedy solution
then do kopt on it whatever we get set it as bestindices

then iterate for 100 times,
start with a randomly generated indices,
do kopt on it and also check if this configuration is already present if present then ignore moving to that state.


kopt should do a local search guided towards local minima,

randomly generated incides should start from random position and then kopt should go towards minima 

may be we could find a global minima.

#####################################################
kopt should do local search that is making objective function or total distance shorten,
I should run kopt in sandbox i.e run kopt individualy and test if it is actually optimizing the objective function aka reducing the total distance if it is infact reducing the total distance that we are reaching a local minima.

start with randomly generated indices and then apply kopt on it.
#######################################################
distance

apply kopt then get the configuration
apply kopt again on above got configuration but also pass list of already seen configurations so that we do not 
do kopt on already seen configuration note(joined edges is part of current kopt loop only and is not shared between different kopts(
I may have to think about this that if i can share joined edges between different kopts!!))

#############################################################

first generate a greedy solution( store this configuration in tabu list )
set best to this if this is best
then do kopt on it one time ( store what ever configuration we get in tabu list)
set best to this if this is best

now iterate for 100 times
    generate a random configuration(if this already seen configuration generate another random)
    set best to this if it is best
    store this in curr
    for 10
        do kopt on (curr)
        if curr already in tabulist
          break
        else
          set best to this if it is best
        again do kopt on curr
        
Note: to kopt pass all the configurations,
inside kopt when doing kopt if if we see already configuration ignore it by ignoring n.
and rest all add to configuration list and set best to this if this is best
