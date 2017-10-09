;; We have two robots
(define (problem test1)
  (:domain floor)

  (:objects
   rob1 - robot
   tile1 tile2 tile3 tile4 tile5 tile6 - tile
   black white - color)

  (:init
   (robot-at rob1 tile1)

   (up tile1 tile4)
   (up tile2 tile5)
   (up tile3 tile6)
   (down tile4 tile1)
   (down tile5 tile2)
   (down tile6 tile3)
   (right tile2 tile1)
   (right tile3 tile2)
   (right tile5 tile4)
   (right tile6 tile5)
   (left tile1 tile2)
   (left tile2 tile3)
   (left tile4 tile5)
   (left tile5 tile6)

   (clear tile2)
   (clear tile3)
   (clear tile4)
   (clear tile5)
   (clear tile6)

   (robot-has rob1 white)

   (available-color black)
   (available-color white)

   )

   (:goal
    (and
        (painted tile1 white)
        (painted tile2 black)
	    ))
    (:metric minimize (total-cost))
    )

