from pddlpy import DomainProblem
domainfile = "domain-04.pddl"; problemfile = "problem-04.pddl"
domprob = DomainProblem(domainfile, problemfile)
action = "paint-down"
tile = "tile_3-1"
for o in domprob.ground_operator(action):
     if("black") in o.precondition_pos:
        print(tile)