[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/anid_WY3)
# Chassis

The Chassis class manages:

* Motor speeds, including calculating target speeds from `SetTwist` commands and reporting back that actual motion.
* The battery monitor. We put that here both for practical (Pre-lab) reasons and because it makes use of `Timer4`, which is managed by `Chassis`.