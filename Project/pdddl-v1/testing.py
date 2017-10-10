from pddlpy import DomainProblem
domainfile = "domain-04.pddl"; problemfile = "problem-04.pddl"
domprob = DomainProblem(domainfile, problemfile)
action = "paint-up"
tile = "tile_3-1"
for o in domprob.ground_operator(action):
    print(o.precondition_pos)
    if(action,"tile_3-1","title_4,1","black") in o.precondition_pos:
        print(tile)